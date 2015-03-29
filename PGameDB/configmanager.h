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



// utility class to parse and retrieve configuration data

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include<map>
#include<string>


class ConfigManager {
public:
  ConfigManager(const char *configfile);
  ~ConfigManager(void);

  const char *Get_ConfigItem(const char *item);

private:
  bool Load_ConfigItems(const char *configfile);

  std::map<std::string, std::string> ConfigParams;
};

#endif
