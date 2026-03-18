
#include "SDL_API/SDL_API.h"

#include "goblin/GoblinUI/GoblinController.hpp"
#include "goblin/Goblin-Core/GoblinBrain.hpp"
#include "SDL_API/window/Window.hpp"


int main() {

    std::cout << "Hello, World!" << std::endl;

    std::cout << "Enter the entry port:\n> ";
    std::string input;
    std::getline(std::cin, input);
    std::cout << "got port: " << input << std::endl;
    gobin::port = input;

    //const gan::Window window2("new din", {500,400}, gan::WindowResizable);

    gobin::GoblinBrain* core;
    try {
        core = (new gobin::GoblinBrain());
    } catch (int) {
        std::cerr << "Could not open the port provided at " << gobin::port << std::endl;
        return 1;
    }

    SDL_Init(SDL_INIT_VIDEO);

    gobin::GoblinController controller("Goblin Goblin", *core);


    SDL_Event e;


    bool running = true;
    while (running) {
        // 1. Process Events
        while (SDL_PollEvent(&e)) {
            controller.handleEvent(e);
            if (e.type == SDL_EVENT_QUIT) running = false;
        }

        core->update();


        // 2. render the controller
        controller.display();
    }

    delete core;

    return 0;
}
