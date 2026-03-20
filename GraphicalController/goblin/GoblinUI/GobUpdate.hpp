#pragma once
#include "gobdef.h"

namespace gobin {

    class GobUpdate {
    private:
        u32 up1 = 0;
        u32 up5 = 0;
        u32 up10 = 0;

        void tick() {
            u32 tick = SDL_GetTicks();
            up1 += tick;
            up5 += tick;
            up10 += tick;
            if (up1 > 10000) {
                update100 = true;
                up1 -= 10000;
            } else update100 = false;
            if (up5 > 50000) {
                update500 = true;
                up5 -= 50000;
            } else update500 = false;
            if (up10 > 100000) {
                update1000 = true;
                up10 -= 100000;
            } else update1000 = false;
        }
        friend class GoblinController;
    public:
        bool update100 = false;
        bool update500 = false;
        bool update1000 = false;
    };
}
