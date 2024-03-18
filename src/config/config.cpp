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

static void pruneCustomDefines(std::shared_ptr<Config::Data>);

static void readTop(std::istream&, std::shared_ptr<Config::Data>);
static void readProp(std::istream&, std::shared_ptr<Config::Data>);

static void writeMessage(std::ostream&);
static void writeTop(std::ostream&, std::shared_ptr<Config::Data>);
static void writeProp(std::ostream&, std::shared_ptr<Config::Data>);

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
            readTop(configFile, config);
            pruneCustomDefines(config);
        } else if (buf.find("CONFIG_PROP") != std::string::npos) {
            readProp(configFile, config);
        } else if (buf.find("CONFIG_PRESETS") != std::string::npos) {

        } else if (buf.find("CONFIG_BUTTONS") != std::string::npos) {

        } else if (buf.find("CONFIG_STYLES") != std::string::npos) {

        }
    }

    configFile.close();
    return config;
}

void Config::writeConfig(const std::string& filename, std::shared_ptr<Data> config) {
    auto configFile{std::ofstream(CONFIGPATH + filename)};
    if (!configFile) {
        Logger::error("Config file \"" + filename + "\" could not be saved!");
        return;
    }

    pruneCustomDefines(config);

    writeMessage(configFile);
    configFile << std::endl;
    writeTop(configFile, config);
    configFile << std::endl;
    writeProp(configFile, config);
}

static void pruneCustomDefines(std::shared_ptr<Config::Data> config) {
    Config::CDefineMap::const_iterator define;
#	define PRUNE(defineName, ...) { \
        if ((define = config->customDefines.find(defineName)) != config->customDefines.end()) { \
            config->customDefines.erase(define); \
            __VA_ARGS__ \
        } \
    }
#	define GETDEF(name, type) std::static_pointer_cast<Config::Setting::type<Config::Setting::DefineBase>>(config->generalDefines->find(name)->second)


    PRUNE("SAVE_STATE",
          GETDEF("SAVE_VOLUME", Numeric)->value = true;
          GETDEF("SAVE_COLOR", Numeric)->value = true;
          GETDEF("SAVE_PRESET", Numeric)->value = true;
          )
    PRUNE("ENABLE_SD")
    PRUNE("ENABLE_MOTION")
    PRUNE("ENABLE_WS2811")
    PRUNE("ENABLE_AUDIO")
    PRUNE("SHARED_POWER_PINS")

#	undef PRUNE
#	undef GETDEF
}

static void readTop(std::istream& fileStream, std::shared_ptr<Config::Data> config) {
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
        if (buf.find(R"(#include "proffieboard_v)") != std::string::npos) {
            auto incPos{strlen(R"(#include ")")};
            auto incEnd{buf.rfind('"')};

            auto versionStr{buf.substr(incPos, incEnd - incPos)};
            for (const auto& [ name, includeFile ] : config->proffieboard.options) {
                if (includeFile == versionStr) {
                    config->proffieboard.value = versionStr;
                    versionStr.clear();
                    break;
                }
            }
            if (!versionStr.empty()) Logger::info("Invalid/Unknown proffieboard version read, using default.");
        }
        if (buf.find("#define") == std::string::npos) continue;

        constexpr auto delims{" \t"};

        auto definePos{buf.find_first_not_of(delims, buf.find("#define") + 7)};
        auto defineEnd{buf.find_first_of(delims, definePos)};

        auto valuePos{buf.find_first_not_of(delims, defineEnd)};
        auto valueEnd{buf.find_last_not_of(delims) + 1};

        auto defineName{buf.substr(definePos, defineEnd - definePos)};
        std::optional<DefineMap::const_iterator> define;
        for (const auto& [ propName, propDefs ] : config->propDefines) {
            if (!propDefs) continue;
            define = propDefs->find(defineName);
            if (define != propDefs->end()) break;
            define.reset();
        }
        if (!define) define = config->generalDefines->find(defineName);

        if (define == config->generalDefines->end()) {
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
            std::static_pointer_cast<Combo<DefineBase>>(define.value()->second)->value = valuePos == std::string::npos ? "" : buf.substr(valuePos, valueEnd - valuePos);
            break;
        }
    }

    Logger::info("Hit end of config file while reading CONFIG_TOP.");
}
static void readProp(std::istream& fileStream, std::shared_ptr<Config::Data> config) {
    std::string buf;
    while (std::getline(fileStream, buf)) {
        buf = buf.substr(0, buf.find("//"));
        if (buf.find("#ifdef") != std::string::npos) {
            Logger::info("Handling unterminated CONFIG_PROP section.");
            // Unget line
            fileStream.unget();
            while (fileStream.peek() != '\n') {
                fileStream.unget();
            }
            fileStream.get();
            return;
        }
        if (buf.find("#endif") != std::string::npos) return;

        if (buf.find("#include") == std::string::npos) continue;

        auto propStart{buf.find(R"("../props/)")};
        auto propEnd{buf.rfind(R"(")")};

        if (propStart == std::string::npos || propStart == propEnd) {
            Logger::info("Prop include malformed, skipping!");
            continue;
        }

        config->selectedProp.value = buf.substr(propStart + 10, propEnd - propStart - 10);
        return;
    }

    Logger::info("Hit end of config file while reading CONFIG_PROP.");
}

static void writeMessage(std::ostream& fileStream) {
    fileStream <<
        "/*" << std::endl <<
        "This configuration file was generated by ProffieConfig " VERSION ", created by Ryryog25." << std::endl <<
        "The tool can be found here: https://github.com/ryryog25/ProffieConfig/wiki/ProffieConfig" << std::endl <<
        "ProffieConfig is an All-In-One utility for managing your Proffieboard." << std::endl <<
        "*/" << std::endl;
}
static void writeTop(std::ostream& fileStream, std::shared_ptr<Config::Data> config) {
    using namespace Config::Setting;
    fileStream << "#ifdef CONFIG_TOP" << std::endl;
    fileStream << "#include \"" << config->proffieboard.value << "\"" << std::endl;
    fileStream << "const unsigned int maxLedsPerStrip = " << config->maxLedsPerStrip.value << ";" << std::endl;
    fileStream << "#define ENABLE_AUDIO" << std::endl;
    fileStream << "#define ENABLE_WS2811" << std::endl;
    fileStream << "#define ENABLE_SD" << std::endl;
    fileStream << "#define ENABLE_MOTION" << std::endl;
    fileStream << "#define SHARED_POWER_PINS" << std::endl;

    auto writeDefine{[&fileStream](std::shared_ptr<Config::Setting::DefineBase> define) {
        switch (define->getType()) {
        case SettingType::TOGGLE: {
            auto castDef{std::static_pointer_cast<Toggle<DefineBase>>(define)};
            if (castDef->value && !castDef->isDisabled()) fileStream << "#define " << castDef->define << castDef->postfix << std::endl;
            break;
        }
        case SettingType::SELECTION: {
            auto castDef{std::static_pointer_cast<Selection<DefineBase>>(define)};
            if (castDef->value && !castDef->isDisabled()) fileStream << "#define " << castDef->define << castDef->postfix << std::endl;
            break;
        }
        case SettingType::NUMERIC: {
            auto castDef{std::static_pointer_cast<Numeric<DefineBase>>(define)};
            if (!castDef->isDisabled()) fileStream << "#define " << castDef->define << " " << castDef->value << castDef->postfix << std::endl;
            break;
        }
        case SettingType::DECIMAL: {
            auto castDef{std::static_pointer_cast<Decimal<DefineBase>>(define)};
            if (!castDef->isDisabled()) fileStream << "#define " << castDef->define << " " << castDef->value << castDef->postfix << std::endl;
            break;
        }
        case SettingType::COMBO: {
            auto castDef{std::static_pointer_cast<Combo<DefineBase>>(define)};
            if (!castDef->isDisabled()) fileStream << "#define " << castDef->define << " " << castDef->value << castDef->postfix << std::endl;
            break;
        }
        }
    }};

    for (const auto& [ defName, define ] : *config->generalDefines) writeDefine(define);
    for (const auto& [ propname, defMap ] : config->propDefines) {
        if (!defMap || config->selectedProp.options.at(propname) != config->selectedProp.value) continue;
        for (const auto& [ defName, define ] : *defMap) writeDefine(define);
    }

    fileStream << "#endif" << std::endl;
}
static void writeProp(std::ostream& fileStream, std::shared_ptr<Config::Data> config) {
    fileStream << "#ifdef CONFIG_PROP" << std::endl;
    fileStream << R"(#include "../props/)" << config->selectedProp.value << '"' << std::endl;
    fileStream << "#endif" << std::endl;
}
