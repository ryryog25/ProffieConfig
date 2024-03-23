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

static std::string typeToString(Style::Type);

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

    auto mismatchError{[&argNum, &argObj](Style::Type expectedType) {
        std::string errmsg;
        errmsg += "Mismatched type at argument ";
        errmsg += std::to_string(argNum);
        errmsg += ". (Expected ";
        errmsg += typeToString(expectedType);
        errmsg += " but got ";
        errmsg += typeToString(argObj->getType());
        errmsg += ")";
        Logger::error(errmsg);
    }};

    switch (arg.type) {
    case Type::WRAPPER:
        Logger::error("Wrapper found inside style!");
        return false;
    case Type::BUILTIN:
        Logger::error("Builtin style found inside style!");
        return false;
    case Type::RAWINT:
        if (argObj) {
            Logger::error("Raw int type for argument " + std::to_string(argNum) + " cannot be a function.");
            return false;
        }
        if (!std::isdigit(argToken.at(0)) && (argToken.size() >= 2 ? !std::isdigit(argToken.at(1)) || argToken.at(0) != '-' : true)) {
            Logger::error("Expected raw int for argument " + std::to_string(argNum) + " but didn't get a number.");
            return false;
        }
        arg.val = std::stoi(argToken);
        break;
    case Type::INT:
        if (!argObj) {
            Logger::error("Int type for argument " + std::to_string(argNum) + " requires function/style base.");
            return false;
        }
        if (argObj->getType() != Type::INT) {
            mismatchError(Type::INT);
            return false;
        }
        arg.val = argObj;
        break;
    case Type::COLOR:
        if (!argObj) {
            auto color{strToColor(argToken)};
            if (color == Color::MAX) {
                Logger::error("Invalid color for argument " + std::to_string(argNum) + ": " + argToken);
                return false;
            }
            arg.val = color;
            break;
        }
        if (argObj->getType() != Type::COLOR) {
            mismatchError(Type::COLOR);
            return false;
        }
        arg.val = argObj;
        break;
    case Type::EFFECT:
        if (!argObj) {
            auto effect{strToEffect(argToken)};
            if (effect == Effect::MAX) {
                Logger::error("Invalid effect for argument " + std::to_string(argNum) + ": " + argToken);
                return false;
            }
            arg.val = effect;
        }
        if (argObj->getType() != Type::COLOR) {
            mismatchError(Type::EFFECT);
            return false;
        }
        arg.val = argObj;
        break;
    }

    return true;
}

static std::string typeToString(Style::Type type) {
    switch (type) {
    case Style::Type::WRAPPER:
        return "Wrapper";
    case Style::Type::BUILTIN:
        return "BuiltIn";
    case Style::Type::INT:
        return "Int";
    case Style::Type::RAWINT:
        return "Raw Int";
    case Style::Type::COLOR:
        return "Color";
    case Style::Type::EFFECT:
        return "Effect";
    }

    return "INVALID";
}
