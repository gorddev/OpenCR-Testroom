#include <iostream>
#include <SDL3/SDL.h>
#include <imgui.h>

#include "goblin/GoblinUI/GoblinController.hpp"
#include "goblin/Goblin-Core/GoblinCore.hpp"
#include "SDL_API/window/Window.hpp"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"


int main() {
    std::cout << "Hello, World!" << std::endl;

    SDL_Event e;


    const gan::Window window2("new din", {500,302}, gan::WindowResizable);

    GoblinCore core;

    goblin::GoblinController controller("Goblin Goblin", core);


    bool running = true;
    while (running) {
        // 1. Process Events
        while (SDL_PollEvent(&e)) {
            controller.handleEvent(e);
            if (e.type == SDL_EVENT_QUIT) running = false;
        }

        // 2. render the controller
        controller.display();
    }

    return 0;
}
