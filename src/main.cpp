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

#include "config/config.h"
#include "appcore/state.h"
#include "test/styleedit.h"
#include "styles/style.h"

class ProffieConfig : public wxApp {
public:
    virtual bool OnInit() override {

        chdir(argv[0].BeforeLast('/'));

        AppCore::State::init();
        auto config{Config::readConfig("problemConfig.h")};
        Config::writeConfig("test/problemConfig.h", config);

#   	ifdef __WXMSW__
        MSWEnableDarkMode();
        if (AttachConsole(ATTACH_PARENT_PROCESS)){
            freopen("CONOUT$", "w", stdout);
            freopen("CONOUT$", "w", stderr);
            freopen("CONIN$", "r", stdin);
        }
#   	endif

        auto style{Style::parseString("StylePtr<WHITE>()")};

        // StyleEdit::generatePage();


        return true;
    }
};

wxIMPLEMENT_APP(ProffieConfig);
