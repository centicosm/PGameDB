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


// Simple form to prompt to save any changes before exiting

#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace PGameDB {

  public ref class SaveOnExitDialog : public System::Windows::Forms::Form
  {
  public:
    SaveOnExitDialog(void)
    {
      InitializeComponent();
    }

  protected:
    ~SaveOnExitDialog()
    {
      if (components)
      {
        delete components;
      }
    }
  private: System::Windows::Forms::SplitContainer^  splitContainer1;
  protected: 
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::Button^  button3;
  private: System::Windows::Forms::Button^  button2;
  private: System::Windows::Forms::Button^  button1;

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
      this->button1 = (gcnew System::Windows::Forms::Button());
      this->button2 = (gcnew System::Windows::Forms::Button());
      this->button3 = (gcnew System::Windows::Forms::Button());
      this->label1 = (gcnew System::Windows::Forms::Label());
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
      this->splitContainer1->Panel1->Controls->Add(this->label1);
      // 
      // splitContainer1.Panel2
      // 
      this->splitContainer1->Panel2->Controls->Add(this->button3);
      this->splitContainer1->Panel2->Controls->Add(this->button2);
      this->splitContainer1->Panel2->Controls->Add(this->button1);
      this->splitContainer1->Size = System::Drawing::Size(335, 114);
      this->splitContainer1->SplitterDistance = 76;
      this->splitContainer1->SplitterWidth = 1;
      this->splitContainer1->TabIndex = 0;
      // 
      // button1
      // 
      this->button1->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->button1->Location = System::Drawing::Point(12, 8);
      this->button1->Name = L"button1";
      this->button1->Size = System::Drawing::Size(75, 23);
      this->button1->TabIndex = 0;
      this->button1->Text = L"Save";
      this->button1->UseVisualStyleBackColor = true;
      // 
      // button2
      // 
      this->button2->DialogResult = System::Windows::Forms::DialogResult::Yes;
      this->button2->Location = System::Drawing::Point(132, 8);
      this->button2->Name = L"button2";
      this->button2->Size = System::Drawing::Size(75, 23);
      this->button2->TabIndex = 1;
      this->button2->Text = L"Save As";
      this->button2->UseVisualStyleBackColor = true;
      // 
      // button3
      // 
      this->button3->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->button3->Location = System::Drawing::Point(248, 8);
      this->button3->Name = L"button3";
      this->button3->Size = System::Drawing::Size(75, 23);
      this->button3->TabIndex = 2;
      this->button3->Text = L"Exit";
      this->button3->UseVisualStyleBackColor = true;
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
        static_cast<System::Byte>(0)));
      this->label1->Location = System::Drawing::Point(47, 23);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(233, 36);
      this->label1->TabIndex = 0;
      this->label1->Text = L"Database has been modified.  \r\nSave your changes before exiting\?";
      this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
      // 
      // SaveOnExitDialog
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(335, 114);
      this->Controls->Add(this->splitContainer1);
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"SaveOnExitDialog";
      this->ShowIcon = false;
      this->ShowInTaskbar = false;
      this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
      this->Text = L"Save Your Changes\?";
      this->splitContainer1->Panel1->ResumeLayout(false);
      this->splitContainer1->Panel1->PerformLayout();
      this->splitContainer1->Panel2->ResumeLayout(false);
      this->splitContainer1->ResumeLayout(false);
      this->ResumeLayout(false);

    }
#pragma endregion
  };
}
