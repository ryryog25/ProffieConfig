#include "lockuptype.h"
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * styles/elements/lockuptype.cpp
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

std::optional<Style::LockupType> Style::strToLockupType(const std::string& inputStr) {
#	define LTMAP(def, str) if (inputStr == str || "LOCKUP_" + inputStr == str) return LockupType::def;

    LOCKUP_TYPES;
    return std::nullopt;

#	undef LTMAP
}

std::optional<std::string> Style::lockupTypeToStr(LockupType lockupType) {
#	define LTMAP(def, str) if (lockupType == LockupType::def) return str;

    LOCKUP_TYPES;
    return std::nullopt;

#	undef LTMAP
}

std::optional<std::string> Style::lockupTypeToHumanStr(LockupType lockupType) {
    return /*defToHumanStr(*/ lockupTypeToStr(lockupType);
}
