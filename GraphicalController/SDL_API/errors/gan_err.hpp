#pragma once

#include "gan_log.hpp"
#include <iostream>



/// The err namespace
namespace gan::err {
    /// Throws an error from a specific source.
    template<typename... MSGS>
    constexpr void panic(const char src[], MSGS&&... msgs) {
        std::cerr << ("\n--------------\n!!PANIC!! Generated from: [")
            << src << "]\n--------------\n";
        if constexpr (sizeof...(MSGS) > 0)
            GAN_AppendLog(src, std::forward<MSGS>(msgs)...);
        std::cerr << GAN_GetLog() << std::endl;
        GAN_ClearLog();
        throw std::runtime_error("");
    }

    /** Throws a panic if the Errcode enum is @code err::ERR@endcode or @code err::PANIC@endcode
     * @param e The errcode you want to test
     * @param src The source of the error
     * @param reason The reason for calling the panic (if unsuccessful) */
    template<typename... MSGS>
    constexpr void panic_if(Errcode e, const char src[], MSGS&&... reason) {
        if (e) return;
        if constexpr (sizeof...(MSGS) > 0)
            GAN_AppendLog(src, std::forward<MSGS>(reason)...);
        std::cerr << GAN_GetLog() << std::endl;
        GAN_ClearLog();
        throw std::runtime_error("");
    }
    /// Unwraps an optional. If unsuccessful, throws.
    /// @param src The source of the error
    /// @param reason The reason for calling unwrap.
    template<typename T, typename... Args>
    constexpr T unwrap(std::optional<T>&& opt, const char* src, Args&&... reason) {
        if (!opt) {
            if constexpr (sizeof...(Args) == 0)
                GAN_AppendLog(src, "Found std::nullopt when unwrapping with err::unwrap(optional<T>).");
            else
                GAN_AppendLog(src, reason...);
            GAN_AppendLog("err::unwrap(optional<T>)", "Called panic on std::nullopt.");
            panic(src);
            throw std::runtime_error("");
        }
        return opt.value();
    }
}

