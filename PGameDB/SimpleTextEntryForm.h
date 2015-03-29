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


// Simple form to implement a generic single line text entry form

#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace PGameDB {

  public ref class SimpleTextEntryForm : public System::Windows::Forms::Form
  {
  public:
    SimpleTextEntryForm(void)
    {
      InitializeComponent();
    }

  protected:
    ~SimpleTextEntryForm()
    {
      if (components)
      {
        delete components;
      }
    }
  private: System::Windows::Forms::SplitContainer^  splitContainer1;
  protected: 
  private: System::Windows::Forms::Label^  SimpleTextEntryLabel;
  private: System::Windows::Forms::TextBox^  SimpleTextEntryTextBox;
  private: System::Windows::Forms::Button^  SimpleTextEntryButtonCancel;
  private: System::Windows::Forms::Button^  SimpleTextEntryButtonOk;

  private:
    System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    void InitializeComponent(void)
    {
      this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
      this->SimpleTextEntryLabel = (gcnew System::Windows::Forms::Label());
      this->SimpleTextEntryTextBox = (gcnew System::Windows::Forms::TextBox());
      this->SimpleTextEntryButtonCancel = (gcnew System::Windows::Forms::Button());
      this->SimpleTextEntryButtonOk = (gcnew System::Windows::Forms::Button());
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
      this->splitContainer1->Panel1->Controls->Add(this->SimpleTextEntryLabel);
      this->splitContainer1->Panel1->Controls->Add(this->SimpleTextEntryTextBox);
      // 
      // splitContainer1.Panel2
      // 
      this->splitContainer1->Panel2->Controls->Add(this->SimpleTextEntryButtonCancel);
      this->splitContainer1->Panel2->Controls->Add(this->SimpleTextEntryButtonOk);
      this->splitContainer1->Size = System::Drawing::Size(316, 122);
      this->splitContainer1->SplitterDistance = 92;
      this->splitContainer1->SplitterWidth = 1;
      this->splitContainer1->TabIndex = 0;
      // 
      // SimpleTextEntryLabel
      // 
      this->SimpleTextEntryLabel->AutoSize = true;
      this->SimpleTextEntryLabel->Location = System::Drawing::Point(39, 23);
      this->SimpleTextEntryLabel->Name = L"SimpleTextEntryLabel";
      this->SimpleTextEntryLabel->Size = System::Drawing::Size(35, 13);
      this->SimpleTextEntryLabel->TabIndex = 1;
      this->SimpleTextEntryLabel->Text = L"label1";
      // 
      // SimpleTextEntryTextBox
      // 
      this->SimpleTextEntryTextBox->Location = System::Drawing::Point(39, 42);
      this->SimpleTextEntryTextBox->Name = L"SimpleTextEntryTextBox";
      this->SimpleTextEntryTextBox->Size = System::Drawing::Size(236, 20);
      this->SimpleTextEntryTextBox->TabIndex = 0;
      // 
      // SimpleTextEntryButtonCancel
      // 
      this->SimpleTextEntryButtonCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->SimpleTextEntryButtonCancel->Location = System::Drawing::Point(229, 3);
      this->SimpleTextEntryButtonCancel->Name = L"SimpleTextEntryButtonCancel";
      this->SimpleTextEntryButtonCancel->Size = System::Drawing::Size(75, 23);
      this->SimpleTextEntryButtonCancel->TabIndex = 1;
      this->SimpleTextEntryButtonCancel->Text = L"Cancel";
      this->SimpleTextEntryButtonCancel->UseVisualStyleBackColor = true;
      this->SimpleTextEntryButtonCancel->Click += gcnew System::EventHandler(this, &SimpleTextEntryForm::SimpleTextEntryButtonCancel_Click);
      // 
      // SimpleTextEntryButtonOk
      // 
      this->SimpleTextEntryButtonOk->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->SimpleTextEntryButtonOk->Location = System::Drawing::Point(12, 3);
      this->SimpleTextEntryButtonOk->Name = L"SimpleTextEntryButtonOk";
      this->SimpleTextEntryButtonOk->Size = System::Drawing::Size(75, 23);
      this->SimpleTextEntryButtonOk->TabIndex = 0;
      this->SimpleTextEntryButtonOk->Text = L"Ok";
      this->SimpleTextEntryButtonOk->UseVisualStyleBackColor = true;
      this->SimpleTextEntryButtonOk->Click += gcnew System::EventHandler(this, &SimpleTextEntryForm::SimpleTextEntryButtonOk_Click);
      // 
      // SimpleTextEntryForm
      // 
      this->AcceptButton = this->SimpleTextEntryButtonOk;
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->CancelButton = this->SimpleTextEntryButtonCancel;
      this->ClientSize = System::Drawing::Size(316, 122);
      this->Controls->Add(this->splitContainer1);
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"SimpleTextEntryForm";
      this->ShowIcon = false;
      this->ShowInTaskbar = false;
      this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
      this->Text = L"SimpleTextEntryForm";
      this->splitContainer1->Panel1->ResumeLayout(false);
      this->splitContainer1->Panel1->PerformLayout();
      this->splitContainer1->Panel2->ResumeLayout(false);
      this->splitContainer1->ResumeLayout(false);
      this->ResumeLayout(false);

    }
#pragma endregion


  public:

    // get the text from the text entry form
    String ^Get_Text(void) {
      return this->SimpleTextEntryTextBox->Text;
    }

    // set the text for the form
    void Set_Text(String ^text) {
      this->SimpleTextEntryTextBox->Text = text;
    }

    // set the label of the form
    void Set_Label(String ^label) {
      this->SimpleTextEntryLabel->Text = label;
    }

    // set the title of the form window
    void Set_Title(String ^title) {
      this->Text = title;
    }


  private: 

    System::Void SimpleTextEntryButtonOk_Click(System::Object^  sender, System::EventArgs^  e) { Close(); };
    System::Void SimpleTextEntryButtonCancel_Click(System::Object^  sender, System::EventArgs^  e) { Close(); }
  };
}
