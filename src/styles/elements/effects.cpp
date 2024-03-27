#include "effects.h"
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * styles/elements/effects.cpp
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

std::optional<BladeStyles::Effect> BladeStyles::strToEffect(const std::string& inputStr) {
#	define EMAP(def, str) if (str == inputStr || str == "EFFECT_" + inputStr) return Effect::def;

    ALL_EFFECTS;
    return std::nullopt;

# 	undef EMAP
}

std::optional<std::string> BladeStyles::effectToStr(Effect effect) {
#	define EMAP(def, str) if (Effect::def == effect) return std::string("EFFECT_") + str;

    ALL_EFFECTS;
    return std::nullopt;

#	undef EMAP
}

std::optional<std::string> BladeStyles::effectToHumanStr(Effect effect) {
    return /* toHumanStr( */ effectToStr(effect);
}
