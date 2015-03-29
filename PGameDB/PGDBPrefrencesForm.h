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


// Form implementing the preference window

#pragma once

#include "PGameDB.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace PGameDB {

  public ref class PGDBPrefrencesForm : public System::Windows::Forms::Form
  {
  public:
    PGDBPrefrencesForm(void)
    {
      InitializeComponent();

      Init_Prefrences();
    }

  protected:
    ~PGDBPrefrencesForm()
    {
      if (components)
      {
        delete components;
      }
    }
  private: System::Windows::Forms::SplitContainer^  splitContainer1;
  private: System::Windows::Forms::Button^  PreferenceCancelButton;
  private: System::Windows::Forms::Button^  PreferenceOkButton;
  private: System::Windows::Forms::NumericUpDown^  TitleshotWidthUpDown;
  private: System::Windows::Forms::Label^  label4;
  private: System::Windows::Forms::NumericUpDown^  ScreenshotWidthUpDown;
  private: System::Windows::Forms::Label^  label3;
  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::NumericUpDown^  NumberScreenShotsUpDown;
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::TextBox^  DataFilePathTextBox;
  private: System::Windows::Forms::Label^  label5;
  private: System::Windows::Forms::TextBox^  DefaultDBTextBox;
  private: System::Windows::Forms::Label^  label6;
  private: System::Windows::Forms::NumericUpDown^  SearchResultsUpDown;
  private: System::Windows::Forms::Label^  label8;
  private: System::Windows::Forms::Label^  label7;
  private: System::Windows::Forms::TextBox^  GuidePathTextBox;

  private: System::Windows::Forms::TextBox^  DocumentationPathTextBox;

  private: System::Windows::Forms::TextBox^  ScraperDLLTextBox;
  private: System::Windows::Forms::Label^  label9;
  protected: 

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
      this->ScraperDLLTextBox = (gcnew System::Windows::Forms::TextBox());
      this->label9 = (gcnew System::Windows::Forms::Label());
      this->label8 = (gcnew System::Windows::Forms::Label());
      this->label7 = (gcnew System::Windows::Forms::Label());
      this->GuidePathTextBox = (gcnew System::Windows::Forms::TextBox());
      this->DocumentationPathTextBox = (gcnew System::Windows::Forms::TextBox());
      this->label6 = (gcnew System::Windows::Forms::Label());
      this->SearchResultsUpDown = (gcnew System::Windows::Forms::NumericUpDown());
      this->label5 = (gcnew System::Windows::Forms::Label());
      this->DefaultDBTextBox = (gcnew System::Windows::Forms::TextBox());
      this->TitleshotWidthUpDown = (gcnew System::Windows::Forms::NumericUpDown());
      this->label4 = (gcnew System::Windows::Forms::Label());
      this->ScreenshotWidthUpDown = (gcnew System::Windows::Forms::NumericUpDown());
      this->label3 = (gcnew System::Windows::Forms::Label());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->NumberScreenShotsUpDown = (gcnew System::Windows::Forms::NumericUpDown());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->DataFilePathTextBox = (gcnew System::Windows::Forms::TextBox());
      this->PreferenceCancelButton = (gcnew System::Windows::Forms::Button());
      this->PreferenceOkButton = (gcnew System::Windows::Forms::Button());
      this->splitContainer1->Panel1->SuspendLayout();
      this->splitContainer1->Panel2->SuspendLayout();
      this->splitContainer1->SuspendLayout();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SearchResultsUpDown))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TitleshotWidthUpDown))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ScreenshotWidthUpDown))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NumberScreenShotsUpDown))->BeginInit();
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
      this->splitContainer1->Panel1->Controls->Add(this->ScraperDLLTextBox);
      this->splitContainer1->Panel1->Controls->Add(this->label9);
      this->splitContainer1->Panel1->Controls->Add(this->label8);
      this->splitContainer1->Panel1->Controls->Add(this->label7);
      this->splitContainer1->Panel1->Controls->Add(this->GuidePathTextBox);
      this->splitContainer1->Panel1->Controls->Add(this->DocumentationPathTextBox);
      this->splitContainer1->Panel1->Controls->Add(this->label6);
      this->splitContainer1->Panel1->Controls->Add(this->SearchResultsUpDown);
      this->splitContainer1->Panel1->Controls->Add(this->label5);
      this->splitContainer1->Panel1->Controls->Add(this->DefaultDBTextBox);
      this->splitContainer1->Panel1->Controls->Add(this->TitleshotWidthUpDown);
      this->splitContainer1->Panel1->Controls->Add(this->label4);
      this->splitContainer1->Panel1->Controls->Add(this->ScreenshotWidthUpDown);
      this->splitContainer1->Panel1->Controls->Add(this->label3);
      this->splitContainer1->Panel1->Controls->Add(this->label2);
      this->splitContainer1->Panel1->Controls->Add(this->NumberScreenShotsUpDown);
      this->splitContainer1->Panel1->Controls->Add(this->label1);
      this->splitContainer1->Panel1->Controls->Add(this->DataFilePathTextBox);
      // 
      // splitContainer1.Panel2
      // 
      this->splitContainer1->Panel2->Controls->Add(this->PreferenceCancelButton);
      this->splitContainer1->Panel2->Controls->Add(this->PreferenceOkButton);
      this->splitContainer1->Size = System::Drawing::Size(391, 297);
      this->splitContainer1->SplitterDistance = 259;
      this->splitContainer1->SplitterWidth = 1;
      this->splitContainer1->TabIndex = 0;
      // 
      // ScraperDLLTextBox
      // 
      this->ScraperDLLTextBox->Location = System::Drawing::Point(16, 228);
      this->ScraperDLLTextBox->Name = L"ScraperDLLTextBox";
      this->ScraperDLLTextBox->Size = System::Drawing::Size(163, 20);
      this->ScraperDLLTextBox->TabIndex = 17;
      // 
      // label9
      // 
      this->label9->AutoSize = true;
      this->label9->Location = System::Drawing::Point(12, 212);
      this->label9->Name = L"label9";
      this->label9->Size = System::Drawing::Size(70, 13);
      this->label9->TabIndex = 16;
      this->label9->Text = L"Scraper DLL:";
      // 
      // label8
      // 
      this->label8->AutoSize = true;
      this->label8->Location = System::Drawing::Point(213, 71);
      this->label8->Name = L"label8";
      this->label8->Size = System::Drawing::Size(63, 13);
      this->label8->TabIndex = 15;
      this->label8->Text = L"Guide Path:";
      // 
      // label7
      // 
      this->label7->AutoSize = true;
      this->label7->Location = System::Drawing::Point(12, 71);
      this->label7->Name = L"label7";
      this->label7->Size = System::Drawing::Size(107, 13);
      this->label7->TabIndex = 14;
      this->label7->Text = L"Documentation Path:";
      // 
      // GuidePathTextBox
      // 
      this->GuidePathTextBox->Location = System::Drawing::Point(216, 87);
      this->GuidePathTextBox->Name = L"GuidePathTextBox";
      this->GuidePathTextBox->Size = System::Drawing::Size(163, 20);
      this->GuidePathTextBox->TabIndex = 13;
      // 
      // DocumentationPathTextBox
      // 
      this->DocumentationPathTextBox->Location = System::Drawing::Point(13, 87);
      this->DocumentationPathTextBox->Name = L"DocumentationPathTextBox";
      this->DocumentationPathTextBox->Size = System::Drawing::Size(166, 20);
      this->DocumentationPathTextBox->TabIndex = 12;
      // 
      // label6
      // 
      this->label6->AutoSize = true;
      this->label6->Location = System::Drawing::Point(213, 162);
      this->label6->Name = L"label6";
      this->label6->Size = System::Drawing::Size(136, 13);
      this->label6->TabIndex = 11;
      this->label6->Text = L"Max. Num. Search Results:";
      // 
      // SearchResultsUpDown
      // 
      this->SearchResultsUpDown->Location = System::Drawing::Point(216, 181);
      this->SearchResultsUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {200, 0, 0, 0});
      this->SearchResultsUpDown->Name = L"SearchResultsUpDown";
      this->SearchResultsUpDown->Size = System::Drawing::Size(163, 20);
      this->SearchResultsUpDown->TabIndex = 10;
      this->SearchResultsUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {25, 0, 0, 0});
      // 
      // label5
      // 
      this->label5->AutoSize = true;
      this->label5->Location = System::Drawing::Point(213, 28);
      this->label5->Name = L"label5";
      this->label5->Size = System::Drawing::Size(93, 13);
      this->label5->TabIndex = 9;
      this->label5->Text = L"Default Database:";
      // 
      // DefaultDBTextBox
      // 
      this->DefaultDBTextBox->Location = System::Drawing::Point(216, 44);
      this->DefaultDBTextBox->Name = L"DefaultDBTextBox";
      this->DefaultDBTextBox->Size = System::Drawing::Size(163, 20);
      this->DefaultDBTextBox->TabIndex = 8;
      // 
      // TitleshotWidthUpDown
      // 
      this->TitleshotWidthUpDown->Location = System::Drawing::Point(216, 131);
      this->TitleshotWidthUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {2000, 0, 0, 0});
      this->TitleshotWidthUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
      this->TitleshotWidthUpDown->Name = L"TitleshotWidthUpDown";
      this->TitleshotWidthUpDown->Size = System::Drawing::Size(163, 20);
      this->TitleshotWidthUpDown->TabIndex = 7;
      this->TitleshotWidthUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
      // 
      // label4
      // 
      this->label4->AutoSize = true;
      this->label4->Location = System::Drawing::Point(213, 115);
      this->label4->Name = L"label4";
      this->label4->Size = System::Drawing::Size(128, 13);
      this->label4->TabIndex = 6;
      this->label4->Text = L"Maximum Titleshot Width:";
      // 
      // ScreenshotWidthUpDown
      // 
      this->ScreenshotWidthUpDown->Location = System::Drawing::Point(16, 181);
      this->ScreenshotWidthUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {2000, 0, 0, 0});
      this->ScreenshotWidthUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
      this->ScreenshotWidthUpDown->Name = L"ScreenshotWidthUpDown";
      this->ScreenshotWidthUpDown->Size = System::Drawing::Size(163, 20);
      this->ScreenshotWidthUpDown->TabIndex = 5;
      this->ScreenshotWidthUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
      // 
      // label3
      // 
      this->label3->AutoSize = true;
      this->label3->Location = System::Drawing::Point(13, 162);
      this->label3->Name = L"label3";
      this->label3->Size = System::Drawing::Size(142, 13);
      this->label3->TabIndex = 4;
      this->label3->Text = L"Maximum Screenshot Width:";
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point(13, 115);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(121, 13);
      this->label2->TabIndex = 3;
      this->label2->Text = L"Number of Screenshots:";
      // 
      // NumberScreenShotsUpDown
      // 
      this->NumberScreenShotsUpDown->Location = System::Drawing::Point(16, 131);
      this->NumberScreenShotsUpDown->Name = L"NumberScreenShotsUpDown";
      this->NumberScreenShotsUpDown->Size = System::Drawing::Size(163, 20);
      this->NumberScreenShotsUpDown->TabIndex = 2;
      this->NumberScreenShotsUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(13, 26);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(112, 13);
      this->label1->TabIndex = 1;
      this->label1->Text = L"Screen/Boxshot Path:";
      // 
      // DataFilePathTextBox
      // 
      this->DataFilePathTextBox->Location = System::Drawing::Point(13, 45);
      this->DataFilePathTextBox->Name = L"DataFilePathTextBox";
      this->DataFilePathTextBox->Size = System::Drawing::Size(166, 20);
      this->DataFilePathTextBox->TabIndex = 0;
      // 
      // PreferenceCancelButton
      // 
      this->PreferenceCancelButton->Location = System::Drawing::Point(249, 5);
      this->PreferenceCancelButton->Name = L"PreferenceCancelButton";
      this->PreferenceCancelButton->Size = System::Drawing::Size(75, 23);
      this->PreferenceCancelButton->TabIndex = 1;
      this->PreferenceCancelButton->Text = L"Cancel";
      this->PreferenceCancelButton->UseVisualStyleBackColor = true;
      this->PreferenceCancelButton->Click += gcnew System::EventHandler(this, &PGDBPrefrencesForm::PreferenceCancelButton_Click);
      // 
      // PreferenceOkButton
      // 
      this->PreferenceOkButton->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->PreferenceOkButton->Location = System::Drawing::Point(71, 5);
      this->PreferenceOkButton->Name = L"PreferenceOkButton";
      this->PreferenceOkButton->Size = System::Drawing::Size(75, 23);
      this->PreferenceOkButton->TabIndex = 0;
      this->PreferenceOkButton->Text = L"Ok";
      this->PreferenceOkButton->UseVisualStyleBackColor = true;
      this->PreferenceOkButton->Click += gcnew System::EventHandler(this, &PGDBPrefrencesForm::PreferenceOkButton_Click);
      // 
      // PGDBPrefrencesForm
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(391, 297);
      this->Controls->Add(this->splitContainer1);
      this->Name = L"PGDBPrefrencesForm";
      this->ShowIcon = false;
      this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
      this->Text = L"PGDB Prefrences";
      this->splitContainer1->Panel1->ResumeLayout(false);
      this->splitContainer1->Panel1->PerformLayout();
      this->splitContainer1->Panel2->ResumeLayout(false);
      this->splitContainer1->ResumeLayout(false);
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SearchResultsUpDown))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TitleshotWidthUpDown))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ScreenshotWidthUpDown))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NumberScreenShotsUpDown))->EndInit();
      this->ResumeLayout(false);

    }
#pragma endregion
  private: 
    // Cancel button clicked
    System::Void PreferenceCancelButton_Click(System::Object^  sender, System::EventArgs^  e) {
      Close();
    }

    // Ok button clicked - save preferences and reparse configuration file
    System::Void PreferenceOkButton_Click(System::Object^  sender, System::EventArgs^  e) {
      Save_Preferences();
      delete gConfMan;
      gConfMan = new ConfigManager("PGameDB.cfg");
      Close();
    }

  public:

    // initialize form with values from file
    void Init_Prefrences(void) {

      const char *datapath = gConfMan->Get_ConfigItem("ShotPath");
      if (datapath) {
        DataFilePathTextBox->Text = gcnew String(datapath);
      }

      const char *titlesize = gConfMan->Get_ConfigItem("MaxTitleSizeX");
      if (titlesize) {
        TitleshotWidthUpDown->Value = atoi(titlesize);
      }

      const char *screensize = gConfMan->Get_ConfigItem("MaxScreenShotSizeX");
      if (screensize) {
        ScreenshotWidthUpDown->Value = atoi(screensize);
      }

      const char *numscreen = gConfMan->Get_ConfigItem("NumScreenShots");
      if (numscreen) {
        NumberScreenShotsUpDown->Value = atoi(numscreen);
      }

      const char *defaultdb = gConfMan->Get_ConfigItem("DefaultDB");
      if (defaultdb) {
        DefaultDBTextBox->Text = gcnew String(defaultdb);
      }

      const char *guidepath = gConfMan->Get_ConfigItem("GuidePath");
      if (guidepath) {
        GuidePathTextBox->Text = gcnew String(guidepath);
      }

      const char *docpath = gConfMan->Get_ConfigItem("DocumentationPath");
      if (docpath) {
        DocumentationPathTextBox->Text = gcnew String(docpath);
      }

      const char *maxsearch = gConfMan->Get_ConfigItem("MaxSearchResults");
      if (maxsearch) {
        SearchResultsUpDown->Value = atoi(maxsearch);
      }

      const char *scraper = gConfMan->Get_ConfigItem("Scraper");
      if (scraper) {
        ScraperDLLTextBox->Text = gcnew String(scraper);
      }
    }

    // save the preferences to file
    void Save_Preferences(void) {
      StreamWriter ^sw = gcnew StreamWriter("PGameDB.cfg");

      sw->WriteLine("ShotPath={0}", DataFilePathTextBox->Text);
      sw->WriteLine("MaxTitleSizeX={0}", TitleshotWidthUpDown->Value);
      sw->WriteLine("DocumentationPath={0}", DocumentationPathTextBox->Text);
      sw->WriteLine("GuidePath={0}", GuidePathTextBox->Text);
      sw->WriteLine("MaxScreenShotSizeX={0}", ScreenshotWidthUpDown->Value);
      sw->WriteLine("NumScreenShots={0}", NumberScreenShotsUpDown->Value);
      sw->WriteLine("DefaultDB={0}", DefaultDBTextBox->Text);
      sw->WriteLine("MaxSearchResults={0}", SearchResultsUpDown->Value);
      sw->WriteLine("Scraper={0}", ScraperDLLTextBox->Text);
      sw->Close();
    }
  };
}
