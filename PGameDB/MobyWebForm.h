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


// Form implementing a web browser to view URLs associated with games

#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace PGameDB {

  public ref class MobyWebForm : public System::Windows::Forms::Form
  {
  public:
    MobyWebForm(String ^url)
    {
      InitializeComponent();

      this->MobyGameBrowser->Navigate(url);
    }

  protected:
    ~MobyWebForm()
    {
      if (components)
      {
        delete components;
      }
    }
  private: System::Windows::Forms::WebBrowser^  MobyGameBrowser;
  protected: 

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
      this->MobyGameBrowser = (gcnew System::Windows::Forms::WebBrowser());
      this->SuspendLayout();
      // 
      // MobyGameBrowser
      // 
      this->MobyGameBrowser->AllowWebBrowserDrop = false;
      this->MobyGameBrowser->Dock = System::Windows::Forms::DockStyle::Fill;
      this->MobyGameBrowser->IsWebBrowserContextMenuEnabled = false;
      this->MobyGameBrowser->Location = System::Drawing::Point(0, 0);
      this->MobyGameBrowser->MinimumSize = System::Drawing::Size(20, 20);
      this->MobyGameBrowser->Name = L"MobyGameBrowser";
      this->MobyGameBrowser->ScriptErrorsSuppressed = true;
      this->MobyGameBrowser->Size = System::Drawing::Size(784, 564);
      this->MobyGameBrowser->TabIndex = 0;
      // 
      // MobyWebForm
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(784, 564);
      this->Controls->Add(this->MobyGameBrowser);
      this->Name = L"MobyWebForm";
      this->ShowIcon = false;
      this->Text = L"Moby Games Summary";
      this->ResumeLayout(false);

    }
#pragma endregion
  };
}
