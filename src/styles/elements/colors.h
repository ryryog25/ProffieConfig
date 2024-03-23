#pragma once
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * styles/elements/colors.h
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

#include <string>

namespace Style {

#define PRIMARY_COLORS \
    CMAP("RED", RED) \
    CMAP("GREEN", GREEN) \
    CMAP("BLUE", BLUE) \
    CMAP("YELLOW", YELLOW) \
    CMAP("CYAN", CYAN) \
    CMAP("MAGENTA", MAGENTA) \
    CMAP("WHITE", WHITE) \
    CMAP("BLACK", BLACK) \

#define NAMED_COLORS \
    CMAP("AliceBlue", ALICE_BLUE) \
    CMAP("Aqua", AQUA) \
    CMAP("Aquamarine", AQUAMARINE) \
    CMAP("Azure", AZURE) \
    CMAP("Bisque", BISQUE) \
    CMAP("Black", BLACK) \
    CMAP("BlanchedAlmond", BLANCHED_ALMOND) \
    CMAP("Blue", BLUE) \
    CMAP("Chartreuse", CHARTREUSE) \
    CMAP("Coral", CORAL) \
    CMAP("Cornsilk", CORNSILK) \
    CMAP("Cyan", CYAN) \
    CMAP("DarkOrange", DARK_ORANGE) \
    CMAP("DeepPink", DEEP_PINK) \
    CMAP("DeepSkyBlue", DEEP_SKY_BLUE) \
    CMAP("DodgerBlue", DODGER_BLUE) \
    CMAP("FloralWhite", FLORAL_WHITE) \
    CMAP("Fuchsia", FUCHSIA) \
    CMAP("GhostWhite", GHOST_WHITE) \
    CMAP("Green", GREEN) \
    CMAP("GreenYellow", GREEN_YELLOW) \
    CMAP("HoneyDew", HONEY_DEW) \
    CMAP("HotPink", HOT_PINK) \
    CMAP("Ivory", IVORY) \
    CMAP("LavenderBlush", LAVENDER_BLUSH) \
    CMAP("LemonChiffon", LEMON_CHIFFON) \
    CMAP("LightCyan", LIGHT_CYAN) \
    CMAP("LightPink", LIGHT_PINK) \
    CMAP("LightSalmon", LIGHT_SALMON) \
    CMAP("LightYellow", LIGHT_YELLOW) \
    CMAP("Lime", LIME) \
    CMAP("Magenta", MAGENTA) \
    CMAP("MintCream", MINT_CREAM) \
    CMAP("MistyRose", MISTY_ROSE) \
    CMAP("Moccasin", MOCCASIN) \
    CMAP("NavajoWhite", NAVAJO_WHITE) \
    CMAP("Orange", ORANGE) \
    CMAP("OrangeRed", ORANGE_RED) \
    CMAP("PapayaWhip", PAPAYA_WHIP) \
    CMAP("PeachPuff", PEACH_PUFF) \
    CMAP("Pink", PINK) \
    CMAP("Red", RED) \
    CMAP("SeaShell", SEA_SHELL) \
    CMAP("Snow", SNOW) \
    CMAP("SpringGreen", SPRING_GREEN) \
    CMAP("SteelBlue", STEEL_BLUE) \
    CMAP("Tomato", TOMATO) \
    CMAP("White", WHITE) \
    CMAP("Yellow", YELLOW) \

#define ALL_COLORS \
    PRIMARY_COLORS \
    NAMED_COLORS

enum class Color {
#	define CMAP(str, name) name,
    NAMED_COLORS
    MAX
# 	undef CMAP
};

Color strToColor(const std::string&);
std::string colorToStr(Color);

}
