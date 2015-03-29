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


// Main interface for the games database

#pragma once

#include <list>
#include <map>
#include "GameDataStructs.h"
#include "PGameDB.h"
#include "GameEditForm.h"
#include "EditListForm.h"
#include "MobySelectionForm.h"
#include "SimpleTextEntryForm.h"
#include "ImageViewerForm.h"
#include "PGDBPrefrencesForm.h"
#include "alphnodesorter.h"

namespace PGameDB {

  using namespace System;
  using namespace System::IO;
  using namespace System::ComponentModel;
  using namespace System::Collections;
  using namespace System::Windows::Forms;
  using namespace System::Data;
  using namespace System::Drawing;
  using namespace System::Collections::Generic;
  using namespace System::Collections;
  using namespace System::Reflection;
  using namespace std;


  public ref class MainForm : public System::Windows::Forms::Form
  {
  public:
    MainForm(String ^loaddb)
    {
      InitializeComponent();

      // initialize random number generator
      RandNumGen = gcnew Random();

      // intialize category data structure
      DBCategories = gcnew DBCats();
      DBCategories->Developer = gcnew LinkedList<String ^>();
      DBCategories->Genres = gcnew List<String ^>();
      DBCategories->Location1 = gcnew LinkedList<String ^>();
      DBCategories->Location2 = gcnew LinkedList<String ^>();
      DBCategories->Location3 = gcnew LinkedList<String ^>();
      DBCategories->Location4 = gcnew LinkedList<String ^>();
      DBCategories->Platforms = gcnew List<String ^>();
      DBCategories->Publisher = gcnew LinkedList<String ^>();
      DBCategories->Series = gcnew LinkedList<String ^>();
      DBCategories->Year = gcnew List<String ^>();
      DBCategories->CustomCategoryMembership = gcnew List<String ^>();
      ActiveCategories.AddLast("Unsorted");

      // initialize various flags associated with the program
      CurrentOpenDB = nullptr;
      SelectedCategory = nullptr;
      SearchNode = nullptr;
      Modified = false;
      ChangedName = false;
      DoneLoadingGamePage = true;
      CurPlatormFilter = nullptr;
      GameList->TreeViewNodeSorter = gcnew AlphNodeSorter();

      // blank blue background
      GameViewer->DocumentText = "<html><body bgcolor=#85A1C0></body></html>"; 

      // determine maximum number of search results to display
      const char *msr = gConfMan->Get_ConfigItem("MaxSearchResults");
      if (msr != NULL) {
        MaxSearchMatches = atoi(msr);
      }
      else {
        MaxSearchMatches = 30;
      }

      // read the platforms from file for the sort by platform feature
      PlatformFilterOn = false;
      GenreFilterOn = false;
      YearFilterOn = false;
      TextFilterOn = false;
      Load_Platforms();
   

	  // init and load the database
	  Init_DefaultDB();

	  // load a database
	  if (loaddb == nullptr) {
		  Load_DefaultDatabase();
	  }
	  else {
		  Load_DatabaseFromName(loaddb);
	  }
	  Load_GenreFilterDropDown();
	  Load_YearFilterDropDown();

      // load up the appropriate scraper
      const char *dll = gConfMan->Get_ConfigItem("Scraper");
      String ^scraper = gcnew String(dll);
      if (!File::Exists(scraper)) {
        MessageBox::Show(String::Concat("Fatal Error: Cannot find scraper library: ", scraper));
        MessageBox::Show("Change the Scraper value in PGameDB.cfg to a valid library.");
        exit(1);
      }

      String ^pth = String::Concat(Directory::GetCurrentDirectory(), "\\");
      pth = String::Concat(pth, scraper);
      ScrapeAssem = Assembly::LoadFrom(pth);
      ScrapeType = ScrapeAssem->GetType("Scraper");
      ScrapeGetFields = ScrapeType->GetMethod("Get_Fields");
      ScrapeGetMatchList = ScrapeType->GetMethod("Get_MatchList");
      GameScraper = Activator::CreateInstance(ScrapeType);

      // make the temp directory
      pth = String::Concat(Directory::GetCurrentDirectory(), "\\");
      pth = String::Concat(pth, "tmp");
      Directory::CreateDirectory(pth);
    }

  protected:
    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    ~MainForm()	{
      if (components)	{
        delete components;
      }

      // delete the temp directory
      String ^pth = String::Concat(Directory::GetCurrentDirectory(), "\\");
      pth = String::Concat(pth, "tmp");
      pth = String::Concat(pth, "\\");
      Directory::Delete(pth, true);
    }

  private: System::Windows::Forms::ToolStrip^  toolStrip1;
  private: System::Windows::Forms::ToolStripButton^  AddGameAutoButton;
  private: System::Windows::Forms::MenuStrip^  PGameDBMenu;
  private: System::Windows::Forms::ToolStripButton^  ManualAddGameButton;
  private: System::Windows::Forms::SplitContainer^  splitContainer1;
  private: System::Windows::Forms::WebBrowser^  GameViewer;
  private: System::Windows::Forms::TreeView^  GameList;
  private: System::Windows::Forms::ToolStripStatusLabel^  PGameDBNumGameLabel;
  private: System::Windows::Forms::ToolStripStatusLabel^  PGameDBNumGameVal;
  private: System::Windows::Forms::StatusStrip^  PGameDBStatusStrip;
  private: System::Windows::Forms::ToolStripMenuItem^  PGDB_FileMenu;
  private: System::Windows::Forms::ToolStripMenuItem^  PGDB_FileMenuLoad;
  private: System::Windows::Forms::ToolStripMenuItem^  PGDB_FileMenuSave;
  private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;

  private: System::Windows::Forms::ToolStripButton^  MobyUpdateButton;
  private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
  private: System::Windows::Forms::ToolStripButton^  AddGameCategoryButton;
  private: System::Windows::Forms::ToolStripMenuItem^  SaveAsMenuItem;
  private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator4;

  private: System::Windows::Forms::ToolStripMenuItem^  editToolStripMenuItem;
  private: System::Windows::Forms::ToolStripMenuItem^  PGDB_EditMenuPreferences;
  private: System::Windows::Forms::ToolStripDropDownButton^  ViewModeDropDownButton;
  private: System::Windows::Forms::ToolStripMenuItem^  UnsortedToolStripMenuItem;
  private: System::Windows::Forms::ToolStripMenuItem^  CategoryToolStripMenuItem;
  private: System::Windows::Forms::ToolStripMenuItem^  CustomToolStripMenuItem;

  private: System::Windows::Forms::ToolStripLabel^  toolStripLabel1;
  private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator3;
  private: System::Windows::Forms::ToolStripButton^  TransferCategoryButton;
  private: System::Windows::Forms::ToolStripMenuItem^  EditCustomCategoriesMenuItem;
  private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator5;
  private: System::Windows::Forms::ToolStripMenuItem^  PGDB_FileMenuNew;
  private: System::Windows::Forms::ToolStripButton^  RandomButton;
  private: System::Windows::Forms::ToolStripMenuItem^  specialToolStripMenuItem;
  private: System::Windows::Forms::ToolStripMenuItem^  noScreenshotsToolStripMenuItem;
  private: System::Windows::Forms::ToolStripMenuItem^  SearchForNoScreenshotsToolStripMenuItem;

  private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator6;








  private: System::Windows::Forms::ToolStripMenuItem^  aboutToolStripMenuItem;
  private: System::Windows::Forms::ToolStripMenuItem^  VersionMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  PGDB_FileMenuExit;



private: System::Windows::Forms::ToolStrip^  toolStrip2;
private: System::Windows::Forms::ToolStripLabel^  toolStripLabel4;
private: System::Windows::Forms::ToolStripDropDownButton^  PlatformFilter;

private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator9;
private: System::Windows::Forms::ToolStripLabel^  toolStripLabel5;
private: System::Windows::Forms::ToolStripDropDownButton^  GenreFilter;

private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator10;




private: System::Windows::Forms::ToolStripLabel^  toolStripLabel7;
private: System::Windows::Forms::ToolStripDropDownButton^  YearFilter;

private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator12;
private: System::Windows::Forms::ToolStripLabel^  toolStripLabel8;
private: System::Windows::Forms::ToolStripTextBox^  TextFilter;
private: System::Windows::Forms::ToolStripMenuItem^  FilterPlatformAll;
private: System::Windows::Forms::ToolStripMenuItem^  FilterPlatformPC;
private: System::Windows::Forms::ToolStripMenuItem^  FilterPlatformConsole;
private: System::Windows::Forms::ToolStripMenuItem^  FilterPlatformHandheld;
private: System::Windows::Forms::ToolStripMenuItem^  FilterPlatformOther;
private: System::Windows::Forms::ToolStripMenuItem^  allToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  allToolStripMenuItem1;
private: System::Windows::Forms::ToolStripLabel^  toolStripLabel2;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator7;
private: System::Windows::Forms::ToolStripLabel^  toolStripLabel3;
private: System::Windows::Forms::ToolStripDropDownButton^  PlayedFilter;

private: System::Windows::Forms::ToolStripMenuItem^  FilterUnplayedNo;
private: System::Windows::Forms::ToolStripMenuItem^  FilterUnplayedYes;



private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator8;
private: System::Windows::Forms::ToolStripButton^  ClearFiltersButton;
private: System::Windows::Forms::ToolStripMenuItem^  FilterUnplayedAll;











  private:
    /// <summary>
    /// Required designer variable.
    /// </summary>
    System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    void InitializeComponent(void)
    {
      System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
      this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
      this->AddGameAutoButton = (gcnew System::Windows::Forms::ToolStripButton());
      this->ManualAddGameButton = (gcnew System::Windows::Forms::ToolStripButton());
      this->MobyUpdateButton = (gcnew System::Windows::Forms::ToolStripButton());
      this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
      this->TransferCategoryButton = (gcnew System::Windows::Forms::ToolStripButton());
      this->AddGameCategoryButton = (gcnew System::Windows::Forms::ToolStripButton());
      this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
      this->toolStripLabel1 = (gcnew System::Windows::Forms::ToolStripLabel());
      this->ViewModeDropDownButton = (gcnew System::Windows::Forms::ToolStripDropDownButton());
      this->UnsortedToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->CategoryToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->CustomToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->toolStripSeparator4 = (gcnew System::Windows::Forms::ToolStripSeparator());
      this->RandomButton = (gcnew System::Windows::Forms::ToolStripButton());
      this->toolStripSeparator6 = (gcnew System::Windows::Forms::ToolStripSeparator());
      this->PGameDBMenu = (gcnew System::Windows::Forms::MenuStrip());
      this->PGDB_FileMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->PGDB_FileMenuNew = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->PGDB_FileMenuLoad = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->PGDB_FileMenuSave = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->SaveAsMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
      this->PGDB_FileMenuExit = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->editToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->EditCustomCategoriesMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->toolStripSeparator5 = (gcnew System::Windows::Forms::ToolStripSeparator());
      this->PGDB_EditMenuPreferences = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->specialToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->noScreenshotsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->SearchForNoScreenshotsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->VersionMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
      this->GameList = (gcnew System::Windows::Forms::TreeView());
      this->GameViewer = (gcnew System::Windows::Forms::WebBrowser());
      this->PGameDBNumGameLabel = (gcnew System::Windows::Forms::ToolStripStatusLabel());
      this->PGameDBNumGameVal = (gcnew System::Windows::Forms::ToolStripStatusLabel());
      this->PGameDBStatusStrip = (gcnew System::Windows::Forms::StatusStrip());
      this->toolStrip2 = (gcnew System::Windows::Forms::ToolStrip());
      this->toolStripLabel2 = (gcnew System::Windows::Forms::ToolStripLabel());
      this->toolStripLabel4 = (gcnew System::Windows::Forms::ToolStripLabel());
      this->PlatformFilter = (gcnew System::Windows::Forms::ToolStripDropDownButton());
      this->FilterPlatformAll = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->FilterPlatformPC = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->FilterPlatformConsole = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->FilterPlatformHandheld = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->FilterPlatformOther = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->toolStripSeparator9 = (gcnew System::Windows::Forms::ToolStripSeparator());
      this->toolStripLabel5 = (gcnew System::Windows::Forms::ToolStripLabel());
      this->GenreFilter = (gcnew System::Windows::Forms::ToolStripDropDownButton());
      this->allToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->toolStripSeparator10 = (gcnew System::Windows::Forms::ToolStripSeparator());
      this->toolStripLabel7 = (gcnew System::Windows::Forms::ToolStripLabel());
      this->YearFilter = (gcnew System::Windows::Forms::ToolStripDropDownButton());
      this->allToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->toolStripSeparator12 = (gcnew System::Windows::Forms::ToolStripSeparator());
      this->toolStripLabel3 = (gcnew System::Windows::Forms::ToolStripLabel());
      this->PlayedFilter = (gcnew System::Windows::Forms::ToolStripDropDownButton());
      this->FilterUnplayedAll = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->FilterUnplayedNo = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->FilterUnplayedYes = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->toolStripSeparator7 = (gcnew System::Windows::Forms::ToolStripSeparator());
      this->toolStripLabel8 = (gcnew System::Windows::Forms::ToolStripLabel());
      this->TextFilter = (gcnew System::Windows::Forms::ToolStripTextBox());
      this->toolStripSeparator8 = (gcnew System::Windows::Forms::ToolStripSeparator());
      this->ClearFiltersButton = (gcnew System::Windows::Forms::ToolStripButton());
      this->toolStrip1->SuspendLayout();
      this->PGameDBMenu->SuspendLayout();
      this->splitContainer1->Panel1->SuspendLayout();
      this->splitContainer1->Panel2->SuspendLayout();
      this->splitContainer1->SuspendLayout();
      this->PGameDBStatusStrip->SuspendLayout();
      this->toolStrip2->SuspendLayout();
      this->SuspendLayout();
      // 
      // toolStrip1
      // 
      this->toolStrip1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(133)), static_cast<System::Int32>(static_cast<System::Byte>(161)), 
        static_cast<System::Int32>(static_cast<System::Byte>(192)));
      this->toolStrip1->ImageScalingSize = System::Drawing::Size(24, 24);
      this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(12) {this->AddGameAutoButton, 
        this->ManualAddGameButton, this->MobyUpdateButton, this->toolStripSeparator2, this->TransferCategoryButton, this->AddGameCategoryButton, 
        this->toolStripSeparator3, this->toolStripLabel1, this->ViewModeDropDownButton, this->toolStripSeparator4, this->RandomButton, 
        this->toolStripSeparator6});
      this->toolStrip1->Location = System::Drawing::Point(0, 24);
      this->toolStrip1->Name = L"toolStrip1";
      this->toolStrip1->Size = System::Drawing::Size(1074, 31);
      this->toolStrip1->TabIndex = 0;
      this->toolStrip1->Text = L"toolStrip1";
      // 
      // AddGameAutoButton
      // 
      this->AddGameAutoButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
      this->AddGameAutoButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"AddGameAutoButton.Image")));
      this->AddGameAutoButton->ImageTransparentColor = System::Drawing::Color::Black;
      this->AddGameAutoButton->Name = L"AddGameAutoButton";
      this->AddGameAutoButton->Size = System::Drawing::Size(28, 28);
      this->AddGameAutoButton->Text = L"Add Game Automatically";
      this->AddGameAutoButton->Click += gcnew System::EventHandler(this, &MainForm::AddGameAutoButton_Click);
      // 
      // ManualAddGameButton
      // 
      this->ManualAddGameButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
      this->ManualAddGameButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"ManualAddGameButton.Image")));
      this->ManualAddGameButton->ImageTransparentColor = System::Drawing::Color::Black;
      this->ManualAddGameButton->Name = L"ManualAddGameButton";
      this->ManualAddGameButton->Size = System::Drawing::Size(28, 28);
      this->ManualAddGameButton->Text = L"Add Game Manually";
      this->ManualAddGameButton->Click += gcnew System::EventHandler(this, &MainForm::ManualAddGameButton_Click);
      // 
      // MobyUpdateButton
      // 
      this->MobyUpdateButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
      this->MobyUpdateButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"MobyUpdateButton.Image")));
      this->MobyUpdateButton->ImageTransparentColor = System::Drawing::Color::Black;
      this->MobyUpdateButton->Name = L"MobyUpdateButton";
      this->MobyUpdateButton->Size = System::Drawing::Size(28, 28);
      this->MobyUpdateButton->Text = L"Update From MobyGames";
      this->MobyUpdateButton->Click += gcnew System::EventHandler(this, &MainForm::MobyUpdateButton_Click);
      // 
      // toolStripSeparator2
      // 
      this->toolStripSeparator2->Margin = System::Windows::Forms::Padding(10, 0, 10, 0);
      this->toolStripSeparator2->Name = L"toolStripSeparator2";
      this->toolStripSeparator2->Size = System::Drawing::Size(6, 31);
      // 
      // TransferCategoryButton
      // 
      this->TransferCategoryButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
      this->TransferCategoryButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"TransferCategoryButton.Image")));
      this->TransferCategoryButton->ImageTransparentColor = System::Drawing::Color::White;
      this->TransferCategoryButton->Name = L"TransferCategoryButton";
      this->TransferCategoryButton->Size = System::Drawing::Size(28, 28);
      this->TransferCategoryButton->Text = L"TransferCategoryButton";
      this->TransferCategoryButton->ToolTipText = L"Add Category to Custom";
      this->TransferCategoryButton->Click += gcnew System::EventHandler(this, &MainForm::TransferCategoryButton_Click);
      // 
      // AddGameCategoryButton
      // 
      this->AddGameCategoryButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
      this->AddGameCategoryButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"AddGameCategoryButton.Image")));
      this->AddGameCategoryButton->ImageTransparentColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), 
        static_cast<System::Int32>(static_cast<System::Byte>(163)), static_cast<System::Int32>(static_cast<System::Byte>(177)));
      this->AddGameCategoryButton->Name = L"AddGameCategoryButton";
      this->AddGameCategoryButton->Size = System::Drawing::Size(28, 28);
      this->AddGameCategoryButton->Text = L"toolStripButton1";
      this->AddGameCategoryButton->ToolTipText = L"Add Game Category";
      this->AddGameCategoryButton->Click += gcnew System::EventHandler(this, &MainForm::AddGameCategoryButton_Click);
      // 
      // toolStripSeparator3
      // 
      this->toolStripSeparator3->Margin = System::Windows::Forms::Padding(5, 0, 0, 0);
      this->toolStripSeparator3->Name = L"toolStripSeparator3";
      this->toolStripSeparator3->Size = System::Drawing::Size(6, 31);
      // 
      // toolStripLabel1
      // 
      this->toolStripLabel1->Margin = System::Windows::Forms::Padding(5, 1, 0, 2);
      this->toolStripLabel1->Name = L"toolStripLabel1";
      this->toolStripLabel1->Size = System::Drawing::Size(69, 28);
      this->toolStripLabel1->Text = L"View Mode:";
      // 
      // ViewModeDropDownButton
      // 
      this->ViewModeDropDownButton->AutoSize = false;
      this->ViewModeDropDownButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
      this->ViewModeDropDownButton->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->UnsortedToolStripMenuItem, 
        this->CategoryToolStripMenuItem, this->CustomToolStripMenuItem});
      this->ViewModeDropDownButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"ViewModeDropDownButton.Image")));
      this->ViewModeDropDownButton->ImageTransparentColor = System::Drawing::Color::Magenta;
      this->ViewModeDropDownButton->Margin = System::Windows::Forms::Padding(-3, 1, 0, 2);
      this->ViewModeDropDownButton->Name = L"ViewModeDropDownButton";
      this->ViewModeDropDownButton->Size = System::Drawing::Size(68, 28);
      this->ViewModeDropDownButton->Text = L"Unsorted";
      this->ViewModeDropDownButton->ToolTipText = L"Viewing Mode";
      // 
      // UnsortedToolStripMenuItem
      // 
      this->UnsortedToolStripMenuItem->Name = L"UnsortedToolStripMenuItem";
      this->UnsortedToolStripMenuItem->Size = System::Drawing::Size(122, 22);
      this->UnsortedToolStripMenuItem->Text = L"Unsorted";
      this->UnsortedToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::UnsortedToolStripMenuItem_Click);
      // 
      // CategoryToolStripMenuItem
      // 
      this->CategoryToolStripMenuItem->Name = L"CategoryToolStripMenuItem";
      this->CategoryToolStripMenuItem->Size = System::Drawing::Size(122, 22);
      this->CategoryToolStripMenuItem->Text = L"Category";
      this->CategoryToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::CategoryToolStripMenuItem_Click);
      // 
      // CustomToolStripMenuItem
      // 
      this->CustomToolStripMenuItem->Name = L"CustomToolStripMenuItem";
      this->CustomToolStripMenuItem->Size = System::Drawing::Size(122, 22);
      this->CustomToolStripMenuItem->Text = L"Custom";
      this->CustomToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::CustomToolStripMenuItem_Click);
      // 
      // toolStripSeparator4
      // 
      this->toolStripSeparator4->Margin = System::Windows::Forms::Padding(5, 0, 15, 0);
      this->toolStripSeparator4->Name = L"toolStripSeparator4";
      this->toolStripSeparator4->Size = System::Drawing::Size(6, 31);
      // 
      // RandomButton
      // 
      this->RandomButton->Alignment = System::Windows::Forms::ToolStripItemAlignment::Right;
      this->RandomButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(133)), static_cast<System::Int32>(static_cast<System::Byte>(161)), 
        static_cast<System::Int32>(static_cast<System::Byte>(192)));
      this->RandomButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
      this->RandomButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"RandomButton.Image")));
      this->RandomButton->ImageTransparentColor = System::Drawing::Color::Magenta;
      this->RandomButton->Margin = System::Windows::Forms::Padding(0, 1, 20, 2);
      this->RandomButton->Name = L"RandomButton";
      this->RandomButton->Size = System::Drawing::Size(28, 28);
      this->RandomButton->Text = L"Random";
      this->RandomButton->ToolTipText = L"Select Random Game";
      this->RandomButton->Click += gcnew System::EventHandler(this, &MainForm::RandomButton_Click);
      // 
      // toolStripSeparator6
      // 
      this->toolStripSeparator6->Alignment = System::Windows::Forms::ToolStripItemAlignment::Right;
      this->toolStripSeparator6->Margin = System::Windows::Forms::Padding(0, 0, 10, 0);
      this->toolStripSeparator6->Name = L"toolStripSeparator6";
      this->toolStripSeparator6->Size = System::Drawing::Size(6, 31);
      // 
      // PGameDBMenu
      // 
      this->PGameDBMenu->BackColor = System::Drawing::Color::LightSteelBlue;
      this->PGameDBMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->PGDB_FileMenu, this->editToolStripMenuItem, 
        this->specialToolStripMenuItem, this->aboutToolStripMenuItem});
      this->PGameDBMenu->Location = System::Drawing::Point(0, 0);
      this->PGameDBMenu->Name = L"PGameDBMenu";
      this->PGameDBMenu->Size = System::Drawing::Size(1074, 24);
      this->PGameDBMenu->TabIndex = 1;
      this->PGameDBMenu->Text = L"menuStrip1";
      // 
      // PGDB_FileMenu
      // 
      this->PGDB_FileMenu->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->PGDB_FileMenuNew, 
        this->PGDB_FileMenuLoad, this->PGDB_FileMenuSave, this->SaveAsMenuItem, this->toolStripSeparator1, this->PGDB_FileMenuExit});
      this->PGDB_FileMenu->Name = L"PGDB_FileMenu";
      this->PGDB_FileMenu->Size = System::Drawing::Size(37, 20);
      this->PGDB_FileMenu->Text = L"File";
      // 
      // PGDB_FileMenuNew
      // 
      this->PGDB_FileMenuNew->Name = L"PGDB_FileMenuNew";
      this->PGDB_FileMenuNew->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::N));
      this->PGDB_FileMenuNew->Size = System::Drawing::Size(141, 22);
      this->PGDB_FileMenuNew->Text = L"New";
      this->PGDB_FileMenuNew->Click += gcnew System::EventHandler(this, &MainForm::PGDB_FileMenuNew_Click);
      // 
      // PGDB_FileMenuLoad
      // 
      this->PGDB_FileMenuLoad->Name = L"PGDB_FileMenuLoad";
      this->PGDB_FileMenuLoad->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::L));
      this->PGDB_FileMenuLoad->Size = System::Drawing::Size(141, 22);
      this->PGDB_FileMenuLoad->Text = L"Load";
      this->PGDB_FileMenuLoad->Click += gcnew System::EventHandler(this, &MainForm::PGDB_FileMenuLoad_Click);
      // 
      // PGDB_FileMenuSave
      // 
      this->PGDB_FileMenuSave->Name = L"PGDB_FileMenuSave";
      this->PGDB_FileMenuSave->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::S));
      this->PGDB_FileMenuSave->Size = System::Drawing::Size(141, 22);
      this->PGDB_FileMenuSave->Text = L"Save";
      this->PGDB_FileMenuSave->Click += gcnew System::EventHandler(this, &MainForm::PGDB_FileMenuSave_Click);
      // 
      // SaveAsMenuItem
      // 
      this->SaveAsMenuItem->Name = L"SaveAsMenuItem";
      this->SaveAsMenuItem->Size = System::Drawing::Size(141, 22);
      this->SaveAsMenuItem->Text = L"Save As";
      this->SaveAsMenuItem->Click += gcnew System::EventHandler(this, &MainForm::SaveAsMenuItem_Click);
      // 
      // toolStripSeparator1
      // 
      this->toolStripSeparator1->Name = L"toolStripSeparator1";
      this->toolStripSeparator1->Size = System::Drawing::Size(138, 6);
      // 
      // PGDB_FileMenuExit
      // 
      this->PGDB_FileMenuExit->Name = L"PGDB_FileMenuExit";
      this->PGDB_FileMenuExit->Size = System::Drawing::Size(141, 22);
      this->PGDB_FileMenuExit->Text = L"Exit";
      this->PGDB_FileMenuExit->Click += gcnew System::EventHandler(this, &MainForm::PGDB_FileMenuExit_Click);
      // 
      // editToolStripMenuItem
      // 
      this->editToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->EditCustomCategoriesMenuItem, 
        this->toolStripSeparator5, this->PGDB_EditMenuPreferences});
      this->editToolStripMenuItem->Name = L"editToolStripMenuItem";
      this->editToolStripMenuItem->Size = System::Drawing::Size(39, 20);
      this->editToolStripMenuItem->Text = L"Edit";
      // 
      // EditCustomCategoriesMenuItem
      // 
      this->EditCustomCategoriesMenuItem->Name = L"EditCustomCategoriesMenuItem";
      this->EditCustomCategoriesMenuItem->Size = System::Drawing::Size(175, 22);
      this->EditCustomCategoriesMenuItem->Text = L"Custom Categories";
      this->EditCustomCategoriesMenuItem->Click += gcnew System::EventHandler(this, &MainForm::EditCustomCategoriesMenuItem_Click);
      // 
      // toolStripSeparator5
      // 
      this->toolStripSeparator5->Name = L"toolStripSeparator5";
      this->toolStripSeparator5->Size = System::Drawing::Size(172, 6);
      // 
      // PGDB_EditMenuPreferences
      // 
      this->PGDB_EditMenuPreferences->Name = L"PGDB_EditMenuPreferences";
      this->PGDB_EditMenuPreferences->Size = System::Drawing::Size(175, 22);
      this->PGDB_EditMenuPreferences->Text = L"Preferences";
      this->PGDB_EditMenuPreferences->Click += gcnew System::EventHandler(this, &MainForm::PGDB_EditMenuPreferences_Click);
      // 
      // specialToolStripMenuItem
      // 
      this->specialToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->noScreenshotsToolStripMenuItem});
      this->specialToolStripMenuItem->Name = L"specialToolStripMenuItem";
      this->specialToolStripMenuItem->Size = System::Drawing::Size(56, 20);
      this->specialToolStripMenuItem->Text = L"Special";
      // 
      // noScreenshotsToolStripMenuItem
      // 
      this->noScreenshotsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->SearchForNoScreenshotsToolStripMenuItem});
      this->noScreenshotsToolStripMenuItem->Name = L"noScreenshotsToolStripMenuItem";
      this->noScreenshotsToolStripMenuItem->Size = System::Drawing::Size(109, 22);
      this->noScreenshotsToolStripMenuItem->Text = L"Search";
      // 
      // SearchForNoScreenshotsToolStripMenuItem
      // 
      this->SearchForNoScreenshotsToolStripMenuItem->Name = L"SearchForNoScreenshotsToolStripMenuItem";
      this->SearchForNoScreenshotsToolStripMenuItem->Size = System::Drawing::Size(156, 22);
      this->SearchForNoScreenshotsToolStripMenuItem->Text = L"No Screenshots";
      this->SearchForNoScreenshotsToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::SearchForNoScreenshotsToolStripMenuItem_Click);
      // 
      // aboutToolStripMenuItem
      // 
      this->aboutToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->VersionMenuItem});
      this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
      this->aboutToolStripMenuItem->Size = System::Drawing::Size(52, 20);
      this->aboutToolStripMenuItem->Text = L"About";
      // 
      // VersionMenuItem
      // 
      this->VersionMenuItem->Name = L"VersionMenuItem";
      this->VersionMenuItem->Size = System::Drawing::Size(113, 22);
      this->VersionMenuItem->Text = L"Version";
      this->VersionMenuItem->Click += gcnew System::EventHandler(this, &MainForm::VersionMenuItem_Click);
      // 
      // splitContainer1
      // 
      this->splitContainer1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->splitContainer1->Location = System::Drawing::Point(0, 83);
      this->splitContainer1->Name = L"splitContainer1";
      // 
      // splitContainer1.Panel1
      // 
      this->splitContainer1->Panel1->Controls->Add(this->GameList);
      // 
      // splitContainer1.Panel2
      // 
      this->splitContainer1->Panel2->Controls->Add(this->GameViewer);
      this->splitContainer1->Size = System::Drawing::Size(1074, 454);
      this->splitContainer1->SplitterDistance = 312;
      this->splitContainer1->TabIndex = 3;
      // 
      // GameList
      // 
      this->GameList->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(133)), static_cast<System::Int32>(static_cast<System::Byte>(161)), 
        static_cast<System::Int32>(static_cast<System::Byte>(192)));
      this->GameList->Dock = System::Windows::Forms::DockStyle::Fill;
      this->GameList->HideSelection = false;
      this->GameList->Location = System::Drawing::Point(0, 0);
      this->GameList->Name = L"GameList";
      this->GameList->Size = System::Drawing::Size(312, 454);
      this->GameList->TabIndex = 0;
      this->GameList->AfterCollapse += gcnew System::Windows::Forms::TreeViewEventHandler(this, &MainForm::GameList_AfterCollapse);
      this->GameList->DoubleClick += gcnew System::EventHandler(this, &MainForm::GameList_DoubleClick);
      this->GameList->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &MainForm::GameList_AfterSelect);
      this->GameList->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MainForm::GameList_KeyPress);
      this->GameList->AfterExpand += gcnew System::Windows::Forms::TreeViewEventHandler(this, &MainForm::GameList_AfterExpand);
      // 
      // GameViewer
      // 
      this->GameViewer->AllowWebBrowserDrop = false;
      this->GameViewer->Dock = System::Windows::Forms::DockStyle::Fill;
      this->GameViewer->Location = System::Drawing::Point(0, 0);
      this->GameViewer->MinimumSize = System::Drawing::Size(20, 20);
      this->GameViewer->Name = L"GameViewer";
      this->GameViewer->Size = System::Drawing::Size(758, 454);
      this->GameViewer->TabIndex = 0;
      this->GameViewer->Navigating += gcnew System::Windows::Forms::WebBrowserNavigatingEventHandler(this, &MainForm::NavigatingHandler);
      this->GameViewer->NewWindow += gcnew System::ComponentModel::CancelEventHandler(this, &MainForm::NewWindowHandler);
      this->GameViewer->DocumentCompleted += gcnew System::Windows::Forms::WebBrowserDocumentCompletedEventHandler(this, &MainForm::DocumentCompletedHandler);
      // 
      // PGameDBNumGameLabel
      // 
      this->PGameDBNumGameLabel->Name = L"PGameDBNumGameLabel";
      this->PGameDBNumGameLabel->Size = System::Drawing::Size(110, 17);
      this->PGameDBNumGameLabel->Text = L"Games in Database:";
      // 
      // PGameDBNumGameVal
      // 
      this->PGameDBNumGameVal->Name = L"PGameDBNumGameVal";
      this->PGameDBNumGameVal->Size = System::Drawing::Size(35, 17);
      this->PGameDBNumGameVal->Text = L"NUM";
      // 
      // PGameDBStatusStrip
      // 
      this->PGameDBStatusStrip->BackColor = System::Drawing::Color::LightSteelBlue;
      this->PGameDBStatusStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->PGameDBNumGameLabel, 
        this->PGameDBNumGameVal});
      this->PGameDBStatusStrip->Location = System::Drawing::Point(0, 537);
      this->PGameDBStatusStrip->Name = L"PGameDBStatusStrip";
      this->PGameDBStatusStrip->Size = System::Drawing::Size(1074, 22);
      this->PGameDBStatusStrip->TabIndex = 2;
      this->PGameDBStatusStrip->Text = L"PGameDBStatus";
      // 
      // toolStrip2
      // 
      this->toolStrip2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(133)), static_cast<System::Int32>(static_cast<System::Byte>(161)), 
        static_cast<System::Int32>(static_cast<System::Byte>(192)));
      this->toolStrip2->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(17) {this->toolStripLabel2, 
        this->toolStripLabel4, this->PlatformFilter, this->toolStripSeparator9, this->toolStripLabel5, this->GenreFilter, this->toolStripSeparator10, 
        this->toolStripLabel7, this->YearFilter, this->toolStripSeparator12, this->toolStripLabel3, this->PlayedFilter, this->toolStripSeparator7, 
        this->toolStripLabel8, this->TextFilter, this->toolStripSeparator8, this->ClearFiltersButton});
      this->toolStrip2->Location = System::Drawing::Point(0, 55);
      this->toolStrip2->Name = L"toolStrip2";
      this->toolStrip2->Size = System::Drawing::Size(1074, 25);
      this->toolStrip2->TabIndex = 4;
      this->toolStrip2->Text = L"toolStrip2";
      // 
      // toolStripLabel2
      // 
      this->toolStripLabel2->Name = L"toolStripLabel2";
      this->toolStripLabel2->Size = System::Drawing::Size(49, 22);
      this->toolStripLabel2->Text = L"Filters - ";
      // 
      // toolStripLabel4
      // 
      this->toolStripLabel4->Name = L"toolStripLabel4";
      this->toolStripLabel4->Size = System::Drawing::Size(56, 22);
      this->toolStripLabel4->Text = L"Platform:";
      // 
      // PlatformFilter
      // 
      this->PlatformFilter->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
      this->PlatformFilter->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->FilterPlatformAll, 
        this->FilterPlatformPC, this->FilterPlatformConsole, this->FilterPlatformHandheld, this->FilterPlatformOther});
      this->PlatformFilter->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"PlatformFilter.Image")));
      this->PlatformFilter->ImageTransparentColor = System::Drawing::Color::Magenta;
      this->PlatformFilter->Name = L"PlatformFilter";
      this->PlatformFilter->Size = System::Drawing::Size(34, 22);
      this->PlatformFilter->Text = L"All";
      // 
      // FilterPlatformAll
      // 
      this->FilterPlatformAll->Name = L"FilterPlatformAll";
      this->FilterPlatformAll->Size = System::Drawing::Size(126, 22);
      this->FilterPlatformAll->Text = L"All";
      this->FilterPlatformAll->Click += gcnew System::EventHandler(this, &MainForm::PlatformFilter_Click);
      // 
      // FilterPlatformPC
      // 
      this->FilterPlatformPC->Name = L"FilterPlatformPC";
      this->FilterPlatformPC->Size = System::Drawing::Size(126, 22);
      this->FilterPlatformPC->Text = L"PC";
      this->FilterPlatformPC->Click += gcnew System::EventHandler(this, &MainForm::PlatformFilter_Click);
      // 
      // FilterPlatformConsole
      // 
      this->FilterPlatformConsole->Name = L"FilterPlatformConsole";
      this->FilterPlatformConsole->Size = System::Drawing::Size(126, 22);
      this->FilterPlatformConsole->Text = L"Console";
      this->FilterPlatformConsole->Click += gcnew System::EventHandler(this, &MainForm::PlatformFilter_Click);
      // 
      // FilterPlatformHandheld
      // 
      this->FilterPlatformHandheld->Name = L"FilterPlatformHandheld";
      this->FilterPlatformHandheld->Size = System::Drawing::Size(126, 22);
      this->FilterPlatformHandheld->Text = L"Handheld";
      this->FilterPlatformHandheld->Click += gcnew System::EventHandler(this, &MainForm::PlatformFilter_Click);
      // 
      // FilterPlatformOther
      // 
      this->FilterPlatformOther->Name = L"FilterPlatformOther";
      this->FilterPlatformOther->Size = System::Drawing::Size(126, 22);
      this->FilterPlatformOther->Text = L"Other";
      this->FilterPlatformOther->Click += gcnew System::EventHandler(this, &MainForm::PlatformFilter_Click);
      // 
      // toolStripSeparator9
      // 
      this->toolStripSeparator9->Name = L"toolStripSeparator9";
      this->toolStripSeparator9->Size = System::Drawing::Size(6, 25);
      // 
      // toolStripLabel5
      // 
      this->toolStripLabel5->Name = L"toolStripLabel5";
      this->toolStripLabel5->Size = System::Drawing::Size(41, 22);
      this->toolStripLabel5->Text = L"Genre:";
      // 
      // GenreFilter
      // 
      this->GenreFilter->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
      this->GenreFilter->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->allToolStripMenuItem});
      this->GenreFilter->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"GenreFilter.Image")));
      this->GenreFilter->ImageTransparentColor = System::Drawing::Color::Magenta;
      this->GenreFilter->Name = L"GenreFilter";
      this->GenreFilter->Size = System::Drawing::Size(34, 22);
      this->GenreFilter->Text = L"All";
      // 
      // allToolStripMenuItem
      // 
      this->allToolStripMenuItem->Name = L"allToolStripMenuItem";
      this->allToolStripMenuItem->Size = System::Drawing::Size(88, 22);
      this->allToolStripMenuItem->Text = L"All";
      // 
      // toolStripSeparator10
      // 
      this->toolStripSeparator10->Name = L"toolStripSeparator10";
      this->toolStripSeparator10->Size = System::Drawing::Size(6, 25);
      // 
      // toolStripLabel7
      // 
      this->toolStripLabel7->Name = L"toolStripLabel7";
      this->toolStripLabel7->Size = System::Drawing::Size(33, 22);
      this->toolStripLabel7->Text = L"Year:";
      // 
      // YearFilter
      // 
      this->YearFilter->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
      this->YearFilter->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->allToolStripMenuItem1});
      this->YearFilter->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"YearFilter.Image")));
      this->YearFilter->ImageTransparentColor = System::Drawing::Color::Magenta;
      this->YearFilter->Name = L"YearFilter";
      this->YearFilter->Size = System::Drawing::Size(34, 22);
      this->YearFilter->Text = L"All";
      // 
      // allToolStripMenuItem1
      // 
      this->allToolStripMenuItem1->Name = L"allToolStripMenuItem1";
      this->allToolStripMenuItem1->Size = System::Drawing::Size(88, 22);
      this->allToolStripMenuItem1->Text = L"All";
      this->allToolStripMenuItem1->Click += gcnew System::EventHandler(this, &MainForm::YearFilter_Click);
      // 
      // toolStripSeparator12
      // 
      this->toolStripSeparator12->Name = L"toolStripSeparator12";
      this->toolStripSeparator12->Size = System::Drawing::Size(6, 25);
      // 
      // toolStripLabel3
      // 
      this->toolStripLabel3->Name = L"toolStripLabel3";
      this->toolStripLabel3->Size = System::Drawing::Size(45, 22);
      this->toolStripLabel3->Text = L"Played:";
      // 
      // PlayedFilter
      // 
      this->PlayedFilter->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
      this->PlayedFilter->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->FilterUnplayedAll, 
        this->FilterUnplayedNo, this->FilterUnplayedYes});
      this->PlayedFilter->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"PlayedFilter.Image")));
      this->PlayedFilter->ImageTransparentColor = System::Drawing::Color::Magenta;
      this->PlayedFilter->Name = L"PlayedFilter";
      this->PlayedFilter->Size = System::Drawing::Size(34, 22);
      this->PlayedFilter->Text = L"All";
      // 
      // FilterUnplayedAll
      // 
      this->FilterUnplayedAll->Name = L"FilterUnplayedAll";
      this->FilterUnplayedAll->Size = System::Drawing::Size(152, 22);
      this->FilterUnplayedAll->Text = L"All";
      this->FilterUnplayedAll->Click += gcnew System::EventHandler(this, &MainForm::FilterUnplayed_Click);
      // 
      // FilterUnplayedNo
      // 
      this->FilterUnplayedNo->Name = L"FilterUnplayedNo";
      this->FilterUnplayedNo->Size = System::Drawing::Size(152, 22);
      this->FilterUnplayedNo->Text = L"No";
      this->FilterUnplayedNo->Click += gcnew System::EventHandler(this, &MainForm::FilterUnplayed_Click);
      // 
      // FilterUnplayedYes
      // 
      this->FilterUnplayedYes->Name = L"FilterUnplayedYes";
      this->FilterUnplayedYes->Size = System::Drawing::Size(152, 22);
      this->FilterUnplayedYes->Text = L"Yes";
      this->FilterUnplayedYes->Click += gcnew System::EventHandler(this, &MainForm::FilterUnplayed_Click);
      // 
      // toolStripSeparator7
      // 
      this->toolStripSeparator7->Name = L"toolStripSeparator7";
      this->toolStripSeparator7->Size = System::Drawing::Size(6, 25);
      // 
      // toolStripLabel8
      // 
      this->toolStripLabel8->Name = L"toolStripLabel8";
      this->toolStripLabel8->Size = System::Drawing::Size(32, 22);
      this->toolStripLabel8->Text = L"Text:";
      // 
      // TextFilter
      // 
      this->TextFilter->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Italic));
      this->TextFilter->Name = L"TextFilter";
      this->TextFilter->Size = System::Drawing::Size(100, 25);
      this->TextFilter->Text = L"Search";
      this->TextFilter->Leave += gcnew System::EventHandler(this, &MainForm::SearchToolStripTextBox_FocusLeave);
      this->TextFilter->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MainForm::SearchToolStripTextBox_KeyPress);
      this->TextFilter->Click += gcnew System::EventHandler(this, &MainForm::SearchToolStripTextBox_Click);
      // 
      // toolStripSeparator8
      // 
      this->toolStripSeparator8->Name = L"toolStripSeparator8";
      this->toolStripSeparator8->Size = System::Drawing::Size(6, 25);
      // 
      // ClearFiltersButton
      // 
      this->ClearFiltersButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(153)), 
        static_cast<System::Int32>(static_cast<System::Byte>(181)), static_cast<System::Int32>(static_cast<System::Byte>(212)));
      this->ClearFiltersButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
      this->ClearFiltersButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"ClearFiltersButton.Image")));
      this->ClearFiltersButton->ImageTransparentColor = System::Drawing::Color::Magenta;
      this->ClearFiltersButton->Margin = System::Windows::Forms::Padding(10, 1, 0, 2);
      this->ClearFiltersButton->Name = L"ClearFiltersButton";
      this->ClearFiltersButton->Size = System::Drawing::Size(72, 22);
      this->ClearFiltersButton->Text = L"Clear Filters";
      this->ClearFiltersButton->Click += gcnew System::EventHandler(this, &MainForm::ClearFiltersButton_Click);
      // 
      // MainForm
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->BackColor = System::Drawing::SystemColors::Control;
      this->ClientSize = System::Drawing::Size(1074, 559);
      this->Controls->Add(this->toolStrip2);
      this->Controls->Add(this->splitContainer1);
      this->Controls->Add(this->PGameDBStatusStrip);
      this->Controls->Add(this->toolStrip1);
      this->Controls->Add(this->PGameDBMenu);
      this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
      this->MainMenuStrip = this->PGameDBMenu;
      this->Name = L"MainForm";
      this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
      this->Text = L"PGameDatabase";
      this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
      this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MainForm::MainFormClosing);
      this->toolStrip1->ResumeLayout(false);
      this->toolStrip1->PerformLayout();
      this->PGameDBMenu->ResumeLayout(false);
      this->PGameDBMenu->PerformLayout();
      this->splitContainer1->Panel1->ResumeLayout(false);
      this->splitContainer1->Panel2->ResumeLayout(false);
      this->splitContainer1->ResumeLayout(false);
      this->PGameDBStatusStrip->ResumeLayout(false);
      this->PGameDBStatusStrip->PerformLayout();
      this->toolStrip2->ResumeLayout(false);
      this->toolStrip2->PerformLayout();
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion


  private:
    System::Void PGDB_FileMenuExit_Click(System::Object^  sender, System::EventArgs^  e) { Close(); }

    // Display version info
    System::Void VersionMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
      char buf[200];
      sprintf(buf, "PGameDB Version %s\nWritten by: Centicosm Software", PGameDBVersion);
      MessageBox::Show(gcnew String(buf));
    }

    // Handle viewing a game
    System::Void GameList_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e);

    // all these functions are concerned about making a game description appear when the name is clicked
    System::Void NavigatingHandler(System::Object^ sender, System::Windows::Forms::WebBrowserNavigatingEventArgs ^e);
    System::Void DocumentCompletedHandler(System::Object^ sender, System::Windows::Forms::WebBrowserDocumentCompletedEventArgs ^e);
    System::Void LinkClickedHandler(System::Object^ sender,  System::Windows::Forms::HtmlElementEventArgs ^e);
    System::Void NewWindowHandler(System::Object^ sender, System::ComponentModel::CancelEventArgs ^e);
    System::Void GameList_DoubleClick(System::Object^  sender, System::EventArgs^  e);

    // Handle various interface and menu functions
    System::Void PGDB_EditMenuPreferences_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void AddGameCategoryButton_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void TransferCategoryButton_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void RandomButton_Click(System::Object^  sender, System::EventArgs^  e);

    
    System::Void GameList_AfterCollapse(System::Object^ sender, TreeViewEventArgs ^e);
    System::Void GameList_AfterExpand(System::Object^ sender, TreeViewEventArgs ^e);

    // searching related methods
    System::Void SearchToolStripTextBox_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void SearchToolStripTextBox_FocusLeave(System::Object^  sender, System::EventArgs^  e);
    System::Void SearchToolStripTextBox_KeyPress(System::Object ^ sender, System::Windows::Forms::KeyPressEventArgs^  e);
    System::Void SearchForNoScreenshotsToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);

    // methods related to various game view filters
    System::Void EditCustomCategoriesMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void UnsortedToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void CategoryToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void CustomToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
    bool Is_GameInCategory(GameDescriptor ^gd, String ^cat);
    System::Void Generate_Categories(GameDescriptor ^gd);
    void Populate_UnsortedView(void);
    void Populate_CategoryView(void);
    void Populate_CustomView(void);
    System::Void Load_Platforms(void);
    System::Void PlatformFilter_Click(System::Object^  sender, System::EventArgs^  e);
    bool Is_GameOnPlatform(GameDescriptor ^gd);
    bool Is_GameInGenre(GameDescriptor ^gd);
    bool Is_GameInYear(GameDescriptor ^gd);
    bool Does_GameHaveText(GameDescriptor ^gd);
    bool MainForm::Is_GamePlayed(GameDescriptor ^gd);
    System::Void Load_GenreFilterDropDown(void);
    System::Void GenreFilter_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void YearFilter_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void Load_YearFilterDropDown(void);
    System::Void ClearFiltersButton_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void FilterUnplayed_Click(System::Object^  sender, System::EventArgs^  e);



    // Handle adding a game to the db
    System::Void AddGameAutoButton_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void Add_GameToGameList(TreeNode ^tn, GameDescriptor ^gd);
    System::Void ManualAddGameButton_Click(System::Object^  sender, System::EventArgs^  e);

    // update the selected game
    System::Void MobyUpdateButton_Click(System::Object^  sender, System::EventArgs^  e);

    // handle rendering the game description
    System::String ^Generate_TitleImageHTML(GameDescriptor ^gd);
    System::String ^Generate_BackCoverImageHTML(GameDescriptor ^gd);
    System::String ^Generate_RatingHtml(GameDescriptor ^gd);
    System::String ^Generate_ScreenShotHTML(GameDescriptor ^gd);
    System::String ^Generate_CustomCategoryHTML(GameDescriptor ^gd);

    // handle saving/loading databases
    System::Void Load_DefaultDatabase(void);
    System::Void Load_Database(String ^db);
    System::Void Load_DatabaseFromName(String ^dbname);
    System::Void PGDB_FileMenuNew_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void PGDB_FileMenuSave_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void SaveAsMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void PGDB_FileMenuLoad_Click(System::Object^  sender, System::EventArgs^  e);
	System::Void Init_DefaultDB(void);

    // handle keyboard navigation
    System::Void GameList_KeyPress(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);

    // misc methods
    System::Void Select_GameByName(String ^gamename);
    System::Void MainFormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e);
    System::Void Display_Games(void);



    // member variables
    System::String ^URL;
    int MaxSearchMatches;

    DBCats ^DBCategories;
    GameDescriptor ^CurrentGameDescriptor;

    LinkedList<GameDescriptor ^> MasterList;  // master list of games

    String ^CurrentOpenDB;                    // current database being used
    TreeNode ^SearchNode;                     // current search view

    LinkedList<String ^> ActiveCategories;    // list of the categories currently being viewed
    LinkedList<String ^> ExpandedCategories;  // list of the categories that are currently expanded
    LinkedList<String ^> CustomViewCategories;    // list of the users custom categories

    String ^SelectedCategory;                 // category currently selected

    String ^CurPlatormFilter;
    LinkedList<String ^> PCPlatforms;         // various platforms under each type used for filtering
    LinkedList<String ^> HandheldPlatforms;
    LinkedList<String ^> ConsolePlatforms;
    LinkedList<String ^> OtherPlatforms;

    bool Modified;                            // has the db been modified since last save

    bool ChangedName;                         // used to detect changes in game names
    TreeNode ^StoredNode;

    bool DoneLoadingGamePage;                 // hacky flag used to prevent rendering errors

    Random ^RandNumGen;

    Assembly ^ScrapeAssem;                    // Variables to store the scraper dll data
    Type ^ScrapeType;
    MethodInfo ^ScrapeGetFields;
    MethodInfo ^ScrapeGetMatchList;
    Object ^GameScraper;
   
    bool PlatformFilterOn;
    bool GenreFilterOn;
    bool YearFilterOn;
    bool TextFilterOn;
    bool PlayedFilterOn;


};
}

