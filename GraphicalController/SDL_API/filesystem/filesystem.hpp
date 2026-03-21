#pragma once

#include "path.hpp"

namespace gan {

    class Engine;

    /// Thin wrapper around the filesystem to allow for easy management
    /// of
    class files {
        /// Initializes the filesystem with the current path.
        static void init_filesystem() {
            internal_path_obj::working_directory = (std::filesystem::current_path());
        }

        friend class Engine;


    public:
        /// constructor that initializes the filesystem
        files() { init_filesystem();}

        /// Sets the assets folder of the engine
        static void set_assets_folder(const char path[]) {
            internal_path_obj::assets_folder = (path);
            if (!std::filesystem::exists(internal_path_obj::assets_folder) || !std::filesystem::is_directory(internal_path_obj::assets_folder)) {
                internal_path_obj::assets_folder = ".."/internal_path_obj::assets_folder;
                if (!std::filesystem::exists(internal_path_obj::assets_folder) || !std::filesystem::is_directory(internal_path_obj::assets_folder)) {
                    err::panic("files::set_assets_folder(const char path[])", " Unable to locate assets folder '", internal_path_obj::assets_folder.c_str(), "'.");
                }
            }
        }

        /// Gets the assets folder of the filesystem
        static path assets() {
            return path(internal_path_obj::assets_folder);
        }

        /// Gets the working directory of the filesystem
        static path working_dir() {
            return path(internal_path_obj::working_directory);
        }

    };

    /// Gets a subdirectory of the assets folder
    inline path dir(const char path[]) {
        const auto len = strlen(path);
        for (size_t i = 0; i < len; i++) {
            if (path[i] == '/' || path[i] == '\\') {
                err::panic("core::dir(const char path[])"
                    , "Cannot initialize a path with '\\' or '/'. Please use the notation:\n"
                       "\tgan::dir(\"directory\")/\"subdir\"/\"file.ext\"");
            }
        }
        return gan::path(std::filesystem::path(path));
    }

    inline files internal_filesystem_setup;

}
