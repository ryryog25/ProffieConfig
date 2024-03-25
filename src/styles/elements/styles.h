#pragma once
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * styles/elements/styles.cpp
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

#include "styles/base.h"
#include "styles/elements/functions.h"

namespace Style::Styles {
#define STYLE(name, humanName, ...) ELEM(name, humanName, COLOR, __VA_ARGS__)
#define FUNCTION(funcName, ...) std::make_shared<funcName>(std::vector<Value>{__VA_ARGS__})
#define INT(num) FUNCTION(Function::Int, num)


// Usage: AlphaL<COLOR, ALPHA>
// COLOR: COLOR or LAYER
// ALPHA: FUNCTION
// Return value: LAYER
// This function makes a color transparent. The ALPHA function specifies just how opaque it should be.
// If ALPHA is zero, the returned color will be 100% transparent. If Alpha is 32768, the returned color will
// be 100% opaque. Note that if COLOR is already transparent, it will be made more transparent. Example:
// If COLOR is 50% opaque, and ALPHA returns 16384, the result will be 25% opaque.
STYLE(AlphaL, "Transparency",
      Arg("Color", COLOR | FUNCTION),
      Arg("Value", INT | FUNCTION)
      )

// To enable Gradient/Mixes constricted within Bump<> and SmoothStep<> layers
// Example: AlphaMixL<Bump<Int<16384>,Int<16384>>,Red,Green,Blue> will produce a gradient within the Bump
//
// I did some testing, this seems to mix the varadic colors based on the input in the way as follows:
// The lower the value, this will return a black, then the first color, etc. As higher values are approached,
// the color will fade back to black, from the last color.
STYLE(AlphaMixL, "Mix With Transparency",
      Arg("Input", INT | FUNCTION),
      Arg("Color #", COLOR)
      )

// Usage: AudioFlicker<A, B>
// Or: AudioFlickerL<B>
// A, B: COLOR
// return value: COLOR
// Mixes between A and B based on audio. Quiet audio
// means more A, loud audio means more B.
// Based on a single sample instead of an average to make it flicker.
STYLE(AudioFlickerL, "Audio Flicker",
      Arg("Flicker Color", COLOR)
      )

STYLE(AudioFlicker, "Audio Flicker",
      Arg("Base Color", COLOR),
      Arg("Flicker Color", COLOR)
      )

// NO DOCUMENTATION
// BladeShortenerWrapper

// Usage: Blast<BASE, BLAST, FADEOUT_MS, WAVE_SIZE, WAVE_MS>
// Or: BlastL<BLAST, FADEOUT_MS, WAVE_SIZE, WAVE_MS>
// BASE, BLAST: COLOR
// FADEOUT_MS: a number (defaults to 150)
// WAVE_SIZE: a number (defaults to 100)
// WAVE_MS: a number (defaults to 400)
// return value: COLOR
// Normally shows BASE, but creates a blast effect using
// the color BLAST when a blast is requested. The effect
// is basically two humps moving out from the blast location.
// The size of the humps can be changed with WAVE_SIZE, note
// that smaller values makes the humps bigger. WAVE_MS determines
// how fast the waves travel. Smaller values makes the waves
// travel slower. Finally FADEOUT_MS determines how fast the
// humps fade back to the base color.
STYLE(Blast, "Blast",
      Arg("Base Color", COLOR),
      Arg("Blast Color", COLOR),
      Arg("Fadeout Time (ms)", INT, 200),
      Arg("Wave Size", INT, 100),
      Arg("Wave Time (ms)", INT, 400),
      Arg("Trigger Effect", EFFECT, Effect::BLAST)
      )

STYLE(BlastL, "Blast",
      Arg("Blast Color", COLOR),
      Arg("Fadeout Time (ms)", INT, 200),
      Arg("Wave Size", INT, 100),
      Arg("Wave Time (ms)", INT, 400),
      Arg("Trigger Effect", EFFECT, Effect::BLAST)
      )

// Usage: BlastFadeout<BASE, BLAST, FADEOUT_MS>
// Or: BlastFadeoutL<BLAST, FADEOUT_MS>
// BASE, BLAST: COLOR
// FADEOUT_MS: a number (defaults to 250)
// return value: COLOR
// Normally shows BASE, but swiches to BLAST when a blast
// is requested and then fades back to BASE. FADEOUT_MS
// specifies out many milliseconds the fade takes.
STYLE(BlastFadeoutL, "Blast Fadeout",
      Arg("Blast Color", COLOR),
      Arg("Fadeout Time (ms)", INT, 250),
      Arg("Trigger Effect", EFFECT, Effect::BLAST)
      )

STYLE(BlastFadeout, "Blast Fadeout",
      Arg("Base Color", COLOR),
      Arg("Blast Color", COLOR),
      Arg("Fadeout Time (ms)", INT, 250),
      Arg("Trigger Effect", EFFECT, Effect::BLAST)
      )

// Usage: OriginalBlast<BASE, BLAST>
// Or: OriginalBlastL<BLAST>
// BASE, BLAST: COLOR
// return value: COLOR
// Normally shows BASE, but creates a blast effect using
// the color BLAST when a blast is requested.
// This was the original blast effect, but it is slow and not
// very configurable.
STYLE(OriginalBlastL, "Original Blast",
      Arg("Blast Color", COLOR),
      Arg("Trigger Effect", EFFECT, Effect::BLAST)
      )

STYLE(OriginalBlast, "Original Blast",
      Arg("Base Color", COLOR),
      Arg("Blast Color", COLOR),
      Arg("Trigger Effect", EFFECT, Effect::BLAST)
      )

// Usage: Blinking<A, B, BLINK_MILLIS, BLINK_PROMILLE>
// or: BlinkingX<A, B, BLINK_MILLIS_FUNC, BLINK_PROMILLE_FUNC>
// or: BlinkingL<B, BLINK_MILLIS_FUNC, BLINK_PROMILLE_FUNC>
// A, B: COLOR
// BLINK_MILLIS: a number
// BLINK_PROMILLE: a number, defaults to 500
// BLINK_MILLIS_FUNC: FUNCTION
// BLINK_PROMILLE_FUNC: FUNCTION
// return value: COLOR
// Switches between A and B.
// A full cycle from A to B and back again takes BLINK_MILLIS milliseconds.
// If BLINK_PROMILLE is 500, we select A for the first half and B for the
// second half. If BLINK_PROMILLE is smaller, we get less A and more B.
// If BLINK_PROMILLE is 0, we get all B.
// If BLINK_PROMILLE is 1000 we get all A.
STYLE(BlinkingL, "Blinking",
      Arg("Blink Color", COLOR),
      Arg("Blink Time (ms)", INT | FUNCTION),
      Arg("Distribution", INT | FUNCTION)
      )
STYLE(BlinkingX, "Blinking",
      Arg("Color 1", COLOR),
      Arg("Color 2", COLOR),
      Arg("Blink Time (ms)", INT | FUNCTION),
      Arg("Distribution", INT | FUNCTION)
      )
STYLE(Blinking, "Blinking",
      Arg("Color 1", COLOR),
      Arg("Color 1", COLOR),
      Arg("Blink Time (ms)", INT),
      Arg("Distribution", INT, 500)
      )

// Usage: BrownNoiseFlicker<A, B, grade>
// Or: BrownNoiseFlickerL<B, grade>
// A, B: COLOR
// grade: int
// return value: COLOR
// Randomly selects between A and B, but keeps nearby
// pixels looking similar.
STYLE(BrownNoiseFlickerL, "Brown Noise Flicker",
      Arg("Flicker Color", COLOR),
      Arg("Grade", INT | FUNCTION)
      )

STYLE(BrownNoiseFlicker, "Brown Noise Flicker",
      Arg("Base Color", COLOR),
      Arg("Flicker Color", COLOR),
      Arg("GRADE", INT) // For some reason the non-L version also has its raw int multiplied by 128
      )

// Usage: ByteOrderStyle<BYTEORDER, COLOR>
// BYTEORDER: Color8::RGB, or one of the other byte orders
// COLOR: COLOR
// return value: COLOR
//
// This shuffles the RGB values around. It's meant to be used
// when you are mixing different kind of pixels on one string.
// While it's not recommended to do so, this should make it
// easier to compensate by re-ordering the channels.
//
// I'll need to revisit this to create a color order type
// STYLE(ByteOrderStyle, "Rearrange Color Bytes",
//       Arg("Color Order", COLORORDER),
//       Arg("Color", COLOR)
//       )

// Usage: SimpleClash<BASE, CLASH_COLOR, CLASH_MILLIS>
// Or: SimpleClashL<CLASH_COLOR, CLASH_MILLIS>
// BASE: COLOR
// CLASH_COLOR: COLOR (defaults to white)
// CLASH_MILLIS: a number (defaults to 40)
// return value: COLOR
// Turns the blade to CLASH_COLOR for CLASH_MILLIS millseconds
// when a clash occurs.
STYLE(SimpleClashL, "Simple Clash",
      Arg("Clash Color", COLOR),
      Arg("Clash Time (ms)", INT, 40),
      Arg("Trigger Effect", EFFECT, Effect::CLASH),
      Arg("Stab Shape", INT | FUNCTION, FUNCTION(SimpleClashL, INT(16384), INT(24000))) // Your guess is as good as mine as to what this does...
      )

STYLE(SimpleClash, "Simple Clash",
      Arg("Base Color", COLOR),
      Arg("Clash Color", COLOR, Color::WHITE),
      Arg("Clash Time (ms)", INT, 40),
      Arg("Trigger Effect", EFFECT, Effect::CLASH),
      Arg("Stab Shape", INT | FUNCTION, FUNCTION(SimpleClashL, INT(16384), INT(24000))) // Your guess is as good as mine as to what this does...
      )

// Usage: LocalizedClash<BASE, CLASH_COLOR, CLASH_MILLIS, CLASH_WIDTH_PERCENT=50>
// Usage: LocalizedClashL<CLASH_COLOR, CLASH_MILLIS, CLASH_WIDTH_PERCENT=50>
// BASE: COLOR
// CLASH_COLOR: COLOR (defaults to white)
// CLASH_MILLIS: a number (defaults to 40)
// return value: COLOR
// Similar to SimpleClash, but lights up a portion of the blade.
// The fraction of the blade is defined by CLASH_WIDTH_PERCENT
// The location of the clash is random within the middle half of the blade.
// Localized clashes should work well with stabs with no modifications.
STYLE(LocalizedClashL, "Localized Clash",
      Arg("Clash Color", COLOR),
      Arg("Clash Time (ms)", INT, 40),
      Arg("Clash Width (%)", INT, 50),
      Arg("Trigger Effect", EFFECT, Effect::CLASH)
      )

STYLE(LocalizedClash, "Localized Clash",
      Arg("Base Color", COLOR),
      Arg("Clash Color", COLOR),
      Arg("Clash Time (ms)", INT, 40),
      Arg("Clash Width (%)", INT, 50),
      Arg("Trigger Effect", EFFECT, Effect::CLASH)
      )

// Ended on clash.h

#undef STYLE
}
