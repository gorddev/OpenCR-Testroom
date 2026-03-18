#pragma once
#include <thread>
#include <memory>
#include "serial/serial.h"

/* Created by Gordie Novak on 3/16/26.
 * Purpose: 
 */


namespace gobin {

    struct ThreadVec {
        std::vector<std::thread*> threads;
    } inline threads;

    void write_data(serial::Serial* serial, const uint8_t* data, size_t size) {

        uint8_t* heap_data = new uint8_t[size];
        memcpy(heap_data, data, size);

        auto write_data_multithread = []
            (serial::Serial* serial,
                uint8_t* data,
                size_t size)
        {
            serial->write(data, size);

        }

        std::thread thr(write_data_multithread, serial, heap_data, size);
        thr.detach();
        const char* heap_data =
    }

}
