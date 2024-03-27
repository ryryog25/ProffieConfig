#include "generator.h"
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * styles/generator.cpp
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

#include "styles/elements/functions.h"
#include "styles/elements/wrappers.h"
#include "styles/elements/builtin.h"
#include "styles/elements/styles.h"

const BladeStyles::Generator::StyleGenerator BladeStyles::Generator::get(const std::string& styleName) {
    StyleGenerator styleGen;
    styleGen = Function::get(styleName);
    if (styleGen) return styleGen;
    styleGen = Wrapper::get(styleName);
    if (styleGen) return styleGen;
    styleGen = Builtin::get(styleName);
    if (styleGen) return styleGen;
    styleGen = Style::get(styleName);
    if (styleGen) return styleGen;

    return nullptr;
}
