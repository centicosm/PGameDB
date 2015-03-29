/*
Copyright (C) 2009 Centicosm Software

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "MainForm.h"
#include "SaveOnExitDialog.h"
#include "MobyWebForm.h"
#include <direct.h>

using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace PGameDB;
using namespace std;



// ------------------ NAVIGATION -------------------------------------------------------------------------
// the following functions are concerned with navigating game data via the html interface
System::Void MainForm::NavigatingHandler(System::Object^ sender, System::Windows::Forms::WebBrowserNavigatingEventArgs ^e) {
  GameViewer->AllowNavigation = false;
}

System::Void MainForm::DocumentCompletedHandler(System::Object^ sender, System::Windows::Forms::WebBrowserDocumentCompletedEventArgs ^e) {
  HtmlElementCollection ^links = GameViewer->Document->Links;
  for each (HtmlElement ^var in links) {
    var->Click += gcnew System::Windows::Forms::HtmlElementEventHandler(this, &MainForm::LinkClickedHandler);
  }
  DoneLoadingGamePage = true;
}

// cancel the navigation so we stay in control of the browser
System::Void MainForm::NewWindowHandler(System::Object^ sender, System::ComponentModel::CancelEventArgs ^e) {
  e->Cancel = true;
}


// handle clicking on elements of the game description (view images, docs, or perform filtering)
System::Void MainForm::LinkClickedHandler(System::Object^ sender,  System::Windows::Forms::HtmlElementEventArgs ^e) {
  HtmlElement ^link = GameViewer->Document->ActiveElement;
  URL = link->GetAttribute("href");

  // handle clicking on an image (but make sure it is not also a doc link
  if ((URL->IndexOf("doc:") == -1) && ((URL->IndexOf(".jpg") != -1) || (URL->IndexOf(".jpeg") != -1) || (URL->IndexOf(".png") != -1) || 
    (URL->IndexOf(".gif") != -1))) {
      URL = URL->Replace("about:", "");

      // the location of the file will be in the screen shot directory, so form the path string
      const char *datapath = gConfMan->Get_ConfigItem("ShotPath");
      if (!datapath) {
        MessageBox::Show("Error: Configuration file corrupted!");
      }
      String ^dp = gcnew String(datapath);

      dp = dp->Replace("/", "\\");
      dp = dp->Replace(".", "");
      dp = String::Concat(Environment::CurrentDirectory, dp);
      dp = String::Concat(dp, URL);

      ImageViewerForm ^ivf = gcnew ImageViewerForm(dp);
      ivf->Show();
      GameViewer->AllowNavigation = false;
  }

  // handle document viewing
  bool docclick = false;
  bool guideclick = false;
  String ^docstr;

  if (URL->IndexOf("doc:", System::StringComparison::OrdinalIgnoreCase) != -1) {
    docclick = true;
    docstr = URL->Substring(4);
  }
  else if (URL->IndexOf("GGuide:", System::StringComparison::OrdinalIgnoreCase) != -1) {
    guideclick = true;
    docstr = URL->Substring(7);
  }

  // it is a document or guide click so figure out the kind of document
  if (docclick || guideclick) {
    bool pdf = false;
    bool pic = false;
    bool audio = false;

    // determine the type of document
    // pdf type
    if (docstr->IndexOf(".pdf", System::StringComparison::OrdinalIgnoreCase) != -1) {
      pdf = true;
    }

    // image types
    if (docstr->IndexOf(".jpg", System::StringComparison::OrdinalIgnoreCase) != -1) {
      pic = true;
    }
    else if (docstr->IndexOf(".jpeg", System::StringComparison::OrdinalIgnoreCase) != -1) {
      pic = true;
    }
    else if (docstr->IndexOf(".png", System::StringComparison::OrdinalIgnoreCase) != -1) {
      pic = true;
    }
    else if (docstr->IndexOf(".gif", System::StringComparison::OrdinalIgnoreCase) != -1) {
      pic = true;
    }

    // audio types
    if (docstr->IndexOf(".mp3", System::StringComparison::OrdinalIgnoreCase) != -1) {
      audio = true;
    } 
    if (docstr->IndexOf(".ogg", System::StringComparison::OrdinalIgnoreCase) != -1) {
      audio = true;
    } 

    // determine the data path from the config file
    const char *datapath = NULL;
    if (docclick) {
      datapath = gConfMan->Get_ConfigItem("DocumentationPath");
    }
    else if (guideclick) {
      datapath = gConfMan->Get_ConfigItem("GuidePath");
    }
    if (!datapath) {
      MessageBox::Show("Error: Configuration file corrupted!");
      return;
    }
    String ^dp = gcnew String(datapath);
    dp = dp->Replace("/", "\\");
    String ^cp = String::Concat(Environment::CurrentDirectory, "\\");
    dp = String::Concat(cp, dp);
    docstr = String::Concat(dp, docstr);


    // if the documentation is a pdf use the os to open it (assumes user has a pdf viewer installed
    if (pdf) {
		try {
			System::Diagnostics::Process::Start(docstr);
		}
		catch(...) {
			MessageBox::Show("There was a problem opening: " + docstr);
		}
    }

    // if the document is an image show it in the image viewer
    else if (pic) {
      ImageViewerForm ^ivf = gcnew ImageViewerForm(docstr);
      ivf->Show();
      GameViewer->AllowNavigation = false;
    }

    // if it is an audio file, use the os to play it
    else if (audio) {
		try {
			System::Diagnostics::Process::Start(docstr);
		}
		catch (...) {
			MessageBox::Show("There was a problem opening: " + docstr);
		}
    }

    // if it is not a known type, handle it with the web viewer
    else {
      MobyWebForm ^mwf = gcnew MobyWebForm(docstr);
      mwf->Show();
    }
  }

  // handle clicking on a link for a category view
  else if (URL->IndexOf(":") != -1) {
    SelectedCategory = URL;
    Populate_CategoryView();
    CategoryToolStripMenuItem_Click(sender, nullptr);
  } 
}
//------------------------------------------------------------------------------------




// ------------------------- FILTERING FUNCTIONS -----------------------------------------------

// handle choosing unsorted from the view drop down menu
System::Void MainForm::UnsortedToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
  ViewModeDropDownButton->Text = "Unsorted";
  Populate_UnsortedView();
}

// handle choosing category from the view drop down menu
System::Void MainForm::CategoryToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
  if (SelectedCategory != nullptr) {
    ViewModeDropDownButton->Text = "Category";
    Populate_CategoryView();
  }
}

// handle choosing custom from the view drop down menu
System::Void MainForm::CustomToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
  ViewModeDropDownButton->Text = "Custom";
  Populate_CustomView();
}



// --------------------------   SEARCH RELATED FUNCTIONS ---------------------------------------------
// handle clearing the search box and setting the font
System::Void MainForm::SearchToolStripTextBox_Click(System::Object^  sender, System::EventArgs^  e) {
  if (TextFilter->Text->Equals("Search")) {
    TextFilter->Text = "";
    TextFilter->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));;
  }
}

// if the search box is empty, put search back in the box
System::Void MainForm::SearchToolStripTextBox_FocusLeave(System::Object^  sender, System::EventArgs^  e) {
  if (TextFilter->Text->Equals("")) {
    TextFilter->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
    TextFilter->Text = "Search";
    TextFilterOn = false;
  }
  else {
    TextFilterOn = true;

    // now show filter the desired view
    Display_Games();
  }
}

// search when enter is pressed
System::Void MainForm::SearchToolStripTextBox_KeyPress(System::Object ^ sender, System::Windows::Forms::KeyPressEventArgs^  e) {
  if (e->KeyChar == '\r') {
    TextFilterOn = true;
    
    // now show filter the desired view
    Display_Games();
  }
}


// special search function to identify games without a screen shots
System::Void MainForm::SearchForNoScreenshotsToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

  ViewModeDropDownButton->Text = "Special";

  GameList->BeginUpdate();
  GameList->Nodes->Clear();
  SearchNode = GameList->Nodes->Add("No Screenshots");

  for each (GameDescriptor ^gd in MasterList) {
    if (gd->Screenshots->Count == 0) {
      Add_GameToGameList(SearchNode, gd);
    }
  }
  SearchNode->Expand();
  GameList->EndUpdate();
}



// ------------------------- CUSTOM CATEGORY FUNCTIONS--------------------------------------
// Adds the currently selected category to the custom category list
System::Void MainForm::TransferCategoryButton_Click(System::Object^  sender, System::EventArgs^  e) {

  // if the category isn't already present in the present category list return
  //   otherwise add it to the list
  if (ViewModeDropDownButton->Text != "Category") {
    return;
  }
  for each (String ^s in CustomViewCategories) {
    if (String::Equals(s, SelectedCategory)) {
      return;
    }
  }
  CustomViewCategories.AddLast(SelectedCategory);
  String ^text = gcnew String("Added custom category: ");

  // reformat the html used to make it display prettier
  String ^s2 = gcnew String(SelectedCategory);
  s2 = s2->Replace("releaseyear:", "Release Year:");
  s2 = s2->Replace("genre:", "Genre:");
  s2 = s2->Replace("developer:", "Developer:");
  s2 = s2->Replace("publisher:", "Publisher:");
  s2 = s2->Replace("location:", "Location:");
  s2 = s2->Replace("completedyear:", "Completed Year:");
  s2 = s2->Replace("completed:", "Completed:");
  s2 = s2->Replace("releaseyear:", "Release Year:");
  s2 = s2->Replace("series:", "Series:");
  s2 = s2->Replace("platform:", "Platform:");
  s2 = s2->Replace("rating:", "Rating:");

  text = String::Concat(text, s2);
  MessageBox::Show(text);

  Modified = true;
}


// edit the custom categories used in custom view
System::Void MainForm::EditCustomCategoriesMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
  List<String ^> ^ll = gcnew List<String ^>();
  for each (String ^s in CustomViewCategories) {
    String ^s2 = gcnew String(s);

    // reformat the html used to make it display prettier
    s2 = s2->Replace("releaseyear:", "Release Year:");
    s2 = s2->Replace("genre:", "Genre:");
    s2 = s2->Replace("developer:", "Developer:");
    s2 = s2->Replace("publisher:", "Publisher:");
    s2 = s2->Replace("location:", "Location:");
    s2 = s2->Replace("completedyear:", "Completed Year:");
    s2 = s2->Replace("completed:", "Completed:");
    s2 = s2->Replace("releaseyear:", "Release Year:");
    s2 = s2->Replace("series:", "Series:");
    s2 = s2->Replace("platform:", "Platform:");
    s2 = s2->Replace("rating:", "Rating:");
    ll->Add(s2);
  }
  EditListForm ^elf = gcnew EditListForm(ll);
  if (elf->ShowDialog(this) ==  ::DialogResult::OK) {

    CustomViewCategories.Clear();
    for each (String ^s in ll) {
      String ^s2 = gcnew String(s);

      // reformat the the pretty html to internal format
      s2 = s2->Replace("Release Year:", "releaseyear:");
      s2 = s2->Replace("Genre:", "genre:");
      s2 = s2->Replace("Developer:", "developer:");
      s2 = s2->Replace("Publisher:", "publisher:");
      s2 = s2->Replace("Location:", "location:");
      s2 = s2->Replace("Completed Year:", "completedyear:");
      s2 = s2->Replace("Completed:", "completed:");
      s2 = s2->Replace("Release Year:", "releaseyear:");
      s2 = s2->Replace("Series:", "series:");
      s2 = s2->Replace("Platform:", "platform:");
      s2 = s2->Replace("Rating:", "rating:");

      if (s2->Contains("releaseyear:")) {
        CustomViewCategories.AddLast(s2);
      }
      else if (s2->Contains("genre:")) {
        CustomViewCategories.AddLast(s2);
      }
      else if (s2->Contains("developer:")) {
        CustomViewCategories.AddLast(s2);
      }
      else if (s2->Contains("publisher:")) {
        CustomViewCategories.AddLast(s2);
      }
      else if (s2->Contains("location:")) {
        CustomViewCategories.AddLast(s2);
      }
      else if (s2->Contains("completedyear:")) {
        CustomViewCategories.AddLast(s2);
      }
      else if (s2->Contains("completed:")) {
        CustomViewCategories.AddLast(s2);
      }
      else if (s2->Contains("releaseyear:")) {
        CustomViewCategories.AddLast(s2);
      }
      else if (s2->Contains("series:")) {
        CustomViewCategories.AddLast(s2);
      }
      else if (s2->Contains("platform:")) {
        CustomViewCategories.AddLast(s2);
      }
      else if (s2->Contains("rating:")) {
        CustomViewCategories.AddLast(s2);
      }
      else {
        CustomViewCategories.AddLast(s2);
      }
    }
    Modified = true;

    if (ViewModeDropDownButton->Text == "Custom") {
      Populate_CustomView();
    }
  }

}

// Handle the event in which the add game category button is pressed
//  user is prompted for a new custom category to add
System::Void MainForm::AddGameCategoryButton_Click(System::Object^  sender, System::EventArgs^  e) {

  if (ViewModeDropDownButton->Text != "Custom") {
    MessageBox::Show("Must be in custom view mode to add game category.");
    return;
  }

  // Collect the game category name from the user
  SimpleTextEntryForm ^ste = gcnew SimpleTextEntryForm();
  ste->Set_Label("Game Category to Add:");
  ste->Set_Title("Add a Game Category");

  if (ste->ShowDialog() == ::DialogResult::OK) {
    bool matchfound = false;
    String ^newcat = ste->Get_Text();

    // determine if we already have a category by that name
    for each (String ^s in CustomViewCategories) {
      if (String::Equals(s, newcat)) {
        matchfound = true;
        break;
      }
    }

    // if we don't, create the new category
    if (!matchfound) {
      ActiveCategories.AddLast(newcat);
      CustomViewCategories.AddLast(newcat);
      TreeNode ^node = GameList->Nodes->Add(newcat);
      node->Name = newcat;
      DBCategories->CustomCategoryMembership->Add(newcat);
      Modified = true;
    }
    else {
      MessageBox::Show("Game category already exists");
    }
  }
}


//  used to track which categories in custom view are expanded
System::Void MainForm::GameList_AfterCollapse(System::Object^ sender, TreeViewEventArgs ^e) {

  ExpandedCategories.Remove(e->Node->Name);
}

//  used to track which categories in custom view are expanded
System::Void MainForm::GameList_AfterExpand(System::Object^ sender, TreeViewEventArgs ^e) {

  if (ExpandedCategories.Contains(e->Node->Name)) {
    return;
  }
  ExpandedCategories.AddLast(e->Node->Name);
}


// ----------------------------------------------------------------------------------------------




// Handle key presses in the main form
System::Void MainForm::GameList_KeyPress(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) { 

  // pressing enter will open the edit window
  if (e->KeyCode == Keys::Enter) {
    GameList_DoubleClick(sender, nullptr);
  }

  // handle if the user presses delete for a particular game
  if (e->KeyCode == Keys::Delete) {

    // make sure something is selected
    if (GameList->SelectedNode == nullptr) {
      return;
    }
    // make sure a category is not selected
    String ^nodename = GameList->SelectedNode->Name;
    array<TreeNode ^> ^nodes = GameList->Nodes->Find(nodename, false);
    if (nodes->Length != 0) {
      return;
    }

    // delete the screenshots associated with the game
    // TODO: screen shots remain deleted even if changes aren't saved
    const char *datapath = gConfMan->Get_ConfigItem("ShotPath");
    if (!datapath) {
      MessageBox::Show("Error: Configuration file corrupted!");
      return;
    }
    String ^imgpath = gcnew String(datapath);
    imgpath = imgpath->Replace("/", "\\");
    imgpath = imgpath->Replace(".", "");
    imgpath = String::Concat(Environment::CurrentDirectory, imgpath);

    GameDescriptor ^gd = cli::safe_cast<GameDescriptor ^>(GameList->SelectedNode->Tag);
    for each (String ^ss in gd->Screenshots) {
      String ^ssloc = String::Concat(imgpath, ss);
      try {
        File::Delete(ssloc);
      } 
      catch (Exception ^) {}
    }

    // remove the game from the master list
    MasterList.Remove(cli::safe_cast<GameDescriptor ^> (GameList->SelectedNode->Tag));

    // remove the game from the treeview
    TreeNode ^nn = GameList->SelectedNode->NextNode;
    GameDescriptor ^ngd = cli::safe_cast<GameDescriptor ^> (nn->Tag);
    String ^gamename = ngd->GameName;
    GameList->Nodes->Remove(GameList->SelectedNode);
    Select_GameByName(gamename);

    // update the number of games in the db
    char buf[100];
    sprintf(buf, "%d", MasterList.Count);
    PGameDBNumGameVal->Text = gcnew String(buf);

    Modified = true;
  }
}



// Function to highlight a given game name
System::Void MainForm::Select_GameByName(String ^gamename) {
  if (gamename == nullptr) {
    return;
  }

  // if it exists select it then render the html
  array<TreeNode ^> ^nodes = GameList->Nodes->Find(gamename, true);
  if (nodes->Length != 0) {
    GameList->SelectedNode = nodes[0];
    StoredNode = GameList->SelectedNode;
    GameList_AfterSelect(nullptr, nullptr);
  }

  // if it doesn't, just make a blank page
  else {
    String ^html = "<html><body bgcolor=#85A1C0></body></html>\n";
    GameViewer->DocumentText = html; 
  }
}



// --------------------------- HTML GAME DESCRIPTION RENDERING------------------------------------------------
// Display the selected game in the html viewer
System::Void MainForm::GameList_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) {

  if (!DoneLoadingGamePage) {
    GameList->SelectedNode = StoredNode;
    return;
  }
  // HACK - used to keep selected node from jumping around after a name change
  if (ChangedName) {
    ChangedName = false;
    GameList->SelectedNode = StoredNode;
    return;
  }

  DoneLoadingGamePage = false;
  StoredNode = GameList->SelectedNode;

  GameViewer->AllowNavigation = true;

  // Hmmmmm
  String ^nodename = GameList->SelectedNode->Name;
  if (GameList->SelectedNode->Parent == nullptr) {
    DoneLoadingGamePage = true;
    return;
  }

  GameDescriptor ^gd = (cli::safe_cast<GameDescriptor ^> (GameList->SelectedNode->Tag));

  // generate html header
  String ^html = "<html><head><STYLE TYPE=\"text/css\">\n";
  html = String::Concat(html, "H1 { font-family:arial, helvetica; font-size:24px; font-weight:bold; color:yellow;}\n");
  html = String::Concat(html, "a { color:black; }\n");
  html = String::Concat(html, "H4 { font-family:arial, helvetica; font-size:16px; font-weight:bold color:black; margin-bottom: 0;}\n");
  html = String::Concat(html, "body { font-family:arial, helvetica; font-size:14px; font-weight:bold color:black; }\n");
  html = String::Concat(html, "div.coverimages { margin: 0px 0px 0px 0px; float: left;}\n");
  html = String::Concat(html, "div.coverimages { margin: 0px 0px 0px 0px; float: left;}\n");
  html = String::Concat(html, "</STYLE></head><body>\n");
  html = String::Concat(html, "<body bgcolor=#85A1C0>\n");


  // generate title screenshot html
  html = String::Concat(html, "<div class=\"coverimages\">");
  html = String::Concat(html, Generate_TitleImageHTML(gd));

  // generate back cover html
  html = String::Concat(html, Generate_BackCoverImageHTML(gd));
  html = String::Concat(html, "</div>");

  // generate title html
  html = String::Concat(html, "<h1 style=\"color:yellow\">");
  //html = String::Concat(html, GameList->SelectedNode->Text);
  html = String::Concat(html, gd->GameName);
  html = String::Concat(html, "</h1>\n");

  //generate developer html
  html = String::Concat(html, "<table border=\"0\">\n");
  if (!String::IsNullOrEmpty(gd->Developer)) {
    html = String::Concat(html, "<tr><td><h4 style=\"color:black\">Developer:</h4></td><td><h4 style=\"color:black\"><a href=\"Developer:");
    html = String::Concat(html, gd->Developer);
    html = String::Concat(html, "\">");
    html = String::Concat(html, gd->Developer);
    html = String::Concat(html, "</a></h4></td></tr>\n");
  }

  //generate publisher html
  if (!String::IsNullOrEmpty(gd->Publisher)) {
    html = String::Concat(html, "<tr><td><h4 style=\"color:black\">Publisher:</h4></td><td><h4 style=\"color:black\"><a href=\"Publisher:");
    html = String::Concat(html, gd->Publisher);
    html = String::Concat(html, "\">");
    html = String::Concat(html, gd->Publisher);
    html = String::Concat(html, "</a></h4></td></tr>\n");
  }

  //generate release year html
  if (!String::IsNullOrEmpty(gd->ReleaseYear)) {
    html = String::Concat(html, "<tr><td><h4 style=\"color:black\">Year:</h4></td><td><h4 style=\"color:black\"><a href=\"ReleaseYear:");
    html = String::Concat(html, gd->ReleaseYear);
    html = String::Concat(html, "\">");
    html = String::Concat(html, gd->ReleaseYear);
    html = String::Concat(html, "</a></h4></td></tr>\n");
  }

  //generate series html
  if (!String::IsNullOrEmpty(gd->Series)) {
    html = String::Concat(html, "<tr><td><h4 style=\"color:black\">Series:</h4></td><td><h4 style=\"color:black\"><a href=\"Series:");
    html = String::Concat(html, gd->Series);
    html = String::Concat(html, "\">");
    html = String::Concat(html, gd->Series);
    html = String::Concat(html, "</a></h4></td></tr>\n");
  }

  html = String::Concat(html, "<tr></tr><tr></tr>\n");


  // generate the genre html
  System::String ^genrehtml = "";
  if (gd->Genres->Count != 0) {
    html = String::Concat(html, "<tr><td><h4 style=\"color:black\">Genre:</h4></td><td><h4 style=\"color:black\">");
  }
  for each (System::String^ gs in gd->Genres) {
    genrehtml = String::Concat(genrehtml, "<a href=\"Genre:");
    genrehtml = String::Concat(genrehtml, gs);
    genrehtml = String::Concat(genrehtml, "\">");
    genrehtml = String::Concat(genrehtml, gs);
    genrehtml = String::Concat(genrehtml, "</a>\n");
  }
  if (gd->Genres->Count != 0) {
    html = String::Concat(html, genrehtml);
    html = String::Concat(html, "</h4></td></tr><td></tr>\n");
  }

  // generate the platform html
  System::String ^plathtml = "";
  if (!String::IsNullOrEmpty(gd->PrimaryPlatform)) {
    html = String::Concat(html, "<tr><td><h4 style=\"color:black\">Platform:</h4></td><td><h4 style=\"color:black\">");
    html = String::Concat(html, "<a href=\"Platform:");
    html = String::Concat(html, gd->PrimaryPlatform);
    html = String::Concat(html, "\">");
    html = String::Concat(html, gd->PrimaryPlatform);
    html = String::Concat(html, "</a></h3></td></tr>\n");
  }

  if (gd->AlternatePlatforms->Count != 0) {
    html = String::Concat(html, "<tr><td><h4 style=\"color:black\">Also On:</h4></td><td><h4 style=\"color:black\">");
  }
  for each (System::String^ gs in gd->AlternatePlatforms) {
    html = String::Concat(html, "<a href=\"Platform:");
    html = String::Concat(html, gs);
    html = String::Concat(html, "\">");
    html = String::Concat(html, gs);
    html = String::Concat(html, "</a>,\n");
  }
  if (gd->AlternatePlatforms->Count != 0) {
    html = String::Concat(html, "</h4></td></tr>");
  }
  html = String::Concat(html, "</table><br>\n");

  //generate description
  if (!String::IsNullOrEmpty(gd->Description)) {
    html = String::Concat(html, gd->Description);
  }

  html = String::Concat(html, "<br><br><br>\n");

  if (!String::IsNullOrEmpty(gd->Notes)) {
    html = String::Concat(html, "<b>Notes:</b>&nbsp;");
    html = String::Concat(html, gd->Notes);
    html = String::Concat(html, "<br><br><br>\n");
  }

  // generate the Documentation html
  System::String ^dochtml = "";
  if (gd->ManualDocumentation->Count != 0) {
    html = String::Concat(html, "<table border=\"0\">\n");
    html = String::Concat(html, "<tr><td><h4 style=\"color:black\">Documentation:</h4></td><td><h4 style=\"color:black\">");

  }
  bool first = true;
  for each (DocumentationID ^gid in gd->ManualDocumentation) {
    if (!first) {
      dochtml = String::Concat(dochtml, "<tr><td></td><td><h4><a href=\"Doc:");
    }
    else {
      dochtml = String::Concat(dochtml, "<a href=\"Doc:");
      first = false;
    }
    dochtml = String::Concat(dochtml, gid->Filename);
    dochtml = String::Concat(dochtml, "\">");
    dochtml = String::Concat(dochtml, gid->FileType);
    dochtml = String::Concat(dochtml, "</a></h4></td></tr>\n");
  }
  if (gd->ManualDocumentation->Count != 0) {
    html = String::Concat(html, dochtml);

    if (gd->HintDocumentation->Count == 0) {
      html = String::Concat(html, "</h4></td></tr></table><br>\n");
    }
    else {
      html = String::Concat(html, "</h4></td></tr> <tr></tr> \n");
    }
  }

  // generate the Hint guide html
  System::String ^hinthtml = "";
  if (gd->HintDocumentation->Count != 0) {

    System::String ^dochtml = "";
    if (gd->ManualDocumentation->Count == 0) {
      html = String::Concat(html, "<table border=\"0\">\n");
    }
    html = String::Concat(html, "<tr><td><h4 style=\"color:black\">Guides:</h4></td><td><h4 style=\"color:black\">");
  }
  first = true;
  for each (DocumentationID ^gid in gd->HintDocumentation) {
    if (!first) {
      hinthtml = String::Concat(hinthtml, "<tr><td></td><td><h4><a href=\"Doc:");
    }
    else {
      hinthtml = String::Concat(hinthtml, "<a href=\"GGuide:");
      first = false;
    }
    hinthtml = String::Concat(hinthtml, gid->Filename);
    hinthtml = String::Concat(hinthtml, "\">");
    hinthtml = String::Concat(hinthtml, gid->FileType);
    dochtml = String::Concat(hinthtml, "</a></h4></td></tr>\n");
  }
  if (gd->HintDocumentation->Count != 0) {
    html = String::Concat(html, hinthtml);
    html = String::Concat(html, "</h4></td></tr></table><br>\n");
  }

  // generate location html
  html = String::Concat(html, "<table border=\"0\">\n");
  html = String::Concat(html, Generate_CustomCategoryHTML(gd));

  html = String::Concat(html, "<tr><td>&nbsp</td></tr>");

  if (!String::IsNullOrEmpty(gd->Location1)) {
    html = String::Concat(html, "<tr><td><h4 style=\"color:black\">Location:</h4></td><td><h4 style=\"color:black\"><a href=\"Location:");
    html = String::Concat(html, gd->Location1);
    html = String::Concat(html, "\">");
    html = String::Concat(html, gd->Location1);
    html = String::Concat(html, "</a></h4></td></tr>\n");
  }
  if (!String::IsNullOrEmpty(gd->Location2)) {
    html = String::Concat(html, "<tr><td></td><td><h4 style=\"color:black\"><a href=\"Location:");
    html = String::Concat(html, gd->Location2);
    html = String::Concat(html, "\">");
    html = String::Concat(html, gd->Location2);
    html = String::Concat(html, "</a></h4></td></tr>\n");
  }
  if (!String::IsNullOrEmpty(gd->Location3)) {
    html = String::Concat(html, "<tr><td></td><td><h4 style=\"color:black\"><a href=\"Location:");
    html = String::Concat(html, gd->Location3);
    html = String::Concat(html, "\">");
    html = String::Concat(html, gd->Location3);
    html = String::Concat(html, "</a></h4></td></tr>\n");
  }
  if (!String::IsNullOrEmpty(gd->Location4)) {
    html = String::Concat(html, "<tr><td></td><td><h4 style=\"color:black\"><a href=\"Location:");
    html = String::Concat(html, gd->Location4);
    html = String::Concat(html, "\">");
    html = String::Concat(html, gd->Location4);
    html = String::Concat(html, "</a></h4></td></tr>\n");
  }
  html = String::Concat(html, "<tr></tr>\n");

  if (!String::IsNullOrEmpty(gd->Completed)) {
    html = String::Concat(html, "<tr><td><h4 style=\"color:black\">Completed:</h4></td><td><h4 style=\"color:black\"><a href=\"Completed:");
    html = String::Concat(html, gd->Completed);
    html = String::Concat(html, "\">");
    html = String::Concat(html, gd->Completed);
    html = String::Concat(html, "</a></h4></td></tr>\n");
  }
  if (!String::IsNullOrEmpty(gd->CompletedYear)) {
    html = String::Concat(html, "<tr><td><h4 style=\"color:black\">Completion Year:&nbsp&nbsp</h4></td><td><h4 style=\"color:black\"><a href=\"CompletedYear:");
    html = String::Concat(html, gd->CompletedYear);
    html = String::Concat(html, "\">");
    html = String::Concat(html, gd->CompletedYear);
    html = String::Concat(html, "</a></h4></td></tr>\n");
  }

  html = String::Concat(html, "<tr><td><h4 style=\"color:black\">Rating:</h4></td><td><h4 style=\"color:black\"><a href=\"Rating:");
  if (String::IsNullOrEmpty(gd->Rating)) {
    html = String::Concat(html, "norating");
  }
  else {
    html = String::Concat(html, gd->Rating);
  }
  html = String::Concat(html, "\">");
  html = String::Concat(html, Generate_RatingHtml(gd));
  html = String::Concat(html, "</a></h4></td></tr>\n");
  html = String::Concat(html, "</table>\n");

  html = String::Concat(html, Generate_ScreenShotHTML(gd));

  html = String::Concat(html, "</body></html>\n");

  GameViewer->DocumentText = html; 
}


// generate the html for the front box shot
System::String ^MainForm::Generate_TitleImageHTML(GameDescriptor ^gd) {
  char bbuf[200];
  System::String ^html = gcnew System::String("");

  // we will size the box shots based on the config file
  int maxsize = 200;
  const char *msize = gConfMan->Get_ConfigItem("MaxTitleSizeX");
  if (msize) {
    maxsize = atoi(msize);
  }
  if (maxsize == 0) {
    maxsize = 200;
  }

  if (String::IsNullOrEmpty(gd->TitleShot)) {
    return html;
  }

  // figure out the image size
  try {
    Bitmap^ titleimage;
    const char *datapath = gConfMan->Get_ConfigItem("ShotPath");
    if (!datapath) {
      MessageBox::Show("Error: Configuration file corrupted!");
    }
    String ^imgpath = gcnew String(datapath);
    imgpath = imgpath->Replace("/", "\\");
    imgpath = imgpath->Replace(".", "");
    imgpath = String::Concat(Environment::CurrentDirectory, imgpath);
    imgpath = String::Concat(imgpath, gd->TitleShot);

    titleimage = gcnew Bitmap(imgpath ,true );
    int x, y;
    x = titleimage->Width;
    y = titleimage->Height;
    float aspect = (float)x / (float)y;

    // set up the html to display the title shot
    html = String::Concat(html, "<a href=\"");
    html = String::Concat(html, gd->TitleShot);
    html = String::Concat(html, "\">");
    html = String::Concat(html, "<img src=\"file:///");
    html = String::Concat(html, imgpath);

    html = String::Concat(html, "\"");

    // resize image based on preferences
    if (x > maxsize) {
      float yvalf = (float)maxsize / (float)x;
      int yvali = (int)((float)y * yvalf);

      sprintf(bbuf, " width=%d, height=%d></a>", maxsize, yvali);
      System::String ^widthstr = gcnew String(bbuf);
      html = String::Concat(html, widthstr);
      html = String::Concat(html, "<br>");
    }
    else {
      sprintf(bbuf, " width=%d, height=%d></a>", x, y);
      System::String ^widthstr = gcnew String(bbuf);
      html = String::Concat(html, widthstr);
      html = String::Concat(html, "<br>");
    }
  }
  catch (ArgumentException^) {
    //MessageBox::Show("Could not find title image file." );
  }
  return html;
}

// generate the html for the back box shot
System::String ^MainForm::Generate_BackCoverImageHTML(GameDescriptor ^gd) {

  System::String ^html = gcnew System::String("");

  // size the image based on the config file
  int maxsize = 200;
  const char *msize = gConfMan->Get_ConfigItem("MaxTitleSizeX");
  if (msize) {
    maxsize = atoi(msize);
  }
  if (maxsize == 0) {
    maxsize = 200;
  }


  if (String::IsNullOrEmpty(gd->BackShot)) {
    return html;
  }

  // figure out the image size
  try {
    Bitmap^ titleimage;
    const char *datapath = gConfMan->Get_ConfigItem("ShotPath");
    if (!datapath) {
      MessageBox::Show("Error: Configuration file corrupted!");
    }
    String ^imgpath = gcnew String(datapath);
    imgpath = imgpath->Replace("/", "\\");
    imgpath = imgpath->Replace(".", "");
    imgpath = String::Concat(Environment::CurrentDirectory, imgpath);
    imgpath = String::Concat(imgpath, gd->BackShot);

    titleimage = gcnew Bitmap(imgpath ,true );
    int x, y;
    x = titleimage->Width;
    y = titleimage->Height;
    float aspect = (float)x / (float)y;

    // set up the html to display the title shot
    html = String::Concat(html, "<a href=\"");
    html = String::Concat(html, gd->BackShot);
    html = String::Concat(html, "\">");
    html = String::Concat(html, "<img src=\"file:///");
    html = String::Concat(html, imgpath);

    html = String::Concat(html, "\"");

    // resize image based on preferences
    if (x > maxsize) {
      float yvalf = (float)maxsize / (float)x;
      int yvali = (int)((float)y * yvalf);
      char tempbuf[500];
      sprintf(tempbuf, " width=%d, height=%d></a>", maxsize, yvali);
      System::String ^widthstr = gcnew String(tempbuf);
      html = String::Concat(html, widthstr);
    }
    else {
      char tempbuf[500];
      sprintf(tempbuf, " width=%d, height=%d></a>", x, y);
      System::String ^widthstr = gcnew String(tempbuf);
      html = String::Concat(html, widthstr);
    }
  }
  catch ( ArgumentException^) {
    //MessageBox::Show("Could not find title image file." );
  }
  return html;
}


// generate the html for the screen shots
System::String ^MainForm::Generate_ScreenShotHTML(GameDescriptor ^gd) {

  System::String ^html = gcnew System::String("");

  // size based on config file setting
  int maxsize = 200;
  const char *msize = gConfMan->Get_ConfigItem("MaxScreenShotSizeX");
  if (msize) {
    maxsize = atoi(msize);
  }
  if (maxsize == 0) {
    maxsize = 200;
  }

  if (gd->Screenshots->Count == 0) {
    return html;
  }

  html = String::Concat(html, "<BR CLEAR=left><br>");
  html = String::Concat(html, "<div class=\"screenshots\">");

  for each (String ^ss in gd->Screenshots) {

    // figure out the image size
    bool validfile = false;
    Bitmap^ titleimage;
    const char *datapath = gConfMan->Get_ConfigItem("ShotPath");
    if (!datapath) {
      MessageBox::Show("Error: Configuration file corrupted!");
    }
    String ^imgpath = gcnew String(datapath);
    imgpath = imgpath->Replace("/", "\\");
    imgpath = imgpath->Replace(".", "");
    imgpath = String::Concat(Environment::CurrentDirectory, imgpath);
    String ^invalidpath = imgpath;
    imgpath = String::Concat(imgpath, ss);

    try {
      titleimage = gcnew Bitmap(imgpath ,true );
      validfile = true;
    }
    catch (ArgumentException^) {
      invalidpath = String::Concat(invalidpath, "invalid.jpg");
      titleimage = gcnew Bitmap(invalidpath ,true );
    }

    int x, y;
    x = titleimage->Width;
    y = titleimage->Height;
    float aspect = (float)x / (float)y;

    // set up the html to display the screen shot
    html = String::Concat(html, "<a href=\"");
    html = String::Concat(html, ss);
    html = String::Concat(html, "\">");

    html = String::Concat(html, "<img src=\"file:///");
    if (validfile) {
      html = String::Concat(html, imgpath);
    }
    else {
      html = String::Concat(html, invalidpath);
    }

    html = String::Concat(html, "\"");

    // resize the image based on preferences
    if (x > maxsize) {
      float yvalf = (float)maxsize / (float)x;
      int yvali = (int)((float)y * yvalf);
      char tempbuf[500];
      sprintf(tempbuf, " width=%d, height=%d style=\"float: left;\"></a>", 320, 200);
      System::String ^widthstr = gcnew String(tempbuf);
      html = String::Concat(html, widthstr);
    }
    else {
      char tempbuf[500];
      sprintf(tempbuf, " width=%d, height=%d style=\"float: left;\"></a>", 320, 200);
      System::String ^widthstr = gcnew String(tempbuf);
      html = String::Concat(html, widthstr);
    }

    html = String::Concat(html, "</div>");

  }
  return html;
}


// generate the html listing the custom categories the game belongs to
String ^MainForm::Generate_CustomCategoryHTML(GameDescriptor ^gd) {

  String ^s = gcnew String("");

  if (gd->CustomCategoryMembership->Count != 0) {
    s = String::Concat(s, "<tr><td><h4 style=\"color:black\">Category Membership:</h4></td><td><h4 style=\"color:black\">");

    for each (String^ gs in gd->CustomCategoryMembership) {
      s = String::Concat(s, "<a href=\"Custom:");
      s = String::Concat(s, gs);
      s = String::Concat(s, "\">");
      s = String::Concat(s, gs);
      s = String::Concat(s, "</a>\n");
    }
    s = String::Concat(s, "</h4></td></tr><td></tr>\n");
  }
  return s;
}

// generate the html which shows the rating given the game
System::String ^MainForm::Generate_RatingHtml(GameDescriptor ^gd) {
  if (!String::IsNullOrEmpty(gd->Rating)) {
    if (String::Equals(gd->Rating, "0.5 Star")) {
      return "<b><font color=\"yellow\" face=\"wingdings\">&#159</font></b>";
    }
    if (String::Equals(gd->Rating, "1.0 Star")) {
      return "<b><font color=\"yellow\" face=\"wingdings\">&#171</font></b>";
    }
    if (String::Equals(gd->Rating, "1.5 Star")) {
      return "<b><font color=\"yellow\" face=\"wingdings\">&#171&#159</font></b>";
    }
    if (String::Equals(gd->Rating, "2.0 Stars")) {
      return "<b><font color=\"yellow\" face=\"wingdings\">&#171&#171</font></b>";
    }
    if (String::Equals(gd->Rating, "2.5 Stars")) {
      return "<b><font color=\"yellow\" face=\"wingdings\">&#171&#171&#159</font></b>";
    }
    if (String::Equals(gd->Rating, "3.0 Stars")) {
      return "<b><font color=\"yellow\" face=\"wingdings\">&#171&#171&#171</font></b>";
    }
    if (String::Equals(gd->Rating, "3.5 Stars")) {
      return "<b><font color=\"yellow\" face=\"wingdings\">&#171&#171&#171&#159</font></b>";
    }
    if (String::Equals(gd->Rating, "4.0 Stars")) {
      return "<b><font color=\"yellow\" face=\"wingdings\">&#171&#171&#171&#171</font></b>";
    }
    if (String::Equals(gd->Rating, "4.5 Stars")) {
      return "<b><font color=\"yellow\" face=\"wingdings\">&#171&#171&#171&#171&#159</font></b>";
    }
    if (String::Equals(gd->Rating, "5.0 Stars")) {
      return "<b><font color=\"yellow\" face=\"wingdings\">&#171&#171&#171&#171&#171</font></b>";
    }
  }
  return "N/A";
}
// --------------------------------------------------------------------------------------


// Generate all the global categories (for the combo boxes) given a game descriptor
System::Void MainForm::Generate_Categories(GameDescriptor ^gd) {

  // make list of all unique series
  bool foundmatch = false;
  for each(String^ ss in DBCategories->Series) {
    if (String::Equals(ss, gd->Series)) {
      foundmatch = true;
      break;
    }
  }
  if (!foundmatch) {
    if (!String::Equals("", gd->Series)) {
      DBCategories->Series->AddLast(gd->Series);
    }
  }

  // make list of all unique developers
  foundmatch = false;
  for each(String^ ss in DBCategories->Developer) {
    if (String::Equals(ss, gd->Developer)) {
      foundmatch = true;
      break;
    }
  }
  if (!foundmatch) {
    if (!String::Equals("", gd->Developer)) {
      DBCategories->Developer->AddLast(gd->Developer);
    }
  }

  // make list of all unique genres
  foundmatch = false;
  for each (String ^gs in gd->Genres) {
    for each(String ^ss in DBCategories->Genres) {
      if (String::Equals(gs, ss)) {
        foundmatch = true;
        break;
      }
    }
    if (!foundmatch) {
      if (!String::Equals("", gs)) {
        DBCategories->Genres->Add(gs);
      }
    }
    else {
      foundmatch = false;
    }
  }

  // make a list of all unique publishers
  foundmatch = false;
  for each(String^ ss in DBCategories->Publisher) {
    if (String::Equals(ss, gd->Publisher)) {
      foundmatch = true;
      break;
    }
  }
  if (!foundmatch) {
    if (!String::Equals("", gd->Publisher)) {
      DBCategories->Publisher->AddLast(gd->Publisher);
    }
  }


  // make a list of all unique platforms
  foundmatch = false;
  for each(String^ ss in DBCategories->Platforms) {
    if (String::Equals(ss, gd->PrimaryPlatform)) {
      foundmatch = true;
      break;
    }
  }
  if (!foundmatch) {
    if (!String::Equals("", gd->PrimaryPlatform)) {
      DBCategories->Platforms->Add(gd->PrimaryPlatform);
    }
  }

  foundmatch = false;
  for each (String ^gs in gd->AlternatePlatforms) {
    for each(String ^ss in DBCategories->Platforms) {
      if (String::Equals(gs, ss)) {
        foundmatch = true;
        break;
      }
    }
    if (!foundmatch) {
      if (!String::Equals("", gs)) {
        DBCategories->Platforms->Add(gs);
      }
    }
    else {
      foundmatch = false;
    }
  }

  // generate list of all custom categories
  foundmatch = false;
  for each (String ^gs in gd->CustomCategoryMembership) {
    for each(String ^ss in DBCategories->CustomCategoryMembership) {
      if (String::Equals(gs, ss)) {
        foundmatch = true;
        break;
      }
    }
    if (!foundmatch) {
      if (!String::Equals("", gs)) {
        DBCategories->CustomCategoryMembership->Add(gs);
      }
    }
    else {
      foundmatch = false;
    }
  }

  DBCategories->CustomCategoryMembership->Sort();

  // make a list of all unique locations
  foundmatch = false;
  for each(String^ ss in DBCategories->Location1) {
    if (String::Equals(ss, gd->Location1)) {
      foundmatch = true;
      break;
    }
  }
  if (!foundmatch) {
    if (!String::Equals("", gd->Location1)) {
      DBCategories->Location1->AddLast(gd->Location1);
    }
  }

  // make a list of all unique locations
  foundmatch = false;
  for each(String^ ss in DBCategories->Location2) {
    if (String::Equals(ss, gd->Location2)) {
      foundmatch = true;
      break;
    }
  }
  if (!foundmatch) {
    if (!String::Equals("", gd->Location2)) {
      DBCategories->Location2->AddLast(gd->Location2);
    }
  }

  // make a list of all unique locations
  foundmatch = false;
  for each(String^ ss in DBCategories->Location3) {
    if (String::Equals(ss, gd->Location3)) {
      foundmatch = true;
      break;
    }
  }
  if (!foundmatch) {
    if (!String::Equals("", gd->Location3)) {
      DBCategories->Location3->AddLast(gd->Location3);
    }
  }

  // make a list of all unique locations
  foundmatch = false;
  for each(String^ ss in DBCategories->Location4) {
    if (String::Equals(ss, gd->Location4)) {
      foundmatch = true;
      break;
    }
  }
  if (!foundmatch) {
    if (!String::Equals("", gd->Location4)) {
      DBCategories->Location4->AddLast(gd->Location4);
    }
  }

  // make a list of all unique years
  foundmatch = false;
  for each(String^ ss in DBCategories->Year) {
    if (String::Equals(ss, gd->ReleaseYear)) {
      foundmatch = true;
      break;
    }
  }

  int index = 0;
  if (!foundmatch) {
    if ((DBCategories->Year->Count == 0) && (!String::Equals("", gd->ReleaseYear))) {
      DBCategories->Year->Insert(index, gd->ReleaseYear);
    }
    else {
      for each(String ^ys in DBCategories->Year) {
        if (String::CompareOrdinal(gd->ReleaseYear, ys) >= 0) {
          if (!String::Equals("", gd->ReleaseYear)) {
            break;
          }
        }
        index++;
      }
      DBCategories->Year->Insert(index, gd->ReleaseYear);
    }
  }
}



// ---------------------------  ADDING OR UPDATING GAME FUNCTIONS --------------------------------------
// add a game to the database automatically via mobygames
System::Void MainForm::AddGameAutoButton_Click(System::Object^  sender, System::EventArgs^  e) {
  SimpleTextEntryForm ^aag = gcnew SimpleTextEntryForm();
  aag->Set_Title("Automated Game Addition");
  aag->Set_Label("Name of Game to Add:");

  WaitForm ^wf = gcnew WaitForm();

  if (aag->ShowDialog(this) == ::DialogResult::OK) {
    wf->Show();
    wf->Refresh();

    // gather the list of matching games from moby
    String ^autoinput = aag->Get_Text();
    autoinput = autoinput->Replace("?", "");
    array<Object ^> ^args = gcnew array<Object ^>(1);
    args[0] = autoinput;
    LinkedList<GameMatch ^> ^mml = static_cast< LinkedList<GameMatch ^> ^>(ScrapeGetMatchList->Invoke(GameScraper, args));

    // create the display of matching games
    MobySelectionForm ^msf;
    if (mml == nullptr) {
      MessageBox::Show("Unable to find a matching game.");
    }
    else {
      msf = gcnew MobySelectionForm(mml);

      int matchcnt = 0;
      for each(GameMatch^ mm in mml) {
        String ^ltitle = String::Concat(mm->Name, " - ");
        ltitle = String::Concat(ltitle, mm->PubYear);
        msf->Add_ListItem(ltitle);
        matchcnt++;
        if (matchcnt == MaxSearchMatches) {
          break;
        }
      }

      // BUG Sometimes the wait window does not hide
      wf->Hide();
      wf->Close();

      // determine the matching game
      if (msf->ShowDialog(this) == ::DialogResult::OK) {

        wf = gcnew WaitForm();
        wf->Show();
        wf->Refresh();

        System::String ^st = msf->MobySelectionList->Text;
        GameMatch ^match;
        for each (GameMatch ^mm in mml) {
          String ^ltitle = String::Concat(mm->Name, " - ");
          ltitle = String::Concat(ltitle, mm->PubYear);
          if (String::Equals(ltitle, st)) {
            match = mm;
            break;
          }
        }

        // gather the data from the game webpage and populate the edit window
        CurrentGameDescriptor = gcnew GameDescriptor();
        array<Object ^> ^args = gcnew array<Object ^>(3);
        args[0] = match->URL;
        args[1] = static_cast<Object ^>(CurrentGameDescriptor);
        ScrapeConfig ^sc = gcnew ScrapeConfig();
        sc->NumScreenShots = 3;
        const char *nss = gConfMan->Get_ConfigItem("NumScreenShots");
        if (nss) {
          sc->NumScreenShots = atoi(nss);
        }
        args[2] = sc;
        ScrapeGetFields->Invoke(GameScraper, args);

        GameEditForm ^gef = gcnew GameEditForm(DBCategories);
        gef->Populate_EditForm(CurrentGameDescriptor, nullptr, false);

        wf->Hide();
        wf->Close();

        if (gef->ShowDialog(this) == ::DialogResult::OK) {
			String ^tmppath = String::Concat(Environment::CurrentDirectory, gcnew String("\\tmp\\"));
			DirectoryInfo^ di = gcnew DirectoryInfo(tmppath);
			for each (FileInfo ^file in di->GetFiles()) {
				
				try {
					file->Delete();
				}
				catch (...) {

				}
			}


          MasterList.AddLast(CurrentGameDescriptor);
          CurrentGameDescriptor->Generate_CategoryMembership();
          Generate_Categories(CurrentGameDescriptor);

          ViewModeDropDownButton->Text = gcnew String("Unsorted");

          Populate_UnsortedView();
          Select_GameByName(CurrentGameDescriptor->GameName);
          if (GameList->SelectedNode != nullptr) {
            GameList->SelectedNode->EnsureVisible();
          }

          Modified = true;

          // update game count 
          char buf[100];
          sprintf(buf, "%d", MasterList.Count);
          PGameDBNumGameVal->Text = gcnew String(buf);
        }
      }      
    }
  }

}



// handle then the update button is pressed
System::Void MainForm::MobyUpdateButton_Click(System::Object^  sender, System::EventArgs^  e) {

  // make sure a game is selected
  if (GameList->SelectedNode != nullptr) {
    // do nothing if a category is selected
    String ^nodename = GameList->SelectedNode->Name;
    array<TreeNode ^> ^nodes = GameList->Nodes->Find(nodename, false);
    if (nodes->Length != 0) {
      return;
    }

    WaitForm ^wf = gcnew WaitForm();
    wf->Show();
    wf->Refresh();

    // generate list of possible matches from web
    System::String ^gamename = GameList->SelectedNode->Text;
    gamename = gamename->Replace("?", "");
    array<Object ^> ^args = gcnew array<Object ^>(1);
    args[0] = gamename;
    LinkedList<GameMatch ^> ^mml = static_cast< LinkedList<GameMatch ^> ^>(ScrapeGetMatchList->Invoke(GameScraper, args));

    // format the match selection screen
    MobySelectionForm ^msf;
    if (mml == nullptr) {
      MessageBox::Show("Unable to find a matching game.");
    }
    else {
      msf = gcnew MobySelectionForm(mml);

      int matchcnt = 0;
      for each(GameMatch^ mm in mml) {
        String ^ltitle = String::Concat(mm->Name, " - ");
        ltitle = String::Concat(ltitle, mm->PubYear);
        msf->Add_ListItem(ltitle);
        matchcnt++;
        if (matchcnt == 50) {
          break;
        }
      }

      // HACK as wait window doesn't always dissapear
      wf->Hide();
      wf->Hide();
      wf->Close();

      // select the game to update from
      if (msf->ShowDialog(this) == ::DialogResult::OK) {
        wf = gcnew WaitForm();
        wf->Show();
        wf->Refresh();

        System::String ^st = msf->MobySelectionList->Text;
        GameMatch ^match;
        for each (GameMatch ^mm in mml) {
          String ^ltitle = String::Concat(mm->Name, " - ");
          ltitle = String::Concat(ltitle, mm->PubYear);
          if (String::Equals(ltitle, st)) {
            match = mm;
            break;
          }
        }

        // update the game data from the scraper
        CurrentGameDescriptor = cli::safe_cast<GameDescriptor ^>(GameList->SelectedNode->Tag);
        String ^oldname = nodename = CurrentGameDescriptor->GameName;
        GameDescriptor ^oeg = gcnew GameDescriptor(CurrentGameDescriptor);
        array<Object ^> ^args = gcnew array<Object ^>(3);
        args[0] = match->URL;
        args[1] = static_cast<Object ^>(CurrentGameDescriptor);
        ScrapeConfig ^sc = gcnew ScrapeConfig();
        sc->NumScreenShots = 3;
        const char *nss = gConfMan->Get_ConfigItem("NumScreenShots");
        if (nss) {
          sc->NumScreenShots = atoi(nss);
        }
        args[2] = sc;
        ScrapeGetFields->Invoke(GameScraper, args);

        // populate the edit window
        GameEditForm ^gef = gcnew GameEditForm(DBCategories);
        gef->Populate_EditForm(CurrentGameDescriptor, oeg, false);
        wf->Close();


        // pop-up the edit window with the new data
        if (gef->ShowDialog() == ::DialogResult::OK) {
			String ^tmppath = String::Concat(Environment::CurrentDirectory, gcnew String("\\tmp\\"));
			DirectoryInfo^ di = gcnew DirectoryInfo(tmppath);
			for each (FileInfo ^file in di->GetFiles()) {
				try {
					file->Delete();
				}
				catch (...) {

				}
			}

          // after updating the game display it and generate new category info
          GameList_AfterSelect(nullptr, nullptr);
          CurrentGameDescriptor->Generate_CategoryMembership();
          Generate_Categories(CurrentGameDescriptor);
          Modified = true;

          // if the user changed the game name we need to resort in custom view, category view, and unsorted view
         if (String::Compare(oldname, CurrentGameDescriptor->GameName) != 0) {
           Display_Games();           
           Select_GameByName(CurrentGameDescriptor->GameName);

            // HACK - Used to keep the selected from jumping,
            ChangedName = true;
          }
        }
      }
    }
    wf->Hide();
    wf->Hide();
    wf->Close();
  }
}


// add game data manually when button pressed
Void MainForm::ManualAddGameButton_Click(System::Object^  sender, System::EventArgs^  e) {

  // populate the edit window with empty data
  GameEditForm ^gef = gcnew GameEditForm(DBCategories);
  GameDescriptor ^gd = gcnew GameDescriptor;
  gef->Populate_EditForm(gd, nullptr, false);

  // open the edit window
  if (gef->ShowDialog() == ::DialogResult::OK) {
	  String ^tmppath = String::Concat(Environment::CurrentDirectory, gcnew String("\\tmp\\"));
	  DirectoryInfo^ di = gcnew DirectoryInfo(tmppath);
	  for each (FileInfo ^file in di->GetFiles()) {
		  try {
			  file->Delete();
		  }
		  catch (...) {

		  }
	  }

    bool inserted = false;
    MasterList.AddLast(gd);
    gd->Generate_CategoryMembership();
    Generate_Categories(gd);

    Populate_UnsortedView();
    Select_GameByName(gd->GameName);
    if (GameList->SelectedNode != nullptr) {
      GameList->SelectedNode->EnsureVisible();
    }
    Modified = true;

    // update number of games in db
    char buf[100];
    sprintf(buf, "%d", MasterList.Count);
    PGameDBNumGameVal->Text = gcnew String(buf);

    ViewModeDropDownButton->Text = "Unsorted";
  }
}

// handle double clicking a game by opening the edit window
System::Void MainForm::GameList_DoubleClick(System::Object^  sender, System::EventArgs^  e) {

  // if there is no selected game just exit
  if (GameList->SelectedNode == nullptr) {
    return;
  }

  // make sure we don't do anything if a category was selected
  String ^nodename = GameList->SelectedNode->Name;
  array<TreeNode ^> ^nodes = GameList->Nodes->Find(nodename, false);
  if (nodes->Length != 0) {
    return;
  }

  // otherwise create the edit window
  GameEditForm ^gef = gcnew GameEditForm(DBCategories);
  GameDescriptor ^gd = safe_cast<GameDescriptor^>(GameList->SelectedNode->Tag);
  String ^oldname = nodename = gd->GameName;
  GameDescriptor ^oldgamed = gcnew GameDescriptor(gd);
  gef->Populate_EditForm(gd, oldgamed, false);
  if (gef->ShowDialog() == ::DialogResult::OK) {
	  String ^tmppath = String::Concat(Environment::CurrentDirectory, gcnew String("\\tmp\\"));
	  DirectoryInfo^ di = gcnew DirectoryInfo(tmppath);
	  for each (FileInfo ^file in di->GetFiles()) {
		  try {
			  file->Delete();
		  }
		  catch (...) {

		  }
	  }

    GameList_AfterSelect(nullptr, nullptr);
    gd->Generate_CategoryMembership();
    Generate_Categories(gd);
    Modified = true;

    // if the user changed the game name we need to resort;
    if (String::Compare(oldname, gd->GameName) != 0) {
      Display_Games();
      Select_GameByName(gd->GameName);

      // HACK - Used to keep the selected game from jumping,
      ChangedName = true;
    }
  }
}


// ----------------------------------------------------------------------------------------




// ------------------------------ View handling -----------------------------------

// Create the master game list
System::Void MainForm::Populate_UnsortedView(void) {
  int gamecnt = 0;

  GameList->BeginUpdate();

  GameList->Nodes->Clear();

  LinkedList<TreeNode ^> ^cl = gcnew LinkedList<TreeNode ^>();
  TreeNode ^node = GameList->Nodes->Add("Unsorted");
  node->Name = "Unsorted";
  cl->AddLast(node);

  for each (GameDescriptor ^gd in MasterList) {

    bool passedfilters = true;

    if (PlatformFilterOn) {
      if (!Is_GameOnPlatform(gd)) {
        passedfilters = false;
      }
    }
    if (passedfilters && GenreFilterOn) {
      if (!Is_GameInGenre(gd)) {
        passedfilters = false;
      }
    }
    if (passedfilters && YearFilterOn) {
      if (!Is_GameInYear(gd)) {
        passedfilters = false;
      }
    }
    if (passedfilters && TextFilterOn) {
      if (!Does_GameHaveText(gd)) {
        passedfilters = false;
      }
    }
    if (passedfilters && PlayedFilterOn) {
      if (!Is_GamePlayed(gd)) {
        passedfilters = false;
      }
    }

    if (passedfilters) {
      Add_GameToGameList(node, gd);
      gamecnt++;
    }
  }
  node->Expand();
  GameList->EndUpdate();

  // update game count 
  char buf[100];
  sprintf(buf, "%d", gamecnt);
  PGameDBNumGameVal->Text = gcnew String(buf);
}

// populate the treeview based on the currently selected category
System::Void MainForm::Populate_CategoryView(void) {
  String ^catname;
  TreeNode ^node;
  String ^curnode = nullptr;
  int gamecnt = 0;

  if (GameList->SelectedNode != nullptr) {
    curnode  = GameList->SelectedNode->Name;
  }

  GameList->BeginUpdate();
  GameList->Nodes->Clear();
  catname = SelectedCategory;

  // make the category names prettier
  catname = catname->Replace("releaseyear:", "Release Year: ");
  catname = catname->Replace("genre:", "Genre: ");
  catname = catname->Replace("developer:", "Developer: ");
  catname = catname->Replace("publisher:", "Publisher: ");
  catname = catname->Replace("location:", "Location: ");
  catname = catname->Replace("completedyear:", "Completed Year: ");
  catname = catname->Replace("completed:", "Completed: ");
  catname = catname->Replace("releaseyear:", "Release Year: ");
  catname = catname->Replace("series:", "Series: ");
  catname = catname->Replace("platform:", "Platform: ");
  catname = catname->Replace("rating:", "Rating: ");
  catname = catname->Replace("custom:", "Group: ");

  node = GameList->Nodes->Add(catname);
  node->Name = catname;

  for each (GameDescriptor ^gd in MasterList) {

    // if the game is in the active category add it to the list
    if (Is_GameInCategory(gd, SelectedCategory)) {

      bool passedfilters = true;

      if (PlatformFilterOn) {
        if (!Is_GameOnPlatform(gd)) {
          passedfilters = false;
        }
      }
      if (passedfilters && GenreFilterOn) {
        if (!Is_GameInGenre(gd)) {
          passedfilters = false;
        }
      }
      if (passedfilters && YearFilterOn) {
        if (!Is_GameInYear(gd)) {
          passedfilters = false;
        }
      }
      if (passedfilters && TextFilterOn) {
        if (!Does_GameHaveText(gd)) {
          passedfilters = false;
        }
      }
      if (passedfilters && PlayedFilterOn) {
        if (!Is_GamePlayed(gd)) {
          passedfilters = false;
        }
      }

      if (passedfilters) {
        Add_GameToGameList(node, gd);
        gamecnt++;
      }
    }
  }
  node->Expand();

  // try to ensure the correct game is highlighted
  if (curnode != nullptr) {
    Select_GameByName(curnode);
  }
  GameList->EndUpdate();
  DoneLoadingGamePage = true;

  // update game count 
  char buf[100];
  sprintf(buf, "%d", gamecnt);
  PGameDBNumGameVal->Text = gcnew String(buf);
}


// populate the treeview based on the custom categories selected by the user
System::Void MainForm::Populate_CustomView(void) {
  TreeNode ^node;
  String ^catname;
  int gamecnt = 0;

  GameList->BeginUpdate();
  GameList->Nodes->Clear();

  // foe each custom category
  for each (String ^cat in CustomViewCategories) {

    catname = cat->Replace("releaseyear:", "Release Year: ");
    catname = catname->Replace("genre:", "Genre: ");
    catname = catname->Replace("developer:", "Developer: ");
    catname = catname->Replace("publisher:", "Publisher: ");
    catname = catname->Replace("location:", "Location: ");
    catname = catname->Replace("completedyear:", "Completed Year: ");
    catname = catname->Replace("completed:", "Completed: ");
    catname = catname->Replace("releaseyear:", "Release Year: ");
    catname = catname->Replace("series:", "Series: ");
    catname = catname->Replace("platform:", "Platform: ");
    catname = catname->Replace("rating:", "Rating: ");
    catname = catname->Replace("custom:", "Group: ");

    node = GameList->Nodes->Add(catname);
    node->Name = catname;

    // if a game is in that category add it to that category
    for each (GameDescriptor ^gd in MasterList) {
      if (Is_GameInCategory(gd, cat)) {
        bool passedfilters = true;

        if (PlatformFilterOn) {
          if (!Is_GameOnPlatform(gd)) {
            passedfilters = false;
          }
        }
        if (passedfilters && GenreFilterOn) {
          if (!Is_GameInGenre(gd)) {
            passedfilters = false;
          }
        }
        if (passedfilters && YearFilterOn) {
          if (!Is_GameInYear(gd)) {
            passedfilters = false;
          }
        }
        if (passedfilters && TextFilterOn) {
          if (!Does_GameHaveText(gd)) {
            passedfilters = false;
          }
        }
        if (passedfilters && PlayedFilterOn) {
          if (!Is_GamePlayed(gd)) {
            passedfilters = false;
          }
        }

        if (passedfilters) {
          Add_GameToGameList(node, gd);
        }
      }
    }
  }
  GameList->EndUpdate();

  // update game count 
  char buf[100];
  sprintf(buf, "%d", gamecnt);
  PGameDBNumGameVal->Text = gcnew String(buf);
}

// Add a game to the treeview in the proper format
System::Void MainForm::Add_GameToGameList(TreeNode ^tn, GameDescriptor ^gd) {

  TreeNode ^gn;
  String ^thestr;
  String ^astr;

  // sort game names as 'name, the' or 'name, a' if appropriate
  if (gd->GameName->Length >= 2) {
    astr = gd->GameName->Substring(0,2);
  }
  if (gd->GameName->Length >= 4) {
    thestr = gd->GameName->Substring(0, 4);
  }
  else {
    thestr = gd->GameName;
  }

  // if the game starts with 'the' or 'a' put it at the end
  if (String::Equals(astr, "A ")) {
    astr = gd->GameName->Substring(2);
    astr = String::Concat(astr, ", A");
    gn = tn->Nodes->Add(astr);
    gn->Name = gd->GameName;
  }
  else if (String::Equals(thestr, "The ")) {
    thestr = gd->GameName->Substring(4);
    thestr = String::Concat(thestr, ", The");
    gn = tn->Nodes->Add(thestr);
    gn->Name = gd->GameName;
  }
  else {
    gn = tn->Nodes->Add(gd->GameName);
    gn->Name = gd->GameName;
  }

  gn->Tag = gd;
}



// check to see if a game is in the given category
bool MainForm::Is_GameInCategory(GameDescriptor ^gd, String ^cat) {

  // if we are looking for membership to a platform, check for membership to the list of all devices that belong to that platform
  if (String::Equals(cat, "Platform:PC")) {
    for each (String ^cats in PCPlatforms) {
      for each (String ^s in gd->CategoryMembership) {
        if (String::Equals(cats, s, System::StringComparison::OrdinalIgnoreCase)) {
          return true;
        }
      }
    }
    return false;
  }
  if (String::Equals(cat, "Platform:Console")) {
    for each (String ^cats in ConsolePlatforms) {
      for each (String ^s in gd->CategoryMembership) {
        if (String::Equals(cats, s, System::StringComparison::OrdinalIgnoreCase)) {
          return true;
        }
      }
    }
    return false;
  }
  if (String::Equals(cat, "Platform:Handheld")) {
    for each (String ^cats in HandheldPlatforms) {
      for each (String ^s in gd->CategoryMembership) {
        if (String::Equals(cats, s, System::StringComparison::OrdinalIgnoreCase)) {
          return true;
        }
      }
    }
    return false;
  }
  if (String::Equals(cat, "Platform:Other")) {
    for each (String ^cats in OtherPlatforms) {
      for each (String ^s in gd->CategoryMembership) {
        if (String::Equals(cats, s, System::StringComparison::OrdinalIgnoreCase)) {
          return true;
        }
      }
    }
    return false;
  }

  // now check other category memberships and custom category memberships
  for each (String ^s in gd->CategoryMembership) {
    if (String::Equals(s, cat, System::StringComparison::OrdinalIgnoreCase)) {
      return true;
    }
  }
  for each (String ^s in gd->CustomCategoryMembership) {
    if (String::Equals(s, cat, System::StringComparison::OrdinalIgnoreCase)) {
      return true;
    }
  }

  return false;
}

bool MainForm::Is_GameOnPlatform(GameDescriptor ^gd) {
  String ^cat = String::Concat("Platform:", PlatformFilter->Text);
  return Is_GameInCategory(gd, cat);
}

bool MainForm::Is_GameInGenre(GameDescriptor ^gd) {
  String ^cat = String::Concat("genre:", GenreFilter->Text);
  return Is_GameInCategory(gd, cat);
}

bool MainForm::Is_GameInYear(GameDescriptor ^gd) {
  String ^cat = String::Concat("releaseyear:", YearFilter->Text);
  return Is_GameInCategory(gd, cat);
}

bool MainForm::Is_GamePlayed(GameDescriptor ^gd) {
  if (String::Equals(PlayedFilter->Text, "No")) {

    // no value indicated not played
    if ((gd->Completed == nullptr) || (gd->Completed->Length == 0)) {
      return true;
    }
    String ^cat = String::Concat("Completed:", PlayedFilter->Text);
    return Is_GameInCategory(gd, cat);
  }

  if ((gd->Completed == nullptr) || (gd->Completed->Length == 0)) {
    return false;
  }
  String ^cat = String::Concat("Completed:", "No");
  bool unplayed = Is_GameInCategory(gd, cat);
  if (unplayed) {
    return false;
  }
  return true;
}

bool MainForm::Does_GameHaveText(GameDescriptor ^gd) {
  bool matched = false;
  String ^searchstr = TextFilter->Text;

  if (gd->GameName->IndexOf(searchstr, System::StringComparison::OrdinalIgnoreCase) != -1) {
    matched = true;
  }
  else if (gd->Publisher->IndexOf(searchstr, System::StringComparison::OrdinalIgnoreCase) != -1) {
    matched = true;
  }
  else if (gd->Developer->IndexOf(searchstr, System::StringComparison::OrdinalIgnoreCase) != -1) {
    matched = true;
  }
  else if (gd->Description->IndexOf(searchstr, System::StringComparison::OrdinalIgnoreCase) != -1) {
    matched = true;
  }
  else if (gd->ReleaseYear->IndexOf(searchstr, System::StringComparison::OrdinalIgnoreCase) != -1) {
    matched = true;
  }
  else if (gd->Series->IndexOf(searchstr, System::StringComparison::OrdinalIgnoreCase) != -1) {
    matched = true;
  }
  else if (gd->PrimaryPlatform->IndexOf(searchstr, System::StringComparison::OrdinalIgnoreCase) != -1) {
    matched = true;
  }
  else if (gd->Location1->IndexOf(searchstr, System::StringComparison::OrdinalIgnoreCase) != -1) {
    matched = true;
  }	
  else if (gd->Location2->IndexOf(searchstr, System::StringComparison::OrdinalIgnoreCase) != -1) {
    matched = true;
  }
  else if (gd->Location3->IndexOf(searchstr, System::StringComparison::OrdinalIgnoreCase) != -1) {
    matched = true;
  }
  else if (gd->Location4->IndexOf(searchstr, System::StringComparison::OrdinalIgnoreCase) != -1) {
    matched = true;
  }
  else if (gd->Notes->IndexOf(searchstr, System::StringComparison::OrdinalIgnoreCase) != -1) {
    matched = true;
  }
  else {

    for each (String ^s in gd->Genres) {
      if (s->IndexOf(searchstr, System::StringComparison::OrdinalIgnoreCase) != -1) {
        matched = true;
      }
    }

    for each (String ^s in gd->AlternatePlatforms) {
      if (s->IndexOf(searchstr, System::StringComparison::OrdinalIgnoreCase) != -1) {
        matched = true;
      }
    }
  }

  return matched;
}

// ----------------------------------------------------------------------------





// -------------------- PLATFORM FILTERING FUNCTIONS -------------------------------

// load each of the platform files to determine which devices belong to that platform and generate the menu options for selecting that device
System::Void MainForm::Load_Platforms(void) {
  String ^pf = "PCPlatforms.txt";

  // load the pc platforms
  if (!File::Exists(pf)) {
    CurrentOpenDB = nullptr;
    MessageBox::Show("Error: platform file not found: " + pf);
    return;
  }
  StreamReader ^sr = gcnew StreamReader(pf);

  while(!sr->EndOfStream) {
    String ^plat = sr->ReadLine();
    if (plat->Length != 0) {
      PCPlatforms.AddLast("Platform:" + plat);

      ToolStripMenuItem ^tsi = gcnew ToolStripMenuItem();
      tsi->Name = plat;
      tsi->Text = plat;
      tsi->Click += gcnew System::EventHandler(this, &MainForm::PlatformFilter_Click);
      FilterPlatformPC->DropDownItems->Add(tsi);
    }
  }
  sr->Close();

  // load the console platforms
  pf = "ConsolePlatforms.txt";
  if (!File::Exists(pf)) {
    CurrentOpenDB = nullptr;
    MessageBox::Show("Error: platform file not found: " + pf);
    return;
  }
  sr = gcnew StreamReader(pf);

  while(!sr->EndOfStream) {
    String ^plat = sr->ReadLine();
    if (plat->Length != 0) {
      ConsolePlatforms.AddLast("Platform:" + plat);

      ToolStripMenuItem ^tsi = gcnew ToolStripMenuItem();
      tsi->Name = plat;
      tsi->Text = plat;
      tsi->Click += gcnew System::EventHandler(this, &MainForm::PlatformFilter_Click);
      FilterPlatformConsole->DropDownItems->Add(tsi);
    }
  }
  sr->Close();


  // load the handheld platforms
  pf = "HandheldPlatforms.txt";
  if (!File::Exists(pf)) {
    CurrentOpenDB = nullptr;
    MessageBox::Show("Error: platform file not found: " + pf);
    return;
  }
  sr = gcnew StreamReader(pf);

  while(!sr->EndOfStream) {
    String ^plat = sr->ReadLine();
    if (plat->Length != 0) {
      HandheldPlatforms.AddLast("Platform:" + plat);

      ToolStripMenuItem ^tsi = gcnew ToolStripMenuItem();
      tsi->Name = plat;
      tsi->Text = plat;
      tsi->Click += gcnew System::EventHandler(this, &MainForm::PlatformFilter_Click);
      FilterPlatformHandheld->DropDownItems->Add(tsi);
    }
  }
  sr->Close();

  // load the other platforms
  pf = "OtherPlatforms.txt";
  if (!File::Exists(pf)) {
    CurrentOpenDB = nullptr;
    MessageBox::Show("Error: platform file not found: " + pf);
    return;
  }
  sr = gcnew StreamReader(pf);

  while(!sr->EndOfStream) {
    String ^plat = sr->ReadLine();
    if (plat->Length != 0) {
      OtherPlatforms.AddLast("Platform:" + plat);

      ToolStripMenuItem ^tsi = gcnew ToolStripMenuItem();
      tsi->Name = plat;
      tsi->Text = plat;
      tsi->Click += gcnew System::EventHandler(this, &MainForm::PlatformFilter_Click);
      FilterPlatformOther->DropDownItems->Add(tsi);
    }
  }
  sr->Close();

}

System::Void MainForm::PlatformFilter_Click(System::Object^  sender, System::EventArgs^  e) {
  String ^nodename;
  if (GameList->SelectedNode != nullptr) {
    nodename = GameList->SelectedNode->Name;
  }
  else {
    nodename = nullptr;
  }

  ToolStripMenuItem ^tsi = safe_cast<ToolStripMenuItem ^>(sender);
  PlatformFilter->Text = tsi->Text;

  // set the platform filter variable
  if (String::Equals(tsi->Text, "All")) {
    PlatformFilterOn = false;
  }
  else {
    PlatformFilterOn = true;
  }

  // now show filter the desired view based on the platform
  Display_Games();
  Select_GameByName(nodename);
}


// Fills the drop down genre filter toolbar list with all the current genres
System::Void MainForm::Load_GenreFilterDropDown(void) {
  GenreFilter->DropDownItems->Clear();

  ToolStripMenuItem ^tsi = gcnew ToolStripMenuItem();
  tsi->Name = "All";
  tsi->Text = "All";
  tsi->Click += gcnew System::EventHandler(this, &MainForm::GenreFilter_Click);
  GenreFilter->DropDownItems->Add(tsi);

  for each (String ^s in DBCategories->Genres) {

    tsi = gcnew System::Windows::Forms::ToolStripMenuItem();
    tsi->Name = s;
    tsi->Text = s;
    tsi->Click += gcnew System::EventHandler(this, &MainForm::GenreFilter_Click);
    GenreFilter->DropDownItems->Add(tsi);
  }
}

// Fills the drop down genre filter toolbar list with all the current genres
System::Void MainForm::Load_YearFilterDropDown(void) {
  YearFilter->DropDownItems->Clear();

  ToolStripMenuItem ^tsi = gcnew ToolStripMenuItem();
  tsi->Name = "All";
  tsi->Text = "All";
  tsi->Click += gcnew System::EventHandler(this, &MainForm::YearFilter_Click);
  YearFilter->DropDownItems->Add(tsi);

  for each (String ^s in DBCategories->Year) {

    tsi = gcnew System::Windows::Forms::ToolStripMenuItem();
    tsi->Name = s;
    tsi->Text = s;
    tsi->Click += gcnew System::EventHandler(this, &MainForm::YearFilter_Click);
    YearFilter->DropDownItems->Add(tsi);
  }
}

// switches to a category view based on a particular genre
System::Void MainForm::YearFilter_Click(System::Object^  sender, System::EventArgs^  e) {

  ToolStripMenuItem ^tsi = safe_cast<ToolStripMenuItem ^>(sender);
  YearFilter->Text = tsi->Name;

  // switch to unsorted view if all was selected
  if (String::Equals(tsi->Name, "All")) {
    YearFilterOn = false;
  }
  // switch to the appropriate category view if anything else was selected
  else {
    YearFilterOn = true;
  }

  // now show filter the desired view
  Display_Games();
}



// switches the genre based filter
System::Void MainForm::GenreFilter_Click(System::Object^  sender, System::EventArgs^  e) {

  ToolStripMenuItem ^tsi = safe_cast<ToolStripMenuItem ^>(sender);
  GenreFilter->Text = tsi->Name;

  // switch to unsorted view if all was selected
  if (String::Equals(tsi->Name, "All")) {
    GenreFilterOn = false;
  }
  // switch to the appropriate category view if anything else was selected
  else {
    GenreFilterOn = true;
  }

   // now show filter the desired view
   Display_Games();
}

// handle the unplayed filter drop down
System::Void MainForm::FilterUnplayed_Click(System::Object^  sender, System::EventArgs^  e) {

  ToolStripMenuItem ^tsi = safe_cast<ToolStripMenuItem ^>(sender);
  PlayedFilter->Text = tsi->Text;

  // switch to unsorted view if all was selected
  if (String::Equals(PlayedFilter->Text, "All")) {
    PlayedFilterOn = false;
  }
  // switch to the appropriate category view if anything else was selected
  else {
    PlayedFilterOn = true;
  }

  // now show filter the desired view
  Display_Games();
}

// clear all filters when button is pressed
System::Void MainForm::ClearFiltersButton_Click(System::Object^  sender, System::EventArgs^  e) {
  GenreFilter->Text = "All";
  GenreFilterOn = false;

  PlatformFilter->Text = "All";
  PlatformFilterOn = false;

  YearFilter->Text = "All";
  YearFilterOn = false;

  TextFilter->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
  TextFilter->Text = "Search";
  TextFilterOn = false;

  PlayedFilter->Text = "All";
  PlayedFilterOn = false;

  Display_Games();
}

//-----------------------------------------------------------------------------------



// select a random game in the current category or in the default category (top category)
System::Void MainForm::RandomButton_Click(System::Object^  sender, System::EventArgs^  e) {

  TreeNode ^curnode = GameList->SelectedNode;
  if (curnode == nullptr) {
    curnode = GameList->Nodes[0];
  }
  else {
    if (curnode->Parent != nullptr) {
      curnode = curnode->Parent;
    }
  }

  int numgames = curnode->Nodes->Count;

  // if there are no games in the category just return
  if (numgames == 0) {
    return;
  }
  int selectedgame = RandNumGen->Next();
  selectedgame = selectedgame % numgames;

  Select_GameByName(curnode->Nodes[selectedgame]->Name);
}



// handle popping up the preference menu
System::Void MainForm::PGDB_EditMenuPreferences_Click(System::Object^  sender, System::EventArgs^  e) {
  String ^oldscraper;

  PGDBPrefrencesForm ^pef = gcnew PGDBPrefrencesForm();

  const char *scraper = gConfMan->Get_ConfigItem("Scraper");
  if (scraper) {
    oldscraper = gcnew String(scraper);
  }

  if (pef->ShowDialog() == ::DialogResult::OK) {

    // reload the scraper library if it changed
    scraper = gConfMan->Get_ConfigItem("Scraper");
    String ^newscraper = gcnew String(scraper);
    if (!String::Equals(oldscraper, newscraper)) {
      if (!File::Exists(newscraper)) {
        MessageBox::Show(String::Concat("Fatal Error: Cannot find scraper library: ", newscraper));
        MessageBox::Show("Change the Scraper value in PGameDB.cfg to a valid library (e.g.  MobyScraper.dll).");
        exit(1);
      }

      String ^pth = String::Concat(Directory::GetCurrentDirectory(), "\\");
      pth = String::Concat(pth, newscraper);
      ScrapeAssem = Assembly::LoadFrom(pth);
      ScrapeType = ScrapeAssem->GetType("Scraper");
      ScrapeGetFields = ScrapeType->GetMethod("Get_Fields");
      ScrapeGetMatchList = ScrapeType->GetMethod("Get_MatchList");
      GameScraper = Activator::CreateInstance(ScrapeType);
    }

    // reread the maxsearch result parameter
    const char *msr = gConfMan->Get_ConfigItem("MaxSearchResults");
    if (msr != NULL) {
      MaxSearchMatches = atoi(msr);
    }
    else {
      MaxSearchMatches = 30;
    }
  }
}



// handle closing the program
System::Void MainForm::MainFormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {

  // if modifications have been made without saving, prompt to save
  if (Modified) {
    SaveOnExitDialog ^soed = gcnew SaveOnExitDialog();
    ::DialogResult dr = soed->ShowDialog(this);

    if (dr == ::DialogResult::Yes) {
      SaveAsMenuItem_Click(sender, e);
    }
    else if (dr == ::DialogResult::OK) {
      PGDB_FileMenuSave_Click(sender, e);
    }
    else if (dr == ::DialogResult::Cancel) {
      // do nothing
    }
  }
}

// show the proper list of games based on the viewing mode
System::Void MainForm::Display_Games(void) {
  if (String::Equals(ViewModeDropDownButton->Text,  "Custom")) {
    Populate_CustomView();
  }
  else if (String::Equals(ViewModeDropDownButton->Text,  "Category")) {
    Populate_CategoryView();
  }
  else {
    Populate_UnsortedView();
  }
}