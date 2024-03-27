#include "bladestyle.h"
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * styles/bladestyle.cpp
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

void BladeStyles::BladeStyle::setArgs(const std::vector<Value>& values) {
    std::string str;
    for (size_t i = 0; i < values.size(); i++) {
        if (i >= args.size()) {
            if (!(args.at(args.size() - 1).type & VARIADIC)) break;
        }
        setArg(i, values.at(i));
    }
}

void BladeStyles::BladeStyle::setArg(size_t index, const Value& value) {
    if (!value) return;
    if (index > args.size()) return;
    auto& arg{args.at(index)};
    if (std::holds_alternative<std::shared_ptr<BladeStyle>>(value.value())) {
        if (!(arg.type & FUNCTION) && !(arg.type & COLOR)) return;
        if (std::get<std::shared_ptr<BladeStyle>>(value.value())->type != arg.type) return;
        arg.val = value.value();
        return;
    }
    switch (arg.type & FLAGMASK) {
    case INT:
    case BITS:
    case BOOL:
        if (!std::holds_alternative<int32_t>(value.value())) /* error */ return;
        break;
    case COLOR:
        if (!std::holds_alternative<Color>(value.value())) /* error */ return;
        break;
    case EFFECT:
        if (!std::holds_alternative<Effect>(value.value())) /* error */ return;
        break;
    case LOCKUPTYPE:
        if (!std::holds_alternative<LockupType>(value.value())) /* error */ return;
        break;
    default:
        /* error */ return;
    }
    arg.val = value.value();
}

const std::vector<BladeStyles::Arg>& BladeStyles::BladeStyle::getArgs() { return args; }
