#include "pconf.h"
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
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

#include <fstream>
#include <string>

using namespace PConf;

PropData readProp(std::string _filename) {
    std::ifstream propFile(_filename);

    PropData prop{};
    enum {
        MLENTRY, // Multi-Line Entry
        SECTION, // PConf Section
        SCANNING, // Scanning/Single-Line Entries
    } scanState{SCANNING};

    std::string buffer;
    while (!false) {
        std::getline(propFile, buffer);

        switch (scanState) {
        case SCANNING:
            size_t seperator;
            if ((seperator = buffer.find(':')) != std::string::npos) {
                Entry entry{};
                auto labelBegin = buffer.find('(');
                auto labelEnd   = buffer.rfind(')');
                if (labelBegin != std::string::npos && labelEnd != std::string::npos) {
                    entry.label = buffer.substr(labelBegin + 1, labelEnd - labelBegin);
                }
                auto labelNumBegin = buffer.find('{');
                auto labelNumEnd   = buffer.rfind('}');
                if (labelNumBegin != std::string::npos && labelNumEnd   != std::string::npos && labelNumBegin < labelNumEnd) {
                    if (std::isdigit(buffer.at(labelNumBegin + 1))) entry.labelNum = std::stoi(&buffer.c_str()[labelNumBegin + 1]);
                }
                // get name etc
            }

            break;
        case SECTION:

            break;
        case MLENTRY:

            break;
        }
    }

    return prop;
};
