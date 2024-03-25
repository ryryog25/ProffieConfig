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
#include "styles/elements/lockuptype.h"

namespace Style {

struct Base;

enum {
    FUNCTION 	= 0b10000000000000,
    VARIADIC	= 0b01000000000000,
    STYLETYPE	= 0b00000000000001,
    BUILTIN		= 0b00000000000010,
    INT			= 0b00000000000100,
    BITS		= 0b00000000001000,
    BOOL		= 0b00000000010000,
    COLOR 		= 0b00000000100000,
    EFFECT		= 0b00000001000000,
    LOCKUPTYPE	= 0b00000010000000,

    FLAGMASK	= (FUNCTION | VARIADIC) ^ 0xFFFF,
    // OFFTYPE?
    // CHANGETYPE?
    // CCTYPE?
};

typedef std::optional<std::variant<int32_t, Color, Effect, LockupType, std::shared_ptr<Base>>> Value;

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

struct Base {
    virtual const std::string_view getName() const = 0;
    virtual const std::string_view getHumanName() const = 0;
    virtual int32_t getType() const = 0;
    virtual std::vector<Arg>& getArgs() = 0;
};

#define ELEM(name, humanName, retType, ...) \
struct name : Base { \
    name() {} \
    name(const std::vector<Value>& valVec) { \
        Arg* arg; \
        for (size_t i = 0; i < valVec.size(); i++) { \
            if (i >= args.size()) { \
                arg = &args.at(args.size() - 1); \
                if (!(arg->type & VARIADIC)) break; \
            } else arg = &args.at(i); \
            const auto& value{valVec.at(i)}; \
            if (!value) continue; \
            if (arg->type & FUNCTION) { \
                if (!std::holds_alternative<std::shared_ptr<Base>>(value.value())) /* error */ continue; \
                if (std::get<std::shared_ptr<Base>>(value.value())->getType() != arg->type) /* error */ continue; \
                arg->val = value.value(); \
                continue; \
            } \
            switch (arg->type & FLAGMASK) { \
            case INT: \
            case BITS: \
            case BOOL: \
                if (!std::holds_alternative<int32_t>(value.value())) /* error */ continue; \
                break; \
            case COLOR: \
                if (!std::holds_alternative<Color>(value.value())) /* error */ continue; \
                break; \
            case EFFECT: \
                if (!std::holds_alternative<Effect>(value.value())) /* error */ continue; \
                break; \
            case LOCKUPTYPE: \
                if (!std::holds_alternative<LockupType>(value.value())) /* error */ continue; \
                break; \
            default: \
                /* error */ continue; \
            } \
            arg->val = value.value(); \
        } \
    } \
    static const std::string_view getNameStatic() { return #name; } \
    virtual const std::string_view getHumanName() const { return humanName; } \
    virtual int32_t getType() const { return retType; } \
    virtual std::vector<Arg>& getArgs() { return args; } \
private: \
    virtual const std::string_view getName() const { return getNameStatic(); } \
    std::vector<Arg> args{ \
        __VA_ARGS__ \
    }; \
};

}
