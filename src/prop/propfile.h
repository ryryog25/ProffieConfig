#pragma once
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * prop/propfile.h
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
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <vector>

namespace PropFile {

struct Data;

std::shared_ptr<Data> readProp(const std::string& filename);

struct Data {
    struct Setting;
    struct Toggle;
    struct Selection;
    struct Numeric;
    struct Decimal;

    struct LayoutItem;
    struct LayoutLevel;

    struct ButtonMap;
    struct ButtonState;
    struct Button;

    std::string name{};
    std::string filename{};

    std::string info{};

    typedef std::unordered_map<std::string, std::shared_ptr<Setting>> SettingMap;
    SettingMap settings;

    typedef std::vector<std::shared_ptr<LayoutItem>> LayoutVec;
    LayoutVec layout;

    std::unordered_map<uint8_t, std::shared_ptr<ButtonMap>> buttonControls;
};

enum class SettingType {
    BASE,
    TOGGLE,
    SELECTION,
    NUMERIC,
    DECIMAL
};

enum class LayoutType {
    ITEM,
    LEVEL
};

struct Data::Setting {
    std::string name;
    std::string description;
    std::string define;

    std::unordered_set<std::string> require;
    bool requireAny{false};

    virtual SettingType getType() const { return SettingType::BASE; }
};

struct Data::Toggle : Setting {
    std::unordered_set<std::string> disable;

    virtual SettingType getType() const { return SettingType::TOGGLE; }
};

struct Data::Selection : Toggle {
    bool output{true};
    std::unordered_set<std::shared_ptr<Selection>> peers;

    virtual SettingType getType() const { return SettingType::SELECTION; }
};

struct Data::Numeric : Setting {
    int32_t min{0};
    int32_t max{100};
    int32_t defaultVal{min};
    int32_t increment{1};

    virtual SettingType getType() const { return SettingType::NUMERIC; }
};

struct Data::Decimal : Setting {
    double min{0};
    double max{0};
    double defaultVal{min};
    double increment{0.1};

    virtual SettingType getType() const { return SettingType::DECIMAL; }
};

struct Data::LayoutItem {
    std::shared_ptr<Setting> setting{nullptr};

    virtual LayoutType getType() const { return LayoutType::ITEM; }
};

struct Data::LayoutLevel : LayoutItem {
    std::string label{""};
    std::vector<std::shared_ptr<LayoutItem>> items;

    enum class Direction {
        HORIZONTAL,
        VERTICAL
    } direction;

    virtual LayoutType getType() const { return LayoutType::LEVEL; }
};

struct Data::ButtonMap {
    std::unordered_set<std::shared_ptr<ButtonState>> states;
    int8_t numButton;
};

struct Data::ButtonState {
    std::string label;
    std::vector<std::shared_ptr<Button>> buttons;
};

struct Data::Button {
    std::string label;
    std::unordered_map<std::shared_ptr<Setting>, std::string> descriptions;
};

}
