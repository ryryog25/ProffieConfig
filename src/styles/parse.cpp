#include "parse.h"
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * styles/parse.cpp
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

#include "generator.h"
#include "elements/lockuptype.h"
#include "log/logger.h"

struct TokenizedStyle {
    std::string name;
    std::vector<std::string> args;
};
static std::optional<TokenizedStyle> tokenizeStyle(const std::string&);
static BladeStyles::Value parseArg(const std::string& token, int32_t type);

static std::string typeToString(int32_t);

std::shared_ptr<BladeStyles::BladeStyle> BladeStyles::parseString(const std::string& styleStr, bool* foundStyle) {
    auto styleTokens{tokenizeStyle(styleStr)};
    if (!styleTokens) return nullptr;

    auto styleGen{Generator::get(styleTokens->name)};
    if (!styleGen) {
        Logger::error("Style not recognized: " + styleTokens->name);
        return nullptr;
    } else if (foundStyle) *foundStyle = true;

    auto style{styleGen({})};
    auto& styleArgs{style->getArgs()};
    if (styleArgs.size() < styleTokens->args.size() && (styleArgs.empty() ? true : !(styleArgs.at(styleArgs.size() - 1).type & VARIADIC))) {
        Logger::error("Too many arguments for: " + styleTokens->name);
        return nullptr;
    }

    for (size_t i = 0; i < styleArgs.size(); i++) {
        if (i >= styleTokens->args.size()) {
            if (!styleArgs.at(i).defaultVal) {
                Logger::error("Not enough arguments for: " + styleTokens->name);
                return nullptr;
            }
            break;
        }

        auto arg{parseArg(styleTokens->args.at(i), styleArgs.at(i).type)};
        if (!arg) {
            Logger::warn("Error while parsing style argument " + std::to_string(i + 1) + ": " + styleTokens->name);
            return nullptr;
        }
        style->setArg(i, arg);
    }

    return style;
}

std::optional<std::string> BladeStyles::asString(const std::shared_ptr<BladeStyle> style) {
    if (!style) return std::nullopt;
    std::string ret{style->name};
    ret += '<';

    const auto& args{style->getArgs()};
    const ValueBase* argValue;
    for (size_t i = 0; i < args.size(); i ++) {
        const auto& arg{args.at(i)};
        if (!arg.val) {
            if (!arg.defaultVal) {
                Logger::error("Missing value for argument \"" + std::string(arg.name) + "\" for style \"" + std::string(style->humanName)+ "\"");
                return std::nullopt;
            }
            argValue = &arg.defaultVal.value();
        } else argValue = &arg.val.value();

        if (std::holds_alternative<std::shared_ptr<BladeStyle>>(*argValue)) {
            auto argStr{asString(std::get<std::shared_ptr<BladeStyle>>(*argValue))};
            if (!argStr) {
                Logger::error("Failed parsing argument \"" + std::string(arg.name) + "\" for style \"" + std::string(style->humanName) + "\"");
                return std::nullopt;
            }
            ret += argStr.value();
            continue;
        }

        switch (arg.type & FLAGMASK) {
        case INT:
            ret += std::to_string(std::get<int32_t>(*argValue));
            break;
        case BITS: {
            ret += "0b";
            auto argVal = std::get<int32_t>(*argValue);
            for (int8_t bit = 0; bit < 16; bit++) {
                ret += ((argVal >> bit) & 0b1) ? '1' : '0';
            }
            break; }
        case BOOL:
            ret += std::get<int32_t>(*argValue) ? "true" : "false";
            break;
        case COLOR: {
            auto colorStr{colorToStr(std::get<Color>(*argValue))};
            if (!colorStr) {
                Logger::error("Error while parsing color for argument \"" + std::string(arg.name) + "\" in style \"" + std::string(style->humanName) + "\"");
                return std::nullopt;
            }
            ret += colorStr.value();
            break; }
        case EFFECT: {
            auto effectStr{effectToStr(std::get<Effect>(*argValue))};
            if (!effectStr) {
                Logger::error("Error while parsing effect for argument \"" + std::string(arg.name) + "\" in style \"" + std::string(style->humanName) + "\"");
                return std::nullopt;
            }
            ret += effectStr.value();
            break; }
        case LOCKUPTYPE: {
            auto lockupStr{lockupTypeToStr(std::get<LockupType>(*argValue))};
            if (!lockupStr) {
                Logger::error("Error while parsing lockup type for argument \"" + std::string(arg.name) + "\" in style \"" + std::string(style->humanName) + "\"");
                return std::nullopt;
            }
            ret += lockupStr.value();
            break; }
        default:
            Logger::error("Unknown/Invalid argument type for argument \"" + std::string(arg.name) + "\" for style \"" + std::string(style->humanName) + "\"");
            return std::nullopt;
        }

        if (i != args.size() - 1) ret += ',';
    }

    ret += '>';
    if (style->type & WRAPPER) ret += "()";
    return ret;
}

static std::optional<TokenizedStyle> tokenizeStyle(const std::string& styleStr) {
    auto styleBegin{styleStr.find_first_not_of(" &\t\n")};
    if (styleBegin == std::string::npos) {
        Logger::warn("Could not tokenize style!", false);
        return std::nullopt;
    }
    auto nameEnd{styleStr.find_first_of("<(", styleBegin)};
    auto styleName{styleStr.substr(styleBegin, nameEnd - styleBegin)};

    TokenizedStyle styleTokens;
    styleTokens.name = styleName;

    if (nameEnd > styleStr.length() || styleStr.at(nameEnd) != '<') return styleTokens;

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
                break;
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

static BladeStyles::Value parseArg(const std::string& token, int32_t type) {
    using namespace BladeStyles;

    bool foundStyle;
    std::shared_ptr<BladeStyles::BladeStyle> argObj{nullptr};
    if (token.find('<') != std::string::npos) argObj = parseString(token, &foundStyle);

    if (argObj) {
        if (!(type & FUNCTION) && !(type & COLOR)) {
            Logger::error("Got unexpected function for argument.");
            return std::nullopt;
        }
        if (argObj->type != type) {
            Logger::error("Mismatched argument type. (Expected " +
                          typeToString(type) +
                          " but got " +
                          typeToString(argObj->type) +
                          ")");
            return std::nullopt;
        }

        return argObj;
    } else if (foundStyle) return std::nullopt;

    switch (type & FLAGMASK) {
    case WRAPPER:
        Logger::error("Wrapper found inside style!");
        return std::nullopt;
    case BUILTIN:
        Logger::error("Builtin style found inside style!");
        return std::nullopt;
    case INT:
        if (!std::isdigit(token.at(0)) && (token.size() >= 2 ? !std::isdigit(token.at(1)) || token.at(0) != '-' : true)) {
            Logger::error("Expected int for argument but didn't get a number.");
            return std::nullopt;
        }
        return std::stoi(token);
    case BOOL:
        // TODO
        return true;
    case BITS:
        // TODO
        return 10; // man idk, we'll get to this later
    case COLOR: {
        auto color{strToColor(token)};
        if (!color) {
            Logger::error("Invalid color for argument: " + token);
            return std::nullopt;
        }
        return color; }
    case EFFECT: {
        auto effect{strToEffect(token)};
        if (!effect) {
            Logger::error("Invalid effect for argument: " + token);
            return std::nullopt;
        }
        return effect; }
    case LOCKUPTYPE: {
        auto lockupType{strToEffect(token)};
        if (!lockupType) {
            Logger::error("Invalid lockup type for argument: " + token);
            return std::nullopt;
        }
        return lockupType; }
    }

    Logger::error("Unknown argument type found for argument: " + token);
    return std::nullopt;
}

static std::string typeToString(int32_t type) {
    switch (type & BladeStyles::FLAGMASK) {
    case BladeStyles::WRAPPER:
        return "Wrapper";
    case BladeStyles::BUILTIN:
        return "BuiltIn";
    case BladeStyles::INT:
        return "Int";
    case BladeStyles::BITS:
        return "Bits";
    case BladeStyles::BOOL:
        return "Boolean";
    case BladeStyles::COLOR:
        return "Color";
    case BladeStyles::COLOR & BladeStyles::TRANSPARENT:
        return "Layer (Transparent Color)";
    case BladeStyles::EFFECT:
        return "Effect";
    case BladeStyles::LOCKUPTYPE:
        return "LockupType";
    }

    return "INVALID";
}
