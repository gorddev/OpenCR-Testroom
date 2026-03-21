#define GOBLIN_DESKTOP
// #define GOBLIN_PI

#include "SDL_API/SDL_API.h"

#include "goblin/GoblinUI/GoblinGUI.hpp"
#include "goblin/Goblin-Core/GoblinBrain.hpp"
#include "Goblin-Core/Verify_Goblin.hpp"
#include "SDL_API/window/Window.hpp"


int main() {

    gan::files::set_assets_folder("assets");

    gobin::GoblinBrain core;
    SDL_Init(SDL_INIT_VIDEO);
    gobin::GoblinGUI controller("Goblin Goblin", core);

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

