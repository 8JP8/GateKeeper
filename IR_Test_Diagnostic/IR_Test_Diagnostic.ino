/*
 * ==============================================================================
 * ESP32-C3 Super Mini - IR Receiver Hardware Diagnostic Tool
 * ==============================================================================
 * Pin: GPIO 0 (IR Receiver Data Out / VS1838B / KY-022)
 * LED: GPIO 8 (Onboard Blue LED, Active LOW)
 * Baud Rate: 115200
 * ==============================================================================
 */

#include <Arduino.h>

const int PIN_IR_RX = 0;
const int PIN_LED = 8;

volatile uint32_t edgeCount = 0;
volatile uint32_t lastEdgeMicros = 0;
volatile bool newSignalDetected = false;

#define MAX_EDGES 256
volatile uint16_t pulseBuffer[MAX_EDGES];
volatile uint16_t pulseCount = 0;
volatile uint32_t lastPulseTime = 0;

void IRAM_ATTR onPinChangeISR() {
  uint32_t now = micros();
  uint32_t delta = now - lastEdgeMicros;
  lastEdgeMicros = now;
  edgeCount++;

  // Blink LED on every transition
  digitalWrite(PIN_LED, digitalRead(PIN_IR_RX));

  if (pulseCount < MAX_EDGES) {
    if (delta > 65535) delta = 65535;
    pulseBuffer[pulseCount++] = (uint16_t)delta;
  }
  lastPulseTime = millis();
  newSignalDetected = true;
}

void setup() {
  Serial.begin(115200);
  delay(1500);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH); // Off

  // Blink LED 3 times to confirm boot
  for (int i = 0; i < 3; i++) {
    digitalWrite(PIN_LED, LOW);
    delay(100);
    digitalWrite(PIN_LED, HIGH);
    delay(100);
  }

  // IR Receivers (VS1838B / KY-022) have an active-low open-collector output.
  // They require an internal pull-up resistor to stay HIGH when idle!
  pinMode(PIN_IR_RX, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIN_IR_RX), onPinChangeISR, CHANGE);

  Serial.println();
  Serial.println("==============================================================");
  Serial.println("   ESP32-C3 SUPER MINI - DEDICATED IR RECEIVER DIAGNOSTIC     ");
  Serial.println("==============================================================");
  Serial.println(" Hardware Wiring:");
  Serial.println("  - IR Receiver OUT / S pin -> GPIO 0 (Right header, Pin 8)");
  Serial.println("  - IR Receiver VCC / + pin -> 3.3V   (Right header, Pin 3)");
  Serial.println("  - IR Receiver GND / - pin -> GND    (Right header, Pin 2)");
  Serial.println("  - Status LED (GPIO 8) will blink on EVERY electrical pulse");
  Serial.println("==============================================================");
  Serial.printf(" Initial State on GPIO 0: %s\n", digitalRead(PIN_IR_RX) == HIGH ? "HIGH (Normal 3.3V Idle)" : "LOW (WARNING: Line is pulled to GND!)");
  Serial.println(" Aim any remote at the sensor and press buttons...");
  Serial.println("==============================================================");
}

uint32_t lastHeartbeat = 0;

void decodeAndPrintPulses() {
  // Make a local copy of pulses
  noInterrupts();
  uint16_t count = pulseCount;
  uint16_t timings[MAX_EDGES];
  for (uint16_t i = 0; i < count; i++) {
    timings[i] = pulseBuffer[i];
  }
  pulseCount = 0;
  interrupts();

  if (count < 4) return; // Glitch

  Serial.println();
  Serial.println("--------------------------------------------------------------");
  Serial.printf(">>> IR BURST DETECTED! Total edges captured: %d\n", count);
  Serial.print("Raw Pulse Timings (us): ");
  for (uint16_t i = 1; i < count && i < 32; i++) {
    Serial.printf("%d ", timings[i]);
  }
  if (count > 32) Serial.print("...");
  Serial.println();

  // 1. Check for standard NEC Protocol (9ms Mark, 4.5ms Space)
  if (count >= 34) {
    uint32_t headerMark = timings[1];
    uint32_t headerSpace = timings[2];
    Serial.printf("Header Mark: %u us, Header Space: %u us\n", headerMark, headerSpace);

    if (headerMark > 6000 && headerMark < 11000 && headerSpace > 3000 && headerSpace < 6000) {
      uint32_t necCode = 0;
      int bitsRead = 0;
      for (uint16_t i = 3; i < count - 1; i += 2) {
        uint32_t space = timings[i + 1];
        if (space > 1000 && space < 2500) {
          necCode = (necCode << 1) | 1;
          bitsRead++;
        } else if (space > 200 && space < 900) {
          necCode = (necCode << 1) | 0;
          bitsRead++;
        }
        if (bitsRead >= 32) break;
      }
      Serial.printf(">> SUCCESS [NEC Protocol]: 0x%08X (%u bits) Dec: %u\n", necCode, bitsRead, necCode);
    }
  }

  // 2. Check for Sony SIRC Protocol (2.4ms Mark, 0.6ms Space)
  if (count >= 14) {
    uint32_t headerMark = timings[1];
    if (headerMark > 1800 && headerMark < 3000) {
      uint32_t sonyCode = 0;
      int bitsRead = 0;
      for (uint16_t i = 2; i < count; i += 2) {
        uint32_t mark = timings[i];
        if (mark > 900 && mark < 1500) {
          sonyCode |= (1 << bitsRead);
          bitsRead++;
        } else if (mark > 400 && mark < 800) {
          bitsRead++;
        }
        if (bitsRead >= 20) break;
      }
      Serial.printf(">> SUCCESS [Sony SIRC Protocol]: 0x%X (%u bits)\n", sonyCode, bitsRead);
    }
  }

  // 3. Fallback Universal 32-bit Hash / Signature
  uint32_t signature = 0;
  for (uint16_t i = 1; i < count; i++) {
    signature = (signature << 1) ^ timings[i];
  }
  Serial.printf(">> Universal Pulse Signature: 0x%08X (Edges: %d)\n", signature, count);
  Serial.println("--------------------------------------------------------------");
}

void loop() {
  uint32_t now = millis();

  // If a burst started and is now idle for > 50ms, decode and print!
  if (newSignalDetected && (now - lastPulseTime > 50)) {
    newSignalDetected = false;
    decodeAndPrintPulses();
  }

  // Periodic heartbeat every 2 seconds
  if (now - lastHeartbeat >= 2000) {
    lastHeartbeat = now;
    int state = digitalRead(PIN_IR_RX);
    Serial.printf("[Monitor] GPIO 0 Pin Level: %s (%d) | Total Edges Seen: %u\n",
                  state == HIGH ? "HIGH (Normal 3.3V Idle)" : "LOW (WARNING: Active / Pulled Low)",
                  state, edgeCount);
  }
}
