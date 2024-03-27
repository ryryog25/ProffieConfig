#pragma once
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * styles/elements/effects.h
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
#include <optional>

namespace BladeStyles {

#define GENERAL_EFFECTS \
    EMAP(NONE, "NONE") \
    EMAP(CLASH, "CLASH") \
    EMAP(CLASH_UPDATE, "CLASH_UPDATE") \
    EMAP(BLAST, "BLAST") \
    EMAP(FORCE, "FORCE") \
    EMAP(STAB, "STAB") \
    EMAP(BOOT, "BOOT") \
    EMAP(LOCKUP_BEGIN, "LOCKUP_BEGIN") \
    EMAP(LOCKUP_END, "LOCKUP_END") \
    EMAP(DRAG_BEGIN, "DRAG_BEGIN") \
    EMAP(DRAG_END, "DRAG_END") \
    EMAP(PREON, "PREON") \
    EMAP(POSTOFF, "POSTOFF") \
    EMAP(IGNITION, "IGNITION") \
    EMAP(RETRACTION, "RETRACTION") \
    EMAP(CHANGE, "CHANGE") \
    EMAP(NEWFONT, "NEWFONT") \
    EMAP(LOW_BATTERY, "LOW_BATTERY") \
    EMAP(POWERSAVE, "POWERSAVE") \
    EMAP(BATTERY_LEVEL, "BATTERY_LEVEL") \
    EMAP(VOLUME_LEVEL, "VOLUME_LEVEL") \
    EMAP(ON, "ON") \
    EMAP(OFF, "OFF") \
    EMAP(FAST_ON, "FAST_ON") \
    EMAP(FAST_OFF, "FAST_OFF") \
    EMAP(QUOTE, "QUOTE") \
    EMAP(SECONDARY_IGNITION, "SECONDARY_IGNITION") \
    EMAP(SECONDARY_RETRACTION, "SECONDARY_RETRACTION") \
    EMAP(OFF_CLASH, "OFF_CLASH") \
    EMAP(NEXT_QUOTE, "NEXT_QUOTE") \
    EMAP(INTERACTIVE_PREON, "INTERACTIVE_PREON") \
    EMAP(INTERACTIVE_BLAST, "INTERACTIVE_BLAST") \
    EMAP(TRACK, "TRACK") \
    EMAP(BEGIN_BATTLE_MODE, "BEGIN_BATTLE_MODE") \
    EMAP(END_BATTLE_MODE, "END_BATTLE_MODE") \
    EMAP(BEGIN_AUTO_BLAST, "BEGIN_AUTIO_BLAST") \
    EMAP(END_AUTO_BLAST, "END_AUTO_BLAST") \
    EMAP(ALT_SOUND, "ALT_SOUND") \
    EMAP(TRANSITION_SOUND, "TRANSITION_SOUND") \
    EMAP(SOUND_LOOP, "SOUND_LOOP") \

#define BLASTER_EFFECTS \
    EMAP(STUN, "STUN") \
    EMAP(FIRE, "FIRE") \
    EMAP(CLIP_IN, "CLIP_IN") \
    EMAP(CLIP_OUT, "CLIP_OUT") \
    EMAP(RELOAD, "RELOAD") \
    EMAP(MODE, "MODE") \
    EMAP(RANGE, "RANGE") \
    EMAP(EMPTY, "EMPTY") \
    EMAP(FULL, "FULL") \
    EMAP(JAM, "JAM") \
    EMAP(UNJAM, "UNJAM") \
    EMAP(PLI_ON, "PLI_ON") \
    EMAP(PLI_OFF, "PLI_OFF") \

#define MINIGAME_EFFECTS \
    EMAP(GAME_START, "GAME_START") \
    EMAP(GAME_ACTION1, "GAME_ACTION1") \
    EMAP(GAME_ACTION2, "GAME_ACTION2") \
    EMAP(GAME_CHOICE, "GAME_CHOICE") \
    EMAP(GAME_RESPONSE1, "GAME_RESPONSE1") \
    EMAP(GAME_RESPONSE2, "GAME_RESPONSE2") \
    EMAP(GAME_RESULT1, "GAME_RESULT1") \
    EMAP(GAME_RESULT2, "GAME_RESULT2") \
    EMAP(GAME_WIN, "GAME_WIN") \
    EMAP(GAME_LOSE, "GAME_LOSE") \

#define USER_EFFECTS \
    EMAP(USER1, "USER1") \
    EMAP(USER2, "USER2") \
    EMAP(USER3, "USER3") \
    EMAP(USER4, "USER4") \
    EMAP(USER5, "USER5") \
    EMAP(USER6, "USER6") \
    EMAP(USER7, "USER7") \
    EMAP(USER8, "USER8") \

#define ERR_EFFECTS \
    EMAP(SD_CARD_NOT_FOUND, "SD_CARD_NOT_FOUND") \
    EMAP(ERROR_IN_FONT_DIRECTORY, "ERROR_IN_FONT_DIRECTORY") \
    EMAP(ERROR_IN_BLADE_ARRAY, "ERROR_IN_BLADE_ARRAY") \
    EMAP(FONT_DIRECTORY_NOT_FOUND, "FONT_DIRECTORY_NOT_FOUND") \

#define ALL_EFFECTS \
    GENERAL_EFFECTS \
    BLASTER_EFFECTS \
    MINIGAME_EFFECTS \
    USER_EFFECTS \

enum class Effect {
#	define EMAP(def, str) def,

    ALL_EFFECTS

#	undef EMAP
};

std::optional<Effect> strToEffect(const std::string&);
std::optional<std::string> effectToStr(Effect);
std::optional<std::string> effectToHumanStr(Effect);

}
