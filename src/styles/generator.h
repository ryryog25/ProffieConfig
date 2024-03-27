#pragma once
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * styles/generator.h
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

#include <memory>
#include <functional>
#include <unordered_map>
#include <vector>

#include "styles/bladestyle.h"

namespace BladeStyles::Generator {

typedef std::function<std::shared_ptr<BladeStyle>(const std::vector<Value>&)> StyleGenerator;
typedef std::unordered_map<std::string, StyleGenerator> StyleMap;

const StyleGenerator get(const std::string& styleName);

#define GENERATORMAP(name, humanName, type, ...) { \
    name, \
    [](const std::vector<Value>& values) -> std::shared_ptr<BladeStyle> { \
        auto style{std::make_shared<BladeStyle>(name, humanName, type, std::vector<Arg>{ __VA_ARGS__ } )}; \
        style->setArgs(values); \
        return style; \
    } \
}

}
