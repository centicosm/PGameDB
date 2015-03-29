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


// Implementation of the game edit form functionality

#include "GameEditForm.h"
#include "EditListForm.h"
#include "MobyWebForm.h"
#include "AddDocForm.h"

using namespace System;
using namespace System::IO;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace PGameDB;

// View the webpage associated with the game
System::Void GameEditForm::ViewWebPageButton_Click(System::Object^  sender, System::EventArgs^  e) {
  if (!String::Equals(URLTextBox->Text, "")) {
    MobyWebForm ^mwf = gcnew MobyWebForm(URLTextBox->Text);
    mwf->Show();
  }
  else {
    MessageBox::Show("No URL associated with game.");
  }
}


// update the game data via moby games upon clicking on the update button
System::Void GameEditForm::GameEditUpdateButton_Click(System::Object^  sender, System::EventArgs^  e) {

  // erase all files in the temp directory whenever we update
  String ^tmppath = String::Concat(Environment::CurrentDirectory, gcnew String("\\tmp\\"));
  array<String^> ^filelist = Directory::GetFiles(tmppath);
  for each (String ^s in filelist) {
    File::Delete(s);
  }

  WaitForm ^wf = gcnew WaitForm();
  wf->Show();
  wf->Refresh();

  // base the search on the title of the game
  String ^autoinput = TitleTextBox->Text;
  autoinput = autoinput->Replace("?", "");

  // retrieve the list of matching search results
  array<Object ^> ^args = gcnew array<Object ^>(1);
  args[0] = autoinput;
  LinkedList<GameMatch ^> ^MobyMatchList = static_cast< LinkedList<GameMatch ^> ^>(ScrapeGetMatchList->Invoke(GameScraper, args));

  wf->Hide();
  MobySelectionForm ^msf;

  // create the list of games that match the search
  if (MobyMatchList == nullptr) {
    MessageBox::Show("Unable to find a matching game.");
  }
  else {
    msf = gcnew MobySelectionForm(MobyMatchList);

    int matchcnt = 0;
    for each(GameMatch^ mm in MobyMatchList) {
      String ^ltitle = String::Concat(mm->Name, " - ");
      ltitle = String::Concat(ltitle, mm->PubYear);
      msf->Add_ListItem(ltitle);
      matchcnt++;
      if (matchcnt == MaxSearchMatches) {
        break;
      }
    }

    // select the search result to use
    if (msf->ShowDialog(this) == ::DialogResult::OK) {
      System::String ^st = msf->MobySelectionList->Text;
      GameMatch ^match;
      for each (GameMatch ^mm in MobyMatchList) {
        String ^ltitle = String::Concat(mm->Name, " - ");
        ltitle = String::Concat(ltitle, mm->PubYear);
        if (String::Equals(ltitle, st)) {
          match = mm;
          break;
        }
      }

      // parse the moby games page for the selected game and populate the edit window
      wf->Show();
      wf->Refresh();

      GameDescriptor ^ngd = gcnew GameDescriptor(CurrentEditGame);

      // call our dll to scrape the game data
      array<Object ^> ^args = gcnew array<Object ^>(3);
      args[0] = match->URL;
      args[1] = static_cast<Object ^>(CurrentEditGame);
      ScrapeConfig ^sc = gcnew ScrapeConfig();
      sc->NumScreenShots = 3;
      const char *nss = gConfMan->Get_ConfigItem("NumScreenShots");
      if (nss) {
        sc->NumScreenShots = atoi(nss);
      }
      args[2] = sc;
      ScrapeGetFields->Invoke(GameScraper, args);

      // and populate the form with the data scraped
      Populate_EditForm(CurrentEditGame, ngd, true);
      wf->Close();
      Refresh();
    }
  }
}



// remove a screen shot on click of remove button
System::Void GameEditForm::RemoveScreenShotButton_Click(System::Object^  sender, System::EventArgs^  e) {
  if (ScreenShotListBox->SelectedItem != nullptr) {
    String ^txt = ScreenShotListBox->SelectedItem->ToString();

    // remove the file from the temp directory
    String ^tmppath = String::Concat(Environment::CurrentDirectory, gcnew String("\\tmp\\"));
    String ^path = String::Concat(tmppath, txt);
    if (File::Exists(path)) {
      File::Delete(path);
    }

    CurrentEditGame->Screenshots->Remove(txt);
    ScreenShotListBox->Items->Remove(txt);
    GameEditTitle_Click(nullptr, nullptr);
  }
}

// choose the front cover image upon button click
// TODO: We need to rename a copy of the file and move it to the temp directory
System::Void GameEditForm::ChooseFrontCoverButton_Click(System::Object^  sender, System::EventArgs^  e) {
  String ^imgfile = Select_ImageFile();
  if (imgfile != nullptr) {
    FrontCoverTextBox->Text = imgfile;
  }
}


// function to select the back cover of the game when the choose button is clicked
// TODO: We need to rename a copy of the file and move it to the temp directory
System::Void GameEditForm::ChooseBackCoverButton_Click(System::Object^  sender, System::EventArgs^  e) {
  String ^imgfile = Select_ImageFile();
  if (imgfile != nullptr) {
    BackCoverTextBox->Text = imgfile;
  }
}


// add a screen shot to the game descriptor upon button click
// TODO: We need to rename a copy of the file and move it to the temp directory
System::Void GameEditForm::AddScreenShotButton_Click(System::Object^  sender, System::EventArgs^  e) {
  String ^imgfile = Select_ImageFile();
  if (imgfile != nullptr) {
    ScreenShotListBox->Items->Add(imgfile);
    ScreenShotListBox->Refresh();
  }
}


// select an image file from the data path
String ^GameEditForm::Select_ImageFile(void) {
  String ^imgfile;
  OpenFileDialog^ ofd = gcnew OpenFileDialog;

  // determine the data path from the config file
  const char *datapath = gConfMan->Get_ConfigItem("ShotPath");
  if (!datapath) {
    MessageBox::Show("Error: Configuration file corrupted!");
  }
  String ^dp = gcnew String(datapath);

  dp = dp->Replace("/", "\\");      
  dp = String::Concat(Environment::CurrentDirectory, dp);
  ofd->InitialDirectory = dp;
  ofd->Filter = "image files (*.gif, *.jpg, *.png)|*.gif;*.jpg;*.png";
  ofd->FilterIndex = 1;
  ofd->RestoreDirectory = true;

  // open a file selection dialog to choose the image
  if (ofd->ShowDialog() == ::DialogResult::OK) {
    String ^fullimg = ofd->FileName;
    GameEditPicture->Load(fullimg);
    GameEditPicture->Refresh();
    fullimg = fullimg->Remove(0, Environment::CurrentDirectory->Length);
    int startindex = fullimg->IndexOf("\\");
    int stopindex = fullimg->IndexOf("\\", startindex + 1);
    fullimg = fullimg->Remove(0, stopindex + 1);
    imgfile = fullimg;
  }
  else {
    imgfile = nullptr;
  }

  return imgfile;
}



// ---- Functions used to display cover shots and screen shots in the edit window

//handle displaying front cover image if the filename is clicked
System::Void GameEditForm::GameEditTitle_Click(System::Object^  sender, System::EventArgs^  e) {
  if (FrontCoverTextBox->Text != nullptr) {
    Display_GameImage(FrontCoverTextBox->Text);
  }
}

//handle displaying back cover image if the filename is clicked
System::Void GameEditForm::GameEditBack_Click(System::Object^  sender, System::EventArgs^  e) {
  if (BackCoverTextBox->Text != nullptr) {
    Display_GameImage(BackCoverTextBox->Text);
  }
}

// function to display a screenshot when clicked upon
System::Void GameEditForm::ScreenShotListBox_Click(System::Object^  sender, System::EventArgs^  e) {
  if (ScreenShotListBox->SelectedItem != nullptr) {
    Display_GameImage(ScreenShotListBox->SelectedItem->ToString());
  }
}

// display the input filename in the picture display box
System::Void GameEditForm::Display_GameImage(String ^filename) {
  const char *datapath = gConfMan->Get_ConfigItem("ShotPath");
  if (!datapath) {
    MessageBox::Show("Error: Configuration file corrupted!");
  }
  String ^invalidpath = String::Concat(gcnew String(datapath), "invalid.jpg");

  String ^dp = String::Concat(Environment::CurrentDirectory, gcnew String("\\tmp\\"));
  dp = dp->Replace("/", "\\");
  dp = dp->Replace(".", "");
  dp = String::Concat(dp, filename);

  // if the file doesn't exist, show the invalid file pic
  if (!System::IO::File::Exists(dp)) {
    invalidpath = String::Concat(Environment::CurrentDirectory, invalidpath);
    dp = invalidpath; 
  }
  GameEditPicture->Load(dp);
  GameEditPicture->Refresh();
}


// Handle editing of genre list from the edit button in the game edit screen
System::Void GameEditForm::EditGenreButton_Click(System::Object^  sender, System::EventArgs^  e) {
  EditListForm ^elf = gcnew EditListForm(DBCategories->Genres);
  if (elf->ShowDialog(this) ==  ::DialogResult::OK) {

    // clear out the check box in the edit form and refill it from the master genre list
    GenreCheckedListBox->Items->Clear();
    for each(String ^s in DBCategories->Genres) {
      if (s != nullptr) {
        GenreCheckedListBox->Items->Add(s);
      }
    }

    // check the appropriate genre checkboxes based on current game data
    if (CurrentEditGame->Genres->Count != 0) {
      for each(String ^s in CurrentEditGame->Genres) {
        int index = GenreCheckedListBox->Items->IndexOf(s);
        if (index != -1) {
          GenreCheckedListBox->SetItemChecked(index, true);
        }
      }
    }
    GenreCheckedListBox->Refresh();
  }
}

// Handle editing of platform list from the edit button in the game edit screen
System::Void GameEditForm::EditPlatformButton_Click(System::Object^  sender, System::EventArgs^  e) {
  EditListForm ^elf = gcnew EditListForm(DBCategories->Platforms);
  if (elf->ShowDialog(this) ==  ::DialogResult::OK) {

    // clear then refill the platform list in the edit window
    AlternatePlatformsCheckedListBox->Items->Clear();
    for each(String ^s in DBCategories->Platforms) {
      if (s != nullptr) {
        AlternatePlatformsCheckedListBox->Items->Add(s);
      }
    }

    // check the appropriate platforms in the edit window
    for each(String ^s in CurrentEditGame->AlternatePlatforms) {
      int index = AlternatePlatformsCheckedListBox->Items->IndexOf(s);
      if (index != -1) {
        AlternatePlatformsCheckedListBox->SetItemChecked(index, true);
      }
    }
    AlternatePlatformsCheckedListBox->Refresh();
  }
}


// edit the set of categories used in the custom category view
System::Void GameEditForm::CategoryEditButton_Click(System::Object^  sender, System::EventArgs^  e) {
  EditListForm ^elf = gcnew EditListForm(DBCategories->CustomCategoryMembership);
  if (elf->ShowDialog(this) ==  ::DialogResult::OK) {

    // clear then refill the custom category widget in the edit window
    CategoriesCheckedListBox->Items->Clear();
    for each(String ^s in DBCategories->CustomCategoryMembership) {
      if (s != nullptr) {
        CategoriesCheckedListBox->Items->Add(s);
      }
    }

    // check the appropriate categories in the edit window
    for each(String ^s in CurrentEditGame->CustomCategoryMembership) {
      int index = CategoriesCheckedListBox->Items->IndexOf(s);
      if (index != -1) {
        CategoriesCheckedListBox->SetItemChecked(index, true);
      }
    }

    CategoriesCheckedListBox->Refresh();
  }
}


// Handle the updating of the game descriptor data structure after the ok button
//   in the edit window is pressed
System::Void GameEditForm::GameEditOkButton_Click(System::Object^  sender, System::EventArgs^  e) {

  // update the game data structure with edited fields
  CurrentEditGame->GameName = TitleTextBox->Text;
  CurrentEditGame->Developer = DeveloperComboBox->Text;
  CurrentEditGame->Publisher = PublisherComboBox->Text;
  CurrentEditGame->Location1 = Location1ComboBox->Text;
  CurrentEditGame->Location2 = Location2ComboBox->Text;
  CurrentEditGame->Location3 = Location3ComboBox->Text;
  CurrentEditGame->Location4 = Location4ComboBox->Text;
  CurrentEditGame->Series = SeriesComboBox->Text;

  CurrentEditGame->Description = DescriptionRichTextBox->Text;
  // make sure there are no line breaks in the text
  if (CurrentEditGame->Description != nullptr) {
    CurrentEditGame->Description = CurrentEditGame->Description->Replace("\n", "<br>");
    CurrentEditGame->Description = CurrentEditGame->Description->Replace("\r", "");
  }

  CurrentEditGame->ReleaseYear = ReleaseYearTextBox->Text;
  CurrentEditGame->PrimaryPlatform = PrimaryPlatformComboBox->Text;
  CurrentEditGame->Notes = NotesRichTextBox->Text;
  // make sure there are no line breaks in the text
  if (CurrentEditGame->Notes != nullptr) {
    CurrentEditGame->Notes = CurrentEditGame->Notes->Replace("\n", "<br>");
    CurrentEditGame->Notes = CurrentEditGame->Notes->Replace("\r", "");
  }

  const char *datapath = gConfMan->Get_ConfigItem("ShotPath");
  if (!datapath) {
    MessageBox::Show("Error: Configuration file corrupted!");
    return;
  }
  String ^imgpath = gcnew String(datapath);
  imgpath = imgpath->Replace("/", "\\");
  imgpath = imgpath->Replace(".", "");
  imgpath = String::Concat(Environment::CurrentDirectory, imgpath);

  String ^tmppath = String::Concat(Environment::CurrentDirectory, gcnew String("\\tmp\\"));

  // if we are updating a preexisting game we need to erase all old shots
  if (OldEditGame != nullptr) {
    try {
      String ^fname = String::Concat(imgpath, OldEditGame->TitleShot);
      File::Delete(fname);
    }
    catch (Exception ^e) {
      e = e;
    }
    try {
      String ^fname = String::Concat(imgpath, OldEditGame->BackShot);
      File::Delete(fname);
    } 
    catch (Exception ^e) {
      e = e;
    }
    for each (String ^ss in OldEditGame->Screenshots) {
      try {
        String ^fname = String::Concat(imgpath, ss);
        File::Delete(fname);
      } 
      catch (Exception ^e) {
        e = e;
      }
    }
  }

  CurrentEditGame->TitleShot = FrontCoverTextBox->Text;
  CurrentEditGame->BackShot = BackCoverTextBox->Text;
  CurrentEditGame->Completed = CompletedComboBox->Text;
  CurrentEditGame->CompletedYear = YearCompletedTextBox->Text;
  CurrentEditGame->Rating = RatingComboBox->Text;
  CurrentEditGame->GameURL = URLTextBox->Text;

  // update genres
  CurrentEditGame->Genres->Clear();
  System::Collections::IEnumerator^ cenum = GenreCheckedListBox->CheckedItems->GetEnumerator();
  while (cenum->MoveNext()) {
    Object^ checkeditem = safe_cast<Object^>(cenum->Current);
    CurrentEditGame->Genres->AddLast(checkeditem->ToString());
  }

  // update alternate platforms
  CurrentEditGame->AlternatePlatforms->Clear();
  cenum = AlternatePlatformsCheckedListBox->CheckedItems->GetEnumerator();
  while (cenum->MoveNext()) {
    Object^ checkeditem = safe_cast<Object^>(cenum->Current);
    CurrentEditGame->AlternatePlatforms->AddLast(checkeditem->ToString());
  }

  // update category membership
  CurrentEditGame->CustomCategoryMembership->Clear();
  cenum = CategoriesCheckedListBox->CheckedItems->GetEnumerator();
  while (cenum->MoveNext()) {
    Object^ checkeditem = safe_cast<Object^>(cenum->Current);
    CurrentEditGame->CustomCategoryMembership->AddLast(checkeditem->ToString());
  }

  // update screenshots
  CurrentEditGame->Screenshots->Clear();
  System::Collections::IEnumerator^ ssenum = ScreenShotListBox->Items->GetEnumerator();
  while (ssenum->MoveNext()) {
    Object^ listitem = safe_cast<Object^>(ssenum->Current);
    CurrentEditGame->Screenshots->AddLast(listitem->ToString());
  }

  // update documentation
  CurrentEditGame->ManualDocumentation->Clear();
  ssenum = DocumentationListBox->Items->GetEnumerator();
  while (ssenum->MoveNext()) {
    Object^ listitem = safe_cast<Object^>(ssenum->Current);
    String ^docname = listitem->ToString();
    int index = docname->IndexOf("  --  ");
    String ^fto = docname->Substring(0, index);
    String ^fno = docname->Substring(index + 6);

    DocumentationID ^gid = gcnew DocumentationID();
    gid->Filename = fno;
    gid->FileType = fto;
    CurrentEditGame->ManualDocumentation->AddLast(gid);
  }

  // update guides
  CurrentEditGame->HintDocumentation->Clear();
  ssenum = GuideListBox->Items->GetEnumerator();
  while (ssenum->MoveNext()) {
    Object^ listitem = safe_cast<Object^>(ssenum->Current);
    String ^docname = listitem->ToString();
    int index = docname->IndexOf("  --  ");
    String ^fto = docname->Substring(0, index);
    String ^fno = docname->Substring(index + 6);

    DocumentationID ^gid = gcnew DocumentationID();
    gid->Filename = fno;
    gid->FileType = fto;
    CurrentEditGame->HintDocumentation->AddLast(gid);
  }

  // now move over the screen shot contents in the temp directory to the shot directory
  try {
    String ^fname = String::Concat(tmppath, CurrentEditGame->TitleShot);
    String ^dname = String::Concat(imgpath, CurrentEditGame->TitleShot);
    if (File::Exists(dname)) {
      File::Delete(dname);
    }
    File::Move(fname, dname);
  }
  catch (Exception ^e) {
    e = e;
  }
  try {
    String ^fname = String::Concat(tmppath, CurrentEditGame->BackShot);
    String ^dname = String::Concat(imgpath, CurrentEditGame->BackShot);
    if (File::Exists(dname)) {
      File::Delete(dname);
    }
    File::Move(fname, dname);
  } 
  catch (Exception ^e) {
    e = e;
  }
  for each (String ^ss in CurrentEditGame->Screenshots) {
    try {
      String ^fname = String::Concat(tmppath, ss);
      String ^dname = String::Concat(imgpath, ss);
      if (File::Exists(dname)) {
        File::Delete(dname);
      }
      File::Copy(fname, dname);
    } 
    catch (Exception ^e) {
      e = e;
    }
  }
  Close();
}


// we want to delete any screen shots if cancel was clicked
System::Void GameEditForm::GameEditCancelButton_Click(System::Object^  sender, System::EventArgs^  e) {

  String ^tmppath = String::Concat(Environment::CurrentDirectory, gcnew String("\\tmp\\"));
  array<String^> ^filelist = Directory::GetFiles(tmppath);
  for each (String ^s in filelist) {
	  try {
		  File::Delete(s);
	  }
	  catch (...) {

	  }
  }
  Close();
}



// fill the edit game window with the data from the game descriptor
void GameEditForm::Populate_EditForm(GameDescriptor ^gd, GameDescriptor ^oeg, bool keepchanges) {
  CurrentEditGame = gd;
  OldEditGame = oeg;

  // if we are editing a game copy all downloaded files over to the temp directory
  if (OldEditGame != nullptr) {
    const char *datapath = gConfMan->Get_ConfigItem("ShotPath");
    if (!datapath) {
      MessageBox::Show("Error: Configuration file corrupted!");
      return;
    }
    String ^imgpath = gcnew String(datapath);
    imgpath = imgpath->Replace("/", "\\");
    imgpath = imgpath->Replace(".", "");
    imgpath = String::Concat(Environment::CurrentDirectory, imgpath);

    String ^tmppath = String::Concat(Environment::CurrentDirectory, gcnew String("\\tmp\\"));

    try {
      String ^fname = String::Concat(imgpath, OldEditGame->TitleShot);
      String ^dname = String::Concat(tmppath, OldEditGame->TitleShot);
      File::Copy(fname, dname);
    }
    catch (Exception ^e) {
      e = e;
    }
    try {
      String ^fname = String::Concat(imgpath, OldEditGame->BackShot);
      String ^dname = String::Concat(tmppath, OldEditGame->BackShot);
      File::Copy(fname, dname);
    } 
    catch (Exception ^e) {
      e = e;
    }
    for each (String ^ss in OldEditGame->Screenshots) {
      try {
        String ^fname = String::Concat(imgpath, ss);
        String ^dname = String::Concat(tmppath, ss);
        File::Copy(fname, dname);
      } 
      catch (Exception ^e) {
        e = e;
      }
    }
  }

  // then fill the combo boxes
  if (!keepchanges) {
    Location1ComboBox->Items->Clear();
    for each(String ^s in DBCategories->Location1) {
      if (s != nullptr) {
        Location1ComboBox->Items->Add(s);
      }
    }

    Location2ComboBox->Items->Clear();
    for each(String ^s in DBCategories->Location2) {
      if (s != nullptr) {
        Location2ComboBox->Items->Add(s);
      }
    }

    Location3ComboBox->Items->Clear();
    for each(String ^s in DBCategories->Location3) {
      if (s != nullptr) {
        Location3ComboBox->Items->Add(s);
      }
    }

    Location4ComboBox->Items->Clear();
    for each(String ^s in DBCategories->Location4) {
      if (s != nullptr) {
        Location4ComboBox->Items->Add(s);
      }
    }
  }

  DeveloperComboBox->Items->Clear();
  for each(String ^s in DBCategories->Developer) {
    if (s != nullptr) {
      DeveloperComboBox->Items->Add(s);
    }
  }

  PublisherComboBox->Items->Clear();
  for each(String ^s in DBCategories->Publisher) {
    if (s != nullptr) {
      PublisherComboBox->Items->Add(s);
    }
  }

  if (!keepchanges) {
    SeriesComboBox->Items->Clear();
    for each(String ^s in DBCategories->Series) {
      if (s != nullptr) {
        SeriesComboBox->Items->Add(s);
      }
    }

    PrimaryPlatformComboBox->Items->Clear();
    for each(String ^s in DBCategories->Platforms) {
      if (s != nullptr) {
        PrimaryPlatformComboBox->Items->Add(s);
      }
    }

    AlternatePlatformsCheckedListBox->Items->Clear();
    for each(String ^s in DBCategories->Platforms) {
      if (s != nullptr) {
        AlternatePlatformsCheckedListBox->Items->Add(s);
      }
    }

    GenreCheckedListBox->Items->Clear();

    CategoriesCheckedListBox->Items->Clear();
    for each(String ^s in DBCategories->CustomCategoryMembership) {
      if (s != nullptr) {
        CategoriesCheckedListBox->Items->Add(s);
      }
    }
  }

  // now fill in the form fields
  TitleTextBox->Text = gd->GameName;
  DeveloperComboBox->Text = gd->Developer;
  PublisherComboBox->Text = gd->Publisher;
  Location1ComboBox->Text = gd->Location1;
  Location2ComboBox->Text = gd->Location2;
  Location3ComboBox->Text = gd->Location3;
  Location4ComboBox->Text = gd->Location4;
  SeriesComboBox->Text = gd->Series;

  if (gd->Description != nullptr) {
    gd->Description = gd->Description->Replace("\n", "<br>");
  }
  DescriptionRichTextBox->Text = gd->Description;

  if (!keepchanges) {
    YearCompletedTextBox->Text = gd->CompletedYear;
    ReleaseYearTextBox->Text = gd->ReleaseYear;
    PrimaryPlatformComboBox->Text = gd->PrimaryPlatform;
    if (gd->PrimaryPlatform == nullptr) {
      PrimaryPlatformComboBox->Text = "PC Windows";
    }

    if (gd->Notes != nullptr) {
      gd->Notes = gd->Notes->Replace("\n", "<br>");
    }
    NotesRichTextBox->Text = gd->Notes;
    FrontCoverTextBox->Text = gd->TitleShot;
    BackCoverTextBox->Text = gd->BackShot;
    CompletedComboBox->Text = gd->Completed;
    RatingComboBox->Text = gd->Rating;
    URLTextBox->Text = gd->GameURL;

    GenreCheckedListBox->Sorted = false;
    if (gd->Genres->Count != 0) {
      for each(String ^s in gd->Genres) {
        GenreCheckedListBox->Items->Add(s, true);
      }
    }
    for each (String ^dbs in DBCategories->Genres) {
      bool matched = false;
      for each (String ^gs in gd->Genres) {
        if (String::Equals(gs, dbs)) {
          matched = true;
        }
      }
      if (!matched) {
        GenreCheckedListBox->Items->Add(dbs, false);
      }
    }

    for each(String ^s in gd->AlternatePlatforms) {
      int index = AlternatePlatformsCheckedListBox->Items->IndexOf(s);
      AlternatePlatformsCheckedListBox->SetItemChecked(index, true);
    }

    for each(String ^s in gd->CustomCategoryMembership) {
      int index = CategoriesCheckedListBox->Items->IndexOf(s);
      CategoriesCheckedListBox->SetItemChecked(index, true);
    }

    DocumentationListBox->Items->Clear();
    for each (DocumentationID ^gid in gd->ManualDocumentation) {
      String ^fn = gid->Filename;
      String ^ft = gid->FileType;

      String ^docline = String::Concat(ft, "  --  ");
      docline = String::Concat(docline, fn);
      DocumentationListBox->Items->Add(docline);
    }

    GuideListBox->Items->Clear();
    for each (DocumentationID ^gid in gd->HintDocumentation) {
      String ^fn = gid->Filename;
      String ^ft = gid->FileType;

      String ^docline = String::Concat(ft, "  --  ");
      docline = String::Concat(docline, fn);
      GuideListBox->Items->Add(docline);
    }
  }

  ScreenShotListBox->Items->Clear();
  for each (String ^s in gd->Screenshots) {
    ScreenShotListBox->Items->Add(s);
  }
  TitleTextBox->Focus();
}


// associate a documentation file with the game
System::Void GameEditForm::AddDocumentationButton_Click(System::Object^  sender, System::EventArgs^  e) {
  AddDocForm ^adf = gcnew AddDocForm(nullptr, nullptr, MANUAL);
  if (adf->ShowDialog(this) ==  ::DialogResult::OK) {
    String ^fn = adf->AddDocFilenameTextBox->Text;
    String ^ft = adf->AddDocDocTypeTextbox->Text;

    String ^docline = String::Concat(ft, "  --  ");
    docline = String::Concat(docline, fn);
    DocumentationListBox->Items->Add(docline);
    DocumentationListBox->Refresh();  
  }
}

// associate a guide with the game
System::Void GameEditForm::AddGuideButton_Click(System::Object^  sender, System::EventArgs^  e) {
  AddDocForm ^adf = gcnew AddDocForm(nullptr, nullptr, GUIDE);
  if (adf->ShowDialog(this) ==  ::DialogResult::OK) {
    String ^fn = adf->AddDocFilenameTextBox->Text;
    String ^ft = adf->AddDocDocTypeTextbox->Text;

    String ^docline = String::Concat(ft, "  --  ");
    docline = String::Concat(docline, fn);
    GuideListBox->Items->Add(docline);
    GuideListBox->Refresh();  
  }
}

// disassociate a documentation file with the game
System::Void GameEditForm::RemoveDocumentationButton_Click(System::Object^  sender, System::EventArgs^  e) {
  if (DocumentationListBox->SelectedItem == nullptr) {
    return;
  }
  String ^txt = DocumentationListBox->SelectedItem->ToString();
  DocumentationListBox->Items->Remove(txt);
  DocumentationListBox->Refresh();
}

// disassociate a guide file with the game
System::Void GameEditForm::RemoveGuideButton_Click(System::Object^  sender, System::EventArgs^  e) {
  if (GuideListBox->SelectedItem == nullptr) {
    return;
  }
  String ^txt = GuideListBox->SelectedItem->ToString();
  GuideListBox->Items->Remove(txt);
  GuideListBox->Refresh();

}

// edit the documentation file associated with a game
System::Void GameEditForm::EditDocumentationButton_Click(System::Object^  sender, System::EventArgs^  e) {
  if (DocumentationListBox->SelectedItem == nullptr) {
    return;
  }
  String ^txt = DocumentationListBox->SelectedItem->ToString();

  int index = txt->IndexOf("  --  ");
  String ^fto = txt->Substring(0, index);
  String ^fno = txt->Substring(index + 6);

  AddDocForm ^adf = gcnew AddDocForm(fno, fto, MANUAL);
  if (adf->ShowDialog(this) ==  ::DialogResult::OK) {

    DocumentationListBox->Items->Remove(txt);
    DocumentationListBox->Refresh();

    String ^fn = adf->AddDocFilenameTextBox->Text;
    String ^ft = adf->AddDocDocTypeTextbox->Text;

    String ^docline = String::Concat(ft, "  --  ");
    docline = String::Concat(docline, fn);
    DocumentationListBox->Items->Add(docline);
    DocumentationListBox->Refresh();  
  }
}

// edit the guide associated with the game
System::Void GameEditForm::EditGuideButton_Click(System::Object^  sender, System::EventArgs^  e) {
  if (GuideListBox->SelectedItem == nullptr) {
    return;
  }
  String ^txt = GuideListBox->SelectedItem->ToString();

  int index = txt->IndexOf("  --  ");
  String ^fto = txt->Substring(0, index);
  String ^fno = txt->Substring(index + 6);

  AddDocForm ^adf = gcnew AddDocForm(fno, fto, GUIDE);

  GuideListBox->Items->Remove(txt);
  GuideListBox->Refresh();

  if (adf->ShowDialog(this) ==  ::DialogResult::OK) {
    String ^fn = adf->AddDocFilenameTextBox->Text;
    String ^ft = adf->AddDocDocTypeTextbox->Text;

    String ^docline = String::Concat(ft, "  --  ");
    docline = String::Concat(docline, fn);
    GuideListBox->Items->Add(docline);
    GuideListBox->Refresh();  
  }
}

