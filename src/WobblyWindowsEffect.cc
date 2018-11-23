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
    connect(KWin::effects, &KWin::EffectsHandler::windowDeleted,
        this, &WobblyWindowsEffect::slotWindowDeleted);
}

WobblyWindowsEffect::~WobblyWindowsEffect()
{
}

void WobblyWindowsEffect::reconfigure(ReconfigureFlags flags)
{
    Q_UNUSED(flags)

    WobblyWindowsConfig::self()->read();

    m_gridResolution = WobblyWindowsConfig::gridResolution();
    m_settings.springConstant = WobblyWindowsConfig::springConstant();
    m_settings.friction = WobblyWindowsConfig::friction();
    m_settings.maximumRange = WobblyWindowsConfig::maximumRange();
}

void WobblyWindowsEffect::prePaintScreen(KWin::ScreenPrePaintData& data, int time)
{
    for (AnimationData& data : m_animations) {
        if (!data.stable) {
            data.stable = !data.model->Step(qMin(time, 16));
        }
    }

    KWin::effects->prePaintScreen(data, time);
}

void WobblyWindowsEffect::postPaintScreen()
{
    auto animationIt = m_animations.begin();
    while (animationIt != m_animations.end()) {
        if ((*animationIt).stable && (*animationIt).destroyable) {
            KWin::EffectWindow* w = animationIt.key();
            const QRectF geometry = w->geometry();
            const QRectF expandedGeometry = w->expandedGeometry();

            const qreal paddingX = geometry.x() - expandedGeometry.x();
            const qreal paddingY = geometry.y() - expandedGeometry.y();

            const QPointF pos = QPointF((*animationIt).model->Extremes().front().x, (*animationIt).model->Extremes().front().y);

            KWin::effects->moveWindow(w, (pos + QPointF(paddingX, paddingY)).toPoint());
            animationIt = m_animations.erase(animationIt);
        } else {
            ++animationIt;
        }
    }

    // TODO: Don't do full repaints.
    KWin::effects->addRepaintFull();

    KWin::effects->postPaintScreen();
}

void WobblyWindowsEffect::prePaintWindow(KWin::EffectWindow* w, KWin::WindowPrePaintData& data, int time)
{
    if (m_animations.contains(w)) {
        data.setTransformed();
        data.quads = data.quads.makeRegularGrid(m_gridResolution, m_gridResolution);
    }

    KWin::effects->prePaintWindow(w, data, time);
}

void WobblyWindowsEffect::paintWindow(KWin::EffectWindow* w, int mask, QRegion region, KWin::WindowPaintData& data)
{
    auto animationIt = m_animations.constFind(w);
    if (animationIt == m_animations.constEnd()) {
        KWin::effects->paintWindow(w, mask, region, data);
        return;
    }

    const QRectF geometry = w->geometry();
    const QRectF expandedGeometry = w->expandedGeometry();

    const qreal paddingX = geometry.x() - expandedGeometry.x();
    const qreal paddingY = geometry.y() - expandedGeometry.y();

    for (int i = 0; i < data.quads.count(); ++i) {
        KWin::WindowQuad& quad = data.quads[i];

        for (int j = 0; j < 4; ++j) {
            KWin::WindowVertex& vertex = quad[j];

            const qreal texcoordU = (paddingX + vertex.x()) / expandedGeometry.width();
            const qreal texcoordV = (paddingY + vertex.y()) / expandedGeometry.height();
            const wobbly::Point transformed = (*animationIt).model->DeformTexcoords(wobbly::Point(texcoordV, texcoordU));

            vertex.move(transformed.x - geometry.x(), transformed.y - geometry.y());
        }
    }

    KWin::effects->paintWindow(w, mask, region, data);
}

bool WobblyWindowsEffect::isActive() const
{
    return !m_animations.isEmpty();
}

void WobblyWindowsEffect::slotWindowStartUserMovedResized(KWin::EffectWindow* w)
{
    if (!w->isUserMove()) {
        return;
    }

    AnimationData& data = m_animations[w];

    if (!data.model) {
        const QRectF expandedGeometry = w->expandedGeometry();
        data.model.reset(new wobbly::Model(
            wobbly::Point(expandedGeometry.x(), expandedGeometry.y()),
            expandedGeometry.width(),
            expandedGeometry.height(),
            m_settings));
    }

    const QPointF grabPosition = cursorPos();

    wobbly::Anchor grabAnchor = data.model->GrabAnchor(wobbly::Point(grabPosition.x(), grabPosition.y()));
    data.grabAnchor.reset(new wobbly::Anchor(std::move(grabAnchor)));
    data.lastGrabPosition = grabPosition;
    data.stable = false;
    data.destroyable = false;

    // TODO: Don't do full repaints.
    KWin::effects->addRepaintFull();
}

void WobblyWindowsEffect::slotWindowStepUserMovedResized(KWin::EffectWindow* w, const QRect& geometry)
{
    Q_UNUSED(geometry)

    if (!w->isUserMove()) {
        return;
    }

    auto animationIt = m_animations.find(w);
    if (animationIt == m_animations.end()) {
        return;
    }

    const QPointF grabPosition = cursorPos();
    const QPointF delta = grabPosition - (*animationIt).lastGrabPosition;
    if (delta.isNull()) {
        return;
    }

    (*animationIt).grabAnchor->MoveBy(wobbly::Vector(delta.x(), delta.y()));
    (*animationIt).lastGrabPosition = grabPosition;
    (*animationIt).stable = false;

    // TODO: Don't do full repaints.
    KWin::effects->addRepaintFull();
}

void WobblyWindowsEffect::slotWindowFinishUserMovedResized(KWin::EffectWindow* w)
{
    auto animationIt = m_animations.find(w);
    if (animationIt == m_animations.end()) {
        return;
    }

    animationIt->grabAnchor.reset();
    animationIt->destroyable = true;

    // TODO: Don't do full repaints.
    KWin::effects->addRepaintFull();
}

void WobblyWindowsEffect::slotWindowDeleted(KWin::EffectWindow* w)
{
    m_animations.remove(w);
}
