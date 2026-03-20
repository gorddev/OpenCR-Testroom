#pragma once
#include "gobdef.h"
#include "types/fstring.hpp"

namespace gobin {

    class GobLog {
    private:
        static constexpr u8 max_logs = 4;
        /// Circular buffer for each of our logs.
        gan::fstring<1000> logs[max_logs];
        /// Current log
        u32 front = 0, back = 0;

    public:
        GobLog() = default;

        template<typename... Args>
        void addLog(Args&&... args) {
            (logs[front] << ... << args);

        }

    };

}
