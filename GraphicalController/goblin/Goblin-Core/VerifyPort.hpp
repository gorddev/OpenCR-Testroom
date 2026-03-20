#pragma once
#include <fstream>
#include <iostream>

#include "GoblinBrain.hpp"
#include "errors/gan_log.hpp"

/* Created by Gordie Novak on 3/18/26.
 * Purpose: 
 */
constexpr char configFile[] = "gob-config.inl";


inline bool verifyInput(std::string& input, uint64_t& baudrate) {
    std::getline(std::cin, input);

    if (input == ":baud") {
        std::cout   << "==| Enter Baudrate (uint64_t) => ";
        while (std::getline(std::cin, input)) {
            if (input == "exit")
                break;
            try {
                baudrate = std::stoull(input);
                std::cout << "==| Baudrate set to: " << baudrate << "\n> ";
                break;
            } catch (std::invalid_argument e) {
                std::cout << "Invalid argument. Must provide an integer. 'exit' to exit.\n> ";
            }
        }
        return verifyInput(input, baudrate);
    }
    return true;

}

inline bool verifyPathIsPort(const std::string& path) {
    if (!std::filesystem::exists(path)) {
        std::cout << "ERR: '" << path << "' does not exist.";
    } else if (std::filesystem::is_character_file(path)) {
        return true;
    } else if (std::filesystem::is_directory(path)) {
        std::cout << "ERR: '" << path << "' is a directory.";
    } else if (std::filesystem::is_regular_file(path)) {
        std::cout << "ERR: '" << path << "' is a regular file.";
    } else if (std::filesystem::is_symlink(path)) {
        std::cout << "ERR: '" << path << "' is a symlink.";
    } else if (std::filesystem::is_socket(path)) {
        std::cout << "ERR: '" << path << "' is a socket.";
    }  else {
        std::cout << "ERR: '" << path << "' is not a character file.";
    }
    return false;
}

inline std::optional<gobin::GoblinBrain> verifyPort() {

    std::string port, input;
    uint64_t baudrate = 1000000;
    std::optional<gobin::GoblinBrain> goblin;

    if (std::filesystem::exists(configFile)) {
        std::ifstream file(configFile);
        std::getline(file, port);
        std::getline(file, input);
        try { baudrate = std::stoull(input); } catch (std::invalid_argument) {}
        file.close();

        if (!port.empty()) {
            std::cout   << "==| Cached serial file in ./gob-config.inl › '" << port << "' "
                        << (std::filesystem::exists(port) ? "(FOUND)\n" : "(NOT FOUND)\n")
                        << "==| Baudrate = " << baudrate << ". Call ':baud' to change.\n"
                        << " -› Press ENTER to use: '" << port  << "', or specify path manually.\n"
                            "> ";
            verifyInput(input, baudrate);
            if (input.empty()) {
                input = port;
                goto jump_verifyPort_gob;
            }
            goto jump_verifyPort_begin_while;
        }
    }

    std::cout << "==| Baudrate = " << baudrate << ". Call ':baud' to change.\n";
    std::cout << "==| Specify path to serial object:\n> ";

    while (verifyInput(input, baudrate)) {
jump_verifyPort_begin_while:
        if (input == "exit")
            return std::nullopt;
        if (input.empty()) {
            std::cout << "> ";
            continue;
        }
        if (input == "err") {
            std::cout << port << "\n> ";
            continue;
        }
        if (!verifyPathIsPort(input)) {
            std::cout << " Try again or 'exit' to exit\n> ";
            continue;
        }

jump_verifyPort_gob:
        goblin = gobin::GoblinBrain::make(input.c_str(), baudrate);

        if (!goblin) {
            std::cout << "Goblin creation failed. Type 'err' for error log, or try again:\n> ";
            port = gan::GAN_GetLog();
            gan::GAN_ClearLog();
            continue;
        }

        std::ofstream ofile(configFile);
        ofile << input << '\n' << baudrate << '\n';
        ofile.close();
        return goblin;
    }
}