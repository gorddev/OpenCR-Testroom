#pragma once
#include <filesystem>

#include "../errors/gan_err.hpp"

namespace gan {

    class internal_path_obj {
        /// Working directory of the filesystem
        inline static std::filesystem::path working_directory
            = std::filesystem::path("@null");

        /// Folder assets are contained in.
        inline static std::filesystem::path assets_folder
            = std::filesystem::path("..")/"assets";

        friend class path;
        friend class files;
    };

    class path {
    private:
        /// The internal filesystem path this represents.
        std::filesystem::path filepath;

        friend class files; ///< Filesystem is a friend class

        void verifyInputString(const char path[]) const {
            const auto len = strlen(path);
            for (size_t i = 0; i < len; i++) {
                if (path[i] == '/' || path[i] == '\\') {
                    err::panic("path::operator/()",
                        "Cannot initialize path \"", (filepath/path).c_str(), "\" with '\\' or '/'. Please use the notation:\n"
                           "\tgan::dir(\"directory\")/\"subdir\"/\"file.ext\"");
                }
            }
        }

    public:
        explicit path(std::filesystem::path filepath)
            : filepath(std::move(filepath)) {}

        explicit path(const char path[]) {
            verifyInputString(path);
            filepath = std::filesystem::path(path);
        }

        /// Add files onto the current path.
        path operator/(const char p[]) {
            verifyInputString(p);
            return path(filepath / p);
        }

        /// Add files onto the current path.
        path operator/(const gan::path& other) {
            return path(filepath / other.filepath);
        }

        path& operator=(const char path[]) {
            verifyInputString(path);
            filepath = std::filesystem::path(path);
            return *this;
        }

        /// Converts into a c_str
        [[nodiscard]] const char* c_str() const {
            return filepath.c_str();
        }

        /// Converts to a standard library filesystem object.
        [[nodiscard]] std::filesystem::path std() const {
            return filepath;
        }

        /// Expands the path into the form (assets_folder/THIS)
        [[nodiscard]] path expand() const {
            return path(internal_path_obj::assets_folder/filepath);
        }

        [[nodiscard]] bool is_regular_file() const {
            const path expanded = expand();
            if (!std::filesystem::exists(expanded.c_str())) {
                GAN_WriteLog("path::exists()",
                    "The file ", expanded.c_str(), " doesn't exist.");
                return false;
            } if (!std::filesystem::is_regular_file(expanded.c_str())) {
                GAN_WriteLog("path::exits()",
                    "The file ", expanded.c_str(), " is not a regular file.");
                return false;
            }
            return true;
        }

        /// Adds a subdirectory to the current path
        path subdir(const char path[]) const {
            return gan::path(filepath / path);
        }
    };
}
