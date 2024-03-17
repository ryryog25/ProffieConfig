#include "state.h"
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * appcore/state.cpp
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

static AppCore::State* stateInstance{nullptr};

AppCore::State::State() {
    propfiles = { "fett263.pconf", "sa22c.pconf", "shtok.pconf", "BC.pconf", "caiwyn.pconf" };
}

void AppCore::State::init() { if (!stateInstance) stateInstance = new State(); }

const AppCore::State* AppCore::getState() { return stateInstance; }
