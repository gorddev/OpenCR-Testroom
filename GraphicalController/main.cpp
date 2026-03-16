#include <iostream>
#include <SDL3/SDL.h>

#include "goblin/GoblinUI/GoblinController.hpp"
#include "goblin/Goblin-Core/GoblinCore.hpp"
#include "SDL_API/window/Window.hpp"


int main() {
    SDL_Init(SDL_INIT_VIDEO);
    std::cout << "Hello, World!" << std::endl;

    SDL_Event e;


    const gan::Window window2("new din", {500,400}, gan::WindowResizable);

    GoblinCore core;

    goblin::GoblinController controller("Goblin Goblin", core);


    bool running = true;
    while (running) {
        // 1. Process Events
        while (SDL_PollEvent(&e)) {
            controller.handleEvent(e);
            std::cerr << "polling event" << std::endl;
            if (e.type == SDL_EVENT_QUIT) running = false;
        }

        std::string s = SDL_GetError();
        if (!s.empty()) {
            std::cerr << s << std::endl;
        }
        auto err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr <<"gler: " << err << std::endl;
        }
        printf("Video driver: %s\n", SDL_GetCurrentVideoDriver());


        // 2. render the controller
        controller.display();
    }

    return 0;
}
