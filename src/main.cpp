/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * main.cpp
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <wx/app.h>

#include "prop/propfile.h"

class ProffieConfig : public wxApp {
public:
  virtual bool OnInit() override {

    chdir(argv[0].BeforeLast('/'));

      PropFile::readProp("resources/props/fett263.pconf");

#   ifdef __WXMSW__
    MSWEnableDarkMode();
    if (AttachConsole(ATTACH_PARENT_PROCESS)){
       freopen("CONOUT$", "w", stdout);
       freopen("CONOUT$", "w", stderr);
       freopen("CONIN$", "r", stdin);
    }
#   endif

    return true;
  }
};

wxIMPLEMENT_APP(ProffieConfig);
