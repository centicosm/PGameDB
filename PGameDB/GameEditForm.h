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


#pragma once

#include "PGameDB.h"
#include "MobySelectionForm.h"
#include "GameDataStructs.h"
#include "WaitForm.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::IO;
using namespace System::Reflection;


namespace PGameDB {

  public ref class GameEditForm : public System::Windows::Forms::Form
  {
  public:
    GameEditForm(DBCats ^cats) {

      // we need the category info so we can build the combo boxes correctly
      DBCategories = cats;
      const char *msr = gConfMan->Get_ConfigItem("MaxSearchResults");
      if (msr != NULL) {
        MaxSearchMatches = atoi(msr);
      }
      else {
        MaxSearchMatches = 30;
      }
      InitializeComponent();

      // load up the appropriate scraper
      const char *dll = gConfMan->Get_ConfigItem("Scraper");
      String ^pth = String::Concat(Directory::GetCurrentDirectory(), "\\");
      pth = String::Concat(pth, gcnew String(dll));
      ScrapeAssem = Assembly::LoadFrom(pth);
      ScrapeType = ScrapeAssem->GetType("Scraper");
      ScrapeGetFields = ScrapeType->GetMethod("Get_Fields");
      ScrapeGetMatchList = ScrapeType->GetMethod("Get_MatchList");
      GameScraper = Activator::CreateInstance(ScrapeType);


    }

  protected:
    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    ~GameEditForm()
    {
      if (components)
      {
        delete components;
      }
    }
  private: System::Windows::Forms::SplitContainer^  splitContainer1;
  private: System::Windows::Forms::TabControl^  GameEditTabControl;
  private: System::Windows::Forms::TabPage^  GeneralTabPage;
  private: System::Windows::Forms::Label^  GenreLabel;
  private: System::Windows::Forms::CheckedListBox^  GenreCheckedListBox;
  private: System::Windows::Forms::ComboBox^  SeriesComboBox;
  private: System::Windows::Forms::ComboBox^  PublisherComboBox;
  private: System::Windows::Forms::ComboBox^  DeveloperComboBox;
  private: System::Windows::Forms::Label^  SeriesLabel;
  private: System::Windows::Forms::Label^  PublisherLabel;
  private: System::Windows::Forms::Label^  DeveloperLabel;
  private: System::Windows::Forms::Label^  TitleLabel;
  private: System::Windows::Forms::TextBox^  TitleTextBox;
  private: System::Windows::Forms::TabPage^  PersonalTabPage;
  private: System::Windows::Forms::Button^  GameEditOkButton;
  private: System::Windows::Forms::Button^  GameEditCancelButton;
  private: System::Windows::Forms::Button^  GameEditUpdateButton;
  private: System::Windows::Forms::Label^  AlternatePlatformsLabel;
  private: System::Windows::Forms::CheckedListBox^  AlternatePlatformsCheckedListBox;
  private: System::Windows::Forms::Label^  PrimaryPlatformLabel;
  private: System::Windows::Forms::ComboBox^  PrimaryPlatformComboBox;
  private: System::Windows::Forms::TabPage^  MediaTabPage;
  private: System::Windows::Forms::ComboBox^  Location4ComboBox;
  private: System::Windows::Forms::Label^  Location4Label;
  private: System::Windows::Forms::ComboBox^  Location3ComboBox;
  private: System::Windows::Forms::Label^  Location3Label;
  private: System::Windows::Forms::ComboBox^  Location2ComboBox;
  private: System::Windows::Forms::Label^  Location2Label;
  private: System::Windows::Forms::ComboBox^  Location1ComboBox;
  private: System::Windows::Forms::Label^  Location1Label;
  private: System::Windows::Forms::Label^  YearCompletedLabel;
  private: System::Windows::Forms::ComboBox^  RatingComboBox;
  private: System::Windows::Forms::Label^  RatingLabel;
  private: System::Windows::Forms::ComboBox^  CompletedComboBox;
  private: System::Windows::Forms::Label^  CompletedLabel;
  private: System::Windows::Forms::Label^  ScreenShotsLabel;
  private: System::Windows::Forms::ListBox^  ScreenShotListBox;
  private: System::Windows::Forms::TextBox^  FrontCoverTextBox;
  private: System::Windows::Forms::Label^  FrontCoverLabel;
  private: System::Windows::Forms::Button^  RemoveScreenShotButton;
  private: System::Windows::Forms::Button^  AddScreenShotButton;
  private: System::Windows::Forms::TextBox^  YearCompletedTextBox;
  private: System::Windows::Forms::TextBox^  ReleaseYearTextBox;
  private: System::Windows::Forms::Label^  ReleaseYearLabel;
  private: System::Windows::Forms::Label^  BackCoverLabel;
  private: System::Windows::Forms::TextBox^  BackCoverTextBox;
  private: System::Windows::Forms::Button^  ChooseBackCoverButton;
  private: System::Windows::Forms::Button^  ChooseFrontCoverButton;
  private: System::Windows::Forms::Button^  EditPlatformButton;
  private: System::Windows::Forms::Button^  EditGenreButton;
  private: System::Windows::Forms::Button^  ViewWebPageButton;
  private: System::Windows::Forms::TabPage^  DescriptionTabPage;
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::TextBox^  URLTextBox;
  private: System::Windows::Forms::RichTextBox^  NotesRichTextBox;
  private: System::Windows::Forms::Label^  NotesLabel;
  private: System::Windows::Forms::RichTextBox^  DescriptionRichTextBox;
  private: System::Windows::Forms::Label^  DescriptionLabel;
  private: System::Windows::Forms::Label^  label3;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::ListBox^  GuideListBox;
  private: System::Windows::Forms::ListBox^  DocumentationListBox;
  private: System::Windows::Forms::PictureBox^  GameEditPicture;
  private: System::Windows::Forms::Button^  RemoveGuideButton;
  private: System::Windows::Forms::Button^  AddGuideButton;
  private: System::Windows::Forms::Button^  RemoveDocumentationButton;
  private: System::Windows::Forms::Button^  AddDocumentationButton;
  private: System::Windows::Forms::Label^  CategoresLabel;
  private: System::Windows::Forms::Button^  CategoryEditButton;
  private: System::Windows::Forms::CheckedListBox^  CategoriesCheckedListBox;
  private: System::Windows::Forms::Button^  EditGuideButton;
  private: System::Windows::Forms::Button^  EditDocumentationButton;

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
      this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
      this->GameEditTabControl = (gcnew System::Windows::Forms::TabControl());
      this->GeneralTabPage = (gcnew System::Windows::Forms::TabPage());
      this->EditPlatformButton = (gcnew System::Windows::Forms::Button());
      this->EditGenreButton = (gcnew System::Windows::Forms::Button());
      this->ReleaseYearTextBox = (gcnew System::Windows::Forms::TextBox());
      this->ReleaseYearLabel = (gcnew System::Windows::Forms::Label());
      this->AlternatePlatformsLabel = (gcnew System::Windows::Forms::Label());
      this->AlternatePlatformsCheckedListBox = (gcnew System::Windows::Forms::CheckedListBox());
      this->PrimaryPlatformLabel = (gcnew System::Windows::Forms::Label());
      this->PrimaryPlatformComboBox = (gcnew System::Windows::Forms::ComboBox());
      this->GenreLabel = (gcnew System::Windows::Forms::Label());
      this->GenreCheckedListBox = (gcnew System::Windows::Forms::CheckedListBox());
      this->SeriesComboBox = (gcnew System::Windows::Forms::ComboBox());
      this->PublisherComboBox = (gcnew System::Windows::Forms::ComboBox());
      this->DeveloperComboBox = (gcnew System::Windows::Forms::ComboBox());
      this->SeriesLabel = (gcnew System::Windows::Forms::Label());
      this->PublisherLabel = (gcnew System::Windows::Forms::Label());
      this->DeveloperLabel = (gcnew System::Windows::Forms::Label());
      this->TitleLabel = (gcnew System::Windows::Forms::Label());
      this->TitleTextBox = (gcnew System::Windows::Forms::TextBox());
      this->DescriptionTabPage = (gcnew System::Windows::Forms::TabPage());
      this->CategoresLabel = (gcnew System::Windows::Forms::Label());
      this->CategoryEditButton = (gcnew System::Windows::Forms::Button());
      this->CategoriesCheckedListBox = (gcnew System::Windows::Forms::CheckedListBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->URLTextBox = (gcnew System::Windows::Forms::TextBox());
      this->NotesRichTextBox = (gcnew System::Windows::Forms::RichTextBox());
      this->NotesLabel = (gcnew System::Windows::Forms::Label());
      this->DescriptionRichTextBox = (gcnew System::Windows::Forms::RichTextBox());
      this->DescriptionLabel = (gcnew System::Windows::Forms::Label());
      this->MediaTabPage = (gcnew System::Windows::Forms::TabPage());
      this->EditGuideButton = (gcnew System::Windows::Forms::Button());
      this->EditDocumentationButton = (gcnew System::Windows::Forms::Button());
      this->RemoveGuideButton = (gcnew System::Windows::Forms::Button());
      this->AddGuideButton = (gcnew System::Windows::Forms::Button());
      this->RemoveDocumentationButton = (gcnew System::Windows::Forms::Button());
      this->AddDocumentationButton = (gcnew System::Windows::Forms::Button());
      this->GameEditPicture = (gcnew System::Windows::Forms::PictureBox());
      this->label3 = (gcnew System::Windows::Forms::Label());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->GuideListBox = (gcnew System::Windows::Forms::ListBox());
      this->DocumentationListBox = (gcnew System::Windows::Forms::ListBox());
      this->ChooseBackCoverButton = (gcnew System::Windows::Forms::Button());
      this->ChooseFrontCoverButton = (gcnew System::Windows::Forms::Button());
      this->BackCoverLabel = (gcnew System::Windows::Forms::Label());
      this->BackCoverTextBox = (gcnew System::Windows::Forms::TextBox());
      this->RemoveScreenShotButton = (gcnew System::Windows::Forms::Button());
      this->AddScreenShotButton = (gcnew System::Windows::Forms::Button());
      this->ScreenShotsLabel = (gcnew System::Windows::Forms::Label());
      this->ScreenShotListBox = (gcnew System::Windows::Forms::ListBox());
      this->FrontCoverTextBox = (gcnew System::Windows::Forms::TextBox());
      this->FrontCoverLabel = (gcnew System::Windows::Forms::Label());
      this->PersonalTabPage = (gcnew System::Windows::Forms::TabPage());
      this->YearCompletedTextBox = (gcnew System::Windows::Forms::TextBox());
      this->YearCompletedLabel = (gcnew System::Windows::Forms::Label());
      this->RatingComboBox = (gcnew System::Windows::Forms::ComboBox());
      this->RatingLabel = (gcnew System::Windows::Forms::Label());
      this->CompletedComboBox = (gcnew System::Windows::Forms::ComboBox());
      this->CompletedLabel = (gcnew System::Windows::Forms::Label());
      this->Location4ComboBox = (gcnew System::Windows::Forms::ComboBox());
      this->Location4Label = (gcnew System::Windows::Forms::Label());
      this->Location3ComboBox = (gcnew System::Windows::Forms::ComboBox());
      this->Location3Label = (gcnew System::Windows::Forms::Label());
      this->Location2ComboBox = (gcnew System::Windows::Forms::ComboBox());
      this->Location2Label = (gcnew System::Windows::Forms::Label());
      this->Location1ComboBox = (gcnew System::Windows::Forms::ComboBox());
      this->Location1Label = (gcnew System::Windows::Forms::Label());
      this->ViewWebPageButton = (gcnew System::Windows::Forms::Button());
      this->GameEditCancelButton = (gcnew System::Windows::Forms::Button());
      this->GameEditUpdateButton = (gcnew System::Windows::Forms::Button());
      this->GameEditOkButton = (gcnew System::Windows::Forms::Button());
      this->splitContainer1->Panel1->SuspendLayout();
      this->splitContainer1->Panel2->SuspendLayout();
      this->splitContainer1->SuspendLayout();
      this->GameEditTabControl->SuspendLayout();
      this->GeneralTabPage->SuspendLayout();
      this->DescriptionTabPage->SuspendLayout();
      this->MediaTabPage->SuspendLayout();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GameEditPicture))->BeginInit();
      this->PersonalTabPage->SuspendLayout();
      this->SuspendLayout();
      // 
      // splitContainer1
      // 
      this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
      this->splitContainer1->Location = System::Drawing::Point(0, 0);
      this->splitContainer1->Name = L"splitContainer1";
      this->splitContainer1->Orientation = System::Windows::Forms::Orientation::Horizontal;
      // 
      // splitContainer1.Panel1
      // 
      this->splitContainer1->Panel1->Controls->Add(this->GameEditTabControl);
      // 
      // splitContainer1.Panel2
      // 
      this->splitContainer1->Panel2->BackColor = System::Drawing::Color::Transparent;
      this->splitContainer1->Panel2->Controls->Add(this->ViewWebPageButton);
      this->splitContainer1->Panel2->Controls->Add(this->GameEditCancelButton);
      this->splitContainer1->Panel2->Controls->Add(this->GameEditUpdateButton);
      this->splitContainer1->Panel2->Controls->Add(this->GameEditOkButton);
      this->splitContainer1->Size = System::Drawing::Size(557, 388);
      this->splitContainer1->SplitterDistance = 353;
      this->splitContainer1->SplitterWidth = 1;
      this->splitContainer1->TabIndex = 0;
      // 
      // GameEditTabControl
      // 
      this->GameEditTabControl->Controls->Add(this->GeneralTabPage);
      this->GameEditTabControl->Controls->Add(this->DescriptionTabPage);
      this->GameEditTabControl->Controls->Add(this->MediaTabPage);
      this->GameEditTabControl->Controls->Add(this->PersonalTabPage);
      this->GameEditTabControl->Dock = System::Windows::Forms::DockStyle::Fill;
      this->GameEditTabControl->Location = System::Drawing::Point(0, 0);
      this->GameEditTabControl->Name = L"GameEditTabControl";
      this->GameEditTabControl->SelectedIndex = 0;
      this->GameEditTabControl->Size = System::Drawing::Size(557, 353);
      this->GameEditTabControl->TabIndex = 1;
      // 
      // GeneralTabPage
      // 
      this->GeneralTabPage->BackColor = System::Drawing::Color::Transparent;
      this->GeneralTabPage->Controls->Add(this->EditPlatformButton);
      this->GeneralTabPage->Controls->Add(this->EditGenreButton);
      this->GeneralTabPage->Controls->Add(this->ReleaseYearTextBox);
      this->GeneralTabPage->Controls->Add(this->ReleaseYearLabel);
      this->GeneralTabPage->Controls->Add(this->AlternatePlatformsLabel);
      this->GeneralTabPage->Controls->Add(this->AlternatePlatformsCheckedListBox);
      this->GeneralTabPage->Controls->Add(this->PrimaryPlatformLabel);
      this->GeneralTabPage->Controls->Add(this->PrimaryPlatformComboBox);
      this->GeneralTabPage->Controls->Add(this->GenreLabel);
      this->GeneralTabPage->Controls->Add(this->GenreCheckedListBox);
      this->GeneralTabPage->Controls->Add(this->SeriesComboBox);
      this->GeneralTabPage->Controls->Add(this->PublisherComboBox);
      this->GeneralTabPage->Controls->Add(this->DeveloperComboBox);
      this->GeneralTabPage->Controls->Add(this->SeriesLabel);
      this->GeneralTabPage->Controls->Add(this->PublisherLabel);
      this->GeneralTabPage->Controls->Add(this->DeveloperLabel);
      this->GeneralTabPage->Controls->Add(this->TitleLabel);
      this->GeneralTabPage->Controls->Add(this->TitleTextBox);
      this->GeneralTabPage->Location = System::Drawing::Point(4, 22);
      this->GeneralTabPage->Name = L"GeneralTabPage";
      this->GeneralTabPage->Padding = System::Windows::Forms::Padding(3);
      this->GeneralTabPage->Size = System::Drawing::Size(549, 327);
      this->GeneralTabPage->TabIndex = 0;
      this->GeneralTabPage->Text = L"General";
      // 
      // EditPlatformButton
      // 
      this->EditPlatformButton->Location = System::Drawing::Point(393, 298);
      this->EditPlatformButton->Name = L"EditPlatformButton";
      this->EditPlatformButton->Size = System::Drawing::Size(41, 23);
      this->EditPlatformButton->TabIndex = 20;
      this->EditPlatformButton->Text = L"Edit";
      this->EditPlatformButton->UseVisualStyleBackColor = true;
      this->EditPlatformButton->Click += gcnew System::EventHandler(this, &GameEditForm::EditPlatformButton_Click);
      // 
      // EditGenreButton
      // 
      this->EditGenreButton->Location = System::Drawing::Point(114, 298);
      this->EditGenreButton->Name = L"EditGenreButton";
      this->EditGenreButton->Size = System::Drawing::Size(40, 23);
      this->EditGenreButton->TabIndex = 19;
      this->EditGenreButton->Text = L"Edit";
      this->EditGenreButton->UseVisualStyleBackColor = true;
      this->EditGenreButton->Click += gcnew System::EventHandler(this, &GameEditForm::EditGenreButton_Click);
      // 
      // ReleaseYearTextBox
      // 
      this->ReleaseYearTextBox->BackColor = System::Drawing::SystemColors::Window;
      this->ReleaseYearTextBox->Location = System::Drawing::Point(11, 125);
      this->ReleaseYearTextBox->Name = L"ReleaseYearTextBox";
      this->ReleaseYearTextBox->Size = System::Drawing::Size(245, 20);
      this->ReleaseYearTextBox->TabIndex = 18;
      // 
      // ReleaseYearLabel
      // 
      this->ReleaseYearLabel->AutoSize = true;
      this->ReleaseYearLabel->Location = System::Drawing::Point(8, 109);
      this->ReleaseYearLabel->Name = L"ReleaseYearLabel";
      this->ReleaseYearLabel->Size = System::Drawing::Size(74, 13);
      this->ReleaseYearLabel->TabIndex = 17;
      this->ReleaseYearLabel->Text = L"Release Year:";
      // 
      // AlternatePlatformsLabel
      // 
      this->AlternatePlatformsLabel->AutoSize = true;
      this->AlternatePlatformsLabel->Location = System::Drawing::Point(290, 153);
      this->AlternatePlatformsLabel->Name = L"AlternatePlatformsLabel";
      this->AlternatePlatformsLabel->Size = System::Drawing::Size(98, 13);
      this->AlternatePlatformsLabel->TabIndex = 16;
      this->AlternatePlatformsLabel->Text = L"Alternate Platforms:";
      // 
      // AlternatePlatformsCheckedListBox
      // 
      this->AlternatePlatformsCheckedListBox->BackColor = System::Drawing::SystemColors::Window;
      this->AlternatePlatformsCheckedListBox->CheckOnClick = true;
      this->AlternatePlatformsCheckedListBox->FormattingEnabled = true;
      this->AlternatePlatformsCheckedListBox->Location = System::Drawing::Point(290, 171);
      this->AlternatePlatformsCheckedListBox->Name = L"AlternatePlatformsCheckedListBox";
      this->AlternatePlatformsCheckedListBox->Size = System::Drawing::Size(251, 124);
      this->AlternatePlatformsCheckedListBox->Sorted = true;
      this->AlternatePlatformsCheckedListBox->TabIndex = 15;
      // 
      // PrimaryPlatformLabel
      // 
      this->PrimaryPlatformLabel->AutoSize = true;
      this->PrimaryPlatformLabel->Location = System::Drawing::Point(290, 109);
      this->PrimaryPlatformLabel->Name = L"PrimaryPlatformLabel";
      this->PrimaryPlatformLabel->Size = System::Drawing::Size(85, 13);
      this->PrimaryPlatformLabel->TabIndex = 14;
      this->PrimaryPlatformLabel->Text = L"Primary Platform:";
      // 
      // PrimaryPlatformComboBox
      // 
      this->PrimaryPlatformComboBox->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::SuggestAppend;
      this->PrimaryPlatformComboBox->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::ListItems;
      this->PrimaryPlatformComboBox->BackColor = System::Drawing::SystemColors::Window;
      this->PrimaryPlatformComboBox->FormattingEnabled = true;
      this->PrimaryPlatformComboBox->Location = System::Drawing::Point(290, 124);
      this->PrimaryPlatformComboBox->Name = L"PrimaryPlatformComboBox";
      this->PrimaryPlatformComboBox->Size = System::Drawing::Size(251, 21);
      this->PrimaryPlatformComboBox->Sorted = true;
      this->PrimaryPlatformComboBox->TabIndex = 13;
      // 
      // GenreLabel
      // 
      this->GenreLabel->AutoSize = true;
      this->GenreLabel->Location = System::Drawing::Point(11, 153);
      this->GenreLabel->Name = L"GenreLabel";
      this->GenreLabel->Size = System::Drawing::Size(39, 13);
      this->GenreLabel->TabIndex = 12;
      this->GenreLabel->Text = L"Genre:";
      // 
      // GenreCheckedListBox
      // 
      this->GenreCheckedListBox->BackColor = System::Drawing::SystemColors::Window;
      this->GenreCheckedListBox->CheckOnClick = true;
      this->GenreCheckedListBox->FormattingEnabled = true;
      this->GenreCheckedListBox->Location = System::Drawing::Point(8, 171);
      this->GenreCheckedListBox->Name = L"GenreCheckedListBox";
      this->GenreCheckedListBox->Size = System::Drawing::Size(248, 124);
      this->GenreCheckedListBox->TabIndex = 11;
      // 
      // SeriesComboBox
      // 
      this->SeriesComboBox->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::SuggestAppend;
      this->SeriesComboBox->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::ListItems;
      this->SeriesComboBox->BackColor = System::Drawing::SystemColors::Window;
      this->SeriesComboBox->FormattingEnabled = true;
      this->SeriesComboBox->Location = System::Drawing::Point(290, 42);
      this->SeriesComboBox->Name = L"SeriesComboBox";
      this->SeriesComboBox->Size = System::Drawing::Size(251, 21);
      this->SeriesComboBox->Sorted = true;
      this->SeriesComboBox->TabIndex = 10;
      // 
      // PublisherComboBox
      // 
      this->PublisherComboBox->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::SuggestAppend;
      this->PublisherComboBox->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::ListItems;
      this->PublisherComboBox->BackColor = System::Drawing::SystemColors::Window;
      this->PublisherComboBox->FormattingEnabled = true;
      this->PublisherComboBox->Location = System::Drawing::Point(290, 81);
      this->PublisherComboBox->Name = L"PublisherComboBox";
      this->PublisherComboBox->Size = System::Drawing::Size(251, 21);
      this->PublisherComboBox->Sorted = true;
      this->PublisherComboBox->TabIndex = 9;
      // 
      // DeveloperComboBox
      // 
      this->DeveloperComboBox->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::SuggestAppend;
      this->DeveloperComboBox->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::ListItems;
      this->DeveloperComboBox->BackColor = System::Drawing::SystemColors::Window;
      this->DeveloperComboBox->FormattingEnabled = true;
      this->DeveloperComboBox->Location = System::Drawing::Point(11, 81);
      this->DeveloperComboBox->Name = L"DeveloperComboBox";
      this->DeveloperComboBox->Size = System::Drawing::Size(245, 21);
      this->DeveloperComboBox->Sorted = true;
      this->DeveloperComboBox->TabIndex = 8;
      // 
      // SeriesLabel
      // 
      this->SeriesLabel->AutoSize = true;
      this->SeriesLabel->Location = System::Drawing::Point(290, 27);
      this->SeriesLabel->Name = L"SeriesLabel";
      this->SeriesLabel->Size = System::Drawing::Size(39, 13);
      this->SeriesLabel->TabIndex = 7;
      this->SeriesLabel->Text = L"Series:";
      // 
      // PublisherLabel
      // 
      this->PublisherLabel->AutoSize = true;
      this->PublisherLabel->Location = System::Drawing::Point(287, 65);
      this->PublisherLabel->Name = L"PublisherLabel";
      this->PublisherLabel->Size = System::Drawing::Size(53, 13);
      this->PublisherLabel->TabIndex = 5;
      this->PublisherLabel->Text = L"Publisher:";
      // 
      // DeveloperLabel
      // 
      this->DeveloperLabel->AutoSize = true;
      this->DeveloperLabel->Location = System::Drawing::Point(8, 65);
      this->DeveloperLabel->Name = L"DeveloperLabel";
      this->DeveloperLabel->Size = System::Drawing::Size(59, 13);
      this->DeveloperLabel->TabIndex = 4;
      this->DeveloperLabel->Text = L"Developer:";
      // 
      // TitleLabel
      // 
      this->TitleLabel->AutoSize = true;
      this->TitleLabel->Location = System::Drawing::Point(8, 27);
      this->TitleLabel->Name = L"TitleLabel";
      this->TitleLabel->Size = System::Drawing::Size(30, 13);
      this->TitleLabel->TabIndex = 1;
      this->TitleLabel->Text = L"Title:";
      // 
      // TitleTextBox
      // 
      this->TitleTextBox->BackColor = System::Drawing::SystemColors::Window;
      this->TitleTextBox->Location = System::Drawing::Point(11, 42);
      this->TitleTextBox->Name = L"TitleTextBox";
      this->TitleTextBox->Size = System::Drawing::Size(245, 20);
      this->TitleTextBox->TabIndex = 0;
      // 
      // DescriptionTabPage
      // 
      this->DescriptionTabPage->Controls->Add(this->CategoresLabel);
      this->DescriptionTabPage->Controls->Add(this->CategoryEditButton);
      this->DescriptionTabPage->Controls->Add(this->CategoriesCheckedListBox);
      this->DescriptionTabPage->Controls->Add(this->label1);
      this->DescriptionTabPage->Controls->Add(this->URLTextBox);
      this->DescriptionTabPage->Controls->Add(this->NotesRichTextBox);
      this->DescriptionTabPage->Controls->Add(this->NotesLabel);
      this->DescriptionTabPage->Controls->Add(this->DescriptionRichTextBox);
      this->DescriptionTabPage->Controls->Add(this->DescriptionLabel);
      this->DescriptionTabPage->Location = System::Drawing::Point(4, 22);
      this->DescriptionTabPage->Name = L"DescriptionTabPage";
      this->DescriptionTabPage->Padding = System::Windows::Forms::Padding(3);
      this->DescriptionTabPage->Size = System::Drawing::Size(549, 327);
      this->DescriptionTabPage->TabIndex = 3;
      this->DescriptionTabPage->Text = L"Description";
      this->DescriptionTabPage->UseVisualStyleBackColor = true;
      // 
      // CategoresLabel
      // 
      this->CategoresLabel->AutoSize = true;
      this->CategoresLabel->Location = System::Drawing::Point(373, 21);
      this->CategoresLabel->Name = L"CategoresLabel";
      this->CategoresLabel->Size = System::Drawing::Size(109, 13);
      this->CategoresLabel->TabIndex = 19;
      this->CategoresLabel->Text = L"Category Membership";
      // 
      // CategoryEditButton
      // 
      this->CategoryEditButton->Location = System::Drawing::Point(440, 288);
      this->CategoryEditButton->Name = L"CategoryEditButton";
      this->CategoryEditButton->Size = System::Drawing::Size(42, 26);
      this->CategoryEditButton->TabIndex = 18;
      this->CategoryEditButton->Text = L"Edit";
      this->CategoryEditButton->UseVisualStyleBackColor = true;
      this->CategoryEditButton->Click += gcnew System::EventHandler(this, &GameEditForm::CategoryEditButton_Click);
      // 
      // CategoriesCheckedListBox
      // 
      this->CategoriesCheckedListBox->FormattingEnabled = true;
      this->CategoriesCheckedListBox->Location = System::Drawing::Point(376, 38);
      this->CategoriesCheckedListBox->Name = L"CategoriesCheckedListBox";
      this->CategoriesCheckedListBox->Size = System::Drawing::Size(164, 244);
      this->CategoriesCheckedListBox->TabIndex = 17;
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(9, 274);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(29, 13);
      this->label1->TabIndex = 14;
      this->label1->Text = L"URL";
      // 
      // URLTextBox
      // 
      this->URLTextBox->Location = System::Drawing::Point(8, 290);
      this->URLTextBox->Name = L"URLTextBox";
      this->URLTextBox->Size = System::Drawing::Size(350, 20);
      this->URLTextBox->TabIndex = 13;
      // 
      // NotesRichTextBox
      // 
      this->NotesRichTextBox->Location = System::Drawing::Point(8, 210);
      this->NotesRichTextBox->Name = L"NotesRichTextBox";
      this->NotesRichTextBox->Size = System::Drawing::Size(350, 61);
      this->NotesRichTextBox->TabIndex = 3;
      this->NotesRichTextBox->Text = L"";
      // 
      // NotesLabel
      // 
      this->NotesLabel->AutoSize = true;
      this->NotesLabel->Location = System::Drawing::Point(9, 194);
      this->NotesLabel->Name = L"NotesLabel";
      this->NotesLabel->Size = System::Drawing::Size(38, 13);
      this->NotesLabel->TabIndex = 2;
      this->NotesLabel->Text = L"Notes:";
      // 
      // DescriptionRichTextBox
      // 
      this->DescriptionRichTextBox->AcceptsTab = true;
      this->DescriptionRichTextBox->AccessibleRole = System::Windows::Forms::AccessibleRole::None;
      this->DescriptionRichTextBox->DetectUrls = false;
      this->DescriptionRichTextBox->Location = System::Drawing::Point(12, 38);
      this->DescriptionRichTextBox->Name = L"DescriptionRichTextBox";
      this->DescriptionRichTextBox->Size = System::Drawing::Size(346, 153);
      this->DescriptionRichTextBox->TabIndex = 1;
      this->DescriptionRichTextBox->Text = L"";
      // 
      // DescriptionLabel
      // 
      this->DescriptionLabel->AutoSize = true;
      this->DescriptionLabel->Location = System::Drawing::Point(9, 21);
      this->DescriptionLabel->Name = L"DescriptionLabel";
      this->DescriptionLabel->Size = System::Drawing::Size(63, 13);
      this->DescriptionLabel->TabIndex = 0;
      this->DescriptionLabel->Text = L"Description:";
      // 
      // MediaTabPage
      // 
      this->MediaTabPage->Controls->Add(this->EditGuideButton);
      this->MediaTabPage->Controls->Add(this->EditDocumentationButton);
      this->MediaTabPage->Controls->Add(this->RemoveGuideButton);
      this->MediaTabPage->Controls->Add(this->AddGuideButton);
      this->MediaTabPage->Controls->Add(this->RemoveDocumentationButton);
      this->MediaTabPage->Controls->Add(this->AddDocumentationButton);
      this->MediaTabPage->Controls->Add(this->GameEditPicture);
      this->MediaTabPage->Controls->Add(this->label3);
      this->MediaTabPage->Controls->Add(this->label2);
      this->MediaTabPage->Controls->Add(this->GuideListBox);
      this->MediaTabPage->Controls->Add(this->DocumentationListBox);
      this->MediaTabPage->Controls->Add(this->ChooseBackCoverButton);
      this->MediaTabPage->Controls->Add(this->ChooseFrontCoverButton);
      this->MediaTabPage->Controls->Add(this->BackCoverLabel);
      this->MediaTabPage->Controls->Add(this->BackCoverTextBox);
      this->MediaTabPage->Controls->Add(this->RemoveScreenShotButton);
      this->MediaTabPage->Controls->Add(this->AddScreenShotButton);
      this->MediaTabPage->Controls->Add(this->ScreenShotsLabel);
      this->MediaTabPage->Controls->Add(this->ScreenShotListBox);
      this->MediaTabPage->Controls->Add(this->FrontCoverTextBox);
      this->MediaTabPage->Controls->Add(this->FrontCoverLabel);
      this->MediaTabPage->Location = System::Drawing::Point(4, 22);
      this->MediaTabPage->Name = L"MediaTabPage";
      this->MediaTabPage->Padding = System::Windows::Forms::Padding(3);
      this->MediaTabPage->Size = System::Drawing::Size(549, 327);
      this->MediaTabPage->TabIndex = 2;
      this->MediaTabPage->Text = L"Media";
      this->MediaTabPage->UseVisualStyleBackColor = true;
      // 
      // EditGuideButton
      // 
      this->EditGuideButton->Location = System::Drawing::Point(379, 296);
      this->EditGuideButton->Name = L"EditGuideButton";
      this->EditGuideButton->Size = System::Drawing::Size(34, 27);
      this->EditGuideButton->TabIndex = 23;
      this->EditGuideButton->Text = L"Edit";
      this->EditGuideButton->UseVisualStyleBackColor = true;
      this->EditGuideButton->Click += gcnew System::EventHandler(this, &GameEditForm::EditGuideButton_Click);
      // 
      // EditDocumentationButton
      // 
      this->EditDocumentationButton->Location = System::Drawing::Point(99, 298);
      this->EditDocumentationButton->Name = L"EditDocumentationButton";
      this->EditDocumentationButton->Size = System::Drawing::Size(34, 26);
      this->EditDocumentationButton->TabIndex = 22;
      this->EditDocumentationButton->Text = L"Edit";
      this->EditDocumentationButton->UseVisualStyleBackColor = true;
      this->EditDocumentationButton->Click += gcnew System::EventHandler(this, &GameEditForm::EditDocumentationButton_Click);
      // 
      // RemoveGuideButton
      // 
      this->RemoveGuideButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F));
      this->RemoveGuideButton->Location = System::Drawing::Point(459, 296);
      this->RemoveGuideButton->Name = L"RemoveGuideButton";
      this->RemoveGuideButton->Size = System::Drawing::Size(35, 27);
      this->RemoveGuideButton->TabIndex = 21;
      this->RemoveGuideButton->Text = L"Del";
      this->RemoveGuideButton->UseVisualStyleBackColor = true;
      this->RemoveGuideButton->Click += gcnew System::EventHandler(this, &GameEditForm::RemoveGuideButton_Click);
      // 
      // AddGuideButton
      // 
      this->AddGuideButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F));
      this->AddGuideButton->Location = System::Drawing::Point(291, 296);
      this->AddGuideButton->Name = L"AddGuideButton";
      this->AddGuideButton->Size = System::Drawing::Size(35, 27);
      this->AddGuideButton->TabIndex = 20;
      this->AddGuideButton->Text = L"Add";
      this->AddGuideButton->UseVisualStyleBackColor = true;
      this->AddGuideButton->Click += gcnew System::EventHandler(this, &GameEditForm::AddGuideButton_Click);
      // 
      // RemoveDocumentationButton
      // 
      this->RemoveDocumentationButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F));
      this->RemoveDocumentationButton->Location = System::Drawing::Point(184, 298);
      this->RemoveDocumentationButton->Name = L"RemoveDocumentationButton";
      this->RemoveDocumentationButton->Size = System::Drawing::Size(34, 26);
      this->RemoveDocumentationButton->TabIndex = 19;
      this->RemoveDocumentationButton->Text = L"Del";
      this->RemoveDocumentationButton->UseVisualStyleBackColor = true;
      this->RemoveDocumentationButton->Click += gcnew System::EventHandler(this, &GameEditForm::RemoveDocumentationButton_Click);
      // 
      // AddDocumentationButton
      // 
      this->AddDocumentationButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F));
      this->AddDocumentationButton->Location = System::Drawing::Point(15, 298);
      this->AddDocumentationButton->Name = L"AddDocumentationButton";
      this->AddDocumentationButton->Size = System::Drawing::Size(34, 26);
      this->AddDocumentationButton->TabIndex = 18;
      this->AddDocumentationButton->Text = L"Add";
      this->AddDocumentationButton->UseVisualStyleBackColor = true;
      this->AddDocumentationButton->Click += gcnew System::EventHandler(this, &GameEditForm::AddDocumentationButton_Click);
      // 
      // GameEditPicture
      // 
      this->GameEditPicture->Location = System::Drawing::Point(324, 66);
      this->GameEditPicture->MaximumSize = System::Drawing::Size(170, 150);
      this->GameEditPicture->MinimumSize = System::Drawing::Size(170, 150);
      this->GameEditPicture->Name = L"GameEditPicture";
      this->GameEditPicture->Size = System::Drawing::Size(170, 150);
      this->GameEditPicture->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
      this->GameEditPicture->TabIndex = 17;
      this->GameEditPicture->TabStop = false;
      // 
      // label3
      // 
      this->label3->AutoSize = true;
      this->label3->Location = System::Drawing::Point(288, 223);
      this->label3->Name = L"label3";
      this->label3->Size = System::Drawing::Size(96, 13);
      this->label3->TabIndex = 16;
      this->label3->Text = L"Hintbooks/Guides:";
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point(12, 223);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(82, 13);
      this->label2->TabIndex = 15;
      this->label2->Text = L"Documentation:";
      // 
      // GuideListBox
      // 
      this->GuideListBox->FormattingEnabled = true;
      this->GuideListBox->HorizontalScrollbar = true;
      this->GuideListBox->Location = System::Drawing::Point(291, 239);
      this->GuideListBox->Name = L"GuideListBox";
      this->GuideListBox->Size = System::Drawing::Size(203, 56);
      this->GuideListBox->TabIndex = 14;
      // 
      // DocumentationListBox
      // 
      this->DocumentationListBox->FormattingEnabled = true;
      this->DocumentationListBox->HorizontalScrollbar = true;
      this->DocumentationListBox->Location = System::Drawing::Point(15, 239);
      this->DocumentationListBox->Name = L"DocumentationListBox";
      this->DocumentationListBox->Size = System::Drawing::Size(203, 56);
      this->DocumentationListBox->TabIndex = 13;
      // 
      // ChooseBackCoverButton
      // 
      this->ChooseBackCoverButton->Location = System::Drawing::Point(501, 30);
      this->ChooseBackCoverButton->Name = L"ChooseBackCoverButton";
      this->ChooseBackCoverButton->Size = System::Drawing::Size(34, 23);
      this->ChooseBackCoverButton->TabIndex = 10;
      this->ChooseBackCoverButton->Text = L"...";
      this->ChooseBackCoverButton->UseVisualStyleBackColor = true;
      this->ChooseBackCoverButton->Click += gcnew System::EventHandler(this, &GameEditForm::ChooseBackCoverButton_Click);
      // 
      // ChooseFrontCoverButton
      // 
      this->ChooseFrontCoverButton->Location = System::Drawing::Point(223, 30);
      this->ChooseFrontCoverButton->Name = L"ChooseFrontCoverButton";
      this->ChooseFrontCoverButton->Size = System::Drawing::Size(35, 23);
      this->ChooseFrontCoverButton->TabIndex = 9;
      this->ChooseFrontCoverButton->Text = L"...";
      this->ChooseFrontCoverButton->UseVisualStyleBackColor = true;
      this->ChooseFrontCoverButton->Click += gcnew System::EventHandler(this, &GameEditForm::ChooseFrontCoverButton_Click);
      // 
      // BackCoverLabel
      // 
      this->BackCoverLabel->AutoSize = true;
      this->BackCoverLabel->Location = System::Drawing::Point(288, 19);
      this->BackCoverLabel->Name = L"BackCoverLabel";
      this->BackCoverLabel->Size = System::Drawing::Size(63, 13);
      this->BackCoverLabel->TabIndex = 8;
      this->BackCoverLabel->Text = L"Back Cover";
      // 
      // BackCoverTextBox
      // 
      this->BackCoverTextBox->Location = System::Drawing::Point(291, 33);
      this->BackCoverTextBox->Name = L"BackCoverTextBox";
      this->BackCoverTextBox->Size = System::Drawing::Size(203, 20);
      this->BackCoverTextBox->TabIndex = 7;
      this->BackCoverTextBox->Click += gcnew System::EventHandler(this, &GameEditForm::GameEditBack_Click);
      this->BackCoverTextBox->MouseHover += gcnew System::EventHandler(this, &GameEditForm::GameEditBack_Click);
      // 
      // RemoveScreenShotButton
      // 
      this->RemoveScreenShotButton->Location = System::Drawing::Point(183, 183);
      this->RemoveScreenShotButton->Name = L"RemoveScreenShotButton";
      this->RemoveScreenShotButton->Size = System::Drawing::Size(75, 23);
      this->RemoveScreenShotButton->TabIndex = 6;
      this->RemoveScreenShotButton->Text = L"Remove";
      this->RemoveScreenShotButton->UseVisualStyleBackColor = true;
      this->RemoveScreenShotButton->Click += gcnew System::EventHandler(this, &GameEditForm::RemoveScreenShotButton_Click);
      // 
      // AddScreenShotButton
      // 
      this->AddScreenShotButton->Location = System::Drawing::Point(15, 183);
      this->AddScreenShotButton->Name = L"AddScreenShotButton";
      this->AddScreenShotButton->Size = System::Drawing::Size(75, 23);
      this->AddScreenShotButton->TabIndex = 5;
      this->AddScreenShotButton->Text = L"Add";
      this->AddScreenShotButton->UseVisualStyleBackColor = true;
      this->AddScreenShotButton->Click += gcnew System::EventHandler(this, &GameEditForm::AddScreenShotButton_Click);
      // 
      // ScreenShotsLabel
      // 
      this->ScreenShotsLabel->AutoSize = true;
      this->ScreenShotsLabel->Location = System::Drawing::Point(12, 66);
      this->ScreenShotsLabel->Name = L"ScreenShotsLabel";
      this->ScreenShotsLabel->Size = System::Drawing::Size(74, 13);
      this->ScreenShotsLabel->TabIndex = 4;
      this->ScreenShotsLabel->Text = L"Screen Shots:";
      // 
      // ScreenShotListBox
      // 
      this->ScreenShotListBox->FormattingEnabled = true;
      this->ScreenShotListBox->Location = System::Drawing::Point(15, 82);
      this->ScreenShotListBox->Name = L"ScreenShotListBox";
      this->ScreenShotListBox->Size = System::Drawing::Size(243, 95);
      this->ScreenShotListBox->TabIndex = 3;
      this->ScreenShotListBox->Click += gcnew System::EventHandler(this, &GameEditForm::ScreenShotListBox_Click);
      // 
      // FrontCoverTextBox
      // 
      this->FrontCoverTextBox->Location = System::Drawing::Point(15, 33);
      this->FrontCoverTextBox->Name = L"FrontCoverTextBox";
      this->FrontCoverTextBox->Size = System::Drawing::Size(202, 20);
      this->FrontCoverTextBox->TabIndex = 1;
      this->FrontCoverTextBox->Click += gcnew System::EventHandler(this, &GameEditForm::GameEditTitle_Click);
      this->FrontCoverTextBox->MouseHover += gcnew System::EventHandler(this, &GameEditForm::GameEditTitle_Click);
      // 
      // FrontCoverLabel
      // 
      this->FrontCoverLabel->AutoSize = true;
      this->FrontCoverLabel->Location = System::Drawing::Point(12, 19);
      this->FrontCoverLabel->Name = L"FrontCoverLabel";
      this->FrontCoverLabel->Size = System::Drawing::Size(65, 13);
      this->FrontCoverLabel->TabIndex = 0;
      this->FrontCoverLabel->Text = L"Front Cover:";
      // 
      // PersonalTabPage
      // 
      this->PersonalTabPage->Controls->Add(this->YearCompletedTextBox);
      this->PersonalTabPage->Controls->Add(this->YearCompletedLabel);
      this->PersonalTabPage->Controls->Add(this->RatingComboBox);
      this->PersonalTabPage->Controls->Add(this->RatingLabel);
      this->PersonalTabPage->Controls->Add(this->CompletedComboBox);
      this->PersonalTabPage->Controls->Add(this->CompletedLabel);
      this->PersonalTabPage->Controls->Add(this->Location4ComboBox);
      this->PersonalTabPage->Controls->Add(this->Location4Label);
      this->PersonalTabPage->Controls->Add(this->Location3ComboBox);
      this->PersonalTabPage->Controls->Add(this->Location3Label);
      this->PersonalTabPage->Controls->Add(this->Location2ComboBox);
      this->PersonalTabPage->Controls->Add(this->Location2Label);
      this->PersonalTabPage->Controls->Add(this->Location1ComboBox);
      this->PersonalTabPage->Controls->Add(this->Location1Label);
      this->PersonalTabPage->Location = System::Drawing::Point(4, 22);
      this->PersonalTabPage->Name = L"PersonalTabPage";
      this->PersonalTabPage->Padding = System::Windows::Forms::Padding(3);
      this->PersonalTabPage->Size = System::Drawing::Size(549, 327);
      this->PersonalTabPage->TabIndex = 1;
      this->PersonalTabPage->Text = L"Personal";
      this->PersonalTabPage->UseVisualStyleBackColor = true;
      // 
      // YearCompletedTextBox
      // 
      this->YearCompletedTextBox->Location = System::Drawing::Point(284, 125);
      this->YearCompletedTextBox->Name = L"YearCompletedTextBox";
      this->YearCompletedTextBox->Size = System::Drawing::Size(257, 20);
      this->YearCompletedTextBox->TabIndex = 13;
      // 
      // YearCompletedLabel
      // 
      this->YearCompletedLabel->AutoSize = true;
      this->YearCompletedLabel->Location = System::Drawing::Point(281, 109);
      this->YearCompletedLabel->Name = L"YearCompletedLabel";
      this->YearCompletedLabel->Size = System::Drawing::Size(85, 13);
      this->YearCompletedLabel->TabIndex = 12;
      this->YearCompletedLabel->Text = L"Year Completed:";
      // 
      // RatingComboBox
      // 
      this->RatingComboBox->FormattingEnabled = true;
      this->RatingComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(10) {L"0.5 Star", L"1.0 Star", L"1.5 Star", L"2.0 Stars", 
        L"2.5 Stars", L"3.0 Stars", L"3.5 Stars", L"4.0 Stars", L"4.5 Stars", L"5.0 Stars"});
      this->RatingComboBox->Location = System::Drawing::Point(284, 81);
      this->RatingComboBox->Name = L"RatingComboBox";
      this->RatingComboBox->Size = System::Drawing::Size(257, 21);
      this->RatingComboBox->TabIndex = 11;
      // 
      // RatingLabel
      // 
      this->RatingLabel->AutoSize = true;
      this->RatingLabel->Location = System::Drawing::Point(281, 64);
      this->RatingLabel->Name = L"RatingLabel";
      this->RatingLabel->Size = System::Drawing::Size(41, 13);
      this->RatingLabel->TabIndex = 10;
      this->RatingLabel->Text = L"Rating:";
      // 
      // CompletedComboBox
      // 
      this->CompletedComboBox->FormattingEnabled = true;
      this->CompletedComboBox->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"No", L"Yes", L"Partial -Quit", L"In Progress"});
      this->CompletedComboBox->Location = System::Drawing::Point(284, 36);
      this->CompletedComboBox->Name = L"CompletedComboBox";
      this->CompletedComboBox->Size = System::Drawing::Size(257, 21);
      this->CompletedComboBox->TabIndex = 9;
      // 
      // CompletedLabel
      // 
      this->CompletedLabel->AutoSize = true;
      this->CompletedLabel->Location = System::Drawing::Point(281, 19);
      this->CompletedLabel->Name = L"CompletedLabel";
      this->CompletedLabel->Size = System::Drawing::Size(60, 13);
      this->CompletedLabel->TabIndex = 8;
      this->CompletedLabel->Text = L"Completed:";
      // 
      // Location4ComboBox
      // 
      this->Location4ComboBox->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::SuggestAppend;
      this->Location4ComboBox->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::ListItems;
      this->Location4ComboBox->FormattingEnabled = true;
      this->Location4ComboBox->Location = System::Drawing::Point(11, 171);
      this->Location4ComboBox->Name = L"Location4ComboBox";
      this->Location4ComboBox->Size = System::Drawing::Size(252, 21);
      this->Location4ComboBox->Sorted = true;
      this->Location4ComboBox->TabIndex = 7;
      // 
      // Location4Label
      // 
      this->Location4Label->AutoSize = true;
      this->Location4Label->Location = System::Drawing::Point(8, 155);
      this->Location4Label->Name = L"Location4Label";
      this->Location4Label->Size = System::Drawing::Size(60, 13);
      this->Location4Label->TabIndex = 6;
      this->Location4Label->Text = L"Location 4:";
      // 
      // Location3ComboBox
      // 
      this->Location3ComboBox->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::SuggestAppend;
      this->Location3ComboBox->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::ListItems;
      this->Location3ComboBox->FormattingEnabled = true;
      this->Location3ComboBox->Location = System::Drawing::Point(11, 125);
      this->Location3ComboBox->Name = L"Location3ComboBox";
      this->Location3ComboBox->Size = System::Drawing::Size(252, 21);
      this->Location3ComboBox->Sorted = true;
      this->Location3ComboBox->TabIndex = 5;
      // 
      // Location3Label
      // 
      this->Location3Label->AutoSize = true;
      this->Location3Label->Location = System::Drawing::Point(8, 109);
      this->Location3Label->Name = L"Location3Label";
      this->Location3Label->Size = System::Drawing::Size(60, 13);
      this->Location3Label->TabIndex = 4;
      this->Location3Label->Text = L"Location 3:";
      // 
      // Location2ComboBox
      // 
      this->Location2ComboBox->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::SuggestAppend;
      this->Location2ComboBox->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::ListItems;
      this->Location2ComboBox->FormattingEnabled = true;
      this->Location2ComboBox->Location = System::Drawing::Point(11, 81);
      this->Location2ComboBox->Name = L"Location2ComboBox";
      this->Location2ComboBox->Size = System::Drawing::Size(252, 21);
      this->Location2ComboBox->Sorted = true;
      this->Location2ComboBox->TabIndex = 3;
      // 
      // Location2Label
      // 
      this->Location2Label->AutoSize = true;
      this->Location2Label->Location = System::Drawing::Point(11, 64);
      this->Location2Label->Name = L"Location2Label";
      this->Location2Label->Size = System::Drawing::Size(60, 13);
      this->Location2Label->TabIndex = 2;
      this->Location2Label->Text = L"Location 2:";
      // 
      // Location1ComboBox
      // 
      this->Location1ComboBox->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::SuggestAppend;
      this->Location1ComboBox->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::ListItems;
      this->Location1ComboBox->FormattingEnabled = true;
      this->Location1ComboBox->Location = System::Drawing::Point(11, 36);
      this->Location1ComboBox->Name = L"Location1ComboBox";
      this->Location1ComboBox->Size = System::Drawing::Size(252, 21);
      this->Location1ComboBox->Sorted = true;
      this->Location1ComboBox->TabIndex = 1;
      // 
      // Location1Label
      // 
      this->Location1Label->AutoSize = true;
      this->Location1Label->Location = System::Drawing::Point(8, 19);
      this->Location1Label->Name = L"Location1Label";
      this->Location1Label->Size = System::Drawing::Size(60, 13);
      this->Location1Label->TabIndex = 0;
      this->Location1Label->Text = L"Location 1:";
      // 
      // ViewWebPageButton
      // 
      this->ViewWebPageButton->Location = System::Drawing::Point(318, 8);
      this->ViewWebPageButton->Name = L"ViewWebPageButton";
      this->ViewWebPageButton->Size = System::Drawing::Size(75, 23);
      this->ViewWebPageButton->TabIndex = 3;
      this->ViewWebPageButton->Text = L"View Page";
      this->ViewWebPageButton->UseVisualStyleBackColor = true;
      this->ViewWebPageButton->Click += gcnew System::EventHandler(this, &GameEditForm::ViewWebPageButton_Click);
      // 
      // GameEditCancelButton
      // 
      this->GameEditCancelButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->GameEditCancelButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->GameEditCancelButton->Location = System::Drawing::Point(470, 8);
      this->GameEditCancelButton->Name = L"GameEditCancelButton";
      this->GameEditCancelButton->Size = System::Drawing::Size(75, 23);
      this->GameEditCancelButton->TabIndex = 2;
      this->GameEditCancelButton->Text = L"Cancel";
      this->GameEditCancelButton->UseVisualStyleBackColor = true;
      this->GameEditCancelButton->Click += gcnew System::EventHandler(this, &GameEditForm::GameEditCancelButton_Click);
      // 
      // GameEditUpdateButton
      // 
      this->GameEditUpdateButton->Location = System::Drawing::Point(143, 8);
      this->GameEditUpdateButton->Name = L"GameEditUpdateButton";
      this->GameEditUpdateButton->Size = System::Drawing::Size(75, 23);
      this->GameEditUpdateButton->TabIndex = 1;
      this->GameEditUpdateButton->Text = L"Update";
      this->GameEditUpdateButton->UseVisualStyleBackColor = true;
      this->GameEditUpdateButton->Click += gcnew System::EventHandler(this, &GameEditForm::GameEditUpdateButton_Click);
      // 
      // GameEditOkButton
      // 
      this->GameEditOkButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
      this->GameEditOkButton->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->GameEditOkButton->Location = System::Drawing::Point(12, 8);
      this->GameEditOkButton->Name = L"GameEditOkButton";
      this->GameEditOkButton->Size = System::Drawing::Size(75, 23);
      this->GameEditOkButton->TabIndex = 0;
      this->GameEditOkButton->Text = L"Ok";
      this->GameEditOkButton->UseVisualStyleBackColor = true;
      this->GameEditOkButton->Click += gcnew System::EventHandler(this, &GameEditForm::GameEditOkButton_Click);
      // 
      // GameEditForm
      // 
      this->AcceptButton = this->GameEditOkButton;
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->BackColor = System::Drawing::SystemColors::ControlLight;
      this->CancelButton = this->GameEditCancelButton;
      this->ClientSize = System::Drawing::Size(557, 388);
      this->Controls->Add(this->splitContainer1);
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"GameEditForm";
      this->ShowIcon = false;
      this->ShowInTaskbar = false;
      this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
      this->Text = L"Edit Game Data";
      this->splitContainer1->Panel1->ResumeLayout(false);
      this->splitContainer1->Panel2->ResumeLayout(false);
      this->splitContainer1->ResumeLayout(false);
      this->GameEditTabControl->ResumeLayout(false);
      this->GeneralTabPage->ResumeLayout(false);
      this->GeneralTabPage->PerformLayout();
      this->DescriptionTabPage->ResumeLayout(false);
      this->DescriptionTabPage->PerformLayout();
      this->MediaTabPage->ResumeLayout(false);
      this->MediaTabPage->PerformLayout();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GameEditPicture))->EndInit();
      this->PersonalTabPage->ResumeLayout(false);
      this->PersonalTabPage->PerformLayout();
      this->ResumeLayout(false);

    }
#pragma endregion

  public:
    void Populate_EditForm(GameDescriptor ^gd, GameDescriptor ^oeg, bool keepchanges);

  private: 
    System::Void GameEditOkButton_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void GameEditCancelButton_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void GameEditTitle_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void GameEditBack_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void ChooseFrontCoverButton_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void ChooseBackCoverButton_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void ScreenShotListBox_Click(System::Object^  sender, System::EventArgs^  e); 
    System::Void EditGenreButton_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void EditPlatformButton_Click(System::Object^  sender, System::EventArgs^  e); 
    System::Void CategoryEditButton_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void AddScreenShotButton_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void RemoveScreenShotButton_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void GameEditUpdateButton_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void ViewWebPageButton_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void AddDocumentationButton_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void AddGuideButton_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void RemoveDocumentationButton_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void RemoveGuideButton_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void EditDocumentationButton_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void EditGuideButton_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void Display_GameImage(String ^filename);
    String ^Select_ImageFile(void);


    // member variables
    GameDescriptor ^CurrentEditGame;          // used so we can remove old screenshots etc
    GameDescriptor ^OldEditGame;
    DBCats ^DBCategories;
    int MaxSearchMatches;                     // Maximum number of search results to return for scraping

    Assembly ^ScrapeAssem;                    // Variables to store the scraper dll data
    Type ^ScrapeType;
    MethodInfo ^ScrapeGetFields;
    MethodInfo ^ScrapeGetMatchList;
    Object ^GameScraper;

  };
}
