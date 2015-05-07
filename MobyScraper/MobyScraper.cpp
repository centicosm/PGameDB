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


// Implementation of the mobygames scraper  plugin

#include "MobyScraper.h"

// get a web page at url and return it as a string

String ^Scraper::Get_Page(String ^url) {

  WebRequest^ request = WebRequest::Create(url);
  request->Credentials = CredentialCache::DefaultCredentials;
  HttpWebResponse^ response = dynamic_cast<HttpWebResponse^>(request->GetResponse());
  Stream^ dataStream = response->GetResponseStream();
  StreamReader^ reader = gcnew StreamReader( dataStream );
  String^ responseFromServer = reader->ReadToEnd();

  reader->Close();
  dataStream->Close();
  response->Close();

  return responseFromServer;
}


// return a list of possible matches for a search string from mobygames
LinkedList<GameMatch ^> ^Scraper::Get_MatchList(String ^text) {

  String ^mobyreq = "http://www.mobygames.com/search/quick/p,-1/q,";
  mobyreq = String::Concat(mobyreq, text);
  mobyreq = String::Concat(mobyreq, "/showOnly,9/");
  String ^page = Get_Page(mobyreq);

  LinkedList<GameMatch ^> ^ml = gcnew LinkedList<GameMatch ^>();

  int endtableindex;
  int startindex = page->IndexOf("searchTitle\">Game:");
  int masterstartindex = startindex;

  int linkindex, lastindex;

  while (startindex != -1) {

    GameMatch ^mm = gcnew GameMatch();

    endtableindex = page->IndexOf("<br clear=\"all\">", startindex);
    String ^entrytext = page->Substring(startindex, endtableindex - startindex); 

    // get the link
    linkindex = entrytext->IndexOf("game");
    linkindex += 4;
    lastindex = entrytext->IndexOf(">", linkindex);

    int tempindex = entrytext->IndexOf("/", linkindex, lastindex - linkindex);
    while (tempindex != -1) {
      tempindex = entrytext->IndexOf("/", tempindex + 1, lastindex - tempindex);
      if (tempindex != -1) {
        linkindex = tempindex;
      }
    }
    mm->URL = entrytext->Substring(linkindex + 1, (lastindex - 1) - (linkindex + 1));

    // make sure that we are right before the game text.
    startindex = entrytext->IndexOf(mm->URL);

    // get game name
    startindex = entrytext->IndexOf(">", startindex);
    lastindex = entrytext->IndexOf("<", startindex);
    mm->Name = entrytext->Substring(startindex + 1, (lastindex) - (startindex + 1));
    mm->Name = Clean_TextSymbols(mm->Name);  

    // get pub and year
    startindex = entrytext->IndexOf("searchDetails", lastindex);
    startindex = entrytext->IndexOf("<em>", startindex);
    lastindex = entrytext->IndexOf("</em>", startindex);
    mm->PubYear = entrytext->Substring(startindex + 4, (lastindex) - (startindex + 1) - 3);
    mm->PubYear = Clean_TextSymbols(mm->PubYear);

    // get platform not implemented
    //lastindex = entrytext->IndexOf("<", startindex);
    //mm->Platform = entrytext->Substring(startindex + 1, (lastindex) - (startindex + 1));
    //mm->Platform = Clean_TextSymbols(mm->Platform);

    ml->AddLast(mm);

    startindex = page->IndexOf("searchTitle\">Game:", masterstartindex + 1);
    masterstartindex = startindex;
  }
  return ml;
}


// Get the fields of the DB from the moby entry
void Scraper::Get_Fields(String ^url, GameDescriptor ^gd, ScrapeConfig ^sc) {
  Config = sc;

  url = String::Concat("http://www.mobygames.com/game/", url);
  int stopindex;

  String ^page = Get_Page(url);

  // grab the title
  int startindex = page->IndexOf("niceHeaderTitle");
  startindex = page->IndexOf(">", startindex);
  startindex = page->IndexOf(">", startindex + 1);
  stopindex = page->IndexOf("<", startindex);
  gd->GameName = page->Substring(startindex + 1, stopindex - startindex - 1);
  gd->GameName = Clean_TextSymbols(gd->GameName);

  // grab a game cover if available
  String ^titleurl = "";
  int conexist = 1;

  int coverstart = page->IndexOf("coreGameCover", stopindex);
  if (coverstart != -1) {

    conexist = page->IndexOf("noCoverArt", coverstart);
    if (conexist == -1) {
      coverstart = page->IndexOf("/", coverstart);
      stopindex = page->IndexOf(">", coverstart);
      titleurl = page->Substring(coverstart, stopindex - coverstart - 1);

      titleurl = String::Concat("http://www.mobygames.com", titleurl);
    }
  }

  // Grab the publisher
  int existcheck = page->IndexOf("Published by", startindex);
  if (existcheck != -1) {
    startindex = page->IndexOf("company", stopindex);
    if (startindex != -1) {
      startindex = page->IndexOf(">", startindex);
      stopindex = page->IndexOf("<", startindex);
      gd->Publisher = page->Substring(startindex + 1, stopindex - startindex - 1);
      gd->Publisher = Clean_TextSymbols(gd->Publisher);
    }
  }

  //grab the developer
  existcheck = page->IndexOf("Developed by", startindex);
  if (existcheck != -1) {
    startindex = page->IndexOf("company", stopindex);
    if (startindex != -1) {
      startindex = page->IndexOf(">", startindex);
      stopindex = page->IndexOf("<", startindex);
      gd->Developer = page->Substring(startindex + 1, stopindex - startindex - 1);
      gd->Developer = Clean_TextSymbols(gd->Developer);
    }
  }

  //grab the release year
  startindex = page->IndexOf("release-info", stopindex);
  if (startindex != -1) {
    startindex = page->IndexOf(">", startindex);
    stopindex = page->IndexOf("<", startindex);
    gd->ReleaseYear = page->Substring(startindex + 1, stopindex - startindex - 1);

    // if there is a full date we just want to get the year
    int commaindex = gd->ReleaseYear->IndexOf(',');
    int commastopindex = gd->ReleaseYear->Length;
    if (commaindex != -1) {
      gd->ReleaseYear = gd->ReleaseYear->Substring(commaindex + 2, 4);
    }
    gd->ReleaseYear = Clean_TextSymbols(gd->ReleaseYear);
  }


  // now actually get the covers
  if ((coverstart != -1)  && (conexist == -1)) {
    if (titleurl->IndexOf("notonfile") == -1) {
      gd->TitleShot = Get_TitleShot(titleurl, gd->GameName, gd->Publisher, gd->ReleaseYear);
    }
    else {
      gd->TitleShot = "";
    }
  }
  else {
    gd->TitleShot = "";
  }
  // grab back cover if available
  if ((conexist == -1) && (titleurl->IndexOf("notonfile") == -1)) {
    gd->BackShot = Get_BackShot(titleurl, gd->GameName, gd->Publisher, gd->ReleaseYear);
  }
  else {
    gd->BackShot = "";
  }

  //grab the description
  startindex = page->IndexOf("Description", stopindex);
  if (startindex != -1) {
    startindex = page->IndexOf(">", startindex);
    stopindex = page->IndexOf("<div", startindex);
    gd->Description = page->Substring(startindex + 1, stopindex - startindex - 1);
    gd->Description = Clean_TextSymbols(gd->Description);
  }

  // grab the screenshots
  gd->Screenshots->Clear();
  LinkedList<String ^> ^screenshots = Get_ScreenShots(url, gd->GameName, 0, gd->Publisher, gd->ReleaseYear);
  for each (String ^s in screenshots) {
    gd->Screenshots->AddLast(s);
  }

  gd->GameURL = url;
}


// grab a specified number of screenshots under curnss
LinkedList<String ^> ^Scraper::Get_ScreenShots(String ^url, String ^name, int curnss, String ^pub, String ^year) {

  name = name->Replace("?", "");

  int numss = Config->NumScreenShots;

  LinkedList<String ^> ^ss = gcnew LinkedList<String ^>();
  numss = numss - curnss;
  if (numss <= 0) {
    return ss;
  }

  url = String::Concat(url, "/screenshots");
  String ^page = Get_Page(url);

  if (page->IndexOf("There are no screenshots") != -1) {
    return ss;
  }

  // count the number of images
  array<int> ^picindex = gcnew array<int>(1000);

  int curindex = 0;
  int piccount = 0;
  while (curindex != -1) {

    //curindex = page->IndexOf("col-xs-4 col-sm-4 col-md-2", curindex + 1);
	curindex = page->IndexOf("div class=\"thumbnail\"", curindex + 1);
    if (curindex != -1) {
      picindex[piccount] = curindex;
      piccount++;
    }
  }

  // set the number of pics to get
  int numpictoget = 0;
  if (piccount < numss) {
    numpictoget = piccount;
  }
  else {
    numpictoget = numss;
  }

  // actually grab the pics
  int startindex;
  int stopindex;
  String ^imgurl;
  for (int i = 0; i < numpictoget; i++) {
    startindex = page->IndexOf("a href=", picindex[(piccount / numpictoget) * i] );
    startindex = page->IndexOf("\"", startindex);
    stopindex = page->IndexOf("\"", startindex + 1);
    imgurl = page->Substring(startindex + 1, stopindex - startindex -1);

    // grab the actual shot cover
    imgurl = String::Concat("http://www.mobygames.com", imgurl);
    String ^txt = Get_Page(imgurl);
    startindex = txt->IndexOf("/strong>");
    startindex = txt->IndexOf("src=", startindex);
    startindex += 5;
    int stopindex = txt->IndexOf("\"", startindex);

    String ^pngurl = txt->Substring(startindex, stopindex - startindex);

    char buf[100];
    sprintf_s(buf, "%d", i);
    String ^filename = String::Concat(name, "__");
    filename = String::Concat(filename, pub);
    filename = String::Concat(filename, year);
    filename = String::Concat(filename, "_ScreenShot");
    String ^tempstr = gcnew String(buf);
    filename = String::Concat(filename, tempstr);
    filename = filename->Replace(":", "");
    filename = filename->Replace(".", "");
    filename = filename->Replace(" ", "_");
    filename = filename->Replace("/", "_");
    filename = filename->Replace("\\", "_");
    filename = filename->Replace("^", "");
    filename = filename->Replace(".", "");
    filename = filename->Replace("*", "");
    filename = String::Concat(filename, ".");

    // handle jpeg/jpg cases
    if (pngurl->IndexOf("jpeg") != -1) {
      filename = String::Concat(filename, pngurl->Substring(pngurl->Length - 4, 4));
    }
    else {
      filename = String::Concat(filename, pngurl->Substring(pngurl->Length - 3, 3));
    }

    String ^img = Get_MobyImage(pngurl, filename);
    ss->AddLast(filename);
  }
  return ss;
}


// grab the front cover
String ^Scraper::Get_TitleShot(String ^url, String ^name, String ^pub, String ^year) {

  name = name->Replace("?", "");

  String ^txt = Get_Page(url);
  int startindex = txt->IndexOf("back&nbsp;to&nbsp;thumbs");

  // no cover found
  if (startindex == -1) {
    return nullptr;
  }

  startindex = txt->IndexOf("src", startindex);
  startindex += 5;
  int stopindex = txt->IndexOf("\"", startindex);
  String ^imgurl = txt->Substring(startindex, stopindex - startindex);

  String ^filename = String::Concat(name, "__");
  filename = String::Concat(filename, pub);
  filename = String::Concat(filename, year);
  filename = String::Concat(filename, "_Title");
  filename = filename->Replace(":", "");
  filename = filename->Replace(".", "");
  filename = filename->Replace(" ", "_");
  filename = filename->Replace("/", "_");
  filename = filename->Replace("\\", "_");
  filename = filename->Replace("^", "");
  filename = filename->Replace(".", "");
  filename = filename->Replace("*", "");
  filename = String::Concat(filename, ".jpg");
  String ^img = Get_MobyImage(imgurl, filename);

  return filename;
}


// grab the back cover too if available
String ^Scraper::Get_BackShot(String ^url, String ^name, String ^pub, String ^year) {
  String ^img;

  name = name->Replace("?", "");

  String ^txt = Get_Page(url);

  int startindex = txt->IndexOf("Front&nbsp;Cover");
  int stopindex = txt->IndexOf("Back&nbsp;Cover", startindex);
  if (stopindex == -1) {
    return nullptr;
  }

  startindex = txt->LastIndexOf("a href", stopindex);
  startindex += 8;
  stopindex = txt->IndexOf("\"", startindex);
  String ^imgurl = txt->Substring(startindex, stopindex - startindex);
  if (imgurl->IndexOf("mobygames") == -1) {
    imgurl = String::Concat("http://www.mobygames.com", imgurl);
  }
  txt = Get_Page(imgurl);


  startindex = txt->IndexOf("back&nbsp;to&nbsp;thumbs");
  startindex = txt->IndexOf("src", startindex);
  startindex += 5;
  stopindex = txt->IndexOf("\"", startindex);
  imgurl = txt->Substring(startindex, stopindex - startindex);

  String ^filename = String::Concat(name, "__");
  filename = String::Concat(filename, pub);
  filename = String::Concat(filename, year);
  filename = String::Concat(filename, "_Back");
  filename = filename->Replace(":", "");
  filename = filename->Replace(".", "");
  filename = filename->Replace(" ", "_");
  filename = filename->Replace("/", "_");
  filename = filename->Replace("\\", "_");
  filename = filename->Replace("^", "");
  filename = filename->Replace(".", "");
  filename = filename->Replace("*", "");
  filename = String::Concat(filename, ".jpg");
  img = Get_MobyImage(imgurl, filename);

  return filename;
}



// clean up html in the fields retreived from moby
String ^Scraper::Clean_TextSymbols(String ^txt) {

  txt = txt->Replace("&#x22;", "\"");
  txt = txt->Replace("&#x26;", "&");
  txt = txt->Replace("&#x27;", "'");
  txt = txt->Replace("&nbsp;", " ");
  txt = txt->Replace("&#xEB;", "e");
  txt = txt->Replace("&#xEA;", "e");
  txt = txt->Replace("&#xE4;", "a");
  txt = txt->Replace("&#x14C;", "O");
  txt = txt->Replace("&#xE9;", "e");
  txt = txt->Replace("&#xE0;", "a");
  txt = txt->Replace("&#xF6;", "o");
  txt = txt->Replace("&#xF4;", "o");
  txt = txt->Replace("&#xED;", "i");
  txt = txt->Replace("&#xB2;", "^2");
  txt = txt->Replace("&#x14D;", "O");
  txt = txt->Replace("&#x2219;", "");
  txt = txt->Replace("&#x39B;", "^");
  txt = txt->Replace("&#xE8;", "e");
  txt = txt->Replace("&#xB3;", "^3");
  txt = txt->Replace("&#xD4;", "O");
  txt = txt->Replace("&#xFB;", "u");
  txt = txt->Replace("&#xEE;", "i");
  txt = txt->Replace("&#xE2;", "a");
  txt = txt->Replace("&#xFD;", "y");
  txt = txt->Replace("&#xE1;", "a");
  txt = txt->Replace("&#xB7;", "-");
  txt = txt->Replace("&#xFC;", "u");
  txt = txt->Replace("&#xF3;", "o");
  txt = txt->Replace("&#xF1;", "n");
  txt = txt->Replace("&#xBD;", ".5");
  txt = txt->Replace("&#x2019;", "'");

  bool removehtml = false;
  int startindex;
  int stopindex;
  while (!removehtml) {
    startindex = txt->IndexOf("<a href");
    if (startindex != -1) {
      stopindex = txt->IndexOf(">", startindex);

      if (stopindex == -1) {
        return txt;
      }
      else {
        txt = txt->Remove(startindex, stopindex - startindex + 1);
      }
    }
    else {
      removehtml = true;
    }
  }
  txt = txt->Replace("\n", "<br>");
  txt = txt->Replace("</a>", "");
  return txt;
}




// get an image at url from mobygames and save it locally as filename
System::String ^Scraper::Get_MobyImage(String ^url, String ^filename) {

  // handle when full url is not available
  if (url->IndexOf("mobygames") == -1) {
    url = String::Concat("http://www.mobygames.com", url);
  }
  WebRequest^ request = WebRequest::Create(url);
  request->Credentials = CredentialCache::DefaultCredentials;
  HttpWebResponse^ response = dynamic_cast<HttpWebResponse^>(request->GetResponse());
  Stream^ datastream = response->GetResponseStream();

  int length = (int)response->ContentLength;
  array<Byte> ^image = gcnew array<Byte>(length);

  for (int i = 0 ; i < length; i++) {
    image[i] = datastream->ReadByte();
  }

  // add the tmp datapath to the filename
  const char *datapath = "./tmp/";
  String ^dp = gcnew String(datapath);
  filename = filename->Replace("/", "");
  filename = String::Concat(dp, filename);
  Stream^ outstream = System::IO::File::Create(filename);
  outstream->Write(image, 0, length);
  outstream->Close();
  datastream->Close();
  response->Close();

  // add the datapath to the filename
/*  const char *datapath = gConfMan->Get_ConfigItem("ShotPath");
  if (!datapath) {
    return nullptr;
  }
  String ^dp = gcnew String(datapath);
  filename = filename->Replace("/", "");
  filename = String::Concat(dp, filename);

  filename = String::Concat(".", filename);
  Stream^ outstream = System::IO::File::Create(filename);
  outstream->Write(image, 0, length);
  outstream->Close();

  datastream->Close();
  response->Close();
*/
  return filename;
}



