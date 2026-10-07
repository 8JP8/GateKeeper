/*
 * ==============================================================================
 * ESP32-C3 Super Mini - GateKeeper Universal RF & IR Home Automation
 * ==============================================================================
 *
 * Board: ESP32-C3 Super Mini (Connected to COM12)
 *
 * WIRING:
 * ------------------------------------------------------------------------------
 * CC1101 433MHz Transceiver:
 * 1. VCC         -> 3.3V (Right Header, Pin 3)   [CRITICAL: 3.3V ONLY!]
 * 2. GND         -> GND  (Right Header, Pin 2)
 * 3. SI / MOSI   -> GPIO6 (Left Header, Pin 2)
 * 4. SCLK / SCK  -> GPIO4 (Right Header, Pin 4)
 * 5. SO / MISO   -> GPIO5 (Left Header, Pin 1)
 * 6. GDO2 (RX)   -> GPIO1 (Right Header, Pin 7)  [RF Demodulated Signal Output]
 * 7. CSN / SS    -> GPIO7 (Left Header, Pin 3)   [SPI Chip Select]
 * 8. GDO0 (TX)   -> GPIO3 (Right Header, Pin 5)  [RF Modulated Signal Input]
 *
 * Infrared (IR):
 * - IR Transmitter TX -> GPIO10 (Left Header, Pin 6)  ['S' pin on module]
 * - IR Transmitter GND-> GND    (Right Header, Pin 2) ['-' pin on module]
 * - IR Receiver RX    -> GPIO0  (Right Header, Pin 8) [TSOP/VS1838B Signal]
 *
 * Analog Sensor:
 * - Analog Sensor ADC -> GPIO2  (Right Header, Pin 6) [ADC1_CH2]
 *
 * Status LED:
 * - Onboard LED       -> GPIO8  (Left Header, Pin 4)  [Blue LED, Active LOW]
 * ==============================================================================
 */

#include <Arduino.h>
#include "Config.h"
#include "StorageManager.h"
#include "RFManager.h"
#include "IRManager.h"
#include "WiFiPortal.h"
#include "WebHandler.h"

void printBanner() {
  Serial.println();
  Serial.println("========================================================");
  Serial.println("     ESP32-C3 GateKeeper - Universal RF & IR Remote     ");
  Serial.println("========================================================");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  printBanner();

  // 1. Storage
  Serial.println("[Init] Initializing persistent storage...");
  storage.begin();
  DeviceSettings& cfg = storage.getSettings();

  // 2. CC1101 RF
  Serial.println("[Init] Initializing CC1101 Transceiver...");
  bool radioOk = rfManager.begin(cfg.rfFreq, cfg.rfPower);
  if (!radioOk) {
    Serial.println("[Init] WARNING: Check CC1101 wiring to GPIO4, 5, 6, 7 and 3.3V.");
  }

  // 3. Infrared & Analog Sensor
  Serial.println("[Init] Initializing Infrared & Analog Sensor...");
  irManager.begin();

  // 4. Wi-Fi
  Serial.println("[Init] Initializing Wi-Fi portal...");
  wifiPortal.begin();

  // 5. Web Server
  Serial.println("[Init] Initializing Web Interface...");
  webHandler.begin();

  Serial.println("========================================================");
  Serial.println(" SYSTEM READY!");
  Serial.printf(" - Hotspot SSID:     %s\n", cfg.apSsid);
  Serial.printf(" - Hotspot Password: %s\n", cfg.apPass);
  Serial.printf(" - Hotspot URL:      http://%s\n", wifiPortal.getApIP().c_str());
  if (wifiPortal.isStaConnected()) {
    Serial.printf(" - Home Network URL: http://%s\n", wifiPortal.getStaIP().c_str());
  }
  Serial.println(" - Local mDNS URL:   http://gatekeeper.local");
  Serial.printf(" - RF Frequency:     %.2f MHz\n", rfManager.getFrequency());
  Serial.println("========================================================");
}

void loop() {
  rfManager.update();
  irManager.update();
  wifiPortal.update();
  webHandler.update();
}
