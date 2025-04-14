#ifndef PROTOCOL_H
#define PROTOCOL_H

namespace protocol {
// Data type constants
constexpr uint8_t ASCII = 'c';        // ASCII mode ('c' for character)
constexpr uint8_t NUMBER = 'd';       // Number mode ('d' for decimal)
constexpr uint8_t BAUD_RATE = 'b';    // Baud rate mode ('b' for baud rate)
constexpr uint8_t MODE_SWITCH = 0xFE; // Reserved byte for mode switching

// Function to convert protocol command to string
inline const char *protocolCommandToStr(uint8_t command) {
  switch (command) {
  case ASCII:
    return "ASCII";
  case NUMBER:
    return "NUMBER";
  case BAUD_RATE:
    return "BAUD_RATE";
  case MODE_SWITCH:
    return "MODE_SWITCH";
  default:
    return "UNKNOWN";
  }
}
} // namespace protocol

#endif // PROTOCOL_H