#pragma once

#include "../../../serialization/command.h"
#include "../../../serialization/packets.h"

/* Created by Gordie Novak on 3/18/26.
 * Purpose: 
 * specifies printing commands for open CR board */

#define CR_PRINT(msg) \
    auto data = gobin::bitcast(msg); \
    Serial.write(data.bits, data.size());

#define CR_EXIT while(1)

inline void CRError(const uint16_t error_code, const uint8_t error_info) {
    const auto data = gobin::bitcast(
        gobin::Command{gobin::COM_ERROR},
        gobin::Error{error_code, error_info});
    Serial.write(data.bits, gobin::com::Data<12>::size());
}


