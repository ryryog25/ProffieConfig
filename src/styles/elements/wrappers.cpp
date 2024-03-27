#include "wrappers.h"
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * styles/elements/wrappers.cpp
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

#include "styles/bladestyle.h"

#define WRAPPER(name, humanName, ...) GENERATORMAP(name, humanName, WRAPPER, __VA_ARGS__ )


const BladeStyles::Generator::StyleGenerator BladeStyles::Wrapper::get(const std::string& styleName) {
    const auto& map{getMap()};
    const auto& mapIt{map.find(styleName)};
    if (mapIt == map.end()) return nullptr;
    return mapIt->second;
}

const BladeStyles::Generator::StyleMap& BladeStyles::Wrapper::getMap() {
    // NEED A BETTER HUMAN NAME for all these
    static Generator::StyleMap map{
        WRAPPER("StylePtr", "StylePtr",
                Arg("Style Contents", COLOR)
                ),

        WRAPPER("ChargingStylePtr", "ChargingStylePtr",
                Arg("Style Contents", COLOR)
                ),

        WRAPPER("NormalPtr", "StyleNormalPtr",
                Arg("Base Color", COLOR),
                Arg("Clash Color", COLOR),
                Arg("Out Time (ms)", INT),
                Arg("In Time (ms)", INT),
                Arg("Lockup Color", COLOR, Color::WHITE),
                Arg("Blast Color", COLOR, Color::WHITE)
                ),

        WRAPPER("NormalPtrX", "StyleNormalPtrX",
                Arg("Base Color", COLOR),
                Arg("Clash Color", COLOR),
                Arg("Out Time (ms)", INT | FUNCTION),
                Arg("In Time (ms)", INT | FUNCTION),
                Arg("Lockup Color", COLOR, Color::WHITE),
                Arg("Blast Color", COLOR, Color::WHITE)
                ),

        WRAPPER("RainbowPtr", "StyleRainbowPtr",
                Arg("Out Time (ms)", INT),
                Arg("In Time (ms)", INT),
                Arg("Clash Color", COLOR, Color::WHITE),
                Arg("Lockup Color", COLOR, Color::WHITE)
                ),

        WRAPPER("RainbowPtrX", "StyleRainbowPtrX",
                Arg("Out Time (ms)", INT | FUNCTION),
                Arg("In Time (ms)", INT | FUNCTION),
                Arg("Clash Color", COLOR, Color::WHITE),
                Arg("Lockup Color", COLOR, Color::WHITE)
                ),

        WRAPPER("StrobePtr", "StyleStrobePtr",
                Arg("Strobe Color", COLOR),
                Arg("Clash Color", COLOR),
                Arg("Frequency", INT),
                Arg("Out Time (ms)", INT),
                Arg("In Time (ms)", INT)
                ),
    };
    return map;
}
