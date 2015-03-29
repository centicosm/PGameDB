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


// PGameDB.cpp : main project file.

#include "stdio.h"
#include "PGameDB.h"
#include "MainForm.h"


using namespace PGameDB;
using namespace std;

ConfigManager *gConfMan;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// This is where we load the config file
  gConfMan = new ConfigManager("PGameDB.cfg");

  // Create the main window and run it
  String ^initdb;
  if (args->Length == 0) {
    initdb = nullptr;
  }
  else {
    initdb = args[0];
  }
    Application::Run(gcnew MainForm(initdb));
	return 0;
}

