#pragma once
#include <Arduino.h>

// ==============================================================================
// ESP32-C3 Super Mini - Pinout Configuration
// ==============================================================================
// CC1101 433MHz Transceiver:
// 1. VCC          -> 3.3V (Right Header, Pin 3)   [CRITICAL: 3.3V ONLY!]
// 2. GND          -> GND  (Right Header, Pin 2)
// 3. SI / MOSI    -> GPIO6 (Left Header, Pin 2)
// 4. SCLK / SCK   -> GPIO4 (Right Header, Pin 4)
// 5. SO / MISO    -> GPIO5 (Left Header, Pin 1)
// 6. GDO2 (RX)    -> GPIO2 (Right Header, Pin 6)  [RF Demodulated RX Data]
// 7. CSN / SS     -> GPIO7 (Left Header, Pin 3)   [SPI Chip Select]
// 8. GDO0 (TX)    -> GPIO3 (Right Header, Pin 5)  [RF Modulated TX Data]
//
// Infrared (IR):
// - IR Transmitter TX -> GPIO10 (Left Header, Pin 6)  [Connect to 'S' pin]
// - IR Receiver RX    -> GPIO0  (Right Header, Pin 8) [Connect to TSOP/VS1838B]
// - Onboard Status LED-> GPIO8  (Left Header, Pin 4)  [Active LOW Blue LED]
// ==============================================================================

#define PIN_CC1101_SCK   4
#define PIN_CC1101_MISO  5
#define PIN_CC1101_MOSI  6
#define PIN_CC1101_CSN   7
#define PIN_CC1101_GDO0  3  // RF TX Modulation
#define PIN_CC1101_GDO2  2  // RF RX Demodulation (GPIO2)

// Infrared (IR) Pins
#define PIN_IR_TX        10 // IR Transmitter output
#define PIN_IR_RX        0  // IR Receiver input

// Onboard Blue LED on ESP32-C3 Super Mini (Active LOW)
#define PIN_LED_STATUS   8

// ==============================================================================
// Defaults
// ==============================================================================
#define DEFAULT_RF_FREQ       433.92f
#define DEFAULT_RF_POWER      10
#define DEFAULT_REPEAT_COUNT  15
#define DEFAULT_PULSE_LENGTH  350
#define DEFAULT_BIT_LENGTH    24
#define DEFAULT_PROTOCOL      1

#define DEFAULT_AP_SSID       "ESP32-Gate-Remote"
#define DEFAULT_AP_PASS       "GatePass1234"
#define DEFAULT_MDNS_HOST     "gatekeeper"

#define MAX_PRESETS           30
#define MAX_RECORDED_SIGNALS  15
