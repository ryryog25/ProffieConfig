#pragma once
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
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
#include <unordered_map>
#include <vector>
#include <stdint.h>

namespace PConf {

struct Entry;
struct Section;

struct PropData;

PropData readProp(std::string filename);

struct Entry {
    std::string name;
    std::string label{""};
    int32_t labelNum{-1};
    std::string value{""};
};

struct Section : Entry {

    std::vector<Entry> entries;
};

struct PropData {

    struct Setting;
    struct Toggle;
    struct Option;
    struct Numeric;
    struct Decimal;

    struct LayoutItem;
    struct LayoutLevel;

    struct ButtonState;
    struct Button;

    std::string name{};
    std::string filename{};

    std::string info{};
    std::unordered_map<std::string, Setting> settings;
    std::vector<LayoutItem> layout;

    std::unordered_map<uint8_t, ButtonState> buttonControls;
};

struct PropData::Setting {
    enum class Type {
        TOGGLE,
        OPTION,
        NUMERIC,
        DECIMAL
    };

    std::string name;
    std::string description;

    std::unordered_set<std::string> requires;
    bool requireAny{false};

    Type type;
};

struct PropData::Toggle : Setting {
    std::unordered_set<std::string> disables;
};

struct PropData::Option : Setting {
    std::unordered_set<PropData::Toggle> options;
    bool output{true};
};

struct PropData::Numeric : Setting {
    int32_t min{0};
    int32_t max{100};
    int32_t defaultVal{min};
    int32_t increment{1};
};

struct PropData::Decimal : Setting {
    double min{0};
    double max{0};
    double defaultVal{min};
    double increment{0.1};
};

struct PropData::LayoutItem {
    std::string label{""};
    Setting* setting{nullptr};
};

struct PropData::LayoutLevel : LayoutItem {
    std::vector<LayoutItem> items;

    enum class Direction {
        HORIZONTAL,
        VERTICAL
    } direction;
};

struct PropData::ButtonState {
    std::string label;
    std::vector<Button> buttons;
};

struct PropData::Button {
    std::string label;
    std::unordered_map<Setting*, std::string> descriptions;
};

};
