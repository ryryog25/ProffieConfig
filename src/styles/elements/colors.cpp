#include "colors.h"
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * styles/elements/colors.cpp
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

std::optional<BladeStyles::Color> BladeStyles::strToColor(const std::string& str) {
#	define CMAP(colorStr, ret) if (str == colorStr) return Color::ret;

    ALL_COLORS;
    return std::nullopt;

#	undef CMAP
}

std::optional<std::string> BladeStyles::colorToStr(Color color) {
#	define CMAP(colorStr, name) if (color == Color::name) return colorStr;

    ALL_COLORS;
    return std::nullopt;

# 	undef CMAP
}
