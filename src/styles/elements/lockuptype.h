#pragma once
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * styles/elements/lockuptype.h
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

#include <optional>
#include <string>

namespace BladeStyles {

#define LOCKUP_TYPES \
    LTMAP(NONE, "NONE") \
    LTMAP(NORMAL, "NORMAL") \
    LTMAP(DRAG, "DRAG") \
    LTMAP(ARMED, "ARMED") \
    LTMAP(AUTOFIRE, "AUTOFIRE") \
    LTMAP(MELT, "MELT") \
    LTMAP(LIGHTNING_BLOCK, "LIGHTNING_BLOCK") \

enum class LockupType {
#	define LTMAP(def, str) def,

    LOCKUP_TYPES

#	undef LTMAP
};

std::optional<LockupType> strToLockupType(const std::string&);
std::optional<std::string> lockupTypeToStr(LockupType);
std::optional<std::string> lockupTypeToHumanStr(LockupType);

}
