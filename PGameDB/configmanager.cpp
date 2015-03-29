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


// implementation of a simple class to read retrieve configuration data

#include <stdlib.h>
#include <string.h>
#include "configmanager.h"


using namespace std;

ConfigManager::ConfigManager(const char *configfile) {
  Load_ConfigItems(configfile);
}

ConfigManager::~ConfigManager(void) {
}

bool ConfigManager::Load_ConfigItems(const char *configfile) {
  FILE *infile;
  char buf1[100], buf2[100], buf3[200];
  int slen, itemnum = 1, cnt, postnum = 0, prenum;
  bool preflag;
  char curchar;
  char *res;

  fopen_s(&infile, configfile, "r");
  if (!infile) {
    return false;
  }

  // now go through each item and read it into a buffer and parse the buffer by hand
  do {
    preflag = false;
    prenum = 0;

    res = fgets(buf3, 200, infile);

    // if we have read a line
    if (res) {
      slen = strlen(buf3);
      for (cnt = 0; cnt < slen; cnt++) {
        curchar = buf3[cnt];

        // we will skip spaces
        if (curchar != ' ') {

          // if we already read the = sign then fill up the value buffer
          if (preflag) {

            // # indicates a comment
            if (curchar == '#') {
              cnt = slen;
              postnum++;
            }
            else {
              buf2[postnum] = curchar;
              postnum++;
            }
          }

          // otherwise fill up the attribute buffer until we hit the equal sign
          else {
            if (curchar == '#') {
              cnt = slen;
            }

            // we got an equal mark so any text in front of that is our attribute
            else if (curchar == '=') {
              buf1[prenum] = 0;
              preflag = true;
              postnum = 0;
            }
            else {
              buf1[prenum] = curchar;
              prenum++;
            }
          } // end attribute parsing
        } // end valid character routine
      } // end input buffer parsing
    }  // end line parsing

    // do some error checking to make sure both attribute and value are something
    if ((prenum == 0) || (postnum == 0) || (!preflag)) {
      fclose(infile);  
      return false;
    }

    // then throw it in the tree
    else {
      buf2[postnum - 1] = 0;
      ConfigParams[buf1] = buf2;
    }

    itemnum++;
  } while(!feof(infile));

  fclose(infile);     

  return true;
}


// get a config item with key item
const char *ConfigManager::Get_ConfigItem(const char *item) {

  map <string, string> :: const_iterator confit = ConfigParams.find(item);

  if (confit == ConfigParams.end()) {
    return NULL;
  }

  return confit->second.c_str();
}


