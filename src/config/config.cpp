#include "config.h"
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * config/config.cpp
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
#include <optional>

#include "config/defaults.h"
#include "log/logger.h"

static void readDefines(std::istream&, std::shared_ptr<Config::Data>);
static void pruneCustomDefines(std::shared_ptr<Config::Data>);

std::shared_ptr<Config::Data> Config::readConfig(const std::string& filename) {
    auto configFile{std::ifstream(CONFIGPATH + filename)};
    if (!configFile) {
        Logger::error("Config file \"" + filename + "\" could not be loaded!");
        return nullptr;
    }

    auto config{Defaults::generateBlankConfig()};
    std::string buf;
    while (std::getline(configFile, buf)) {
        if (buf.find("#ifdef") == std::string::npos) continue;

        if (buf.find("CONFIG_TOP") != std::string::npos)	{
            readDefines(configFile, config);
            pruneCustomDefines(config);
        } else if (buf.find("CONFIG_PROP") != std::string::npos) {

        } else if (buf.find("CONFIG_PRESETS") != std::string::npos) {

        } else if (buf.find("CONFIG_BUTTONS") != std::string::npos) {

        } else if (buf.find("CONFIG_STYLES") != std::string::npos) {

        }
    }


    return config;
}

void Config::writeConfig(const std::string& filename, const Data& config) {

}


static void readDefines(std::istream& fileStream, std::shared_ptr<Config::Data> config) {
    using namespace Config::Setting;
    std::string buf;

    while (std::getline(fileStream, buf)) {
        buf = buf.substr(0, buf.find("//"));
        if (buf.find("#ifdef") != std::string::npos) {
            Logger::info("Handling unterminated CONFIG_TOP section.");
            // Unget line
            fileStream.unget();
            while (fileStream.peek() != '\n') {
                fileStream.unget();
            }
            fileStream.get();
            return;
        }
        if (buf.find("#endif") != std::string::npos) return;
        if (buf.find("maxLedsPerStrip") != std::string::npos) {
            auto numPos{buf.find_first_not_of(" \t=", buf.find('='))};
            if (!std::isdigit(buf.at(numPos))) {
                Logger::info("maxLedsPerStrip value missing, skipping.");
                continue;
            }
            config->maxLedsPerStrip.value = std::stoi(buf.substr(numPos));
        }
        if (buf.find("#define") == std::string::npos) continue;

        constexpr auto delims{" \t"};

        auto definePos{buf.find_first_not_of(delims, buf.find("#define") + 7)};
        auto defineEnd{buf.find_first_of(delims, definePos)};

        auto valuePos{buf.find_first_not_of(delims, defineEnd)};
        auto valueEnd{buf.find_last_not_of(delims) + 1};

        auto defineName{buf.substr(definePos, defineEnd - definePos)};
        std::optional<DefineMap::const_iterator> define{config->generalDefines.find(defineName)};
        for (const auto& [ propName, propDefs ] : config->propDefines) {
            if (define != config->generalDefines.end()) break;
            define = propDefs->find(defineName);
            if (define != propDefs->end()) break;
            define.reset();
        }

        if (!define) {
            config->customDefines.emplace(defineName, valuePos == std::string::npos ? "" : buf.substr(valuePos, valueEnd - valuePos));
            continue;
        }

        switch (define.value()->second->getType()) {
        case SettingType::TOGGLE:
            std::static_pointer_cast<Toggle<DefineBase>>(define.value()->second)->value = true;
            break;
        case SettingType::SELECTION:
            std::static_pointer_cast<Selection<DefineBase>>(define.value()->second)->value = true;
            break;
        case SettingType::NUMERIC: {
            auto castDef{std::static_pointer_cast<Numeric<DefineBase>>(define.value()->second)};
            if (!std::isdigit(buf.at(valuePos))) {
                Logger::info("Numeric define \"" + castDef->name + "\" has non-numeric value in config, skipping.");
                continue;
            }
            castDef->value = std::stoi(buf.substr(valuePos));
            break;
        }
        case SettingType::DECIMAL: {
            auto castDef{std::static_pointer_cast<Decimal<DefineBase>>(define.value()->second)};
            if (!std::isdigit(buf.at(valuePos))) {
                Logger::info("Decimal define \"" + castDef->name + "\" has non-numeric value in config, skipping.");
                continue;
            }
            castDef->value = std::stod(buf.substr(valuePos));
            break;
        }
        case SettingType::COMBO:
            std::static_pointer_cast<Combo<DefineBase>>(define.value()->second)->value = valuePos == std::string::npos ? "" : buf.substr(valuePos, valueEnd - valuePos - 1);
            break;
        }
    }
}
static void pruneCustomDefines(std::shared_ptr<Config::Data> config) {
    Config::CDefineMap::const_iterator define;
#	define PRUNE(defineName, ...) { \
        if ((define = config->customDefines.find(defineName)) != config->customDefines.end()) { \
            config->customDefines.erase(define); \
            __VA_ARGS__ \
        } \
    }
#	define GETDEF(name, type) std::static_pointer_cast<Config::Setting::type<Config::Setting::DefineBase>>(config->generalDefines.find(name)->second)


    PRUNE("SAVE_STATE",
          GETDEF("SAVE_VOLUME", Numeric)->value = true;
          GETDEF("SAVE_COLOR", Numeric)->value = true;
          GETDEF("SAVE_PRESET", Numeric)->value = true;
          )
    PRUNE("ENABLE_SD")
    PRUNE("ENABLE_MOTION")
    PRUNE("ENABLE_WS2811")
    PRUNE("ENABLE_AUDIO")

#	undef PRUNE
#	undef GETDEF
}
