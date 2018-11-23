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

    connect(KWin::effects, &KWin::EffectsHandler::windowStartUserMovedResized,
        this, &WobblyWindowsEffect::slotWindowStartUserMovedResized);
    connect(KWin::effects, &KWin::EffectsHandler::windowStepUserMovedResized,
        this, &WobblyWindowsEffect::slotWindowStepUserMovedResized);
    connect(KWin::effects, &KWin::EffectsHandler::windowFinishUserMovedResized,
        this, &WobblyWindowsEffect::slotWindowFinishUserMovedResized);
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

void WobblyWindowsEffect::prePaintScreen(KWin::ScreenPrePaintData& data, int time)
{
    KWin::effects->prePaintScreen(data, time);
}

void WobblyWindowsEffect::postPaintScreen()
{
    KWin::effects->postPaintScreen();
}

void WobblyWindowsEffect::prePaintWindow(KWin::EffectWindow* w, KWin::WindowPrePaintData& data, int time)
{
    KWin::effects->prePaintWindow(w, data, time);
}

void WobblyWindowsEffect::paintWindow(KWin::EffectWindow* w, int mask, QRegion region, KWin::WindowPaintData& data)
{
    KWin::effects->paintWindow(w, mask, region, data);
}

bool WobblyWindowsEffect::isActive() const
{
    return !m_animations.isEmpty();
}

void WobblyWindowsEffect::slotWindowStartUserMovedResized(KWin::EffectWindow* w)
{
    Q_UNUSED(w)
}

void WobblyWindowsEffect::slotWindowStepUserMovedResized(KWin::EffectWindow* w, const QRect& geometry)
{
    Q_UNUSED(w)
    Q_UNUSED(geometry)
}

void WobblyWindowsEffect::slotWindowFinishUserMovedResized(KWin::EffectWindow* w)
{
    Q_UNUSED(w)
}
