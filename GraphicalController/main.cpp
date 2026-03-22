#include <unistd.h>
#include <sys/fcntl.h>
#ifdef __linux__
#define GOBLIN_PI
#else
#define GOBLIN_DESKTOP
#endif

#include "SDL_API/SDL_API.h"

#include "goblin/GoblinUI/GoblinGUI.hpp"
#include "goblin/Goblin-Core/GoblinCore.hpp"
#include "Goblin-Core/Verify_Goblin.hpp"
#include "SDL_API/window/Window.hpp"


int main() {

    // How we transmit data to the open CR>
    gobin::GoblinCore core;
    // How we handle mouse events.
    gan::MouseState mouse;

    #ifdef GOBLIN_DESKTOP
    SDL_Init(SDL_INIT_VIDEO);                               //< If we want the desktop controller
    gan::files::set_assets_folder("assets");                //< finds the assets folder for the GUI controller.
    gobin::GoblinGUI gui("Goblin Goblin", core);         //< visual controller.
    SDL_Event e; //< event we use to determine various input types.
    #endif

    bool running = true;
    while (running) {

        #ifdef GOBLIN_DESKTOP
        // Process events.
        while (SDL_PollEvent(&e)) {
            gui.handleEvent(e);
            switch (e.type) {
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            case SDL_EVENT_WINDOW_HIDDEN:
            case SDL_EVENT_QUIT:
                running = false;
                break;
            case SDL_EVENT_MOUSE_WHEEL:
                mouse.updateMouseWheelSDLEvent(e, mouse);
            default:
                break;
            }
        }
        mouse.updateWithSDL();

        #else
        int fd = open("/dev/input/mice", O_RDONLY);
        unsigned char data[3];
        while (read(fd, data, sizeof(data)) > 0) {
            mouse.manualUpdateMouse1(mouse, data[0] & 0x1);
            mouse.manualUpdateMouse2(mouse, data[0] & 0x2);
        }
        close(fd);
        #endif



        if (!core.motors.empty()) {
            // move all motors forward or back
            if (mouse.isLeftClicked() && !mouse.isRightClicked()) {
                for (auto& m: core.motors) {
                    if (m.motor_id != 11) {
                        m.vel = 20;
                        m.mode = gobin::WHEEL_MODE;
                        m.flagForUpdate();
                    }
                }
            } else if (mouse.isRightClicked() && !mouse.isLeftClicked()) {
                for (auto& m: core.motors) {
                    if (m.motor_id != 11) {
                        m.vel = -20;
                        m.mode = gobin::WHEEL_MODE;
                        m.flagForUpdate();
                    }
                }
            } else {
                for (auto& m: core.motors) {
                    if (m.motor_id != 11) {
                        if (m.vel != 0) {
                            m.vel = 0;
                            m.flagForUpdate();
                        } else break;
                    }
                }
            }


            // turn!
            if (core.motors[4].mode != gobin::JOINT_MODE) {
                core.motors[4].mode = gobin::JOINT_MODE;
                core.motors[4].flagForUpdate();
            } else {
                if (mouse.isX1Clicked() && !mouse.isX2Clicked()) {
                    core.brain_log.record(core.motors[4].motor_id);
                    core.motors[4].pos -= 10;
                    core.motors[4].mode = gobin::JOINT_MODE;
                    if (core.motors[4].pos < 300) core.motors[4].pos = 300;
                    core.motors[4].flagForUpdate();
                } else if (mouse.isX2Clicked() && !mouse.isX1Clicked()) {
                    core.motors[4].pos += 10;
                    if (core.motors[4].pos > 1000) core.motors[4].pos = 1000;
                    core.motors[4].mode = gobin::JOINT_MODE;
                    core.motors[4].flagForUpdate();
                }
                if (mouse.isMiddleClicked()) {
                    core.motors[4].pos = 700;
                    core.motors[4].mode = gobin::JOINT_MODE;
                    core.motors[4].flagForUpdate();
                }
            }
        }

        // motor motor
        core.update();

        #ifdef GOBLIN_DESKTOP
        gui.display();  // Render the controller.
        #endif
    }


    std::cout << "~exiting program...\n";

    return 0;
}

