#include "defaults.h"
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * config/defaults.cpp
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

#include "appcore/state.h"
#include "log/logger.h"
#include "prop/propfile.h"

static void initializeProffieboard(Config::Define::Combo&);
static void initializeSelectedProp(Config::Define::Combo&);
static void initializePropDefines(Config::PropMap&);
static void initializeGeneralDefines(Config::Define::DefineMap&);

std::shared_ptr<Config::Data> Config::Defaults::generateBlankConfig() {
    auto appState{AppCore::getState()};
    if (!appState) {
        Logger::error("ProffieConfig not yet initialized! Cannot generate new config!");
        return nullptr;
    }

    auto config{std::make_shared<Config::Data>()};

    initializeProffieboard(config->proffieboard);
    initializeSelectedProp(config->selectedProp);
    initializePropDefines(config->propDefines);
    initializeGeneralDefines(config->generalDefines);

    return config;
}

static void initializeProffieboard(Config::Define::Combo& proffieboard) {
    proffieboard.name = "Proffieboard Version";
    proffieboard.description = "";
    proffieboard.pureDef = true;

    proffieboard.options = {
        { "Proffieboard V1", "#include \"proffieboard_v1_config.h\"" },
        { "Proffieboard V2", "#include \"proffieboard_v2_config.h\"" },
        { "Proffieboard V3", "#include \"proffieboard_v3_config.h\"" },
        };

    proffieboard.value = "Proffieboard V3";
}

static void initializeSelectedProp(Config::Define::Combo& selectedProp) {
    selectedProp.name = "Control Profile (Prop File)";
    selectedProp.description = "The controls and features preset (customizable depending on profile) for your saber.\n"
                               "Also referred to as \"Prop File,\" referencing the way it's handled in the underlying ProffieOS";
    selectedProp.pureDef = false;

    auto props{PropFile::getPropData(AppCore::getState()->propfiles)};
    for (const auto& prop : props) {
        selectedProp.options.emplace(prop->name, prop->filename);
    }
}

static void initializePropDefines(Config::PropMap& propDefines) {
    auto props{PropFile::getPropData(AppCore::getState()->propfiles)};
    for (const auto& prop : props) {
        propDefines.emplace(prop->name, prop->settings);
    }
}

static void initializeGeneralDefines(Config::Define::DefineMap& generalDefines) {
#	define DEFINE(type, ...) { \
        auto entry{std::make_shared<type>()}; \
        __VA_ARGS__ \
        generalDefines.emplace(entry->define, entry); \
    }
    using namespace Config::Define;

    DEFINE(Combo,
           entry->name = "Orientation";
           entry->define = "ORIENTATION";
           entry->description = "The orientation of the Proffieboard in the saber.";
           entry->options = {
               { "FETs Towards Blade", "ORIENTATION_FETS_TOWARDS_BLADE" },
               { "USB Towards Blade", "ORIENTATION_USB_TOWARDS_BLADE" },
               { "USB CCW From Blade", "ORIENTATION_USB_CCW_FROM_BLADE" },
               { "USB CW From Blade", "ORIENTATION_USB_CW_FROM_BLADE" },
               { "Top Towards Blade", "ORIENTATION_TOP_TOWARDS_BLADE" },
               { "Bottom Towards Blade", "ORIENTATION_BOTTOM_TOWARDS_BLADE" },
               };
           entry->value = "FETs Towards Blade";
           )
    DEFINE(Numeric,
           entry->name = "Number of Buttons";
           entry->define = "NUM_BUTTONS";
           entry->description = "Number of buttons your saber has.\n"
                                "Please note not all prop files support all possible numbers of buttons, and controls may change depending on how many buttons are specified.";
           entry->min = 0;
           entry->max = 3;
           entry->value = 2;
           )
    DEFINE(Numeric,
           entry->name = "Max Volume";
           entry->define = "VOLUME";
           entry->description = "Maximum adjustable volume level. (Can be decreased in volume menu)\n"
                                "1500 is a good starting value for most speakers, and it is not recommended to go past 2000 unless you know what you are doing, as this can damage your speaker";
           entry->min = 0;
           entry->max = 5000;
           entry->increment = 100;
           entry->value = 1500;
           )
    DEFINE(Decimal,
           entry->name = "Clash Threshold";
           entry->define = "CLASH_THRESHOLD_G";
           entry->description = "Force required to trigger a clash effect.\nMeasured in Gs.";
           entry->min = 0;
           entry->max = 10;
           entry->value = 3;
           entry->increment = 0.1;
           )
    DEFINE(Numeric,
           entry->name = "PLI Timeout";
           entry->define = "PLI_OFF_TIME";
           entry->description = "Time since last activity until PLI is turned off.";
           entry->min = 1;
           entry->max = 60;
           entry->value = 2;
           )
    DEFINE(Numeric,
           entry->name = "Idle Timeout";
           entry->define = "IDLE_OFF_TIME";
           entry->description = "Time since last activity until always-on accents power off";
           entry->min = 1;
           entry->max = 60;
           entry->value = 10;
           )
    DEFINE(Numeric,
           entry->name = "Motion Timeout";
           entry->define = "MOTION_TIMEOUT";
           entry->description = "Time since last activity until motion (Gesture Controls) is disabled.";
           entry->min = 1;
           entry->max = 60;
           entry->value = 15;
           )
    DEFINE(Numeric, // This'll require special treatment
           entry->name = "WS281X Max LEDs";
           entry->define = "const unsigned int maxLedsPerStrip =";
           entry->description = "Maximum number of LEDs in a WS281X blade per strip.\n"
                                "NOT blade length, that is configured on the Blades page, only increase this if you have a longer blade,"
                                "otherwise there's no need to change this setting.";
           entry->min = 1;
           entry->max = 0xFFFF;
           entry->value = 144;
           entry->pureDef = false;
           )
    DEFINE(Toggle,
           entry->name = "Save Volume";
           entry->define = "SAVE_VOLUME";
           entry->description = "Save the volume level between board restarts.";
           )
    DEFINE(Toggle,
           entry->name = "Save Preset";
           entry->define = "SAVE_PRESET";
           entry->description = "Save the selected preset between board restarts.";
           )
    DEFINE(Toggle,
           entry->name = "Save Color";
           entry->define = "SAVE_COLOR_CHANGE";
           entry->description = "Save color changes between board restarts.";
           )
    DEFINE(Toggle,
           entry->name = "Enable OLED";
           entry->define = "ENABLE_SSD1306";
           entry->description = "Enable displaying bitmaps/preset names on an OLED display.";
           )
    DEFINE(Toggle,
           entry->name = "Disable Color Change";
           entry->define = "DISABLE_COLOR_CHANGE";
           entry->description = "Completely disable any/all color change options.";
           )
    DEFINE(Toggle,
           entry->name = "Disable Talkie";
           entry->define = "DISABLE_TALKIE";
           entry->description = "Replace the builtin talkie error messages with beeps.";
           )
    DEFINE(Toggle,
           entry->name = "Disable Basic Parser Styles";
           entry->define = "DISABLE_BASIC_PARSER_STYLES";
           entry->description = "Disable the availability of basic styles in the ProffieOS Workbench.";
           )
    DEFINE(Toggle,
           entry->name = "Disable Diagnostic Commands";
           entry->define = "DISABLE_DIAGNOSTIC_COMMANDS";
           entry->description = "Disable Serial Monitor commands used for board diagnostics.";
           )



#	undef DEFINE
}
