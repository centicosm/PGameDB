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



// Functions for saving, loading, importing databases, etc.

#include "MainForm.h"
#include "pgdb_utilityfunctions.h"
#include <direct.h>
#include <vector>

using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace PGameDB;
using namespace std;


// create a new database
System::Void MainForm::PGDB_FileMenuNew_Click(System::Object^  sender, System::EventArgs^  e) {
  ActiveCategories.Clear();
  CustomViewCategories.Clear();
  MasterList.Clear();
  DBCategories->CustomCategoryMembership->Clear();
  DBCategories->Developer->Clear();
  DBCategories->Genres->Clear();
  DBCategories->Location1->Clear();
  DBCategories->Location2->Clear();
  DBCategories->Location3->Clear();
  DBCategories->Location4->Clear();
  DBCategories->Platforms->Clear();
  DBCategories->Publisher->Clear();
  DBCategories->Series->Clear();
  DBCategories->Year->Clear();

  Populate_UnsortedView();

  PGameDBNumGameVal->Text = gcnew String("0");
}


// Save database as a new name
System::Void MainForm::SaveAsMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

  // open a dialog so they can set the new name
  FileDialog^ sfd = gcnew SaveFileDialog;
  String ^dp = gcnew String(Environment::CurrentDirectory + "\\");

  sfd->InitialDirectory = dp;
  sfd->Filter = "PGameDB Files (*.pgdb)|*.pgdb";
  sfd->FilterIndex = 1;
  sfd->RestoreDirectory = true;

  if (sfd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
    CurrentOpenDB = sfd->FileName;
    PGDB_FileMenuSave_Click(sender, e);
  }
}


// Save current database
System::Void MainForm::PGDB_FileMenuSave_Click(System::Object^  sender, System::EventArgs^  e) {

  StreamWriter ^sw = gcnew StreamWriter(CurrentOpenDB);

  // write the custom categories to file
  sw->WriteLine("{0}", CustomViewCategories.Count);
  for each (String ^s in CustomViewCategories) {
    sw->WriteLine(s);
  }

  // write all possible custom categories
  sw->WriteLine("{0}", DBCategories->CustomCategoryMembership->Count);
  for each (String ^s in DBCategories->CustomCategoryMembership) {
    sw->WriteLine(s);
  }

  // write the possible platforms to file
  sw->WriteLine("{0}", DBCategories->Platforms->Count);
  for each (String ^s in DBCategories->Platforms) {
    sw->WriteLine(s);
  }

  // write the possible genres to file
  sw->WriteLine("{0}", DBCategories->Genres->Count);
  for each (String ^s in DBCategories->Genres) {
    sw->WriteLine(s);
  }

  // write individual game data to file
  for each (GameDescriptor ^gd in MasterList) {
    sw->WriteLine(gd->GameName);
    sw->WriteLine(gd->Developer);
    sw->WriteLine(gd->Publisher);
    sw->WriteLine(gd->Description);
    sw->WriteLine(gd->ReleaseYear);
    sw->WriteLine(gd->Series);
    sw->WriteLine(gd->PrimaryPlatform);
    sw->WriteLine(gd->TitleShot);
    sw->WriteLine(gd->BackShot);
    sw->WriteLine(gd->Rating);
    sw->WriteLine(gd->Completed);
    sw->WriteLine(gd->CompletedYear);
    sw->WriteLine(gd->Location1);
    sw->WriteLine(gd->Location2);
    sw->WriteLine(gd->Location3);
    sw->WriteLine(gd->Location4);
    sw->WriteLine(gd->Notes);
    sw->WriteLine(gd->GameURL);

    sw->WriteLine("{0}", gd->Genres->Count);
    for each (String ^s in gd->Genres) {
      sw->WriteLine(s);
    }
    sw->WriteLine("{0}", gd->AlternatePlatforms->Count);
    for each (String ^s in gd->AlternatePlatforms) {
      sw->WriteLine(s);
    }
    sw->WriteLine("{0}", gd->Screenshots->Count);
    for each (String ^s in gd->Screenshots) {
      sw->WriteLine(s);
    }
    sw->WriteLine("{0}", gd->CustomCategoryMembership->Count);
    for each (String ^s in gd->CustomCategoryMembership) {
      sw->WriteLine(s);
    }
    sw->WriteLine("{0}", gd->ManualDocumentation->Count);
    for each (DocumentationID ^g in gd->ManualDocumentation) {
      sw->WriteLine(g->Filename);
      sw->WriteLine(g->FileType);
    }
    sw->WriteLine("{0}", gd->HintDocumentation->Count);
    for each (DocumentationID ^g in gd->HintDocumentation) {
      sw->WriteLine(g->Filename);
      sw->WriteLine(g->FileType);
    }


  }
  sw->Close();

  Modified = false;
}



// Load a database file via the file menu
System::Void MainForm::PGDB_FileMenuLoad_Click(System::Object^  sender, System::EventArgs^  e) {
  FileDialog^ sfd = gcnew OpenFileDialog;

  String ^dp = gcnew String(Environment::CurrentDirectory + "\\");

  sfd->InitialDirectory = dp;
  sfd->Filter = "PGameDB Files (*.pgdb)|*.pgdb";
  sfd->FilterIndex = 1;
  sfd->RestoreDirectory = true;

  if (sfd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
    dp = String::Concat(sfd->FileName);
    CurrentOpenDB = sfd->FileName;
    Load_Database(dp);

    this->ViewModeDropDownButton->Text = "Unsorted";
    Populate_UnsortedView();
    SearchNode = nullptr;
  }
}


System::Void MainForm::Init_DefaultDB(void) {
	String ^dp = gcnew String(Environment::CurrentDirectory + "\\");
	const char *fname = gConfMan->Get_ConfigItem("DefaultDB");
	if (!fname) {
		return;
	}
	String ^filename = gcnew String(fname);
	dp = String::Concat(dp, filename);
	if (!File::Exists(dp)) {
		MessageBox::Show("Default database (" + dp + ") does not exist. Initializing");

		for each (String ^s in PCPlatforms) DBCategories->Platforms->Add(s->Replace("Platform:", ""));
		for each (String ^s in ConsolePlatforms) DBCategories->Platforms->Add(s->Replace("Platform:", ""));
		for each (String ^s in HandheldPlatforms) DBCategories->Platforms->Add(s->Replace("Platform:", ""));
		for each (String ^s in OtherPlatforms) DBCategories->Platforms->Add(s->Replace("Platform:", ""));
		
		CurrentOpenDB = dp;
		PGDB_FileMenuSave_Click(nullptr, nullptr);
		try {
			Directory::CreateDirectory("Shots");
			Directory::CreateDirectory("Docs");
			Directory::CreateDirectory("Guides");
		}
		catch (...) {

		}
	}
}

/*String ^pf = "PCPlatforms.txt";

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
  FilterPlatformOther->DropDownItems->Add(tsi);*/

// load the default database as specified in the config file
System::Void MainForm::Load_DefaultDatabase(void) {

  String ^dp = gcnew String(Environment::CurrentDirectory + "\\");
  const char *fname = gConfMan->Get_ConfigItem("DefaultDB");
  if (!fname) {
    return;
  }
  String ^filename = gcnew String(fname);
  dp = String::Concat(dp, filename);
  CurrentOpenDB = dp;
  Load_Database(dp);
}

// load the database with the given name
System::Void MainForm::Load_DatabaseFromName(String ^dbname) {
  String ^dp = gcnew String(Environment::CurrentDirectory + "\\");

  String ^filename = gcnew String(dbname);
  dp = String::Concat(dp, filename);
  CurrentOpenDB = dp;
  Load_Database(dp);
}


// actually read the database from the file
System::Void MainForm::Load_Database(String ^dp) {

  int cnt;
  GameList->Nodes->Clear();
  TreeNode ^parentnode = GameList->Nodes->Add("Unsorted", "Unsorted");
  parentnode->Name = "Unsorted";
  if (!File::Exists(dp)) {
    CurrentOpenDB = nullptr;
    MessageBox::Show("Error: Database not found: " + dp);
    return;
  }
  StreamReader ^sr = gcnew StreamReader(dp);

  // clear out all the old information
  ActiveCategories.Clear();
  CustomViewCategories.Clear();
  MasterList.Clear();
  DBCategories->CustomCategoryMembership->Clear();
  DBCategories->Developer->Clear();
  DBCategories->Genres->Clear();
  DBCategories->Location1->Clear();
  DBCategories->Location2->Clear();
  DBCategories->Location3->Clear();
  DBCategories->Location4->Clear();
  DBCategories->Platforms->Clear();
  DBCategories->Publisher->Clear();
  DBCategories->Series->Clear();
  DBCategories->Year->Clear();

  // read the custom categories
  String ^txtcnt = sr->ReadLine();
  cnt = System::Convert::ToInt32(txtcnt);
  for (int i = 0; i < cnt; i++) {
    String ^line = sr->ReadLine();
    CustomViewCategories.AddLast(line);
  }

  // read all possible custom categories
  txtcnt = sr->ReadLine();
  cnt = System::Convert::ToInt32(txtcnt);
  for (int i = 0; i < cnt; i++) {
    String ^line = sr->ReadLine();
    DBCategories->CustomCategoryMembership->Add(line);
  }

  // read all possible platforms
  txtcnt = sr->ReadLine();
  cnt = System::Convert::ToInt32(txtcnt);
  for (int i = 0; i < cnt; i++) {
    String ^line = sr->ReadLine();
    DBCategories->Platforms->Add(line);
  }

  // read all possible genres
  txtcnt = sr->ReadLine();
  cnt = System::Convert::ToInt32(txtcnt);
  array<String ^> ^as;
  Array::Resize(as, cnt);
  for (int i = 0; i < cnt; i++) {
    String ^line = sr->ReadLine();
    DBCategories->Genres->Add(line);
    as[i] = line;
  }
  DBCategories->Genres->Sort();

  // read the game data
  while (!sr->EndOfStream ) {
    GameDescriptor ^gd = gcnew GameDescriptor();
    String ^line = sr->ReadLine();
    if (String::Equals(line,"")) {
      MessageBox::Show("ss");
    }
    gd->GameName = line;

    line = sr->ReadLine();
    gd->Developer = line;

    line = sr->ReadLine();
    gd->Publisher = line;

    line = sr->ReadLine();
    gd->Description = line;

    line = sr->ReadLine();
    gd->ReleaseYear = line;

    line = sr->ReadLine();
    gd->Series = line;

    line = sr->ReadLine();
    gd->PrimaryPlatform = line;

    line = sr->ReadLine();
    gd->TitleShot = line;

    line = sr->ReadLine();
    gd->BackShot = line;

    line = sr->ReadLine();
    gd->Rating = line;

    line = sr->ReadLine();
    gd->Completed = line;

    line = sr->ReadLine();
    gd->CompletedYear = line;

    line = sr->ReadLine();
    gd->Location1 = line;

    line = sr->ReadLine();
    gd->Location2 = line;

    line = sr->ReadLine();
    gd->Location3 = line;

    line = sr->ReadLine();
    gd->Location4 = line;

    line = sr->ReadLine();
    gd->Notes = line;

    line = sr->ReadLine();
    gd->GameURL = line;

    txtcnt = sr->ReadLine();
    cnt = System::Convert::ToInt32(txtcnt);
    for (int i = 0; i < cnt; i++) {
      String ^line = sr->ReadLine();
      gd->Genres->AddLast(line);
    }


    txtcnt = sr->ReadLine();
    cnt = System::Convert::ToInt32(txtcnt);
    for (int i = 0; i < cnt; i++) {
      String ^line = sr->ReadLine();
      gd->AlternatePlatforms->AddLast(line);
    }

    txtcnt = sr->ReadLine();
    cnt = System::Convert::ToInt32(txtcnt);
    for (int i = 0; i < cnt; i++) {
      String ^line = sr->ReadLine();
      gd->Screenshots->AddLast(line);
    }

    txtcnt = sr->ReadLine();
    cnt = System::Convert::ToInt32(txtcnt);
    for (int i = 0; i < cnt; i++) {
      String ^line = sr->ReadLine();
      gd->CustomCategoryMembership->AddLast(line);
    }

    // read the documentation data
    txtcnt = sr->ReadLine();
    cnt = System::Convert::ToInt32(txtcnt);
    for (int i = 0; i < cnt; i++) {
      String ^line1 = sr->ReadLine();
      String ^line2 = sr->ReadLine();
      DocumentationID ^gid = gcnew DocumentationID();
      gid->Filename = line1;
      gid->FileType = line2;
      gd->ManualDocumentation->AddLast(gid);
    }
    txtcnt = sr->ReadLine();
    cnt = System::Convert::ToInt32(txtcnt);
    for (int i = 0; i < cnt; i++) {
      String ^line1 = sr->ReadLine();
      String ^line2 = sr->ReadLine();
      DocumentationID ^gid = gcnew DocumentationID();
      gid->Filename = line1;
      gid->FileType = line2;
      gd->HintDocumentation->AddLast(gid);
    }    

    // add the game
    MasterList.AddLast(gd);

    // generate categories based on game data
    Generate_Categories(gd);

    // precompute the game's category membership
    gd->Generate_CategoryMembership();
  }

  ExpandedCategories.AddLast(parentnode->Name);
  sr->Close();

  Populate_UnsortedView();

  // update the game count
  char buf[100];
  sprintf_s(buf, "%d", MasterList.Count);
  PGameDBNumGameVal->Text = gcnew String(buf);
}


