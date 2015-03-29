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


// implementation of list editing form

#include "EditListForm.h"

using namespace PGameDB;

// get the list of items from the list form
List<String ^> ^EditListForm::Get_ItemList(void) {
  return ItemList;
}

// Add an item to the list being edited
System::Void EditListForm::AddListItemButton_Click(System::Object^  sender, System::EventArgs^  e) {
  SimpleTextEntryForm ^ail = gcnew SimpleTextEntryForm();
  ail->Set_Title("Add List Item");
  ail->Set_Label("List Item to Add:");
  if (ail->ShowDialog(this) == ::DialogResult::OK) {
    String ^txt = ail->Get_Text();
    bool match = false;
    for each (String ^s in ItemList) {
      if (String::Equals(s, txt)) {
        match = true;
        break;
      }
    }

    // only add the item if it is not a duplicate of one already there
    if (!match) {
      ItemList->Add(txt);
      EditListListBox->Items->Add(txt);
      Refresh();
    }
  }
}

// close the edit list form when cancel button is pressed
System::Void EditListForm::EditListCancelButton_Click(System::Object^  sender, System::EventArgs^  e) {
  Close();
}

// delete an item from list being edited when the delete button is pressed
System::Void EditListForm::DeleteListItem_Click(System::Object^  sender, System::EventArgs^  e) {
  String ^txt = EditListListBox->SelectedItem->ToString();
  ItemList->Remove(txt);
  EditListListBox->Items->Remove(txt);
  EditListListBox->Refresh();
}


// modify the selected list item when modify button is clicked
System::Void EditListForm::ModifyListItemButton_Click(System::Object^  sender, System::EventArgs^  e) {

  if (EditListListBox->SelectedItem != nullptr) {
    String ^origtxt = this->EditListListBox->SelectedItem->ToString();
    SimpleTextEntryForm ^eli = gcnew SimpleTextEntryForm();
    eli->Set_Text(origtxt);
    eli->Set_Title("Edit List Member");
    eli->Set_Label("New List Element Name:");
    if (eli->ShowDialog(this) == ::DialogResult::OK) {
      String ^txt = eli->Get_Text();
      ItemList->Remove(origtxt);
      EditListListBox->Items->Remove(origtxt);

      bool match = false;
      for each (String ^s in ItemList) {
        if (String::Equals(s, txt)) {
          match = true;
          break;
        }
      }

      // only add the item if it is not a duplicate of one already there
      if (!match) {
        ItemList->Add(txt);
        EditListListBox->Items->Add(txt);
        Refresh();
      }
    }
  }
}
