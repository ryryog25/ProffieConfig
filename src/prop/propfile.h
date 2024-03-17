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

#include "config/settings.h"

namespace PropFile {

struct Data;

std::vector<std::shared_ptr<Data>> getPropData(const std::vector<std::string>& pconfs);

struct Data {
    struct LayoutItem;
    struct LayoutLevel;

    struct ButtonMap;
    struct ButtonState;
    struct Button;

    std::string name{};
    std::string filename{};

    std::string info{};

    std::shared_ptr<Config::Define::DefineMap> settings;

    typedef std::vector<std::shared_ptr<LayoutItem>> LayoutVec;
    std::shared_ptr<LayoutVec> layout;

    typedef std::unordered_map<uint8_t, std::shared_ptr<ButtonMap>> Buttons;
    std::shared_ptr<Buttons> buttonControls{std::make_shared<Buttons>()};
};

enum class LayoutType {
    ITEM,
    LEVEL
};

struct Data::LayoutItem {
    std::shared_ptr<Config::Define::DefineBase> setting{nullptr};

    virtual LayoutType getType() const { return LayoutType::ITEM; }
};

struct Data::LayoutLevel : LayoutItem {
    std::string label{""};
    std::shared_ptr<std::vector<std::shared_ptr<LayoutItem>>> items;

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
    std::unordered_map<std::shared_ptr<Config::Define::DefineBase>, std::string> descriptions;
};

}
