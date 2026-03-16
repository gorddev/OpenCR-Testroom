#pragma once

/* Created by Gordie Novak on 3/15/26. */

namespace gan {

    struct vec2 {
        union {
            struct { float x, y; };
            struct { float w, h; };
        };
    };

}
