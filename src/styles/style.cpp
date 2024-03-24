#include "style.h"
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * styles/style.cpp
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

#include "generator.h"
#include "log/logger.h"

struct TokenizedStyle {
    std::string name;
    std::vector<std::string> args;
};
static std::optional<TokenizedStyle> tokenizeStyle(const std::string&);
static bool parseArg(const std::string& argTok, Style::Arg& arg, size_t argNum);

static std::string typeToString(int32_t);

std::shared_ptr<Style::Base> Style::parseString(const std::string& styleStr) {
    auto styleTokens{tokenizeStyle(styleStr)};
    if (!styleTokens) return nullptr;

    auto styleGen{getGenerator(styleTokens->name)};
    if (!styleGen) {
        Logger::error("Style not recognized: " + styleTokens->name);
        return nullptr;
    }

    auto style{styleGen()};
    auto styleArgs{style->getArgs()};
    if (styleArgs.size() < styleTokens->args.size()) {
        Logger::error("Too many arguments for style element: " + styleTokens->name);
        return nullptr;
    }


    std::shared_ptr<Style::Base> argElement;
    for (size_t i = 0; i < styleArgs.size(); i++) {
        if (i >= styleTokens->args.size()) {
            if (!styleArgs.at(i).defaultVal) {
                Logger::error("Not enough arguments for style element: " + styleTokens->name);
                return nullptr;
            }
            break;
        }

        if (!parseArg(styleTokens->args.at(i), styleArgs.at(i), i)) {
            Logger::warn("Error while parsing style: " + styleTokens->name);
            return nullptr;
        }

        // Now check each type
        // if (styleTokens->args.at(i).)
    }

    return style;
}

std::optional<std::string> Style::asString(const std::shared_ptr<Base>) {

}

static std::optional<TokenizedStyle> tokenizeStyle(const std::string& styleStr) {
    auto styleBegin{styleStr.find_first_not_of(" \t\n")};
    if (styleBegin == std::string::npos) {
        Logger::warn("Could not tokenize style!", false);
        return std::nullopt;
    }
    auto nameEnd{styleStr.find_first_of("<(", styleBegin)};
    if (nameEnd == std::string::npos) {
        Logger::warn("Could not tokenize style name!", false);
        return std::nullopt;
    }
    auto styleName{styleStr.substr(styleBegin, nameEnd - styleBegin)};

    TokenizedStyle styleTokens;
    styleTokens.name = styleName;

    if (styleStr.at(nameEnd) != '<') return styleTokens;

    auto argSubStr{styleStr.substr(nameEnd + 1)};
    std::string buf;
    int32_t depth{1};
    char c;
    for (size_t i = 0; i < argSubStr.size(); i++) {
        c = argSubStr.at(i);
        if (c == ' ') continue;
        if (c == '\n') continue;
        if (c == '\t') continue;
        if (c == '\r') continue;

        if (c == '<') depth++;
        else if (c == '>') {
            depth--;
            if (depth < 0) {
                Logger::warn("Error parsing arguments for style: " + styleName);
                return std::nullopt;
            }
            if (depth == 0) {
                styleTokens.args.push_back(buf);
                continue;
            }
        }

        else if (c == ',' && depth == 1) {
            styleTokens.args.push_back(buf);
            buf.clear();
            continue;
        }

        buf += c;
    }
    if (depth != 0) {
        Logger::warn("Mismatched <> in style: " + styleName);
        return std::nullopt;
    }

    return styleTokens;
}

static bool parseArg(const std::string& argToken, Style::Arg& arg, size_t argNum) {
    using namespace Style;
    argNum++; // 0 to 1-based indexing for errors

    std::shared_ptr<Style::Base> argObj;
    if (argToken.find('<') != std::string::npos) argObj = Style::parseString(argToken);
    else argObj = nullptr;

    if (arg.type & FUNCTION) {
        if (!argObj) return false;
        if (argObj->getType() != arg.type) {
            Logger::error("Mismatched function type at argument " +
                          std::to_string(argNum) +
                          ". (Expected " +
                          typeToString(arg.type) +
                          " but got " +
                          typeToString(argObj->getType()) +
                          ")");
            return false;
        }

        arg.val = argObj;
        return true;
    }


    // BUILTIN		= 0b00000000000010,
    // INT			= 0b00000000000100,
    // BITS		= 0b00000000001000,
    // BOOL		= 0b00000000010000,
    // COLOR 		= 0b00000000100000,
    // EFFECT		= 0b00000001000000,
    // LOCKUPTYPE	= 0b00000010000000,

    switch (arg.type & FLAGMASK) {
    case STYLETYPE:
        Logger::error("Style found inside style!");
        return false;
    case BUILTIN:
        Logger::error("Builtin style found inside style!");
        return false;
    case INT:
        if (!std::isdigit(argToken.at(0)) && (argToken.size() >= 2 ? !std::isdigit(argToken.at(1)) || argToken.at(0) != '-' : true)) {
            Logger::error("Expected int for argument " + std::to_string(argNum) + " but didn't get a number.");
            return false;
        }
        arg.val = std::stoi(argToken);
        return true;
    case BOOL:
        // TODO
        return true;
    case BITS:
        // TODO
        return true;
    case COLOR: {
        auto color{strToColor(argToken)};
        if (!color) {
            Logger::error("Invalid color for argument " + std::to_string(argNum) + ": " + argToken);
            return false;
        }
        arg.val = color;
        return true; }
    case EFFECT: {
        auto effect{strToEffect(argToken)};
        if (!effect) {
            Logger::error("Invalid effect for argument " + std::to_string(argNum) + ": " + argToken);
            return false;
        }
        arg.val = effect;
        return true; }
    case LOCKUPTYPE: {
        auto lockupType{strToEffect(argToken)};
        if (!lockupType) {
            Logger::error("Invalid lockup type for argument " + std::to_string(argNum) + ": " + argToken);
            return false;
        }
        arg.val = lockupType;
        return true; }
    }

    Logger::error("Unknown argument type found for argument " + std::to_string(argNum) + ": " + argToken);
    return false;
}

static std::string typeToString(int32_t type) {
    switch (type) {
    case Style::STYLETYPE:
        return "Wrapper";
    case Style::BUILTIN:
        return "BuiltIn";
    case Style::INT:
        return "Int";
    case Style::BITS:
        return "Bits";
    case Style::BOOL:
        return "Boolean";
    case Style::COLOR:
        return "Color";
    case Style::EFFECT:
        return "Effect";
    case Style::LOCKUPTYPE:
        return "LockupType";
    }

    return "INVALID";
}
