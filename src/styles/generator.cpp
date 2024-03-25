#include "generator.h"
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * styles/generator.cpp
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
#include <memory>

#include "styles/elements/functions.h"
#include "styles/elements/styletypes.h"
#include "styles/elements/builtin.h"

std::function<std::shared_ptr<Style::Base>(void)> Style::getGenerator(const std::string_view& styleStr) {
#   define MAPENTRY(styleName) \
    { styleName::getNameStatic(), []{ return std::static_pointer_cast<Style::Base>(std::make_shared<styleName>()); } }

    static std::unordered_map<std::string_view,std::function<std::shared_ptr<Base>(void)>> styleMap{
        MAPENTRY(Wrapper::StylePtr),
        MAPENTRY(Wrapper::ChargingStylePtr),
        MAPENTRY(Wrapper::NormalPtr),
        MAPENTRY(Wrapper::NormalPtrX),
        MAPENTRY(Wrapper::RainbowPtr),
        MAPENTRY(Wrapper::RainbowPtrX),
        MAPENTRY(Wrapper::StrobePtr),
        MAPENTRY(Builtin::style_charging),
        MAPENTRY(Builtin::style_pov),
        MAPENTRY(Function::Int),
        MAPENTRY(Function::AltF),
        MAPENTRY(Function::SyncAltToVarianceF),
        MAPENTRY(Function::SyncAltToVarianceL),
        MAPENTRY(Function::BatteryLevel),
        MAPENTRY(Function::BladeAngle),
        MAPENTRY(Function::BladeAngleX),
        MAPENTRY(Function::BlastF),
        MAPENTRY(Function::BlastFadeoutF),
        MAPENTRY(Function::OriginalBlastF),
        MAPENTRY(Function::Blinking),
        MAPENTRY(Function::BrownNoiseF),
        MAPENTRY(Function::SlowNoise),
        MAPENTRY(Function::Bump),
        MAPENTRY(Function::HumpFlickerF),
        MAPENTRY(Function::HumpFlickerFX),
        MAPENTRY(Function::CenterDistF),
        MAPENTRY(Function::ChangeSlowly),
        MAPENTRY(Function::CircularSectionF),
        MAPENTRY(Function::ClampF),
        MAPENTRY(Function::ClampFX),
        MAPENTRY(Function::ClashImpactF),
        MAPENTRY(Function::ClashImpactFX),
        MAPENTRY(Function::Divide),
        MAPENTRY(Function::EffectPulse),
        MAPENTRY(Function::LockupPulseF),
        MAPENTRY(Function::IncrementWithReset),
        MAPENTRY(Function::EffectIncrementF),
        MAPENTRY(Function::EffectPosition),
        MAPENTRY(Function::HoldPeakF),
        MAPENTRY(Function::Ifon),
        MAPENTRY(Function::InOutFunc),
        MAPENTRY(Function::InOutFuncX),
        MAPENTRY(Function::InOutFuncTD),
        MAPENTRY(Function::InOutHelperF),
        MAPENTRY(Function::IncrementModuloF),
        MAPENTRY(Function::ThresholdPulseF),
        MAPENTRY(Function::IncrementF),
        MAPENTRY(Function::IntArg),
        MAPENTRY(Function::IntSelect),
        MAPENTRY(Function::IsBetween),
        MAPENTRY(Function::IsLessThan),
        MAPENTRY(Function::IsGreaterThan),
        MAPENTRY(Function::LinearSectionF),
        MAPENTRY(Function::MarbleF),
        MAPENTRY(Function::ModF),
        MAPENTRY(Function::Mult),
        MAPENTRY(Function::Percentage),
        MAPENTRY(Function::OnSparkF),
        MAPENTRY(Function::RampF),
        MAPENTRY(Function::RandomF),
        MAPENTRY(Function::RandomPerLEDF),
        MAPENTRY(Function::RandomBlinkF),
        MAPENTRY(Function::Scale),
        MAPENTRY(Function::InvertF),
        MAPENTRY(Function::SequenceF),
        MAPENTRY(Function::Sin),
        MAPENTRY(Function::Saw),
        MAPENTRY(Function::PulsingF),
    };

    auto mapIt{styleMap.find(styleStr)};
    if (mapIt == styleMap.end())
        return nullptr;
    return mapIt->second;
#   undef MAPENTRY
}
