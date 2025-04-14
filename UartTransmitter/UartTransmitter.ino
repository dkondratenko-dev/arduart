#include <Arduino.h>

#include "Protocol.h"   // Include protocol constants
#include "UartConfig.h" // Include the configuration header
#include "UartIO.h"

// Initialize UART with defined constants
UartIO uart(config::TxPin, config::RxPin, config::BaudRate);

void setup() {
    Serial.begin(9600);

    Serial.print("Transmitter Started. v");
    Serial.println(UartIO::getVersion());

    Serial.print("ParityBit: ");
    Serial.println(config::cUseParityBit ? "Enabled" : "Disabled");
    
    uart.begin();

    runTheTest();
}

void runTheTest() {
    transmitAsciiData();
    transmitNumberData();

    changeTheBaudRate();

    transmitAsciiData();
    transmitNumberData();
}

void transmitAsciiData() {
    sendSwitchMode(protocol::ASCII);

    const char* asciiData = "This is UART ASCII symbols transmition test!";
    Serial.println("Sening (ASCII) symbols: ");
    Serial.println(asciiData);
    
    for (const char* ptr = asciiData; *ptr != '\0'; ++ptr) {
        uart.uart_send(*ptr);
        dataSendDelay();
    }

    Serial.println("(ASCII) symbols sent!");
}

void transmitNumberData() {
    sendSwitchMode(protocol::NUMBER);

    constexpr uint8_t numbers[] = {0, 1, 2, 3, 4, 5, 100, 255};

    for (uint8_t data : numbers) {
        uart.uart_send(data);
        Serial.print("Sent Number: ");
        Serial.println(data);
        dataSendDelay(); // Delay for clarity
    }
}

// Function to send a command to switch mode on the receiver
void sendSwitchMode(const uint8_t mode) {
    uart.uart_send(protocol::MODE_SWITCH);
    addStabilityDelay();
    uart.uart_send(mode); // Switch to the specified mode
    addStabilityDelay();
    delay(10); // Add a delay to allow the receiver to process the mode switch

    Serial.print(">> Sending switch mode: ");
    Serial.println(protocol::protocolCommandToStr(mode));
}

void changeTheBaudRate() {
    // Change baud rate to 19200
    sendSwitchMode(protocol::BAUD_RATE);

    constexpr unsigned long newBaudRate = 19200;
    uart.uart_send(newBaudRate >> 8); // High byte of the baud rate
    addStabilityDelay();
    uart.uart_send(newBaudRate & 0xFF); // Low byte of the baud rate
    delay(1000); // Delay for clarity

    // Change the baud rate for the transmitter
    uart.setBaudRate(newBaudRate);
    Serial.println("Baud rate changed to 19200");
}

// Small delay for stability
void addStabilityDelay() {
    delay(1);
}

// Delay between characters
void dataSendDelay() {
    delay(config::DataSendDelayMs);
}

void loop() {
    // Empty loop since data is sent only once in setup()
}
