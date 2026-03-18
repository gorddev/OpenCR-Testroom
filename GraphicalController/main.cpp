#include <iostream>
#include <SDL3/SDL.h>

#include "goblin/GoblinUI/GoblinController.hpp"
#include "goblin/Goblin-Core/GoblinBrain.hpp"
#include "SDL_API/window/Window.hpp"


int main() {
    SDL_Init(SDL_INIT_VIDEO);
    std::cout << "Hello, World!" << std::endl;

    SDL_Event e;


    //const gan::Window window2("new din", {500,400}, gan::WindowResizable);

    gobin::GoblinBrain core;

    gobin::GoblinController controller("Goblin Goblin", core);


    bool running = true;
    while (running) {
        // 1. Process Events
        while (SDL_PollEvent(&e)) {
            controller.handleEvent(e);
            if (e.type == SDL_EVENT_QUIT) running = false;
        }

        core.update();


        // 2. render the controller
        controller.display();
    }

    return 0;
}
