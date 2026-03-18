#pragma once

/* Created by Gordie Novak on 3/16/26.
 * Purpose: 
 */

#include<memory>
#include <bitset>
#include <array>
#include <cstddef>
#include <stdexcept>

namespace gobin {

    struct dataPair {
        Command command;
        uint8_t* data;
        size_t size;
    };

    template <size_t slots, size_t max_bits = 100>
    class bit_buffer_c {
    public:
        using bit_arr = std::pair<uint8_t[max_bits], size_t>;
    private:
        std::array<bit_arr, slots> buffer;
        std::size_t head;   // next write position
        std::size_t cur_size;   // number of valid elements
    public:

        bit_buffer_c()
            : head(0), cur_size(0) {}

        void push(const uint8_t* data, std::size_t size) {
            if (size > max_bits)
                throw std::runtime_error("bit_buffer_c overflow");

            memcpy(buffer[head].first, data, size);
            buffer[head].second = size;

            head = (head + 1) % slots;

            if (cur_size < slots)
                ++cur_size;
        }

        dataPair pop() {
            if (cur_size == 0)
                throw std::out_of_range("bit_buffer_c empty");

            std::size_t tail = (head + slots - cur_size) % slots;

            auto& slot = buffer[tail];

            dataPair ret;
            memcpy(&ret.command, slot.first, sizeof(Command));

            ret.data = slot.first + sizeof(Command);
            ret.size = slot.second;

            --cur_size;

            return ret;
        }

        std::size_t size() const { return cur_size; }
        static constexpr std::size_t capacity() { return slots; }

        bool empty() const { return cur_size == 0; }
        bool full()  const { return cur_size == slots; }

    };
}
