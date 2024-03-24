#pragma once
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * styles/elements/functions.h
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

/*
 * For the ones that make sense to be boolean output, TRUE is 32768 and FALSE is 0
 * For those that return a range, the range is nearly always 0 to 32768, same goes
 * for those that expect a range input. Nothing here is based off # of leds.
 */

/*
 * I've noticed X functions seem to be the "core" functions and tend to take INTs,
 * while non-X functions (if there's an X variant) take raw ints, and wrap the X version.
 */

namespace Style::Function {

#define FUNC(name, humanName, ...) ELEM(name, humanName, INT | FUNCTION, __VA_ARGS__)
#define INT(num) std::make_shared<Int>(std::vector<Arg>{num})

// Usage: Int<N>
// Returns N
// N: a number
// return value: INTEGER
//
// This needs to be first since it's used by everything else
FUNC(Int, "Number",
     Arg("Number", INT)
     )

// Usage: AltF
// return value: INTEGER
// Returns current_alternative for use in ColorSelect<>, TrSelect<> or IntSelect<>
FUNC(AltF, "Alternative")

// Usage: SyncAltToVarianceF
// return value: INTEGER (always zero)
// Enables Bidirectional synchronization between ALT and VARIANCE.
// If variance changes, so does alt, if alt changes, so does variance.
FUNC(SyncAltToVarianceF, "Sync Alternative and Variance")

// Usage: SyncAltToVarianceL
// return value: LAYER (transparent)
// Synchronizes alt to variance, just put it somewhere in the layer stack. (but not first)
ELEM(SyncAltToVarianceL, "Sync Alternative and Variance", COLOR)

// Usage: BatteryLevel
// Returns 0-32768 based on battery level.
// returned value: INTEGER
FUNC(BatteryLevel, "Battery Level")


// Usage: BladeAngleX<MIN, MAX>
// Returns 0-32768 based on angle of blade
// MIN : FUNCTION (defaults to Int<0>)
// MAX : FUNCTION (defaults to Int<32768>)
// MIN and MAX specifies the range of angles which are used.
// For MIN and MAX 0 means down and 32768 means up and 16384 means
// pointing towards the horizon.
// So if MIN=16484 and MAX=32768, BladeAngle will return zero when you
// point the blade towards the horizon and 32768 when you point it
// straight up. Any angle below the horizon will also return zero.
// returned value: FUNCTION, same for all leds
FUNC(BladeAngle, "Blade Angle",
     Arg("Min", INT, 0),
     Arg("Max", INT, 32768)
     )

FUNC(BladeAngleX, "Blade Angle",
     Arg("Min", INT | FUNCTION, INT(0)),
     Arg("Max", INT | FUNCTION, INT(32768))
     )

// Usage: BlastF<FADEOUT_MS, WAVE_SIZE, WAVE_MS, EFFECT>
// FADOUT_MS: a number (defaults to 200)
// WAVE_SIZE: a number (defaults to 100)
// WAVE_MS: a number (defaults to 400)
// EFFECT: a BladeEffectType (defaults to EFFECT_BLAST)
// returned value: FUNCTION
// This function is intended to Mix<> or AlphaL<>, when a
// a blast  occurs, it makes a wave starting at the blast.
// location (which is currently random) and travels out
// from that direction. At the peak, this function returns
// 32768 and when there is no blash it returns zero.
// The FADOUT_MS controls how long it takes the wave to
// fade out. The WAVE_SIZE controls the width of the wave.
// The WAVE_MS parameter controls the speed of the waves.
// EFFECT can be used to trigger this effect by something
// other than a blast effect.
FUNC(BlastF, "Blast",
     Arg("Fadeout (ms)", 	INT, 200),
     Arg("Wave Size", 		INT, 100),
     Arg("Wave Time (ms)",	INT, 400),
     Arg("Trigger Effect", EFFECT, Effect::BLAST)
     )

// Usage: BlastFadeoutF<FADEOUT_MS, EFFECT>
// FADEOUT_MS: a number (defaults to 250)
// EFFECT: a BladeEffectType (defaults to EFFECT_BLAST)
// return value: FUNCTION
// NOrmally returns 0, but returns up to 32768 when the
// selected effect occurs. Then if fades back to zero over
// FADEOUT_MS milliseconds.
FUNC(BlastFadeoutF, "Blast Fadeout",
     Arg("Fade Time (ms)", INT, 250),
     Arg("Trigger Effect", EFFECT, Effect::BLAST)
     )

// Usage: OriginalBlastF<EFFECT>
// EFFECT: a BladeEffectType (defaults to EFFECT_BLAST)
// return value: FUNCTION
// Original blast function. Normally returns zero, but
// returns up to 32768 when the selected effect occurs.
FUNC(OriginalBlastF, "Original Blast",
     Arg("Trigger Effect", EFFECT, Effect::BLAST)
     )

// Usage: BlinkingF<A, B, BLINK_MILLIS_FUNC, BLINK_PROMILLE_FUNC>
// BLINK_MILLIS: a number
// BLINK_PROMILLE: a number, defaults to 500
// BLINK_MILLIS_FUNC: FUNCTION
// BLINK_PROMILLE_FUNC: FUNCTION
// return value: FUNCTION
// Switches between 0 and 32768
// A full cycle from 0 to 328768 and back again takes BLINK_MILLIS milliseconds.
// If BLINK_PROMILLE is 500, we select A for the first half and B for the
// second half. If BLINK_PROMILLE is smaller, we get less A and more B.
// If BLINK_PROMILLE is 0, we get all 0.
// If BLINK_PROMILLE is 1000 we get all 32768.
FUNC(Blinking, "BlinkingF",
     Arg("Time (ms)", INT),
     Arg("Distribution", INT)
     )

// Usage: BrownNoiseF<GRADE>
// return value: FUNCTION
// Returns a value between 0 and 32768 with nearby pixels being similar.
// GRADE controls how similar nearby pixels are.
FUNC(BrownNoiseF, "Brown Noise",
     Arg("Grade", INT | FUNCTION)
     )

// Usage: SlowNoise<SPEED>
// return value: FUNCTION
// Returns a value between 0 and 32768 which changes randomly up and
// down over time. All pixels gets the same value.
// SPEED controls how quickly the value changes.
FUNC(SlowNoise, "Slow Noise",
     Arg("Speed", INT | FUNCTION)
     )

// Usage: Bump<BUMP_POSITION, BUMP_WIDTH_FRACTION>
// Returns different values for each LED, forming a bump shape.
// If BUMP_POSITION is 0, bump will be at the hilt.
// If BUMP_POSITION is 32768, the bump will be at the tip.
// If BUMP_WIDTH_FRACTION is 1, bump will be extremely narrow.
// If BUMP_WIDTH_FRACTION is 32768, it will fill up most/all of the blade.
// BUMP_POSITION, BUMP_WIDTH_FRACTION: INTEGER
FUNC(Bump, "Bump",
     Arg("Position", INT | FUNCTION),
     Arg("Width", INT | FUNCTION, INT(16385))
     )

// Usage: HumpFlickerFX<FUNCTION>
// or: HumpFlickerF<N>
// FUNCTION: FUNCTION
// N: NUMBER
// return value: INTEGER
// Creates hump shapes that randomize over the blade.
// The returned INTEGER is the size of the humps.
// Large values can give the blade a shimmering look,
// while small values look more like speckles.
FUNC(HumpFlickerF, "Hump Flicker",
     Arg("Hump Width", INT)
     )

FUNC(HumpFlickerFX, "Hump Flicker",
     Arg("Hump Width", INT | FUNCTION)
     )

// Usage: Remap<CenterDistF<CENTER>,COLOR>
// Distributes led COLOR from CENTER
// CENTER : FUNCTION (defaults to Int<16384>)
//
//
FUNC(CenterDistF, "Center Distribution",
     Arg("Center", INT | FUNCTION, INT(16384))
     )

// Usage: ChangeSlowly<F, SPEED>
// Changes F by no more than SPEED values per second.
// F, SPEED: FUNCTION
// return value: FUNCTION, same for all LEDs
FUNC(ChangeSlowly, "Change Slowly",
     Arg("Input", INT | FUNCTION),
     Arg("Speed", INT | FUNCTION)
     )

// Usage: CircularSectionF<POSITION, FRACTION>
// POSITION: FUNCTION position on the circle or blade, 0-32768
// FRACTION: FUNCTION how much of the blade to light up, 0 = none, 32768 = all of it
// return value: FUNCTION
// Returns 32768 for LEDs near the position with wrap-around.
// Could be used with MarbleF<> for a marble effect, or with
// Saw<> for a spinning/colorcycle type effect.
// Example: If POSITION = 0 and FRACTION = 16384, then this function
// will return 32768 for the first 25% and the last 25% of the blade
// and 0 for the rest of the LEDs.
FUNC(CircularSectionF, "Circular Section",
     Arg("Position", INT | FUNCTION),
     Arg("Fraction", INT | FUNCTION)
     )

// Usage: ClampF<F, MIN, MAX>
// Or:    ClampFX<F, MINCLASS, MAXCLASS>
// Clamps value between MIN and MAX
// F, MIN, MAX: INTEGER
// MINCLASS, MAXCLASS: FUNCTION
// return value: INTEGER
FUNC(ClampF, "Clamp",
     Arg("Input", INT | FUNCTION),
     Arg("Min", INT),
     Arg("Max", INT)
     )

FUNC(ClampFX, "Clamp",
     Arg("Input", INT | FUNCTION),
     Arg("Min", INT | FUNCTION),
     Arg("Max", INT | FUNCTION)
     )

// Usage: ClampF<F, MIN, MAX>
// Or:    ClampFX<F, MINCLASS, MAXCLASS>
// Clamps value between MIN and MAX
// F, MIN, MAX: INTEGER
// MINCLASS, MAXCLASS: FUNCTION
// return value: INTEGER
FUNC(ClashImpactF, "Clash Impact",
     Arg("Min", INT),
     Arg("Max", INT)
     )

FUNC(ClashImpactFX, "Clash Impact",
     Arg("Min", INT | FUNCTION),
     Arg("Max", INT | FUNCTION)
     )

// Usage: Divide<F, V>
// Divide F by V
// If V = 0, returns 0
// F, V: FUNCTION,
// return value: FUNCTION
// Please note that Divide<> isn't an exact inverse of Mult<> because mult uses fixed-point mathematics
// (it divides the result by 32768) while Divide<> doesn't, it just returns F / V
FUNC(Divide, "Divide",
     Arg("Numerator", INT | FUNCTION),
     Arg("Denominator", INT | FUNCTION)
     )

// Usage: EffectPulse<EFFECT>
// EFFECT: BladeEffectType
// Returns 32768 once for each time the given effect occurs.
FUNC(EffectPulse, "Pulse On Effect",
     Arg("Trigger Effect", EFFECT)
     )

// Usage: LockupPulseF<LOCKUP_TYPE>
// LOCKUP_TYPE: a SaberBase::LockupType
// Returns 32768 once for each time the given lockup occurs.
FUNC(LockupPulseF, "Pulse On Lockup",
     Arg("Trigger Lockup", LOCKUPTYPE)
     )

// Usage: IncrementWithReset<PULSE, RESET_PULSE, MAX, I>
// PULSE: FUNCTION (pulse type)
// RESET_PULSEE: FUNCTION (pulse type) defaults to Int<0> (no reset)
// MAX, I: FUNCTION
// Starts at zero, increments by I each time the PULSE occurse.
// If it reaches MAX it stays there.
// Resets back to zero when RESET_PULSE occurs.
FUNC(IncrementWithReset, "Increment With Reset",
     Arg("Increment Pulse", INT | FUNCTION),
     Arg("Pulse Reset", 	INT | FUNCTION, INT(0)),
     Arg("Max", 			INT | FUNCTION, INT(32768)),
     Arg("Increment", 		INT | FUNCTION, INT(1))
     )

// Usage: EffectIncrementF<EFFECT, MAX, I>
// Increases by value I (up to MAX) each time EFFECT is triggered
// If current value + I = MAX, it returns 0.
// If adding I exceeds MAX, the function returns 0 + any remainder in excesss of MAX
// I, MAX = numbers
// return value: INTEGER
FUNC(EffectIncrementF, "Increment On Effect",
     Arg("Trigger Effect", EFFECT),
     Arg("Max", INT | FUNCTION, INT(32768)),
     Arg("Increment", INT | FUNCTION, INT(1))
     )

// Usage: EffectPosition<>
// Or: EffectPosition<EFFECT>
// EFFECT: effect type
// return value: INTEGER
//
// EffectPosition returns the position of a particular effect. 0 = base, 32768 = tip.
// For now, this location is random, but may be set explicitly in the future.
// When used as EffectPosition<> inside a TransitionEffectL whose EFFECT is already specified,
// then it will automatically use the right effect.
FUNC(EffectPosition, "Effect Position",
     Arg("Effect", EFFECT, Effect::NONE)
     )

// Usage: HoldPeakF<F, HOLD_MILLIS, SPEED>
// Holds Peak value of F for HOLD_MILLIS.
// then transitions down over SPEED to current F
// F, HOLD_MILLIS and SPEED: FUNCTION
// return value: FUNCTION, same for all LEDs
FUNC(HoldPeakF, "Hold Peak Value",
     Arg("Input", INT | FUNCTION),
     Arg("Hold Time (ms)", INT | FUNCTION),
     Arg("Ramp Down Speed", INT | FUNCTION)
     )

// Usage: Ifon<A, B>
// Returns A if saber is on, B otherwise.
// A, B: INTEGER
// return value: INTEGER
FUNC(Ifon, "If On",
     Arg("Value if On", INT | FUNCTION),
     Arg("Value if Off", INT | FUNCTION)
     )

// InOutFunc<OUT_MILLIS, IN_MILLIS>
// IN_MILLIS, OUT_MILLIS: a number
// RETURN VALUE: FUNCTION
// 0 when off, 32768 when on, takes OUT_MILLIS to go from 0 to 32768
// takes IN_MILLIS to go from 32768 to 0.
// NEED A BETTER HUMAN NAME
FUNC(InOutFunc, "In Out Func",
     Arg("Out Time (ms)", INT),
     Arg("In Time (ms)", INT)
     )

FUNC(InOutFuncX, "In Out Func",
     Arg("Out Time (ms)", INT | FUNCTION),
     Arg("In Time (ms)", INT | FUNCTION)
     )

// Thermal Detonator?
// NO DOCUMENTATION
// NEED A BETTER HUMAN NAME
FUNC(InOutFuncTD, "In Out Func TD",
     Arg("Out Time (ms)", INT),
     Arg("In Time (ms)", INT),
     Arg("Explode Time (ms)", INT)
     )

// NO DOCUMENTATION
// NEED A BETTER HUMAN NAME
FUNC(InOutHelperF, "In Out Helper",
     Arg("Input", INT | FUNCTION),
     Arg("Allow Disable", BOOL, true) // Add a bool Type?
     )

// Usage: IncrementModulo<PULSE, MAX, INCREMENT>
// PULSE: FUNCTION (pulse type)
// MAX: FUNCTION (not zero) defaults to Int<32768>
// INCREMENT: FUNCTION defaults to Int<1>
// Increments by I each time PULSE occurs wraps around when
// it reaches MAX.
// The documentation is incorrect, the name does end in F
FUNC(IncrementModuloF, "Increment With Wrap",
     Arg("Pulse Input", INT | FUNCTION),
     Arg("Max", INT | FUNCTION, INT(32768)),
     Arg("Increment", INT | FUNCTION, INT(1))
     )

// Usage: ThresholdPulseF<F, THRESHOLD, HYST_PERCENT>
// F: FUNCTION
// THRESHOLD: FUNCTION (defaults to Int<32768>)
// HYST_PERCENT: FUNCTION (defaults to Int<66>
// Returns 32768 once when F > THRESHOLD, then waits until
// F < THRESHOLD * HYST_PERCENT / 100 before going back
// to the initial state (waiting for F > THRESHOLD).
// BETTER HUMAN NAME?
FUNC(ThresholdPulseF, "Pulse on Threshold",
     Arg("Input", INT | FUNCTION),
     Arg("Threshold", INT | FUNCTION, INT(32768)),
     Arg("Hysteresis %", INT | FUNCTION, INT(66))
     )

// Usage: IncrementF<F, V, MAX, I, HYST_PERCENT>
// Increases by value I (up to MAX) each time F >= V
// Detection resets once F drops below V * HYST_PERCENT
// if greater than MAX returns 0
// F, V, I, MAX = numbers
// HYST_PERCENT = percent (defaults to 66)
// return value: INTEGER
// NOTE: this function is designed to separate "events" for use with *Select styles.
// This function may break up SwingSpeed effects or other continuous responsive functions.
FUNC(IncrementF, "Increment on Input",
     Arg("Input", INT | FUNCTION),
     Arg("Increment Threshold", INT | FUNCTION, INT(32768)),
     Arg("Max Value", INT | FUNCTION, INT(32768)),
     Arg("Increment", INT | FUNCTION, INT(1)),
     Arg("Hysteresis %", INT | FUNCTION, INT(66))
     )

// NO DOCUMENTATION
// I think this is for the edit mode args?
// This is what "Arg" runs through:
// void init(int argnum) {
// 	char default_value[16];
// 	itoa(value_, default_value, 10);
// 	const char* arg = CurrentArgParser->GetArg(argnum, "INT", default_value);
// 	if (arg) {
//     value_ = strtol(arg, NULL, 0);
// 	}
// }
// So maybe it gets an arg from the list based on the arg num?
// UPDATE: I'm almost certain this is what it does based on some investigation,
// RgbArg does the same thing...
FUNC(IntArg, "Get Int Arg",
     Arg("Arg", INT | FUNCTION),
     Arg("Default", INT | FUNCTION)
     )

// Usage: IntSelect<SELECTION, Int1, Int2...>
// SELECTION: FUNCTION
// Returns SELECTION of N
// If SELECTION is 0, the first integer is returned, if SELECTIOn is 1, the second and so forth.
// N: numbers
// return value: INTEGER
FUNC(IntSelect, "Select Based on Value",
     Arg("Selection #", INT | FUNCTION | VARIADIC)
)

// Usage: IsBetween<F, BOTTOM, TOP>
// Returns 0 or 32768 based F > BOTTOM and < TOP
// F, BOTTOM, TOP: INTEGER
// return value: INTEGER
FUNC(IsBetween, "Is Between",
     Arg("Input", INT | FUNCTION),
     Arg("Min", INT | FUNCTION),
     Arg("Max", INT | FUNCTION)
     )

// Usage: IsLessThan<F, V>
// Returns 0 or 32768 based on V
// If F < V returns 32768, if F >= V returns 0
// F, V: INTEGER
// return value: INTEGER
FUNC(IsLessThan, "Is Less Than",
     Arg("Input", INT | FUNCTION),
     Arg("Compare", INT | FUNCTION)
     )

FUNC(IsGreaterThan, "Is Greater Than",
     Arg("Input", INT | FUNCTION),
     Arg("Compare", INT | FUNCTION)
     )

// Usage: LayerFunctions<F1, F2, ...>
// F1, F2: FUNCTIONS
// return value: FUNCTION
// Returns (32768 - (32768 - F1) * (32768 * F2) / 32768)
// This is the same as 1-(1-F1)*(1-F2), but multiplied by 32768.
// Basically Mix<LayerFunctions<F1, F2>, A, B> is the same as Mix<F2, Mix<F1, A, B>, B>.
//
// This one makes no sense to me... will need to revisit
// FUNC(LayerFunctions, "LayerFunctions", INT | FUNCTION
//
// )

// Usage: LinearSectionF<POSITION, FRACTION>
// POSITION: FUNCTION position on the blade, 0-32768
// FRACTION: FUNCTION how much of the blade to light up, 0 = none
// return value: FUNCTION
// creates a "block" of pixels at POSITION taking up FRACTION of blade
FUNC(LinearSectionF, "Linear Section",
     Arg("Position", INT | FUNCTION),
     Arg("Section Size", INT | FUNCTION)
     )

// Usage: MarbleF<OFFSET, FRICTION, ACCELERATION, GRAVITY>
// OFFSET: FUNCTION  0-32768, adjust until "down" represents is actually down
// FRICTION: FUNCTION, higher values makes the marble slow down, usually a constant
// ACCELERATION: FUNCTION, a function specifying how much speed to add to the marble
// GRAVITY: FUNCTION higher values makes the marble heavier
// return value: FUNCTION  0-32768, representing point on a circle
// This is intended for a small ring of neopixels.
// It runs a simulation of a marble trapped in a circular
// track and returns the position of that marble.
// Meant to be used with CircularSectionF to turn the marble
// position into a lighted up section.
FUNC(MarbleF, "Marble Simulation",
     Arg("Direction Offset", INT | FUNCTION),
     Arg("Friction", INT | FUNCTION),
     Arg("Acceleration", INT | FUNCTION),
     Arg("Gravity", INT | FUNCTION)
     )

// Usage: ModF<F, MAX>
// F: FUNCTION
// MAX: FUNCTION (not zero)
// When F is greater than MAX, F wraps to 0
// When F is less than 0, F wraps to MAX
// returns Integer
FUNC(ModF, "Modulo",
     Arg("Input", INT | FUNCTION),
     Arg("Divisor", INT | FUNCTION)
     )

// Usage: Mult<F, V>
// Fixed point multiplication of values F * V,
// fixed point 16.15 arithmetic (32768 = 1.0)
// (2*2 would not result in 4),
// (16384 * 16384 = 8192, representation of 0.5*0.5=0.25)
// most blade functions use this method of fixed point calculations
// F, V: INTEGER,
// return value: INTEGER
FUNC(Mult, "Multiply",
     Arg("Input 1", INT | FUNCTION),
     Arg("Input 2", INT | FUNCTION)
     )

// Usage: Percentage<F, V>
// Gets Percentage V of value F,
// Percentages over 100% are allowed and will effectively be a multiplier.
// F, V: INTEGER
// example Percentage<Int<16384>,25>
// this will give you 25% of Int<16384> and returns Int<4096>
// return value: INTEGER
FUNC(Percentage, "Percent Of",
     Arg("Input", INT | FUNCTION),
     Arg("Percent", INT)
     )

// Usage: OnsparkF<MILLIS>
// MILLIS: FUNCTION (defaults to Int<200>)
// return value: FUNCTION
// When the blade turns on, this function starts returning
// 32768, then fades back to zero over MILLIS milliseconds.
// This is intended to be used with Mix<> or AlphaL<> to
// to create a flash of color or white when the blade ignites.
FUNC(OnSparkF, "On Spark",
     Arg("Fade Time (ms)", INT | FUNCTION)
     )

// Returns led as value between 0 ~ 32768
// Keeps existing mapping for pixels when used with Remap<>
// Example: Remap<RampF,COLOR>
// NEED A BETTER HUMAN NAME
FUNC(RampF, "LED Ramp")

// Usage: RandomF
// Return value: FUNCTION
// Returns a random number between 0 and 32768.
// All LEDS gets the same value.
FUNC(RandomF, "Random")

// Usage: RandomPerLEDF
// Return value: FUNCTION
// Returns a random number between 0 and 32768.
// Each LED gets a different random value.
FUNC(RandomPerLEDF, "Random Per LED")

// Usage: EffectRandomF<EFFECT>
// Returns a random value between 0 and 32768 each time EVENT is triggered
// return value: INTEGER
//
// Except it's each time an EFFECT is triggered, not EVENT.
// RandomEffect makes more sense for a name to me...
FUNC(EffectRandom, "Random On Effect",
     Arg("Trigger Effect", EFFECT)
     )

// Usage: RandomBlinkF<MILLIHZ>
// MILLHZ: FUNCTION
// Randomly returns either 0 or 32768 for each LED. The returned value
// is held, but changed to a new random value MILLIHZ * 1000 times per
// second.
// NEED A BETTER HUMAN NAME
FUNC(RandomBlinkF, "Random Per Time",
     Arg("Time (mHz)", INT | FUNCTION)
     )

// Usage: Scale<F, A, B>
// Changes values in range 0 - 32768 to A-B
// F, A, B: INTEGER
// return value: INTEGER
FUNC(Scale, "Scale",
     Arg("Input", INT | FUNCTION),
     Arg("Min", INT | FUNCTION),
     Arg("Max", INT | FUNCTION)
     )

// To simplify inverting a function's returned value
// Example InvertF<BladeAngle<>> will return 0 when up and 32768 when down
//
// This actually just uses Scale<F, Int<32768>, Int<0>> under the hood, so
// is an example I suppose...
FUNC(InvertF, "Invert",
     Arg("Input", INT | FUNCTION)
     )

// usage: SequenceF<millis_per_bits, bits, 0b0000000000000000, ....>
// millis_per_bit: a number, millseconds spent on each bit
// bits: a number, number of bits before we loop around to the beginning
// 0b0000000000000000: 16-bit binary numbers containing the actual sequence.
//
// Returns 32768 if the current bit in the sequence is 1, 0 otherwise.
// The number of 16-bit binary numbers should be at least |bits| / 16, rounded up.
// Note that if not all bits are used within the 16-bit number.
// Example, an SOS pattern:
// SequenceF<100, 37, 0b0001010100011100, 0b0111000111000101, 0b0100000000000000>
FUNC(SequenceF, "Binary Sequence",
     Arg("Time Per Bit (ms)", INT),
     Arg("Number of Bits", INT),
     Arg("Bit Section #", BITS | VARIADIC)
)

// Usage: Sin<RPM, LOW, HIGH>
// pulses between LOW - HIGH RPM times per minute
// LOW: INTEGER (defaults to Int<0>)
// HIGH: INTEGER (defaults to Int<32768>)
// RPM: INTEGER
// return value: INTEGER
FUNC(Sin, "Sin",
     Arg("RPM", INT | FUNCTION),
     Arg("Min", INT | FUNCTION, INT(0)),
     Arg("Max", INT | FUNCTION, INT(32768))
     )

// NO DOCUMENTATION
FUNC(Saw, "Saw",
     Arg("RPM", INT | FUNCTION),
     Arg("Min", INT | FUNCTION, INT(0)),
     Arg("Max", INT | FUNCTION, INT(32768))
     )

// NO DOCUMENTATION
FUNC(PulsingF, "Pulsing",
     Arg("Time (ms)", INT | FUNCTION)
     )

// Left off at slice.h

}

