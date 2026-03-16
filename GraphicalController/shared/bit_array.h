#pragma once

/* Created by Gordie Novak on 3/16/26.
 * Purpose: 
 * Just a simple bit array*/

namespace goblin {
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
        void write(T* src, unsigned int index = 0) {
            *reinterpret_cast<T*>(bits + index) = *src;
        }
    };

} // namespace goblin
