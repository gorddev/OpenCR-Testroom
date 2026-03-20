#pragma once

#include "../command/command.h"
#include "crdef.h"
#include "crerror.h"

/* Created by Gordie Novak on 3/18/26.
 * Purpose: 
 * specifies printing commands for open CR board */

constexpr u16 CR_PRINT_MAX_LEN = 150;

/* ***************************************** */

inline void CRError(const gobin::CRErrorCode error_code, const u8 error_info) {
    const auto data = gobin::com::bitcast(
        gobin::Command{gobin::COM_ERROR, gobin::T_STATUS, 0},
        gobin::Error{error_code, error_info});
    Serial.write(data.bits, data.size());
}

/* ***************************************** */

template<u16 N>
void CRPrint(const char (&msg)[N]) {
    static_assert(N < CR_PRINT_MAX_LEN, "Maximum print length exceeded");
    auto data = gobin::com::bitcast(msg);
    Serial.write(data.bits, data.size());
}

inline void CRPrint(const char msg[], const u16 len) {
    if (len > CR_PRINT_MAX_LEN) {
        CRError(gobin::PRINT_OVERFLOW, 0);
        return;
    }
    u8 buffer[CR_PRINT_MAX_LEN]{};
    gobin::com::bitpush(buffer, msg, len + sizeof(gobin::Command), {gobin::COM_STRING, gobin::T_PRINT_CONSOLE, ':'});
    Serial.write(buffer, len + gobin::com::com_size*2 - 1);
}

inline void CRPrint(const u8 msg[], const u16 len) {
    CRPrint(reinterpret_cast<const char*>(msg), len);
}

/* ***************************************** */

[[noreturn]] inline void CRExit() {
    CRPrint("CR: Exiting");
    while (true);
}

/* ***************************************** */

[[noreturn]] inline void CRPanic(const char msg[]) {
    CRPrint("!! CRPanic() !!");
    CRPrint(msg, strlen(msg));
    CRExit();
}

/* ***************************************** */
inline void CRCommand(gobin::Command c, const void* data, const u16 data_len) {
    u8 buffer[CR_PRINT_MAX_LEN]{};
    c.size = sizeof(gobin::Command) + data_len;
    memcpy(buffer, &c, sizeof(gobin::Command));
    memcpy(buffer + sizeof(gobin::Command), data, data_len);
    Serial.write(buffer, c.size);
}

template<typename T>
void CRCommand(gobin::Command c, const T& t) {
    u8 buffer[CR_PRINT_MAX_LEN]{};
    c.size = sizeof(gobin::Command) + sizeof(T);
    memcpy(buffer, &c, sizeof(gobin::Command));
    memcpy(buffer + sizeof(gobin::Command), &t, sizeof(T));
    Serial.write(buffer, c.size);
}




/// converts an integer to a constant character
inline const char* itostr(int num) {

    auto reverse = [](char* str, int length) {
        int start = 0;
        int end = length - 1;
        while (start < end) {
            char temp = str[start];
            str[start] = str[end];
            str[end] = temp;
            start++;
            end--;
        }
    };

    char buf[12];
    int i = 0;
    bool isNegative = false;
    if (num == 0) {
        buf[i++] = '0';
        buf[i] = '\0';
    } else {
        if (num < 0) {
            isNegative = true;
            num = abs(num);
        }
        while (num != 0) {
            buf[i++] = (num % 10) + '0';
            num /= 10;
        }
        if (isNegative) {
            buf[i++] = '-';
        }
        buf[i] = '\0';
        reverse(buf, i);
    }
    static char static_buf[12];
    for (int j = 0; j <= i; ++j) {
        static_buf[j] = buf[j];
    }

    return static_buf;
}