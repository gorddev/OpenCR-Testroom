#pragma once
#ifndef OPEN_CR
#include <concepts>
#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#endif

// Determines how data will be sent over the serial connection
// to the gobin.

namespace gobin {

constexpr uint8_t COM_START = '~';
constexpr uint8_t COM_END = '\0';
  
/// Declares how the data should be used
enum ComType : uint8_t {
  COM_GET = 'G',
  COM_SET = 'V',
  COM_CHANGE = 'C',
  COM_STRING = 'S',
  COM_INFO = 'I',
  COM_ERROR = 'E'
};

// Declares where the data should target
enum ComTarget : uint8_t {
  T_PRINT_CONSOLE = 'p',
  T_MOTOR_VELOCITY = 'v',
  T_MOTOR_POSITION = 'j',
  T_STATUS = 'i'
};

// Contains additional information about the data
union ComSpecifier {
  uint8_t motor_id;
  uint8_t id;
  uint8_t info;
  uint8_t reserved;
};

namespace com {

/// The total size of a single command, contents + size + COM_END.
constexpr uint8_t com_size =
    sizeof(uint8_t) * 4 + sizeof(uint32_t) + sizeof(uint8_t);

/// The size of any given serialization of a type
template <typename T> constexpr uint32_t serial_size = sizeof(T) + com_size;

/// This is just an array of bits.
template <uint32_t N> struct Data {
  uint8_t bits[N];

  /// Size of this specific data in bytes
  [[nodiscard]] static constexpr uint8_t size() { return N; }

  /// Gets the data at the given index.
  uint8_t &operator[](uint64_t index) { return bits[index]; }

  /// Automatic conversion to an array.
  operator uint8_t *() { return bits; }
};

} // namespace com

/// The command information that will be sent over serial.
struct Command {
  static constexpr uint8_t default_spec = ':';
  uint8_t start = COM_START;
  ComType type;
  ComTarget target;
  ComSpecifier spec;
  uint32_t size;

  Command(ComType type, ComTarget target = T_PRINT_CONSOLE,
          uint8_t spec = default_spec)
      : type(type), target(target), spec(ComSpecifier{.reserved = spec}) {}
};

// turns a command into a raw buffer of data.
template <typename T>
com::Data<com::serial_size<T>> bitcast(Command c, const T &t) {
  com::Data<com::serial_size<T>> data;
  data[0] = COM_START;
  data[1] = c.type;
  data[2] = c.target;
  data[3] = c.spec.reserved;
  *reinterpret_cast<uint32_t *>(data.bits + 4) = com::serial_size<T>;
  data[data.size() - 1] = COM_END;
  memcpy(data.bits + 8, &t, sizeof(T));
  return data;
}

template <uint32_t num_bits>
com::Data<num_bits + com::com_size> bitcast(Command c, const void *raw_data) {
  com::Data<num_bits + com::com_size> data;
  data[0] = COM_START;
  data[1] = c.type;
  data[2] = c.target;
  data[3] = c.spec.reserved;
  *reinterpret_cast<uint32_t *>(data.bits + 4) = num_bits + com::com_size;
  data[data.size() - 1] = COM_END;
  memcpy(data.bits + 8, raw_data, num_bits);
  return data;
}

template <uint32_t N>
com::Data<N + com::com_size> bitcast(const char (&str)[N]) {
  com::Data<N + com::com_size> data;
  data[0] = COM_START;
  data[1] = COM_STRING;
  data[2] = T_PRINT_CONSOLE;
  data[3] = Command::default_spec;
  data[data.size() - 1] = COM_END;
  *reinterpret_cast<uint32_t *>(data.bits + 4) = N + com::com_size;
  memcpy(data.bits + 8, str, N);
  return data;
}

#ifndef OPEN_CR

#include <algorithm>

inline void display_char(const char &c) {
  switch (c) {
  case '\n':
    std::cout << "\\n";
    break;
  case '\r':
    std::cout << "\\r";
    break;
  case '\0':
    std::cout << "\\0";
    break;
  case '\t':
    std::cout << "\\t";
    break;
  case '\f':
    std::cout << "\\f";
    break;
  case '\a':
    std::cout << "\\a";
    break;
  case '\v':
    std::cout << "\\v";
    break;
  case '\b':
    std::cout << "\\b";
    break;
  default:
    std::cout << c;
    break;
  }
}

inline void bitprint(const uint8_t *raw_data, uint32_t size) {
  std::cout << "========================\n==bitread @"
            << reinterpret_cast<const void *>(raw_data) << "==\n"
            << " index\t| deci\t| char\n"
            << "————————————————————————\n";
  constexpr uint32_t col = 10;
  constexpr uint32_t row = 5;
  constexpr uint32_t max = col * row;
  uint32_t run_size = size;
  for (uint32_t r = 0; r * col < size; r += row) {
    for (uint32_t i = 0; i < col; i++) {
      for (uint32_t k = 0; i + col * (k) < std::min(run_size, max); k++) {
        std::cout << "[" << i + (r + k) * col
                  << "]\t: " << raw_data[i + (r + k) * col] + 0 << "  \t: \'";
        display_char(raw_data[i + (r + k) * col]);
        std::cout << "\'\t|  ";
      }
      std::cout << '\n';
    }
    std::cout << "————————————————————————\n";
    run_size -= max;
  }
  if (raw_data[size - 1] == '\0') {
    std::cout << "> ";
    const char *t;
    for (int i = 0; i < size; i++) {
      display_char(raw_data[i]);
    }
    std::cout << std::endl;
  }

  std::cout << "==============" << std::endl;
}

#endif

inline const Command &com_from_bits(const uint8_t *raw_data) {
  return *reinterpret_cast<const Command *>(raw_data);
}

inline const uint8_t *data_from_bits(const uint8_t *raw_data) {
  return raw_data + sizeof(Command);
}

} // namespace gobin
