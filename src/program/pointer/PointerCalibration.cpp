/*
    Copyright 2015-2026 Clément Gallet <clement.gallet@ens-lyon.org>

    This file is part of libTAS.

    libTAS is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libTAS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libTAS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PointerCalibration.h"

#include <cmath>
#include <cstdlib>

/* Don't measure a scale over a shorter distance, rounding would make it wrong */
static const int MIN_SCALE_DISTANCE = 64;

void PointerCalibration::Axis::update(int compositor, int window)
{
    if (has_anchor && (std::abs(compositor - anchor_compositor) >= MIN_SCALE_DISTANCE)) {
        double measured = static_cast<double>(window - anchor_window) / (compositor - anchor_compositor);

        /* Snap the rounding errors of the usual case */
        if (std::abs(std::abs(measured) - 1.0) < 0.03)
            measured = (measured > 0) ? 1.0 : -1.0;

        /* The window may have moved between both samples, which gives any
         * value: only accept what makes sense, and is measured twice. */
        double magnitude = std::abs(measured);
        if ((magnitude >= 0.2) && (magnitude <= 5.0)) {
            if (has_candidate && (std::abs(candidate - measured) < 0.03))
                scale = measured;
            candidate = measured;
            has_candidate = true;
        }
        anchor_compositor = compositor;
        anchor_window = window;
    }
    else if (!has_anchor) {
        has_anchor = true;
        anchor_compositor = compositor;
        anchor_window = window;
    }

    offset = window - scale * compositor;
}

void PointerCalibration::update(int compositor_x, int compositor_y, int window_x, int window_y)
{
    /* Time that the offset between both positions must stay the same for a
     * sample to be trusted */
    static const auto STABLE_TIME = std::chrono::milliseconds(100);

    auto now = std::chrono::steady_clock::now();

    if (has_last && ((window_x != last_window_x) || (window_y != last_window_y)))
        tracking = true;

    /* Xwayland and the compositor don't update at the same time, so a pair of
     * positions may be from different times while the pointer moves. Then the
     * offset that they give jumps. When it stays the same, either the pointer
     * rests or both follow it, and the pair can be trusted. This also measures
     * again a window that moved or was resized, without the pointer having to
     * rest on it. */
    double implied_x = window_x - x_axis.scale * compositor_x;
    double implied_y = window_y - y_axis.scale * compositor_y;
    if (!has_last ||
        (std::abs(implied_x - last_implied_x) > 1.0) || (std::abs(implied_y - last_implied_y) > 1.0)) {
        last_change = now;
        last_implied_x = implied_x;
        last_implied_y = implied_y;
    }

    last_compositor_x = compositor_x;
    last_compositor_y = compositor_y;
    last_window_x = window_x;
    last_window_y = window_y;
    has_last = true;

    if (!tracking)
        return;

    bool stable = (now - last_change) >= STABLE_TIME;

    /* A first sample is still better than nothing if the pointer never stops
     * before leaving the window, but it may pair positions of different times,
     * so any stable sample replaces it. */
    if (!stable && calibrated)
        return;

    /* If only the compositor saw the pointer move since the last stable sample,
     * then Xwayland did not follow it, and these positions are unrelated. */
    if (sample_stable &&
        ((compositor_x != sample_compositor_x) || (compositor_y != sample_compositor_y)) &&
        (window_x == sample_window_x) && (window_y == sample_window_y))
        return;

    /* Don't measure a scale from a sample that may be wrong */
    if (!sample_stable && calibrated) {
        x_axis.has_anchor = false;
        y_axis.has_anchor = false;
    }

    x_axis.update(compositor_x, window_x);
    y_axis.update(compositor_y, window_y);
    sample_compositor_x = compositor_x;
    sample_compositor_y = compositor_y;
    sample_window_x = window_x;
    sample_window_y = window_y;
    sample_stable = stable;
    calibrated = true;
}

bool PointerCalibration::isStale(int compositor_x, int compositor_y, int window_x, int window_y)
{
    /* Xwayland and the compositor can each be a frame or two ahead of the other */
    static const int STALE_READS = 3;

    if (!has_synced) {
        has_synced = true;
        synced_compositor_x = compositor_x;
        synced_compositor_y = compositor_y;
        synced_window_x = window_x;
        synced_window_y = window_y;
        reads_behind = 0;
        return false;
    }

    /* Both are in sync if, on each axis, they both moved or both did not move
     * since they last were. Only look at that, as the scale may not be known
     * yet, and an axis may be flipped. */
    bool compositor_moved_x = (compositor_x != synced_compositor_x);
    bool compositor_moved_y = (compositor_y != synced_compositor_y);
    bool window_moved_x = (window_x != synced_window_x);
    bool window_moved_y = (window_y != synced_window_y);

    if ((compositor_moved_x == window_moved_x) && (compositor_moved_y == window_moved_y)) {
        synced_compositor_x = compositor_x;
        synced_compositor_y = compositor_y;
        synced_window_x = window_x;
        synced_window_y = window_y;
        reads_behind = 0;
        return false;
    }

    /* Xwayland only moving is a delay of the compositor, not a stale position,
     * but the compositor moving alone for a few reads means that Xwayland lost
     * the pointer. */
    if ((compositor_moved_x && !window_moved_x) || (compositor_moved_y && !window_moved_y))
        reads_behind++;
    return reads_behind >= STALE_READS;
}

bool PointerCalibration::toWindow(int compositor_x, int compositor_y, int& window_x, int& window_y) const
{
    if (!calibrated)
        return false;

    window_x = static_cast<int>(std::lround(x_axis.scale * compositor_x + x_axis.offset));
    window_y = static_cast<int>(std::lround(y_axis.scale * compositor_y + y_axis.offset));
    return true;
}

void PointerCalibration::reset()
{
    *this = PointerCalibration();
}
