#pragma once
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * styles/elements/wrappers.cpp
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

ELEM(StylePtr, "StylePtr", Type::WRAPPER,
     Arg("Style Contents", Type::COLOR)
     )

ELEM(ChargingPtr, "ChargingStylePtr", Type::WRAPPER,
     Arg("Style Contents", Type::COLOR)
     )

ELEM(NormalPtr, "StyleNormalPtr", Type::WRAPPER,
     Arg("Base Color", Type::COLOR),
     Arg("Clash Color", Type::COLOR),
     Arg("Out Time (ms)", Type::RAWINT),
     Arg("In Time (ms)", Type::RAWINT),
     Arg("Lockup Color", Type::COLOR, Color::WHITE),
     Arg("Blast Color", Type::COLOR, Color::WHITE)
     )

ELEM(NormalPtrX, "StyleNormalPtrX", Type::WRAPPER,
     Arg("Base Color", Type::COLOR),
     Arg("Clash Color", Type::COLOR),
     Arg("Out Time (ms)", Type::INT),
     Arg("In Time (ms)", Type::INT),
     Arg("Lockup Color", Type::COLOR, Color::WHITE),
     Arg("Blast Color", Type::COLOR, Color::WHITE)
     )

ELEM(RainbowPtr, "StyleRainbowPtr", Type::WRAPPER,
     Arg("Out Time (ms)", Type::RAWINT),
     Arg("In Time (ms)", Type::RAWINT),
     Arg("Clash Color", Type::COLOR, Color::WHITE),
     Arg("Lockup Color", Type::COLOR, Color::WHITE)
     )

ELEM(RainbowPtrX, "StyleRainbowPtrX", Type::WRAPPER,
     Arg("Out Time (ms)", Type::INT),
     Arg("In Time (ms)", Type::INT),
     Arg("Clash Color", Type::COLOR, Color::WHITE),
     Arg("Lockup Color", Type::COLOR, Color::WHITE)
     )

ELEM(StrobePtr, "StyleStrobePtr", Type::WRAPPER,
     Arg("Strobe Color", Type::COLOR),
     Arg("Clash Color", Type::COLOR),
     Arg("Frequency", Type::RAWINT),
     Arg("Out Time (ms)", Type::RAWINT),
     Arg("In Time (ms)", Type::RAWINT)
     )

}
