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



// Simple generic form to edit lists (Genres, platforms, etc).

#pragma once

#include "SimpleTextEntryForm.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace PGameDB {

  public ref class EditListForm : public System::Windows::Forms::Form
  {
  public:
    EditListForm(List<String ^> ^itemlist) {

      ItemList = itemlist;
      InitializeComponent();

      // populate the list form with initial values
      for each (String ^s in ItemList) {
        this->EditListListBox->Items->Add(s);
      }
    }


  protected:
    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    ~EditListForm()
    {
      if (components)
      {
        delete components;
      }
    }
  private: System::Windows::Forms::SplitContainer^  splitContainer1;
  protected: 
  private: System::Windows::Forms::SplitContainer^  splitContainer2;


  private: System::Windows::Forms::Button^  DeleteListItem;

  private: System::Windows::Forms::Button^  ModifyListItemButton;



  private: System::Windows::Forms::Button^  AddListItemButton;
  private: System::Windows::Forms::Button^  EditListCancelButton;


  private: System::Windows::Forms::Button^  EditListOkButton;
  private: System::Windows::Forms::ListBox^  EditListListBox;



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
      this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
      this->EditListListBox = (gcnew System::Windows::Forms::ListBox());
      this->DeleteListItem = (gcnew System::Windows::Forms::Button());
      this->ModifyListItemButton = (gcnew System::Windows::Forms::Button());
      this->AddListItemButton = (gcnew System::Windows::Forms::Button());
      this->EditListCancelButton = (gcnew System::Windows::Forms::Button());
      this->EditListOkButton = (gcnew System::Windows::Forms::Button());
      this->splitContainer1->Panel1->SuspendLayout();
      this->splitContainer1->Panel2->SuspendLayout();
      this->splitContainer1->SuspendLayout();
      this->splitContainer2->Panel1->SuspendLayout();
      this->splitContainer2->Panel2->SuspendLayout();
      this->splitContainer2->SuspendLayout();
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
      this->splitContainer1->Panel1->Controls->Add(this->splitContainer2);
      // 
      // splitContainer1.Panel2
      // 
      this->splitContainer1->Panel2->Controls->Add(this->EditListCancelButton);
      this->splitContainer1->Panel2->Controls->Add(this->EditListOkButton);
      this->splitContainer1->Size = System::Drawing::Size(284, 252);
      this->splitContainer1->SplitterDistance = 218;
      this->splitContainer1->SplitterWidth = 1;
      this->splitContainer1->TabIndex = 0;
      // 
      // splitContainer2
      // 
      this->splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
      this->splitContainer2->IsSplitterFixed = true;
      this->splitContainer2->Location = System::Drawing::Point(0, 0);
      this->splitContainer2->Name = L"splitContainer2";
      // 
      // splitContainer2.Panel1
      // 
      this->splitContainer2->Panel1->Controls->Add(this->EditListListBox);
      // 
      // splitContainer2.Panel2
      // 
      this->splitContainer2->Panel2->Controls->Add(this->DeleteListItem);
      this->splitContainer2->Panel2->Controls->Add(this->ModifyListItemButton);
      this->splitContainer2->Panel2->Controls->Add(this->AddListItemButton);
      this->splitContainer2->Size = System::Drawing::Size(284, 218);
      this->splitContainer2->SplitterDistance = 196;
      this->splitContainer2->SplitterWidth = 1;
      this->splitContainer2->TabIndex = 0;
      // 
      // EditListListBox
      // 
      this->EditListListBox->Dock = System::Windows::Forms::DockStyle::Fill;
      this->EditListListBox->FormattingEnabled = true;
      this->EditListListBox->Location = System::Drawing::Point(0, 0);
      this->EditListListBox->Name = L"EditListListBox";
      this->EditListListBox->Size = System::Drawing::Size(196, 212);
      this->EditListListBox->Sorted = true;
      this->EditListListBox->TabIndex = 0;
      // 
      // DeleteListItem
      // 
      this->DeleteListItem->Location = System::Drawing::Point(6, 139);
      this->DeleteListItem->Name = L"DeleteListItem";
      this->DeleteListItem->Size = System::Drawing::Size(75, 23);
      this->DeleteListItem->TabIndex = 2;
      this->DeleteListItem->Text = L"Delete";
      this->DeleteListItem->UseVisualStyleBackColor = true;
      this->DeleteListItem->Click += gcnew System::EventHandler(this, &EditListForm::DeleteListItem_Click);
      // 
      // ModifyListItemButton
      // 
      this->ModifyListItemButton->Location = System::Drawing::Point(6, 90);
      this->ModifyListItemButton->Name = L"ModifyListItemButton";
      this->ModifyListItemButton->Size = System::Drawing::Size(75, 23);
      this->ModifyListItemButton->TabIndex = 1;
      this->ModifyListItemButton->Text = L"Modify";
      this->ModifyListItemButton->UseVisualStyleBackColor = true;
      this->ModifyListItemButton->Click += gcnew System::EventHandler(this, &EditListForm::ModifyListItemButton_Click);
      // 
      // AddListItemButton
      // 
      this->AddListItemButton->Location = System::Drawing::Point(6, 38);
      this->AddListItemButton->Name = L"AddListItemButton";
      this->AddListItemButton->Size = System::Drawing::Size(75, 23);
      this->AddListItemButton->TabIndex = 0;
      this->AddListItemButton->Text = L"Add";
      this->AddListItemButton->UseVisualStyleBackColor = true;
      this->AddListItemButton->Click += gcnew System::EventHandler(this, &EditListForm::AddListItemButton_Click);
      // 
      // EditListCancelButton
      // 
      this->EditListCancelButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->EditListCancelButton->Location = System::Drawing::Point(197, 5);
      this->EditListCancelButton->Name = L"EditListCancelButton";
      this->EditListCancelButton->Size = System::Drawing::Size(75, 23);
      this->EditListCancelButton->TabIndex = 1;
      this->EditListCancelButton->Text = L"Cancel";
      this->EditListCancelButton->UseVisualStyleBackColor = true;
      this->EditListCancelButton->Click += gcnew System::EventHandler(this, &EditListForm::EditListCancelButton_Click);
      // 
      // EditListOkButton
      // 
      this->EditListOkButton->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->EditListOkButton->Location = System::Drawing::Point(12, 5);
      this->EditListOkButton->Name = L"EditListOkButton";
      this->EditListOkButton->Size = System::Drawing::Size(75, 23);
      this->EditListOkButton->TabIndex = 0;
      this->EditListOkButton->Text = L"Ok";
      this->EditListOkButton->UseVisualStyleBackColor = true;
      // 
      // EditListForm
      // 
      this->AcceptButton = this->EditListOkButton;
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->CancelButton = this->EditListCancelButton;
      this->ClientSize = System::Drawing::Size(284, 252);
      this->Controls->Add(this->splitContainer1);
      this->Name = L"EditListForm";
      this->ShowIcon = false;
      this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
      this->Text = L"Edit List";
      this->splitContainer1->Panel1->ResumeLayout(false);
      this->splitContainer1->Panel2->ResumeLayout(false);
      this->splitContainer1->ResumeLayout(false);
      this->splitContainer2->Panel1->ResumeLayout(false);
      this->splitContainer2->Panel2->ResumeLayout(false);
      this->splitContainer2->ResumeLayout(false);
      this->ResumeLayout(false);

    }

  public:
    List<String ^> ^Get_ItemList(void);

#pragma endregion
  private: 
    System::Void AddListItemButton_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void EditListCancelButton_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void DeleteListItem_Click(System::Object^  sender, System::EventArgs^  e);
    System::Void ModifyListItemButton_Click(System::Object^  sender, System::EventArgs^  e);

    // class members
    List<String ^> ^ItemList;
  };
}
