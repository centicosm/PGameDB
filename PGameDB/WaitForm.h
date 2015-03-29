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



// simple form to implement a please wait prompy

#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace PGameDB {

  public ref class WaitForm : public System::Windows::Forms::Form
  {
  public:
    WaitForm(void)
    {
      InitializeComponent();
    }

  protected:
    ~WaitForm()
    {
      if (components)
      {
        delete components;
      }
    }
  private: System::Windows::Forms::Label^  WaitLabel;
  protected: 

  private:
    System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    void InitializeComponent(void)
    {
      this->WaitLabel = (gcnew System::Windows::Forms::Label());
      this->SuspendLayout();
      // 
      // WaitLabel
      // 
      this->WaitLabel->AutoSize = true;
      this->WaitLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
        static_cast<System::Byte>(0)));
      this->WaitLabel->Location = System::Drawing::Point(12, 9);
      this->WaitLabel->Name = L"WaitLabel";
      this->WaitLabel->Size = System::Drawing::Size(234, 25);
      this->WaitLabel->TabIndex = 0;
      this->WaitLabel->Text = L"Loading, Please Wait...";
      // 
      // WaitForm
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(258, 43);
      this->Controls->Add(this->WaitLabel);
      this->MaximizeBox = false;
      this->MaximumSize = System::Drawing::Size(274, 79);
      this->MinimizeBox = false;
      this->MinimumSize = System::Drawing::Size(274, 79);
      this->Name = L"WaitForm";
      this->ShowIcon = false;
      this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
      this->Text = L"Message";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  };
}
