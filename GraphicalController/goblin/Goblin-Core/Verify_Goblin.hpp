#pragma once
#include <fstream>
#include <filesystem>
#include "GoblinCore.hpp"

/* Created by Gordie Novak on 3/18/26.
 * Purpose: 
 */

namespace gobin {

    namespace verify {

        inline bool isPort(const char path[], gobin::GoblinLog& log) {
            if (!std::filesystem::exists(path)) {
                log.record("ERR: the path '", path, "' does not exist.");
            } else if (std::filesystem::is_character_file(path)) {
                return true;
            } else if (std::filesystem::is_regular_file(path)) {
                #ifdef __WIN32__
                return true;
                #else
                log.record("ERR: '", path, "' is a regular file, not a serial port.");
                #endif
            }else if (std::filesystem::is_directory(path)) {
                log.record("ERR: '", path, "' is a directory, not a serial port.");
            } else if (std::filesystem::is_symlink(path)) {
                log.record("ERR: '", path, "' is a symlink, not a serial port.");
            } else if (std::filesystem::is_socket(path)) {
                log.record("ERR: '", path, "' is a socket, not a port.");
            }  else {
                log.record("ERR: '", path, "' is not a valid serial port file.");
            }
            return false;
        }

    } // namespace verify

    namespace data {

        constexpr char configFile[] = "gob-config.inl";

        inline bool convertToBaudrate(const std::string& baud_str, u64* baudrate, gobin::GoblinLog& log) {
            try { *baudrate = std::stoull(baud_str); }
            catch (std::exception&) {
                log.record("Unable to convert ", baud_str.c_str(), " to u64.");
                return false;
            }
            return true;
        }

        inline bool getConfigData(std::string* port, u64* baudrate, gobin::GoblinLog& log) {
            if (std::filesystem::exists(configFile)) {
                std::ifstream file(configFile);

                if (!file.is_open()) {
                    log.record("Unable to open config file for reading ", configFile, ".");
                    return false;
                }

                std::string baud_str;
                std::string port_str;

                std::getline(file, port_str);
                std::getline(file , baud_str);

                *port = port_str;

                return convertToBaudrate(baud_str, baudrate, log);
            }
            return false;
        }

        inline bool writeConfigData(const std::string& port, u64 baudrate, gobin::GoblinLog& log) {
            std::ofstream o_file(configFile);
            if (!o_file.is_open()) {
                log.record("Unable to open config file for writing ", configFile, ".");
                return false;
            }
            o_file << port << std::endl << baudrate << std::endl;
            o_file.close();
            return true;
        }
    }
}