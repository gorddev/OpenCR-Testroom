
#include <fstream>
#include <filesystem>
#include "SDL_API/SDL_API.h"

#include "goblin/GoblinUI/GoblinController.hpp"
#include "goblin/Goblin-Core/GoblinBrain.hpp"
#include "Goblin-Core/VerifyPort.hpp"
#include "SDL_API/window/Window.hpp"


int main() {

    std::cout << "===============================\n"
                 "==|    Goblin Controller    |==\n"
                 "-------------------------------\n";

    // try to get our goblin
    auto opt = verifyPort();
    if (opt == std::nullopt) {
        std::cout << "~exiting program...\n";
        return 0;
    }
    gobin::GoblinBrain core = std::move(opt.value());

    std::cout << "================================\n"
                 "==| Initializing GController |==\n"
                 "-------------------------------\n";

    SDL_Init(SDL_INIT_VIDEO);
    gobin::GoblinController controller("Goblin Goblin", core);

    SDL_Event e;

    bool running = true;
    while (running) {
        // Process events.
        while (SDL_PollEvent(&e)) {
            controller.handleEvent(e);
            if (e.type == SDL_EVENT_QUIT) running = false;
        }

        // motor motor
        core.update();

        // Render the controller.
        controller.display();
    }


    std::cout << "~exiting program...\n";

    return 0;
}

