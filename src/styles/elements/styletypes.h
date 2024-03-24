#pragma once
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * styles/elements/styletypes.cpp
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

namespace Style::Wrapper {

#define STYLETYPE(name, humanName, ...) ELEM(name, humanName, STYLETYPE, __VA_ARGS__)

// NEED A BETTER HUMAN NAME for all these tbh

STYLETYPE(StylePtr, "StylePtr",
          Arg("Style Contents", COLOR)
          )

STYLETYPE(ChargingStylePtr, "ChargingStylePtr",
          Arg("Style Contents", COLOR)
          )

STYLETYPE(NormalPtr, "StyleNormalPtr",
          Arg("Base Color", COLOR),
          Arg("Clash Color", COLOR),
          Arg("Out Time (ms)", INT),
          Arg("In Time (ms)", INT),
          Arg("Lockup Color", COLOR, Color::WHITE),
          Arg("Blast Color", COLOR, Color::WHITE)
          )

STYLETYPE(NormalPtrX, "StyleNormalPtrX",
          Arg("Base Color", COLOR),
          Arg("Clash Color", COLOR),
          Arg("Out Time (ms)", INT | FUNCTION),
          Arg("In Time (ms)", INT | FUNCTION),
          Arg("Lockup Color", COLOR, Color::WHITE),
          Arg("Blast Color", COLOR, Color::WHITE)
          )

STYLETYPE(RainbowPtr, "StyleRainbowPtr",
          Arg("Out Time (ms)", INT),
          Arg("In Time (ms)", INT),
          Arg("Clash Color", COLOR, Color::WHITE),
          Arg("Lockup Color", COLOR, Color::WHITE)
          )

STYLETYPE(RainbowPtrX, "StyleRainbowPtrX",
          Arg("Out Time (ms)", INT | FUNCTION),
          Arg("In Time (ms)", INT | FUNCTION),
          Arg("Clash Color", COLOR, Color::WHITE),
          Arg("Lockup Color", COLOR, Color::WHITE)
          )

STYLETYPE(StrobePtr, "StyleStrobePtr",
          Arg("Strobe Color", COLOR),
          Arg("Clash Color", COLOR),
          Arg("Frequency", INT),
          Arg("Out Time (ms)", INT),
          Arg("In Time (ms)", INT)
          )

#undef STYLETYPE
}
