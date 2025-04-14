#ifndef UART_CONFIG_H
#define UART_CONFIG_H

namespace config {
constexpr int RxPin = 2;
constexpr int TxPin = 3;
constexpr unsigned long BaudRate = 9600;
constexpr bool cUseParityBit = false;
constexpr int DataSendDelayMs = 50;
} // namespace config

#endif // UART_CONFIG_H