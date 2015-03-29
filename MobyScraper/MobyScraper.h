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


// a moby games scraper for the game db
#include "../PGameDB/configmanager.h"

#pragma once

using namespace System;
using namespace System::IO;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Data;
using namespace System::Net;

// Get_Fields and Get_MatchList must exist in any scraper used by the program

public ref class Scraper {
public:
  Scraper(void) {};
  ~Scraper(void) {};

  // Get the fields of the DB from the moby entry
  void Get_Fields(String ^url, GameDescriptor ^gd, ScrapeConfig ^sc);

  // get the list of matching games based on search
  LinkedList<GameMatch ^> ^Get_MatchList(String ^text);

private:

  // aux functions to help scrape moby pages
  String ^Get_Page(String ^url);
  String ^Get_MobyImage(String ^url, String ^gamename); 
  String ^Clean_TextSymbols(String ^txt);
  String ^Get_TitleShot(System::String ^url, System::String ^name, String ^pub, String ^year);
  String ^Get_BackShot(String ^url, String ^name, String ^pub, String ^year);
  LinkedList<String ^> ^Get_ScreenShots(String ^url, String ^name, int curnss, String ^pub, String ^year);

  ScrapeConfig ^Config;
};





