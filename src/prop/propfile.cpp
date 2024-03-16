#include "propfile.h"
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * prop/propfile.cpp
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

#include <fstream>
#include <string>

#include "log/logger.h"
#include "pconf/pconf.h"

static PropFile::Data::SettingMap parsePropSettings(const std::shared_ptr<PConf::Section> settingsSection);
static PropFile::Data::LayoutVec parsePropLayout(const std::shared_ptr<PConf::Section> layoutSection, const PropFile::Data::SettingMap& settings);
static std::shared_ptr<PropFile::Data::ButtonMap> parsePropButtons(const std::shared_ptr<PConf::Section> buttonsSection, const PropFile::Data::SettingMap& settings);

static void checkSettingCommon(const std::shared_ptr<PConf::Entry> entry, std::shared_ptr<PropFile::Data::Setting> setting);
static std::shared_ptr<PropFile::Data::Toggle> generateToggle(const std::shared_ptr<PConf::Section> toggleSection);
static std::vector<std::shared_ptr<PropFile::Data::Selection>> generateOptionSelections(const std::shared_ptr<PConf::Section> optionSection);
static std::shared_ptr<PropFile::Data::Numeric> generateNumeric(const std::shared_ptr<PConf::Section> numericSection);
static std::shared_ptr<PropFile::Data::Decimal> generateDecimal(const std::shared_ptr<PConf::Section> decimalSection);

static PropFile::Data::LayoutVec recurseLayout(const std::shared_ptr<PConf::Section> section, const PropFile::Data::SettingMap& settings);

static std::vector<std::shared_ptr<PropFile::Data::Button>> parseButtonState(const std::vector<std::shared_ptr<PConf::Entry>>& buttonEntries, const PropFile::Data::SettingMap& settings);

std::shared_ptr<PropFile::Data> PropFile::readProp(const std::string& filename) {
    std::ifstream propFile(filename);

    auto prop{std::make_shared<Data>()};
    bool foundSect;

    while (!propFile.eof()) {
        auto entry{PConf::readEntry(propFile, foundSect)};
        if (foundSect) {
            auto sect{PConf::readSection(propFile)};
            if (!sect) continue;
            if (sect->name == "SETTINGS") prop->settings = parsePropSettings(sect);
            else if (sect->name == "LAYOUT") {
                if (prop->settings.empty()) {
                    Logger::warn("Missing/out of order settings section, ignoring layout section!", false);
                    continue;
                }
                prop->layout = parsePropLayout(sect, prop->settings);
            } else if (sect->name == "BUTTONS") {
                auto buttonMap{parsePropButtons(sect, prop->settings)};
                if (buttonMap) prop->buttonControls.emplace(buttonMap->numButton, buttonMap);
            }
        }
        if (!entry) continue;
        if (entry->name == "NAME") prop->name = entry->value.value_or("");
        if (entry->name == "FILENAME") prop->filename = entry->value.value_or("");
        if (entry->name == "INFO") prop->info = entry->value.value_or("");
    }

    return prop;
};

static PropFile::Data::SettingMap parsePropSettings(const std::shared_ptr<PConf::Section> settingsSection) {
    PropFile::Data::SettingMap settings;

    for (const auto& entry : settingsSection->entries) {
        if (entry->getType() != PConf::DataType::SECTION) {
            Logger::warn("Stray entry in settings! (" + entry->name + ")", false);
            continue;
        }


        if (entry->name == "TOGGLE") {
            auto toggle{generateToggle(std::static_pointer_cast<PConf::Section>(entry))};
            if (toggle) settings.emplace(toggle->define, toggle);
        } else if (entry->name == "OPTION") {
            for (const auto& selection : generateOptionSelections(std::static_pointer_cast<PConf::Section>(entry))) {
                settings.emplace(selection->define, selection);
            }
        } else if (entry->name == "NUMERIC") {
            auto numeric{generateNumeric(std::static_pointer_cast<PConf::Section>(entry))};
            if (numeric) settings.emplace(numeric->define, numeric);
        } else if (entry->name == "DECIMAL") {
            auto decimal{generateDecimal(std::static_pointer_cast<PConf::Section>(entry))};
            if (decimal) settings.emplace(decimal->define, decimal);
        }
    }

    return settings;
}  static void checkSettingCommon(const std::shared_ptr<PConf::Entry> entry, std::shared_ptr<PropFile::Data::Setting> setting) {
    if (entry->name == "NAME") setting->name = entry->value.value_or("");
    else if (entry->name == "DESCRIPTION") setting->description = entry->value.value_or("");
    else if (entry->name == "REQUIRE" || entry->name == "REQUIREANY") {
        setting->require = PConf::setFromValue(entry->value);
        setting->requireAny = entry->name == "REQUIREANY";
    }
}

static std::shared_ptr<PropFile::Data::Toggle> generateToggle(const std::shared_ptr<PConf::Section> toggleSection) {
    if (!toggleSection->label) {
        Logger::warn("Setting has no define, skipping!");
        return nullptr;
    }

    auto toggle{std::make_shared<PropFile::Data::Toggle>()};
    toggle->define = toggleSection->label.value();
    for (const auto& entry : toggleSection->entries) {
        checkSettingCommon(entry, toggle);
        if (entry->name == "DISABLE") toggle->disable = PConf::setFromValue(entry->value);
    }

    return toggle;
}

static std::vector<std::shared_ptr<PropFile::Data::Selection>> generateOptionSelections(const std::shared_ptr<PConf::Section> optionSection) {
    std::vector<std::shared_ptr<PropFile::Data::Selection>> selections;
    for (const auto& entry : optionSection->entries) {
        if (entry->name == "SELECTION" && entry->getType() == PConf::DataType::SECTION) {
            if (!entry->label) {
                Logger::warn("Setting has no define, skipping!");
                continue;
            }

            auto selection{std::make_shared<PropFile::Data::Selection>()};
            selection->define = entry->label.value();
            for (const auto& selectionEntry : std::static_pointer_cast<PConf::Section>(entry)->entries) {
                checkSettingCommon(selectionEntry, selection);
                if (entry->name == "DISABLE") selection->disable = PConf::setFromValue(entry->value);
                if (selectionEntry->name == "OUTPUT") selection->output = selectionEntry->value.value_or("TRUE") == "FALSE" ? false : true;
            }

            selections.push_back(selection);
        }
    }

    for (const auto& selection : selections) {
        for (const auto& peer : selections) {
            if (peer == selection) continue;
            selection->peers.insert(peer);
        }
    }

    return selections;
}

static auto isNum = [](const std::string& str) -> bool {
    if (str.empty()) return false;
    if (str.length() == 1) return std::isdigit(str.at(0));

    return (std::isdigit(str.at(0)) || (str.at(0) == '-' && std::isdigit(str.at(1))));
};


static std::shared_ptr<PropFile::Data::Numeric> generateNumeric(const std::shared_ptr<PConf::Section> numericSection) {
    if (!numericSection->label) {
        Logger::warn("Setting has no define, skipping!");
        return nullptr;
    }

    auto numeric{std::make_shared<PropFile::Data::Numeric>()};
    numeric->define = numericSection->label.value();
    for (const auto& entry : numericSection->entries) {
        checkSettingCommon(entry, numeric);
        const auto& val = entry->value.value_or(" ");
        if (entry->name == "MIN" && isNum(val)) numeric->min = stoi(val);
        if (entry->name == "MAX" && isNum(val)) numeric->max = stoi(val);
        if (entry->name == "DEFAULT" && isNum(val)) numeric->defaultVal = stoi(val);
        if (entry->name == "INCREMENT" && isNum(val)) numeric->increment = stoi(val);
    }

    return numeric;
}

static std::shared_ptr<PropFile::Data::Decimal> generateDecimal(const std::shared_ptr<PConf::Section> decimalSection) {
    if (!decimalSection->label) {
        Logger::warn("Setting has no define, skipping!");
        return nullptr;
    }

    auto decimal{std::make_shared<PropFile::Data::Decimal>()};
    decimal->define = decimalSection->label.value();
    for (const auto& entry : decimalSection->entries) {
        checkSettingCommon(entry, decimal);
        const auto& val = entry->value.value_or(" ");
        if (entry->name == "MIN" && isNum(val)) decimal->min = stod(val);
        if (entry->name == "MAX" && isNum(val)) decimal->max = stod(val);
        if (entry->name == "DEFAULT" && isNum(val)) decimal->defaultVal = stod(val);
        if (entry->name == "INCREMENT" && isNum(val)) decimal->increment = stod(val);
    }

    return decimal;
}

static PropFile::Data::LayoutVec parsePropLayout(const std::shared_ptr<PConf::Section> layoutSection, const PropFile::Data::SettingMap& settings) {
    PropFile::Data::LayoutVec layoutVec;

    return recurseLayout(layoutSection, settings);
}

static PropFile::Data::LayoutVec recurseLayout(std::shared_ptr<PConf::Section> section, const PropFile::Data::SettingMap& settings) {
    std::vector<std::shared_ptr<PropFile::Data::LayoutItem>> items;

    for (const auto& entry : section->entries) {
        if (entry->name == "SETTING") {
            if (!entry->label) {
                Logger::warn("Layout entry missing label, skipping!", false);
                continue;
            }
            const auto& setting{settings.find(entry->label.value())};
            if (setting == settings.end()) {
                Logger::warn("Layout entry has unknown setting, skipping! (" + entry->label.value() + ")", false);
                continue;
            }
            auto layoutItem{std::make_shared<PropFile::Data::LayoutItem>()};
            layoutItem->setting = setting->second;
            items.push_back(layoutItem);
        } else if (entry->name == "HORIZONTAL") {
            if (entry->getType() != PConf::DataType::SECTION) {
                Logger::warn("Horizontal layout section interpreted as entry due to syntax error, skipping!" + (entry->label ? " (" + entry->label.value() + ")" : ""), false);
                continue;
            }
            auto layoutLevel{std::make_shared<PropFile::Data::LayoutLevel>()};
            layoutLevel->direction = PropFile::Data::LayoutLevel::Direction::HORIZONTAL;
            layoutLevel->label = entry->label.value_or("");
            layoutLevel->items = recurseLayout(std::static_pointer_cast<PConf::Section>(entry), settings);
            items.push_back(layoutLevel);
        } else if (entry->name == "VERTICAL") {
            if (entry->getType() != PConf::DataType::SECTION) {
                Logger::warn("Vertical layout section interpreted as entry due to syntax error, skipping!" + (entry->label ? " (" + entry->label.value() + ")" : ""), false);
                continue;
            }
            auto layoutLevel{std::make_shared<PropFile::Data::LayoutLevel>()};
            layoutLevel->direction = PropFile::Data::LayoutLevel::Direction::VERTICAL;
            layoutLevel->label = entry->label.value_or("");
            layoutLevel->items = recurseLayout(std::static_pointer_cast<PConf::Section>(entry), settings);
            items.push_back(layoutLevel);
        }
    }

    return items;
}

static std::shared_ptr<PropFile::Data::ButtonMap> parsePropButtons(const std::shared_ptr<PConf::Section> buttonsSection, const PropFile::Data::SettingMap& settings) {
    if (buttonsSection->labelNum.value_or(-1) < 0) {
        Logger::warn("Button section missing numeric label, ignoring!", false);
        return nullptr;
    }

    auto buttonMap{std::make_shared<PropFile::Data::ButtonMap>()};
    buttonMap->numButton = buttonsSection->labelNum.value();

    for (const auto& stateEntry : buttonsSection->entries) {
        if (stateEntry->name == "STATE") {
            if (!stateEntry->label) {
                Logger::warn("Button state section missing label, skipping!", false);
                continue;
            }
            if (stateEntry->getType() != PConf::DataType::SECTION) {
                Logger::warn("Button state \"" + stateEntry->label.value() + "\" interpreted as entry due to syntax error, skipping!", false);
                continue;
            }

            auto state{std::make_shared<PropFile::Data::ButtonState>()};
            state->label = stateEntry->label.value();
            state->buttons = parseButtonState(std::static_pointer_cast<PConf::Section>(stateEntry)->entries, settings);

            buttonMap->states.insert(state);
        }
    }


    return buttonMap;
}

static std::vector<std::shared_ptr<PropFile::Data::Button>> parseButtonState(const std::vector<std::shared_ptr<PConf::Entry>>& buttonEntries, const PropFile::Data::SettingMap& settings) {
    std::vector<std::shared_ptr<PropFile::Data::Button>> buttons;
    for (const auto& buttonEntry : buttonEntries) {
        if (!buttonEntry->label) {
            Logger::warn("Button section missing label, skipping!", false);
            continue;
        }
        if (buttonEntry->getType() != PConf::DataType::SECTION) {
            Logger::warn("Button section \"" + buttonEntry->label.value() + "\" interpreted as entry due to syntax error, skipping!", false);
            continue;
        }

        auto button{std::make_shared<PropFile::Data::Button>()};
        button->label = buttonEntry->label.value();

        for (const auto& description : std::static_pointer_cast<PConf::Section>(buttonEntry)->entries) {
            const auto& setting = settings.find(description->label.value_or(""));
            button->descriptions.emplace((setting == settings.end() ? nullptr : setting->second), description->value.value_or(""));
        }

        buttons.push_back(button);
    }

    return buttons;
}
