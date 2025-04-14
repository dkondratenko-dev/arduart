#include "UartIO.h"
#include "UartConfig.h"
#include "Version.h"

// Constructor: Initializes the UART object with TX/RX pins and baud rate
UartIO::UartIO(const uint8_t txPin, const uint8_t rxPin,
               const unsigned long baudRate)
    : _txPin(txPin), _rxPin(rxPin), _baudRate(baudRate) {
  setBaudRate(baudRate); // Calculate bit duration based on baud rate
}

// Configures the baud rate and calculates the bit duration
void UartIO::setBaudRate(const unsigned long baudRate) {
  _baudRate = baudRate;
  _bitDuration = 1000000UL / _baudRate; // Duration of one bit in microseconds
}

// Sets up the TX and RX pins for UART communication
void UartIO::begin() noexcept {
  pinMode(_txPin, OUTPUT);
  pinMode(_rxPin, INPUT);
  digitalWrite(_txPin, HIGH); // Set TX line to idle state (HIGH)
}

// Sends a byte of data over UART
void UartIO::uart_send(const uint8_t data) {
  digitalWrite(_txPin, LOW); // Start bit
  delayMicroseconds(_bitDuration);

  uint8_t parityBit = 0; // Used to calculate parity
  uint8_t tempData = data;

  // Send 8 data bits, LSB first
  for (int i = 0; i < 8; ++i) {
    const bool bit = tempData & 1; // Extract the least significant bit
    sendBit(bit);                  // Transmit the bit
    parityBit ^= bit;              // Update parity bit
    tempData >>= 1;                // Shift to the next bit
  }

  // Conditionally send the parity bit
  if constexpr (config::cUseParityBit) {
    sendBit(parityBit); // Send the calculated parity bit
  }

  digitalWrite(_txPin, HIGH); // Stop bit
  delayMicroseconds(_bitDuration);
}

// Receives a byte of data over UART
bool UartIO::uart_receive(uint8_t *const data) {
  while (digitalRead(_rxPin) == HIGH)
    ;                                  // Wait for start bit
  delayMicroseconds(_bitDuration / 2); // Align to the middle of the start bit

  uint8_t receivedData = 0;
  uint8_t parityBit = 0;

  // Receive 8 data bits, LSB first
  for (int i = 0; i < 8; ++i) {
    const bool bit = receiveBit(); // Read the next bit
    receivedData >>= 1;            // Shift received data
    if (bit) {
      receivedData |= 0x80; // Set MSB if the bit is 1
    }
    parityBit ^= bit; // Update parity bit
  }

  // Conditionally check the parity bit
  if constexpr (config::cUseParityBit) {
    const bool receivedParityBit = receiveBit(); // Read the parity bit
    if (parityBit != receivedParityBit) {
      Serial.println("Parity error detected!"); // Log parity error
      return false;                             // Return false on parity error
    }
  }

  *data = receivedData; // Store received data
  return true;          // Successful reception
}

// Transmits a single bit over UART
void UartIO::sendBit(const bool bit) {
  digitalWrite(_txPin, bit ? HIGH : LOW); // Set TX pin to HIGH or LOW
  delayMicroseconds(_bitDuration);        // Wait for the bit duration
}

// Receives a single bit over UART
bool UartIO::receiveBit() {
  delayMicroseconds(_bitDuration);    // Wait for the bit duration
  return digitalRead(_rxPin) == HIGH; // Read the RX pin state
}

// Returns the version of the library
const char *UartIO::getVersion() noexcept {
  return VERSION; // Defined in Version.h
}
