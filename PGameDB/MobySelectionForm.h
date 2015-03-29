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


// Form used to display matching search results from scraping
#pragma once

#include "GameDataStructs.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace PGameDB {


  public ref class MobySelectionForm : public System::Windows::Forms::Form
  {
  public:
    MobySelectionForm(LinkedList<GameMatch ^> ^mmm) {
      MobyMatchList = mmm;

      InitializeComponent();

      MaxItemSize = 0;
    }

  protected:
    ~MobySelectionForm()
    {
      if (components)
      {
        delete components;
      }
    }
  private: System::Windows::Forms::SplitContainer^  splitContainer1;
  public: System::Windows::Forms::ListBox^  MobySelectionList;
  protected: 

  private: System::Windows::Forms::Button^  MobyCancelButton;
  private: System::Windows::Forms::Button^  MobySelectButton;

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
      this->MobySelectionList = (gcnew System::Windows::Forms::ListBox());
      this->MobyCancelButton = (gcnew System::Windows::Forms::Button());
      this->MobySelectButton = (gcnew System::Windows::Forms::Button());
      this->splitContainer1->Panel1->SuspendLayout();
      this->splitContainer1->Panel2->SuspendLayout();
      this->splitContainer1->SuspendLayout();
      this->SuspendLayout();
      // 
      // splitContainer1
      // 
      this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
      this->splitContainer1->IsSplitterFixed = true;
      this->splitContainer1->Location = System::Drawing::Point(0, 0);
      this->splitContainer1->Name = L"splitContainer1";
      this->splitContainer1->Orientation = System::Windows::Forms::Orientation::Horizontal;
      // 
      // splitContainer1.Panel1
      // 
      this->splitContainer1->Panel1->Controls->Add(this->MobySelectionList);
      // 
      // splitContainer1.Panel2
      // 
      this->splitContainer1->Panel2->Controls->Add(this->MobyCancelButton);
      this->splitContainer1->Panel2->Controls->Add(this->MobySelectButton);
      this->splitContainer1->Size = System::Drawing::Size(326, 204);
      this->splitContainer1->SplitterDistance = 174;
      this->splitContainer1->SplitterWidth = 1;
      this->splitContainer1->TabIndex = 3;
      // 
      // MobySelectionList
      // 
      this->MobySelectionList->Dock = System::Windows::Forms::DockStyle::Fill;
      this->MobySelectionList->FormattingEnabled = true;
      this->MobySelectionList->Location = System::Drawing::Point(0, 0);
      this->MobySelectionList->Name = L"MobySelectionList";
      this->MobySelectionList->ScrollAlwaysVisible = true;
      this->MobySelectionList->Size = System::Drawing::Size(326, 173);
      this->MobySelectionList->TabIndex = 0;
      // 
      // MobyCancelButton
      // 
      this->MobyCancelButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->MobyCancelButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->MobyCancelButton->Location = System::Drawing::Point(239, 3);
      this->MobyCancelButton->Name = L"MobyCancelButton";
      this->MobyCancelButton->Size = System::Drawing::Size(75, 23);
      this->MobyCancelButton->TabIndex = 1;
      this->MobyCancelButton->Text = L"Cancel";
      this->MobyCancelButton->UseVisualStyleBackColor = true;
      this->MobyCancelButton->Click += gcnew System::EventHandler(this, &MobySelectionForm::MobyCancelButton_Click);
      // 
      // MobySelectButton
      // 
      this->MobySelectButton->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->MobySelectButton->Location = System::Drawing::Point(12, 3);
      this->MobySelectButton->Name = L"MobySelectButton";
      this->MobySelectButton->Size = System::Drawing::Size(75, 23);
      this->MobySelectButton->TabIndex = 0;
      this->MobySelectButton->Text = L"Select";
      this->MobySelectButton->UseVisualStyleBackColor = true;
      this->MobySelectButton->Click += gcnew System::EventHandler(this, &MobySelectionForm::MobySelectButton_Click);
      // 
      // MobySelectionForm
      // 
      this->AcceptButton = this->MobySelectButton;
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->AutoScroll = true;
      this->AutoSize = true;
      this->CancelButton = this->MobyCancelButton;
      this->ClientSize = System::Drawing::Size(326, 204);
      this->Controls->Add(this->splitContainer1);
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"MobySelectionForm";
      this->ShowIcon = false;
      this->ShowInTaskbar = false;
      this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
      this->Text = L"Select Game To Update From";
      this->splitContainer1->Panel1->ResumeLayout(false);
      this->splitContainer1->Panel2->ResumeLayout(false);
      this->splitContainer1->ResumeLayout(false);
      this->ResumeLayout(false);

    }
#pragma endregion
  private:
    int MaxItemSize;
    LinkedList<GameMatch ^> ^MobyMatchList;

  public: 

    // add a search result to the form and resize the form appropriately
    void Add_ListItem(String ^listtitle) {
      System::Drawing::Size sds = System::Windows::Forms::TextRenderer::MeasureText(listtitle, this->MobySelectionList->Font);
      int renlen = sds.Width;
      this->MobySelectionList->Items->Add(listtitle);
      if (renlen > MaxItemSize) {
        MaxItemSize = renlen;
        this->Width = renlen + 35;
      }
      if (Width < 242) {
        Width = 242;
      }
    }


  private: 
    System::Void MobySelectButton_Click(System::Object^  sender, System::EventArgs^  e) { Close(); }
    System::Void MobyCancelButton_Click(System::Object^  sender, System::EventArgs^  e) { Close(); }

  };
}
