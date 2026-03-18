#pragma once

/* Created by Gordie Novak on 3/16/26.
 * Purpose: 
 * Just a simple bit array*/

namespace gobin {
    // just an array with a predefined size
    template<unsigned int s>
    struct bit_array {

        unsigned char bits[s];

        unsigned char& operator[](unsigned int i) {
            return bits[i];
        }

        unsigned char* data() {
            return bits;
        }

        static constexpr unsigned int size() noexcept {
            return s;
        }

        template<typename T>
        void write_obj(T* src, unsigned int index = 0) {
            *reinterpret_cast<T*>(bits + index) = *src;
        }

        void write_mem(const void* src, size_t index, size_t num_bits) {
            std::memcpy(bits + index, src, num_bits);
        }

        void write_byte(uint8_t byte, size_t index) {
            bits[index] = byte;
        }
    };

} // namespace goblin
