#pragma once

#include "../types/fstring.hpp"
#include <type_traits>

// made by gordie feb 20th

namespace gan {

    namespace err {
        /// Enum representing whether something was ok, or something errored
        enum Errcode {
            ERR = 0, PANIC = 0,
            OK = 1, SUCCESS = 1
        };
    }

    /// Holds the error buffer and number of functions called before the program is meant to error.
    /// Contains an internal "call_chain" num (number of functions called) and a character buffer of size 1024.
    class internal_log_handler {
    public:
        constexpr static uint16_t buff_size = 1024;
    private:
        inline static fstring<buff_size> error_buffer;   ///< "Fixed String" of size 1024 that contains the errors.
        inline static uint16_t call_chain;          ///< Number of functions called.

        // friends
        friend inline const char* GAN_GetLog();           //
        friend inline void GAN_ClearLog();                //
        friend inline fstring<buff_size>& GAN_RawAppendLog();  //
        // friends


        static const char* getError_c_str() {
            return error_buffer.c_str();
        }

        static fstring<buff_size>& get_raw_buffer() {
            return error_buffer;
        }

    public:

        // deleted constructors
        internal_log_handler(const internal_log_handler&) = delete;             //
        internal_log_handler& operator=(const internal_log_handler&) = delete;  //
        internal_log_handler(const internal_log_handler&&) = delete;            //
        internal_log_handler& operator=(const internal_log_handler&&) = delete; //
        // deleted constructors

        static void write_log(const char err[]) {
            error_buffer = err;
        }

        static uint16_t get_call_chain_count() {
            return call_chain;
        }

        static void increment_call_chain_count() {
            call_chain++;
        }

        // ReSharper disable once CppDFAConstantFunctionResult
        static bool has_error() {
            return !error_buffer.empty();
        }

        static void clear_call_chain_count() {
            call_chain = 0;
        }
    };

    /// Clears the current error in the error buffer & resets the call chain.
    inline void GAN_ClearLog() {
        internal_log_handler::error_buffer.clear();
        internal_log_handler::call_chain = 0;
    }

    /// Gets the raw underlying fstring of the error buffer.
    inline fstring<internal_log_handler::buff_size>& GAN_RawAppendLog() {
        return internal_log_handler::get_raw_buffer();
    }

    /// Appends an error to the current buffer, increasing the call chain count by one.
    template<typename... Args>
    void GAN_AppendLog(const char src[], Args&&... args) {
        auto& buffer = GAN_RawAppendLog();
        auto chain = internal_log_handler::get_call_chain_count();
        if (chain) {
            buffer << "\n::+" << chain << " \t=> {" << src << "}\n";
        }
        else {
            buffer << "::src" << " \t=> {" << src << "}\n";
        }
        (..., (buffer << args));
        internal_log_handler::increment_call_chain_count();
    }

    template<typename T>
    concept GAN_can_error =
        std::is_convertible_v<T, const char*>
        || std::is_fundamental_v<T>
        || std::is_convertible_v<T, unsigned>
        || std::is_unsigned_v<T>;
    /// Writes an error to the error buffer, overwriting the current error & resetting the call chain count.
    template<typename... Args>
        requires (GAN_can_error<Args> && ...)
    void GAN_WriteLog(const char src[], Args&&... args) {
        internal_log_handler::clear_call_chain_count();

        internal_log_handler::write_log("::src \t=> {");

        GAN_RawAppendLog() << src << "}\n";
        (..., (GAN_RawAppendLog() << args));
        GAN_RawAppendLog() << "\n";

        internal_log_handler::increment_call_chain_count();
    }

    /// Gets the current error stored in the error buffer. If no error is stored, checks OpenGL for an error.
    /// Otherwise returns blank.
    inline const char* GAN_GetLog() {
        if (internal_log_handler::has_error()) {
            return internal_log_handler::getError_c_str();
        }

        return "";
    }
}