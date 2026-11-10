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

#ifndef LIBTAS_POINTERCALIBRATION_H_INCLUDED
#define LIBTAS_POINTERCALIBRATION_H_INCLUDED

#include <chrono>

/* Converts between the coordinates of the compositor and the ones of a window,
 * which no compositor needs to give us: while the pointer is on the window, we
 * know the position in both, as Xwayland reports it. This keeps working if the
 * window moves. Each axis is `window = scale * compositor + offset`, as the scale
 * of Xwayland can differ and compositors disagree on the direction of the axes of
 * a flipped output. */
class PointerCalibration {
public:
    /* The pointer is on the window, with these positions */
    void update(int compositor_x, int compositor_y, int window_x, int window_y);

    /* Xwayland can keep saying that the pointer is on the window after it left.
     * Returns if its position looks stale, which is when the compositor sees the
     * pointer move and Xwayland does not, for a few reads. Must be called once
     * per read. */
    bool isStale(int compositor_x, int compositor_y, int window_x, int window_y);

    /* The pointer is not on the window */
    void left() { tracking = false; }

    /* Get the window position from a compositor position, if we can tell */
    bool toWindow(int compositor_x, int compositor_y, int& window_x, int& window_y) const;

    void reset();

private:
    struct Axis {
        double scale = 1.0;
        double offset = 0.0;

        /* Position of the last sample, used to measure the scale */
        bool has_anchor = false;
        int anchor_compositor = 0;
        int anchor_window = 0;

        /* A scale needs to be measured twice before being used */
        bool has_candidate = false;
        double candidate = 0.0;

        void update(int compositor, int window);
    };

    Axis x_axis, y_axis;
    bool calibrated = false;

    /* Xwayland and the compositor don't update at the same time, and a screencast
     * only gives positions with its frames, so only trust samples taken after
     * the pointer has not moved for a while. */
    std::chrono::steady_clock::time_point last_change;
    bool has_last = false;
    int last_compositor_x = 0, last_compositor_y = 0;
    int last_window_x = 0, last_window_y = 0;

    /* Before any motion, Xwayland gives a position that means nothing, and it says
     * that the pointer is on the window at that place. Only trust it after seeing
     * it move. */
    bool tracking = false;

    /* Last positions that were used as a sample */
    int sample_compositor_x = 0, sample_compositor_y = 0;
    int sample_window_x = 0, sample_window_y = 0;

    /* If the last sample was taken while the pointer was at rest */
    bool sample_stable = false;

    /* For isStale(): the positions when both last agreed */
    bool has_synced = false;
    int synced_compositor_x = 0, synced_compositor_y = 0;
    int synced_window_x = 0, synced_window_y = 0;
    int reads_behind = 0;
};

#endif
