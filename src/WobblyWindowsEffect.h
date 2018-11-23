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

#pragma once

// kwineffects
#include <kwineffects.h>

class WobblyWindowsEffect : public KWin::Effect {
    Q_OBJECT

public:
    WobblyWindowsEffect();
    ~WobblyWindowsEffect() override;

    void reconfigure(ReconfigureFlags flags) override;

    int requestedEffectChainPosition() const override;

private:
    int m_gridResolution;
    qreal m_springConstant;
    qreal m_friction;
    qreal m_maximumRange;
};

inline int WobblyWindowsEffect::requestedEffectChainPosition() const
{
    return 55;
}
