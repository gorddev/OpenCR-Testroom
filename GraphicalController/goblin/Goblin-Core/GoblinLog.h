#pragma once
#include "../gobdef.h"
#include "../../SDL_API/types/fstring.hpp"

int main();

namespace gobin {

    class GoblinLog {
    private:
        static constexpr u32 max_logs = 14;
        static constexpr u32 log_size = 200;
        /// Circular buffer for each of our logs.
        gan::fstring<log_size> logs[max_logs];
        /// Current log
        u32 head = 0, tail = 0;

    public:
        /// Adds a log to the goblin log.
        template<typename... Args>
        void record(Args&&... args) {
            head = (head + 1) % max_logs;
            logs[head].clear();
            (logs[head] << ... << args);
            if (head == tail) { tail = (tail + 1) % max_logs; }
        }

        GoblinLog() = default;
        /// Returns true if there are no logs to display.
        [[nodiscard]] bool empty() {
            return head == tail;
        }

        /// Returns the most recent log.
        /// @warning Unsafe. Does not throw error on crash.
        gan::fstring<log_size>& front() {
            return logs[head];
        }

        /// Returns the back log
        gan::fstring<log_size>& back() {
            return logs[(tail + max_logs - 1)%max_logs];
        }


        class iterator {
            gan::fstring<log_size>* ptr;
            u32 index = 0;
        public:
            iterator(gan::fstring<log_size>* ptr, u32 index) : ptr(ptr), index(index) {}
            const gan::fstring<log_size>& operator++(int) {
                auto* ret = (ptr + index);
                index = (index + max_logs - 1) % max_logs;
                return *ret;
            }
            iterator& operator++() {
                index = (index + max_logs - 1) % max_logs;
                return *this;
            }
            bool operator==(const iterator& other) const {
                return index == other.index;
            }
            gan::fstring<log_size>& operator*() const {
                return *(ptr + index);
            }
        };

        /// Returns the front iterator to the goblin log
        iterator begin() {
            return iterator{logs, head};
        }

        /// Returns the end iterator to the goblin log
        iterator end() {
            return iterator{logs, tail};
        }

        /// Clears out all the logs in the goblin log file.
        void clear() {
            head = tail = 0;
            logs[0].clear();
        }

    };

}
