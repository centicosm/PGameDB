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



#ifndef ALPHNODESORTER_H
#define ALPHNODESORTER_H

using namespace System::Data;
using namespace System::Collections::Generic;
using namespace System::Collections;


// a utility function/class used to sort the treenodes alphabetically
ref class AlphNodeSorter : public System::Collections::IComparer {
public:
  virtual int Compare(Object^ x, Object^ y) {
    TreeNode^ tx = (TreeNode^)x;
    TreeNode^ ty = (TreeNode^)y;
    return String::Compare(tx->Text, ty->Text);
  }
};


#endif