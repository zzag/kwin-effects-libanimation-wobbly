/*
 * Copyright (C) 2018 Vlad Zagorodniy <vladzzag@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// own
#include "WobblyWindowsEffect.h"

// auto-generated
#include "WobblyWindowsConfig.h"

WobblyWindowsEffect::WobblyWindowsEffect()
{
    reconfigure(ReconfigureAll);
}

WobblyWindowsEffect::~WobblyWindowsEffect()
{
}

void WobblyWindowsEffect::reconfigure(ReconfigureFlags flags)
{
    Q_UNUSED(flags)

    WobblyWindowsConfig::self()->read();

    m_gridResolution = WobblyWindowsConfig::gridResolution();
    m_springConstant = WobblyWindowsConfig::springConstant();
    m_friction = WobblyWindowsConfig::friction();
    m_maximumRange = WobblyWindowsConfig::maximumRange();
}
