#ifndef UARTIO_H
#define UARTIO_H

#include <Arduino.h>

class UartIO final {
public:
  /**
   * @brief Constructor to initialize UART communication.
   * @param txPin Transmit pin.
   * @param rxPin Receive pin.
   * @param baudRate Communication speed in bits per second.
   */
  explicit UartIO(const uint8_t txPin, const uint8_t rxPin,
                  const unsigned long baudRate);

  /**
   * @brief Destructor.
   */
  ~UartIO() = default;

  // Disable copy constructor and copy assignment operator
  UartIO(const UartIO &) = delete;
  UartIO &operator=(const UartIO &) = delete;

  /**
   * @brief Begins UART communication.
   */
  void begin() noexcept;

  /**
   * @brief Sets the baud rate for UART communication.
   * @param baudRate Communication speed in bits per second.
   */
  void setBaudRate(const unsigned long baudRate);

  /**
   * @brief Sends a byte of data via UART.
   * @param data Byte to send.
   */
  void uart_send(const uint8_t data);

  /**
   * @brief Receives a byte of data via UART.
   * @param data Pointer to store the received byte.
   * @return True if data was successfully received, false otherwise.
   */
  bool uart_receive(uint8_t *const data);

  /**
   * @brief Gets the version of the UartIO library.
   * @return Version string.
   */
  static const char *getVersion() noexcept;

private:
  /**
   * @brief Sends a single bit via UART.
   * @param bit The bit to send (true for 1, false for 0).
   */
  void sendBit(const bool bit);

  /**
   * @brief Receives a single bit via UART.
   * @return The received bit (true for 1, false for 0).
   */
  bool receiveBit();

private:
  const uint8_t _txPin;       ///< Transmit pin
  const uint8_t _rxPin;       ///< Receive pin
  unsigned long _baudRate;    ///< Communication speed in bits per second
  unsigned long _bitDuration; ///< Duration of a single bit in microseconds
};

#endif // UARTIO_H
