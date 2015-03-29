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



// Form to associate a documentation file to a game

#pragma once

#include "PGameDB.h"
#include "MobySelectionForm.h"
#include "GameDataStructs.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace PGameDB {

  public ref class AddDocForm : public System::Windows::Forms::Form
  {
  public:
    AddDocForm(String ^filename, String ^desc, PGameDocType dt)
    {
      InitializeComponent();

      // fill in form values if passed
      AddDocFilenameTextBox->Text = filename;
      AddDocDocTypeTextbox->Text = desc;
      CurDocType = dt;

    }

  protected:
    ~AddDocForm()
    {
      if (components)
      {
        delete components;
      }
    }
  private: System::Windows::Forms::SplitContainer^  splitContainer1;
  public: System::Windows::Forms::TextBox^  AddDocFilenameTextBox;

  private: System::Windows::Forms::Button^  button2;
  private: System::Windows::Forms::Button^  button1;
  public: System::Windows::Forms::TextBox^  AddDocDocTypeTextbox;

  private: System::Windows::Forms::Button^  AddDocumentFileButton;

  private: System::Windows::Forms::Label^  label2;
  private: System::Windows::Forms::Label^  label1;
  protected: 

  private:
    /// <summary>
    /// Required designer variable.
    /// </summary>
    System::ComponentModel::Container ^components;

    PGameDocType CurDocType;


#pragma region Windows Form Designer generated code
    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    void InitializeComponent(void)
    {
      this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->AddDocDocTypeTextbox = (gcnew System::Windows::Forms::TextBox());
      this->AddDocumentFileButton = (gcnew System::Windows::Forms::Button());
      this->AddDocFilenameTextBox = (gcnew System::Windows::Forms::TextBox());
      this->button2 = (gcnew System::Windows::Forms::Button());
      this->button1 = (gcnew System::Windows::Forms::Button());
      this->splitContainer1->Panel1->SuspendLayout();
      this->splitContainer1->Panel2->SuspendLayout();
      this->splitContainer1->SuspendLayout();
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
      this->splitContainer1->Panel1->Controls->Add(this->label2);
      this->splitContainer1->Panel1->Controls->Add(this->label1);
      this->splitContainer1->Panel1->Controls->Add(this->AddDocDocTypeTextbox);
      this->splitContainer1->Panel1->Controls->Add(this->AddDocumentFileButton);
      this->splitContainer1->Panel1->Controls->Add(this->AddDocFilenameTextBox);
      // 
      // splitContainer1.Panel2
      // 
      this->splitContainer1->Panel2->Controls->Add(this->button2);
      this->splitContainer1->Panel2->Controls->Add(this->button1);
      this->splitContainer1->Size = System::Drawing::Size(419, 90);
      this->splitContainer1->SplitterDistance = 60;
      this->splitContainer1->SplitterWidth = 1;
      this->splitContainer1->TabIndex = 0;
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point(235, 9);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(86, 13);
      this->label2->TabIndex = 4;
      this->label2->Text = L"Document Type:";
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(12, 9);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(52, 13);
      this->label1->TabIndex = 3;
      this->label1->Text = L"Filename:";
      // 
      // AddDocDocTypeTextbox
      // 
      this->AddDocDocTypeTextbox->Location = System::Drawing::Point(238, 24);
      this->AddDocDocTypeTextbox->Name = L"AddDocDocTypeTextbox";
      this->AddDocDocTypeTextbox->Size = System::Drawing::Size(169, 20);
      this->AddDocDocTypeTextbox->TabIndex = 2;
      // 
      // AddDocumentFileButton
      // 
      this->AddDocumentFileButton->Location = System::Drawing::Point(172, 25);
      this->AddDocumentFileButton->Name = L"AddDocumentFileButton";
      this->AddDocumentFileButton->Size = System::Drawing::Size(37, 19);
      this->AddDocumentFileButton->TabIndex = 1;
      this->AddDocumentFileButton->Text = L"...";
      this->AddDocumentFileButton->UseVisualStyleBackColor = true;
      this->AddDocumentFileButton->Click += gcnew System::EventHandler(this, &AddDocForm::AddDocumentFileButton_Click);
      // 
      // AddDocFilenameTextBox
      // 
      this->AddDocFilenameTextBox->Location = System::Drawing::Point(12, 24);
      this->AddDocFilenameTextBox->Name = L"AddDocFilenameTextBox";
      this->AddDocFilenameTextBox->Size = System::Drawing::Size(154, 20);
      this->AddDocFilenameTextBox->TabIndex = 0;
      // 
      // button2
      // 
      this->button2->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->button2->Location = System::Drawing::Point(332, 3);
      this->button2->Name = L"button2";
      this->button2->Size = System::Drawing::Size(75, 23);
      this->button2->TabIndex = 1;
      this->button2->Text = L"Cancel";
      this->button2->UseVisualStyleBackColor = true;
      // 
      // button1
      // 
      this->button1->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->button1->Location = System::Drawing::Point(15, 3);
      this->button1->Name = L"button1";
      this->button1->Size = System::Drawing::Size(75, 23);
      this->button1->TabIndex = 0;
      this->button1->Text = L"Ok";
      this->button1->UseVisualStyleBackColor = true;
      // 
      // AddDocForm
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(419, 90);
      this->Controls->Add(this->splitContainer1);
      this->Name = L"AddDocForm";
      this->ShowIcon = false;
      this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
      this->Text = L"Add Document";
      this->splitContainer1->Panel1->ResumeLayout(false);
      this->splitContainer1->Panel1->PerformLayout();
      this->splitContainer1->Panel2->ResumeLayout(false);
      this->splitContainer1->ResumeLayout(false);
      this->ResumeLayout(false);

    }
#pragma endregion
  private: 

    // handle file selection when adding a document to the game descriptor
    System::Void AddDocumentFileButton_Click(System::Object^  sender, System::EventArgs^  e) {

      OpenFileDialog^ ofd = gcnew OpenFileDialog;

      // determine the data path from the config file
      const char *datapath = NULL;

      if (CurDocType == MANUAL) {
        datapath = gConfMan->Get_ConfigItem("DocumentationPath");
      }
      else if (CurDocType == GUIDE) {
        datapath = gConfMan->Get_ConfigItem("GuidePath");
      }
      if (!datapath) {
        MessageBox::Show("Error: Configuration file corrupted!");
        return;
      }
      String ^dp = gcnew String(datapath);

      dp = dp->Replace("/", "\\");      
      dp = String::Concat(Environment::CurrentDirectory, dp);
      ofd->InitialDirectory = dp;
      ofd->Filter = "documentation files (*.*) |*.*";
      ofd->FilterIndex = 1;
      ofd->RestoreDirectory = true;

      // open a file selection dialog to choose the guide
      if (ofd->ShowDialog() == ::DialogResult::OK) {
        String ^fulldoc = ofd->FileName;
        fulldoc = fulldoc->Remove(0, Environment::CurrentDirectory->Length);
        int startindex = fulldoc->IndexOf("\\");
        int stopindex = fulldoc->IndexOf("\\", startindex + 1);
        fulldoc = fulldoc->Remove(0, stopindex + 1);
        AddDocFilenameTextBox->Text = fulldoc;
      }
      else {
        AddDocFilenameTextBox->Text = "";
      }
    }
  };
}
