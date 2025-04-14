#include <Arduino.h>

#include "Protocol.h"   // Include the protocol constants
#include "UartConfig.h" // Include the configuration header
#include "UartIO.h"

UartIO uart(config::TxPin, config::RxPin, config::BaudRate);

uint8_t currentMode = protocol::ASCII; // Default mode is ASCII

void setup() {
    Serial.begin(9600);

    Serial.print("Receiver Started. v");
    Serial.println(UartIO::getVersion());

    Serial.print("ParityBit: ");
    Serial.println(config::cUseParityBit ? "Enabled" : "Disabled");

    uart.begin();
}

void loop() {
    uint8_t receivedData = 0;

    // Receive the data
    if (!uart.uart_receive(&receivedData)) {
        dataReceiveDelay();
        return;
    }

    if (receivedData == protocol::MODE_SWITCH) {
        // Print new line after switching from ASCII mode to another mode for better readability
        if(protocol::ASCII == currentMode) {
            Serial.println();
        }

        // Receive the new mode identifier
        if (uart.uart_receive(&currentMode)) {
            Serial.print(">> Switched to mode: ");
            Serial.println(protocol::protocolCommandToStr(currentMode));
        } else {
            Serial.println("Error: Failed to receive mode.");
        }
        return;
    }

    switch (currentMode) {
        case protocol::ASCII:
            Serial.print((char)receivedData);
            break;

        case protocol::NUMBER:
            Serial.print("Received (Number): ");
            Serial.println(receivedData);
            break;

        case protocol::BAUD_RATE: {
            // Receive the high byte of the baud rate
            uint8_t baudRateHigh = receivedData;
            Serial.print("BaudRate High Byte: ");
            Serial.println(baudRateHigh);

            // Receive the low byte of the baud rate
            if (!uart.uart_receive(&receivedData)) {
                Serial.println("Error: Failed to receive BaudRate Low Byte.");
                delay(100);
                return;
            }

            uint8_t baudRateLow = receivedData;
            Serial.print("BaudRate Low Byte: ");
            Serial.println(baudRateLow);

            // Combine high and low bytes into a single baud rate value
            unsigned long newBaudRate = ((unsigned long)baudRateHigh << 8) | baudRateLow;

            // Display the new baud rate
            Serial.print("Baud rate changed to ");
            Serial.println(newBaudRate);

            // Update the UART baud rate
            uart.setBaudRate(newBaudRate);
            break;
        }

        default:
            Serial.println("Error: Unknown mode received.");
            break;
    }

    dataReceiveDelay(); // Delay for readability in serial output
}

void dataReceiveDelay() {
    delay(config::DataSendDelayMs);
}
