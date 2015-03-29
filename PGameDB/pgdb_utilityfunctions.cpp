#include "stdafx.h"
#include "MainForm.h"
#include "mobyscrape.h"
#include "pgdb_utilityfunctions.h"

using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace PGameDB;
using namespace std;


// generate intrinsic category membership list (platforms, series, etc)
void Generate_CategoryMembership(GameDescriptor ^gd) {
  gd->CategoryMembership->Clear();

  if (gd->Series != nullptr) {
    String ^mystr = String::Concat("Series:", gd->Series);
    gd->CategoryMembership->AddLast(mystr);
  }

  if (gd->Developer != nullptr) {
    String ^mystr = String::Concat("Developer:", gd->Developer);
    gd->CategoryMembership->AddLast(mystr);
  }

  if (gd->Publisher != nullptr) {
    String ^mystr = String::Concat("Publisher:", gd->Publisher);
    gd->CategoryMembership->AddLast(mystr);
  }
  if (gd->Completed != nullptr) {
    String ^mystr = String::Concat("Completed:", gd->Completed);
    gd->CategoryMembership->AddLast(mystr);
  }
  if (gd->CompletedYear != nullptr) {
    String ^mystr = String::Concat("CompletedYear:", gd->CompletedYear);
    gd->CategoryMembership->AddLast(mystr);
  }
  if (gd->Location1 != nullptr) {
    String ^mystr = String::Concat("Location:", gd->Location1);
    gd->CategoryMembership->AddLast(mystr);
  }
  if (gd->Location2 != nullptr) {
    String ^mystr = String::Concat("Location:", gd->Location2);
    gd->CategoryMembership->AddLast(mystr);
  }
  if (gd->Location3 != nullptr) {
    String ^mystr = String::Concat("Location:", gd->Location3);
    gd->CategoryMembership->AddLast(mystr);
  }
  if (gd->Location4 != nullptr) {
    String ^mystr = String::Concat("Location:", gd->Location4);
    gd->CategoryMembership->AddLast(mystr);
  }
  if (gd->PrimaryPlatform != nullptr) {
    String ^mystr = String::Concat("Platform:", gd->PrimaryPlatform);
    gd->CategoryMembership->AddLast(mystr);
  }
  for each (String ^s in gd->AlternatePlatforms) {
    String ^mystr = String::Concat("Platform:",s);
    gd->CategoryMembership->AddLast(mystr);
  }
  if (gd->ReleaseYear != nullptr) {
    String ^mystr = String::Concat("ReleaseYear:", gd->ReleaseYear);
    gd->CategoryMembership->AddLast(mystr);
  }
  for each (String ^s in gd->Genres) {
    String ^mystr = String::Concat("Genre:", s);
    gd->CategoryMembership->AddLast(mystr);
  }
}
