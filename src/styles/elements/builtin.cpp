#include "builtin.h"
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * styles/elements/builtin.cpp
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

#include "styles/generator.h"

#define BUILTIN(name, humanName, ...) GENERATORMAP(name, humanName, BUILTIN, __VA_ARGS__)

const BladeStyles::Generator::StyleGenerator BladeStyles::Builtin::get(const std::string& styleName) {
    const auto& map{getMap()};
    const auto& mapIt{map.find(styleName)};
    if (mapIt == map.end()) return nullptr;
    return mapIt->second;
}

const BladeStyles::Generator::StyleMap& BladeStyles::Builtin::getMap() {
    static Generator::StyleMap map{
        BUILTIN("style_charging", "Charging Style"),
        BUILTIN("style_pov", "POV Style"),
    };
    return map;
}