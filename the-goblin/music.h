#pragma once

/* Created by Gordie Novak on 3/20/26.
 * Purpose: 
 */

#include <DynamixelWorkbench.h>


namespace music {
    constexpr int C3 = 262;
    constexpr int Cs3 = 277;
    constexpr int D3 = 294;
    constexpr int Ds3 = 311;
    constexpr int E3 = 330;
    constexpr int F3 = 349;
    constexpr int Fs3 = 370;
    constexpr int G3 = 391;
    constexpr int Ab3 = 415;
    constexpr int A3 = 440;
    constexpr int Bb3 = 466;
    constexpr int B = 493;

    inline void mary_had_lamb_meat() {

        tone(0, E3, 250);
        delay(250);
        tone(0, D3, 150);
        delay(150);
        tone(0, C3, 150);
        delay(150);
        tone(0, D3, 200);
        delay(200);
        tone(0, E3, 300);
        delay(300);
        tone(0, E3, 300);
        delay(300);
        tone(0, E3, 300);
        delay(300);
    }
}