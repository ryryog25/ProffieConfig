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

#include <functional>
#include <memory>

#include "styles/elements/functions.h"

std::function<std::shared_ptr<Style::Base>(void)> Style::getGenerator(const std::string_view& styleStr) {
#   define MAPENTRY(styleName) \
    { styleName::getNameStatic(), []{ return std::static_pointer_cast<Style::Base>(std::make_shared<styleName>()); } }

    static std::unordered_map<std::string_view, std::function<std::shared_ptr<Base>(void)>> styleMap{
        MAPENTRY(Functions::Clamp),
        MAPENTRY(Functions::Int)
    };

    auto mapIt{styleMap.find(styleStr)};
    if (mapIt == styleMap.end()) return nullptr;
    return mapIt->second;
#   undef MAPENTRY
}
