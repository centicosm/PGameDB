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



#ifndef GAMEDESCRIPTOR_H
#define GAMEDESCRIPTOR_H

#include <string>
#include <list>

using namespace System;
using namespace Collections;
using namespace Data;
using namespace Collections::Generic;

// class used to store search matches when scraping
public ref class GameMatch {
public:
  GameMatch(void) {}
  ~GameMatch(void) {}

  String ^Name;
  String ^Platform;
  String ^URL;
  String ^PubYear;
};

// used to pass config information to the scraper
public ref class ScrapeConfig {
public:
  ScrapeConfig(void) {}
  ~ScrapeConfig(void) {}

  int NumScreenShots;
};

// type used in documentation system
enum PGameDocType {
  MANUAL,
  GUIDE
};

// class used to describe a document associated with a game
public ref class DocumentationID {
public:
  String ^Filename;
  String ^FileType;
};


// core data structure used to store game data
public ref class GameDescriptor {
public:
  GameDescriptor(void) {
    Genres = gcnew LinkedList<String ^>();
    AlternatePlatforms = gcnew LinkedList<String ^>();
    Screenshots = gcnew LinkedList<String ^>();
    CategoryMembership = gcnew LinkedList<String ^>();
    CustomCategoryMembership = gcnew LinkedList<String ^>();
    ManualDocumentation = gcnew LinkedList<DocumentationID ^>();
    HintDocumentation = gcnew LinkedList<DocumentationID ^>();
  }

  ~GameDescriptor(void) {}

  GameDescriptor(GameDescriptor ^gd) {
    GameName = gcnew String(gd->GameName);
    Publisher = gcnew String(gd->Publisher);
    Developer = gcnew String(gd->Developer);
    Description = gcnew String(gd->Description);
    ReleaseYear = gcnew String(gd->ReleaseYear);
    Series = gcnew String(gd->Series);
    Genres = gcnew LinkedList<String ^>(gd->Genres);
    PrimaryPlatform = gcnew String(gd->PrimaryPlatform);
    AlternatePlatforms = gcnew LinkedList<String ^>(gd->AlternatePlatforms);
    TitleShot = gcnew String(gd->TitleShot);
    BackShot = gcnew String(gd->BackShot);
    Screenshots = gcnew LinkedList<String ^>(gd->Screenshots);
    Rating = gcnew String(gd->Rating);
    Completed = gcnew String(gd->Completed);
    CompletedYear = gcnew String(gd->CompletedYear);
    Location1 = gcnew String(gd->Location1);
    Location2 = gcnew String(gd->Location2);
    Location3 = gcnew String(gd->Location3);
    Location4 = gcnew String(gd->Location4);
    Notes = gcnew String(Notes);
  }

  String ^GameName;
  String ^Publisher;
  String ^Developer;
  String ^Description;
  String ^ReleaseYear;
  String ^Series;
  LinkedList<String ^> ^Genres;
  String ^PrimaryPlatform;
  LinkedList<String ^> ^AlternatePlatforms;
  String ^TitleShot;
  String ^BackShot;
  LinkedList<String ^> ^Screenshots;
  String ^Rating;
  String ^Completed;
  String ^CompletedYear;
  String ^Location1;
  String ^Location2;
  String ^Location3;
  String ^Location4;
  String ^Notes;

  LinkedList<DocumentationID ^> ^ManualDocumentation;
  LinkedList<DocumentationID ^> ^HintDocumentation;

  LinkedList<String ^> ^CategoryMembership;
  LinkedList<String ^> ^CustomCategoryMembership;
  String ^GameURL;


  // generate intrinsic category membership list (platforms, series, etc)
  void Generate_CategoryMembership(void) {
    CategoryMembership->Clear();

    if (Series != nullptr) {
      String ^mystr = String::Concat("Series:", Series);
      CategoryMembership->AddLast(mystr);
    }

    if (Developer != nullptr) {
      String ^mystr = String::Concat("Developer:", Developer);
      CategoryMembership->AddLast(mystr);
    }
    if (Rating != nullptr) {
      String ^mystr = String::Concat("Rating:", Rating);
      CategoryMembership->AddLast(mystr);
    }
    if (Publisher != nullptr) {
      String ^mystr = String::Concat("Publisher:", Publisher);
      CategoryMembership->AddLast(mystr);
    }
    if (Completed != nullptr) {
      String ^mystr = String::Concat("Completed:", Completed);
      CategoryMembership->AddLast(mystr);
    }
    if (CompletedYear != nullptr) {
      String ^mystr = String::Concat("CompletedYear:", CompletedYear);
      CategoryMembership->AddLast(mystr);
    }
    if (Location1 != nullptr) {
      String ^mystr = String::Concat("Location:", Location1);
      CategoryMembership->AddLast(mystr);
    }
    if (Location2 != nullptr) {
      String ^mystr = String::Concat("Location:", Location2);
      CategoryMembership->AddLast(mystr);
    }
    if (Location3 != nullptr) {
      String ^mystr = String::Concat("Location:", Location3);
      CategoryMembership->AddLast(mystr);
    }
    if (Location4 != nullptr) {
      String ^mystr = String::Concat("Location:", Location4);
      CategoryMembership->AddLast(mystr);
    }
    if (PrimaryPlatform != nullptr) {
      String ^mystr = String::Concat("Platform:", PrimaryPlatform);
      CategoryMembership->AddLast(mystr);
    }
    for each (String ^s in AlternatePlatforms) {
      String ^mystr = String::Concat("Platform:",s);
      CategoryMembership->AddLast(mystr);
    }
    if (ReleaseYear != nullptr) {
      String ^mystr = String::Concat("ReleaseYear:", ReleaseYear);
      CategoryMembership->AddLast(mystr);
    }
    for each (String ^s in Genres) {
      String ^mystr = String::Concat("Genre:", s);
      CategoryMembership->AddLast(mystr);
    }

    for each (String ^s in CustomCategoryMembership) {
      String ^mystr = String::Concat("Custom:", s);
      CategoryMembership->AddLast(mystr);
    }

  }



};

// class used to store the sets of possible values for combo-boxes, etc
public ref class DBCats {
public:
  DBCats(void) {}

  ~DBCats(void) {}

  LinkedList<String ^> ^Series;
  LinkedList<String ^> ^Developer;
  LinkedList<String ^> ^Publisher;
  List<String ^> ^Platforms;
  List<String ^> ^Genres;
  List<String ^> ^Year;
  LinkedList<String ^> ^Location1;
  LinkedList<String ^> ^Location2;
  LinkedList<String ^> ^Location3;
  LinkedList<String ^> ^Location4;
  List<String ^> ^CustomCategoryMembership;

};

#endif