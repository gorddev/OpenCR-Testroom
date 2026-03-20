#pragma once


// Determines how data will be sent over the serial connection
// to the gobin.

namespace gobin {

/* ********************************************* */
// base command definitions

constexpr uint8_t COM_START = '~';
constexpr uint8_t COM_END = '\0';
  
/// Declares how the data should be used
enum ComType : uint8_t {
  COM_NONE = 0,
  COM_GET = 'G',
  COM_SET = 'V',
  COM_CHANGE = 'C',
  COM_STRING = 'S',
  COM_INFO = 'I',
  COM_ERROR = 'E',
  COM_ECHO = '>',
};

// Declares where the data should target
enum ComTarget : uint8_t {
  T_NONE = 0,
  T_PRINT_CONSOLE = 'p',
  T_MOTOR_VELOCITY = 'v',
  T_MOTOR_POSITION = 'x',
  T_MOTOR_JOINT_MODE = 'j',
  T_MOTOR_WHEEL_MODE = 'w',
  T_MOTOR_ID = 'i',
  T_MOTOR_LIST = 'l',
  T_STATUS = 's',
};

// Contains additional information about the data


/* ********************************************* */
/* ********************************************* */
/* ********************************************* */



/* ********************************************* */
// raw data handling

namespace com {

/// The total size of a single command, contents + size + COM_END.
constexpr uint16_t com_size =
    sizeof(uint8_t) * 4 + sizeof(uint16_t) + sizeof(uint8_t);

/// The size of any given serialization of a type
template <typename T> constexpr uint16_t serial_size = sizeof(T) + com_size;

/// This is just an array of bits.
template <uint16_t N>
struct Data {
  uint8_t bits[N];

  /// Size of this specific data in bytes
  [[nodiscard]] static constexpr uint8_t size() { return N; }
  /// Gets the data at the given index.
  uint8_t &operator[](uint64_t index) { return bits[index]; }
};

} // namespace com

/* ********************************************* */









/* ********************************************* */
// Crown jewel:the command

/// The command information that will be sent over serial.
#pragma pack(push)
  #pragma pack(1)
struct Command {
  uint8_t start = COM_START;
  ComType type;
  ComTarget target;
  union {
    uint8_t motor_id, info, num_motors, reserved;
  };
  uint16_t size = 0;

  Command(const ComType type, const ComTarget target = T_PRINT_CONSOLE, const uint8_t spec = ':')
      : type(type), target(target), reserved(spec) {}
};
#pragma pack(pop)


/* ********************************************* */






/* ********************************************* */
// Casts commands and data into sendable packets.

namespace com {
  /// Turns a command and a given object into bits.
  template <typename T>
  Data<serial_size<T>> bitcast(Command c, const T &t) {
    Data<serial_size<T>> data;
    data[0] = COM_START;
    data[1] = c.type;
    data[2] = c.target;
    data[3] = c.reserved;
    *reinterpret_cast<uint16_t *>(data.bits + 4) = serial_size<T>;
    data[data.size() - 1] = COM_END;
    memcpy(data.bits + sizeof(Command), &t, sizeof(T));
    return data;
  }

  /// Turns a command a given amount of raw data into a bit array.
  template <uint16_t num_bits>
  Data<num_bits + com_size> bitcast(Command c, const void *raw_data) {
    Data<num_bits + com_size> data;
    data[0] = COM_START;
    data[1] = c.type;
    data[2] = c.target;
    data[3] = c.reserved;
    *reinterpret_cast<uint16_t *>(data.bits + 4) = num_bits + com_size;
    data[data.size() - 1] = COM_END;
    memcpy(data.bits + sizeof(Command), raw_data, num_bits);
    return data;
  }

  inline Data<com_size> bitcast(Command c) {
    Data<com_size> data;
    data[0] = COM_START;
    data[1] = c.type;
    data[2] = c.target;
    data[3] = c.reserved;
    *reinterpret_cast<uint16_t *>(data.bits + 4) = com_size;
    data[data.size() - 1] = COM_END;
    return data;
  }

  // String bitcast

  /// Turns a string into a sendable data command.
  template <uint16_t N>
  Data<N + com_size> bitcast(const char (&str)[N]) {
    Data<N + com_size> data;
    data[0] = COM_START;
    data[1] = COM_STRING;
    data[2] = T_PRINT_CONSOLE;
    data[3] = ':';
    data[data.size() - 1] = COM_END;
    *reinterpret_cast<uint16_t *>(data.bits + 4) = N + com_size;
    memcpy(data.bits + sizeof(Command), str, N);
    return data;
  }

  /// Pushes a command
  inline void bitpush(uint8_t* dst, const void* src, const uint16_t len, const Command c) {
    dst[0] = COM_START;
    dst[1] = c.type;
    dst[2] = c.target;
    dst[3] = c.reserved;
    *reinterpret_cast<uint16_t *>(dst + 4) = len + com_size;
    dst[len - 1] = COM_END;
    memcpy(dst + sizeof(Command), src, len);
  }

/* ********************************************* */


/// Grabs a command from the head of some raw data.
inline const Command& comcast(const void *raw_data) {
  return *reinterpret_cast<const Command *>(raw_data);
}

/// Gets the offset of a command
inline const uint8_t* comoffset(const void *raw_data) {
  return static_cast<const uint8_t *>(raw_data) + sizeof(Command);
}

/// Gets the pointer to data from a raw buffer.
template<typename T>
inline const T& datacast(const void *raw_data) {
  return *reinterpret_cast<const T *>(raw_data);
}

} //namespace com


} // namespace gobin
