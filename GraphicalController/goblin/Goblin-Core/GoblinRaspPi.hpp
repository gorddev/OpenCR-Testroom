#pragma once

/* Created by Gordie Novak on 3/21/26.
 * Purpose:
 */

#include "GoblinCore.hpp"

void goblin_raspberry_pi(GoblinBrain& core) {

    std::cout << "===============================\n"
                 "==|    Goblin Controller    |==\n"
                 "-------------------------------\n";

    // try to get our goblin
    auto opt = verifyPort();
    if (opt == std::nullopt) {
        std::cout << "~exiting program...\n";
        return 0;
    }

    std::cout << "================================\n"
                 "==| Initializing GController |==\n"
                 "-------------------------------\n";
}
