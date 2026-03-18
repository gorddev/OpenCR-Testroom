#include "command.h"

struct MyData {
  uint8_t vel;
  uint8_t pos;
  uint8_t huh;
};

using namespace gobin;

int main(void) {

  std::cout << "Testing command interface.\n";

  MyData m{3, 4, 56};

  auto data = bitcast(Command{com::Type::COM_GET, com::T_MOTOR_VELOCITY, 4}, m);

  bitprint(data.bits, data.size());

  const char str[] = "Heyefouehaofiasdfgg";
  std::cout << "size of str: " << sizeof(str) << std::endl;
  auto data2 = bitcast<sizeof(str)>(Command{com::COM_STRING}, str);

  std::cout << "string size?? " << sizeof(str) << std::endl;

  bitprint(data2.bits, data2.size());

  std::cout << "Attempting to do raw string literal:" << std::endl;
  auto data3 =
      bitcast("String literal sending now! We're gonna make the length real "
              "long so it can't even handle it! Isn't that craaaaaaaaazy?");
  bitprint(data3, data3.size());

  return 0;
}
