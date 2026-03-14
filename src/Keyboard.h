#pragma once

namespace keys {

    static struct Keyboard {
        bool k_map[26];

        // updates the given character to be true.
        void set(char c) {
            if (c >= 'a' && c <= 'z') {
                k_map[c - 'a'] = true;
            }
        }
        // returns true if the key is held down
        bool isDown(char c) const {
            if (c >= 'a' && c <= 'z') {
                return k_map[c - 'a'];
            } if (c >= 'A' && c <= 'Z') {
                return k_map[c - 'A'];
            }
            return false;
        }
        // flushes all keys to false
        void flush() {
            for (int i = 0; i < 26; i++) {
                k_map[i] = false;
            }
        }
    } key_b;

    // updates the keyboard with any given keypresses.
    inline void read() {
        while (Serial.available() > 0) {
            const char c = static_cast<char>(Serial.read()); // Read the character
            if (c >= 'A' && c <= 'Z') {
                key_b.k_map[c - 'A'] = !key_b.k_map[c - 'A'];
            } else if (c >= 'a' && c <= 'z') {
                key_b.k_map[c - 'a'] = !key_b.k_map[c - 'a'];
            }
        }
    }

    // Clears all the keys in the keyboard
    inline void flush() {
        key_b.flush();
    }

    // Returns true if the given key is held down
    inline bool is_held(const char c) {
        return key_b.isDown(c);
    }
}
