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

#include "sdltimer.h"
#include "sdldynapi.h"

#include "logging.h"
#include "DeterministicTimer.h"
#include "TimeHolder.h"
#include "checkpoint/ThreadManager.h"

#include <list>
#include <mutex>

namespace libtas {

namespace {

struct SDLTimer {
    sdl2::SDL_TimerID id;
    Uint32 interval;
    sdl2::SDL_NewTimerCallback callback;
    void *param;

    /* Deterministic time at which the callback must be called */
    TimeHolder deadline;
};

std::mutex timers_mutex;
std::list<SDLTimer> timers;
sdl2::SDL_TimerID last_timer_id = 0;

/* Timer callbacks may wait for events and run timers again */
bool processing_timers = false;

TimeHolder msToTime(Uint32 ms)
{
    return TimeHolder(ms / 1000, (ms % 1000) * 1000000);
}

}

/* Override */ sdl2::SDL_TimerID SDL_AddTimer(Uint32 interval, sdl2::SDL_NewTimerCallback callback, void *param)
{
    LOGTRACE(LCF_TIMERS | LCF_SDL, "Add SDL Timer with call after %d ms", interval);

    if (!callback)
        return 0;

    std::lock_guard<std::mutex> lock(timers_mutex);

    SDLTimer timer;
    timer.id = ++last_timer_id;
    timer.interval = interval;
    timer.callback = callback;
    timer.param = param;
    timer.deadline = TimeHolder(DeterministicTimer::get().getTicks()) + msToTime(interval);
    timers.push_back(timer);

    return timer.id;
}

/* Override */ SDL_bool SDL_RemoveTimer(sdl2::SDL_TimerID id)
{
    LOGTRACE(LCF_TIMERS | LCF_SDL, "Remove SDL Timer %d", id);

    std::lock_guard<std::mutex> lock(timers_mutex);

    for (auto it = timers.begin(); it != timers.end(); ++it) {
        if (it->id == id) {
            timers.erase(it);
            return SDL_TRUE;
        }
    }
    return SDL_FALSE;
}

/* SDL3 timers have different callback signatures, they are still run by SDL */
/* Override */ sdl3::SDL_TimerID sdl3::SDL_AddTimer(Uint32 interval, SDL_TimerCallback callback, void *userdata)
{
    LOGTRACE(LCF_TIMERS | LCF_SDL | LCF_TODO, "Add SDL Timer with call after %d ms", interval);
    return ORIG_SDL3_CALL(SDL_AddTimer, (interval, callback, userdata));
}

/* Override */ bool sdl3::SDL_RemoveTimer(SDL_TimerID id)
{
    LOGTRACE(LCF_TIMERS | LCF_SDL | LCF_TODO, "Remove SDL Timer %d", id);
    return ORIG_SDL3_CALL(SDL_RemoveTimer, (id));
}

void processSDLTimers()
{
    if (processing_timers)
        return;
    processing_timers = true;

    TimeHolder now = DeterministicTimer::get().getTicks();

    while (true) {
        SDLTimer timer;
        {
            std::lock_guard<std::mutex> lock(timers_mutex);

            /* Get the expired timer with the earliest deadline, timers with
             * the same deadline are run in the order they were added */
            auto next = timers.end();
            for (auto it = timers.begin(); it != timers.end(); ++it) {
                if (!(it->deadline > now) && (next == timers.end() || it->deadline < next->deadline))
                    next = it;
            }
            if (next == timers.end())
                break;
            timer = *next;
        }

        LOG(LL_DEBUG, LCF_TIMERS | LCF_SDL, "Run SDL Timer %d", timer.id);

        /* The lock is not held, because the callback may add or remove timers */
        Uint32 interval = timer.callback(timer.interval, timer.param);

        std::lock_guard<std::mutex> lock(timers_mutex);
        for (auto it = timers.begin(); it != timers.end(); ++it) {
            if (it->id != timer.id)
                continue;

            if (interval == 0) {
                timers.erase(it);
            }
            else {
                /* Schedule from the previous deadline and not from the
                 * current time, so that a timer running more often than the
                 * frame rate is called the right number of times */
                it->interval = interval;
                it->deadline += msToTime(interval);
            }
            break;
        }
    }

    processing_timers = false;
}

bool advanceSDLTimers()
{
    if (!ThreadManager::isMainThread())
        return false;

    {
        std::lock_guard<std::mutex> lock(timers_mutex);
        if (timers.empty())
            return false;
    }

    /* Advance time as if the game was sleeping, which may also trigger a
     * non-draw frame */
    DeterministicTimer::get().addDelay(msToTime(1));
    processSDLTimers();
    return true;
}

}
