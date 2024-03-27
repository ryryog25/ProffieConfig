#pragma once
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * styles/bladestyle.h
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

#include <stdint.h>
#include <vector>
#include <memory>
#include <string_view>
#include <variant>
#include <optional>

#include "styles/elements/colors.h"
#include "styles/elements/effects.h"
#include "styles/elements/lockuptype.h"

namespace BladeStyles {

struct BladeStyle;

enum {
    FUNCTION 	= 0b10000000000000,
    VARIADIC	= 0b01000000000000,
    TRANSPARENT = 0b00100000000000,
    WRAPPER		= 0b00000000000001,
    BUILTIN		= 0b00000000000010,
    INT			= 0b00000000000100,
    BITS		= 0b00000000001000,
    BOOL		= 0b00000000010000,
    COLOR 		= 0b00000000100000,
    EFFECT		= 0b00000001000000,
    LOCKUPTYPE	= 0b00000010000000,

    FLAGMASK	= (FUNCTION | VARIADIC | TRANSPARENT) ^ 0xFFFF,
    // OFFTYPE?
    // CHANGETYPE?
    // CCTYPE?
};

typedef std::variant<int32_t, Color, Effect, LockupType, std::shared_ptr<BladeStyle>> ValueBase;
typedef std::optional<ValueBase> Value;

struct Arg {
    Arg() = delete;
    Arg(const char* _name, const int32_t& _type, const Value& _defVal = std::nullopt) :
        name(_name),
        type(_type),
        defaultVal(_defVal) {}

    const std::string_view name;
    const int32_t type;
    const Value defaultVal;
    Value val{std::nullopt};
};

struct BladeStyle {
    BladeStyle() = delete;
    BladeStyle(const char* name, const char* humanName, int32_t type, const std::vector<Arg>& args) :
        name(name), humanName(humanName), type(type), args(args) {}

    const std::string_view name;
    const std::string_view humanName;
    const int32_t type;

    const std::vector<Arg>& getArgs();
    void setArgs(const std::vector<Value>&);
    void setArg(size_t, const Value&);
    // void addVariadic(const Value&);

private:
    std::vector<Arg> args;
};

}
