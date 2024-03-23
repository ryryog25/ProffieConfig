#pragma once
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * styles/base.h
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

namespace Style {

struct Base;

enum class Type {
    WRAPPER, // StylePtr, etc.
    BUILTIN, // built-in styles like &style_charging
    INT, // Function, Int<>
    RAWINT, // int
    COLOR, // If it returns a color, it can go straight into a wrapper
    EFFECT,
};

typedef std::optional<std::variant<int32_t, Color, Effect, std::shared_ptr<Base>>> Value;

struct Arg {
    Arg() = delete;
    Arg(const char* _name, const Type& _type, const Value& _defVal = std::nullopt) {
        name = _name;
        type = _type;
        defaultVal = _defVal;
    }

    std::string_view name;
    Type type;
    Value defaultVal;
    Value val{std::nullopt};
};

struct Base {
    virtual std::string_view getName() = 0;
    virtual Type getType() = 0;
    virtual std::vector<Arg>& getArgs() = 0;
};

#define ELEM(name, strName, type, ...) \
struct name : Base { \
    static std::string_view getNameStatic() { return strName; } \
    virtual Type getType() { return type; } \
    virtual std::vector<Arg>& getArgs() { \
        static std::vector<Arg> args{ \
            __VA_ARGS__ \
        }; \
        return args; \
    } \
private: \
    virtual std::string_view getName() { return getNameStatic(); } \
};

}
