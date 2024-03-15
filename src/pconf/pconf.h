#pragma once
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * pconf/pconf.h
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

#include <string>
#include <unordered_set>
#include <vector>
#include <optional>
#include <memory>
#include <stdint.h>

namespace PConf {

struct Entry;
struct Section;

std::shared_ptr<Entry> readEntry(std::istream& inStream, bool& isSection);
std::shared_ptr<Section> readSection(std::istream& inStream);
std::unordered_set<std::string> setFromValue(const std::optional<std::string>& value);

enum class DataType {
    ENTRY,
    SECTION
};

struct Entry {
    std::string name;
    std::optional<std::string> value{std::nullopt};
    std::optional<std::string> label{std::nullopt};
    std::optional<int32_t> labelNum{std::nullopt};

    virtual DataType getType() const { return DataType::ENTRY; }
};

struct Section : Entry {
    std::vector<std::shared_ptr<Entry>> entries;

    virtual DataType getType() const { return DataType::SECTION; }
};

}
