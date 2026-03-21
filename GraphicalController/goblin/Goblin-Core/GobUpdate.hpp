#pragma once

#include "gobdef.h"
#include <SDL3/SDL.h>

namespace gobin {

    class GobUpdate {
    private:
        u32 up0_25 = 0; //< update every .25 seconds
        u32 up0_50 = 0; //< update every .5 seconds
        u32 up1_00 = 0; //< update every second
        u32 up5_00 = 0; //< update every 5 seconds
        u32 last_tick = 0;
        bool editable = false;

        void tick() {
            const u32 new_tick = SDL_GetTicks();
            const u32 tick = new_tick - last_tick;
            last_tick = new_tick;
            up0_25 += tick;
            up0_50+= tick;
            up1_00 += tick;
            up5_00 += tick;
            if (up0_25 > 250) {
                t250ms = true;
                up0_25 -= 250;
            } else t250ms = false;
            if (up0_50> 500) {
                t500ms = true;
                up0_50-= 500;
            } else t500ms = false;
            if (up1_00 > 1000) {
                t1s = true;
                up1_00 -= 1000;
            } else t1s = false;
            if (up5_00 > 5000) {
                up5_00 -= 5000;
                t5s = true;
            } else t5s = false;
        }
        friend class GoblinBrain;

        void editToggle(bool b) {
            editable = b;
        }
    public:
        bool t250ms = false;
        bool t500ms = false;
        bool t1s = false;
        bool t5s = false;

        [[nodiscard]] bool edit() const {
            return editable;
        }
    };
}
