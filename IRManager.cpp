#include "IRManager.h"

IRManager irManager;

#define MAX_IR_PULSES 350

static volatile uint16_t irPulseBuffer[MAX_IR_PULSES];
static volatile uint16_t irPulseCount = 0;
static volatile uint32_t irLastEdgeTime = 0;
static volatile uint32_t irLastPulseMs = 0;
static volatile bool irNewBurst = false;

// Interrupt Service Routine on PIN_IR_RX (GPIO0)
void IRAM_ATTR irRxISR() {
  uint32_t now = micros();
  uint32_t delta = now - irLastEdgeTime;
  irLastEdgeTime = now;

  // Visual feedback: toggle LED on incoming carrier transitions
  digitalWrite(PIN_LED_STATUS, digitalRead(PIN_IR_RX));

  if (irPulseCount < MAX_IR_PULSES) {
    irPulseBuffer[irPulseCount++] = (delta > 65535) ? 65535 : (uint16_t)delta;
  }
  irLastPulseMs = millis();
  irNewBurst = true;
}

IRManager::IRManager() : signalAvailable(false), listeningActive(false), lastRxTime(0) {
  memset(&latestIR, 0, sizeof(latestIR));
}

void IRManager::begin() {
  // Use ESP32 hardware PWM (LEDC) for rock-solid 38.000 kHz carrier with 0 jitter
  ledcAttach(PIN_IR_TX, 38000, 8);
  ledcWrite(PIN_IR_TX, 0); // Carrier OFF initially (held LOW)

  // VS1838B / KY-022 has an active-low open-collector output requiring pullup
  pinMode(PIN_IR_RX, INPUT_PULLUP);

  // Start listening immediately
  startListening();
  Serial.println("[IR] Hardware 38kHz PWM initialized on GPIO10. IR RX (GPIO0) listening with INPUT_PULLUP.");
}

void IRManager::startListening() {
  if (!listeningActive) {
    irPulseCount = 0;
    irNewBurst = false;
    irLastEdgeTime = micros();
    attachInterrupt(digitalPinToInterrupt(PIN_IR_RX), irRxISR, CHANGE);
    listeningActive = true;
    Serial.println("[IR] RX Interrupt attached on GPIO0");
  }
}

void IRManager::stopListening() {
  if (listeningActive) {
    detachInterrupt(digitalPinToInterrupt(PIN_IR_RX));
    listeningActive = false;
    irPulseCount = 0;
    irNewBurst = false;
    Serial.println("[IR] RX Interrupt detached on GPIO0");
  }
}

bool IRManager::isListening() const {
  return listeningActive;
}

void IRManager::send38kHzBurst(uint32_t durationUs) {
  // 1/3 duty cycle (85/255) at 38kHz hardware PWM: optimal carrier for TSOP/VS1838 receivers
  ledcWrite(PIN_IR_TX, 85);
  delayMicroseconds(durationUs);
  ledcWrite(PIN_IR_TX, 0);
}

void IRManager::sendSpace(uint32_t durationUs) {
  ledcWrite(PIN_IR_TX, 0);
  delayMicroseconds(durationUs);
}

bool IRManager::transmitNEC(uint32_t code, uint8_t bits, uint8_t repeats) {
  uint8_t bitLen = (bits > 0 && bits <= 32) ? bits : 32;
  uint8_t numReps = (repeats > 0) ? repeats : 3;

  digitalWrite(PIN_LED_STATUS, LOW); // LED ON
  Serial.printf("[IR TX] Transmitting NEC Code: 0x%08X (%d bits, %d reps)\n", code, bitLen, numReps);

  for (uint8_t r = 0; r < numReps; r++) {
    send38kHzBurst(9000);
    sendSpace(4500);

    for (int i = bitLen - 1; i >= 0; i--) {
      bool b = (code >> i) & 1;
      send38kHzBurst(560);
      sendSpace(b ? 1690 : 560);
    }

    send38kHzBurst(560);
    sendSpace(40000);
  }

  digitalWrite(PIN_LED_STATUS, HIGH); // LED OFF
  return true;
}

bool IRManager::transmitSony(uint32_t code, uint8_t bits, uint8_t repeats) {
  uint8_t bitLen = (bits > 0) ? bits : 12;
  uint8_t numReps = (repeats > 0) ? repeats : 3;

  digitalWrite(PIN_LED_STATUS, LOW);
  Serial.printf("[IR TX] Transmitting Sony Code: 0x%X (%d bits, %d reps)\n", code, bitLen, numReps);

  for (uint8_t r = 0; r < numReps; r++) {
    send38kHzBurst(2400);
    sendSpace(600);

    for (int i = 0; i < bitLen; i++) {
      bool b = (code >> i) & 1;
      send38kHzBurst(b ? 1200 : 600);
      sendSpace(600);
    }
    delay(25);
  }

  digitalWrite(PIN_LED_STATUS, HIGH);
  return true;
}

bool IRManager::transmitPanasonic(uint32_t code, uint8_t bits, uint8_t repeats) {
  uint8_t bitLen = (bits > 0 && bits <= 32) ? bits : 32;
  uint8_t numReps = (repeats > 0) ? repeats : 2;

  digitalWrite(PIN_LED_STATUS, LOW);
  Serial.printf("[IR TX] Transmitting Panasonic/Kaseikyo Code: 0x%08X (%d bits, %d reps)\n", code, bitLen, numReps);

  for (uint8_t r = 0; r < numReps; r++) {
    // Header mark 3500us, space 1750us
    send38kHzBurst(3500);
    sendSpace(1750);

    for (int i = bitLen - 1; i >= 0; i--) {
      bool b = (code >> i) & 1;
      send38kHzBurst(430);
      sendSpace(b ? 1300 : 430);
    }

    send38kHzBurst(430);
    sendSpace(40000);
  }

  digitalWrite(PIN_LED_STATUS, HIGH);
  return true;
}

bool IRManager::transmitRaw(const uint16_t* timings, uint16_t count, uint8_t repeats) {
  if (timings == nullptr || count == 0) return false;

  uint8_t numReps = (repeats > 0) ? repeats : 2;
  digitalWrite(PIN_LED_STATUS, LOW);
  Serial.printf("[IR TX] Transmitting Raw Signal: %d pulses, %d repeats\n", count, numReps);

  for (uint8_t r = 0; r < numReps; r++) {
    for (uint16_t i = 0; i < count; i++) {
      if (i % 2 == 0) {
        send38kHzBurst(timings[i]); // Even index in rawTimings = MARK
      } else {
        sendSpace(timings[i]);      // Odd index in rawTimings = SPACE
      }
    }
    if (r < numReps - 1) {
      sendSpace(40000); // 40ms inter-frame gap between repeats
    }
  }

  digitalWrite(PIN_LED_STATUS, HIGH);
  return true;
}

void IRManager::update() {
  if (!listeningActive) return;

  uint32_t now = millis();
  // Frame finished if idle for > 65ms after last edge (covers entire frame + inter-frame gap + repeats)
  if (irNewBurst && (now - irLastPulseMs > 65)) {
    irNewBurst = false;
    decodeCapturedPulses();
  }
}

void IRManager::decodeCapturedPulses() {
  noInterrupts();
  uint16_t count = irPulseCount;
  uint16_t timings[MAX_IR_PULSES];
  for (uint16_t i = 0; i < count && i < MAX_IR_PULSES; i++) {
    timings[i] = irPulseBuffer[i];
  }
  irPulseCount = 0;
  interrupts();

  if (count < 6) return;

  uint32_t headerMark = timings[1];
  uint32_t headerSpace = timings[2];
  uint32_t decodedCode = 0;
  uint8_t decodedBits = 0;
  char detectedProto[16] = "IR Remote";

  // 1. NEC Protocol (9000us mark, 4500us space)
  if (headerMark >= 7000 && headerMark <= 11000 && headerSpace >= 3000 && headerSpace <= 6000 && count >= 34) {
    for (uint16_t i = 3; i < count - 1; i += 2) {
      decodedCode <<= 1;
      if (timings[i + 1] > 1000) {
        decodedCode |= 1;
      }
      decodedBits++;
      if (decodedBits >= 32) break;
    }
    strncpy(detectedProto, "NEC", sizeof(detectedProto) - 1);
  }
  // 2. Kaseikyo / Midea / Panasonic / Sharp AC & TV (3500us mark, 1750us space)
  else if (headerMark >= 3000 && headerMark <= 4200 && headerSpace >= 1200 && headerSpace <= 2200 && count >= 20) {
    for (uint16_t i = 3; i < count - 1; i += 2) {
      decodedCode <<= 1;
      if (timings[i + 1] > 800) {
        decodedCode |= 1;
      }
      decodedBits++;
      if (decodedBits >= 32) break;
    }
    strncpy(detectedProto, "Kaseikyo/Midea", sizeof(detectedProto) - 1);
  }
  // 3. Samsung Protocol (4500us mark, 4500us space)
  else if (headerMark >= 3800 && headerMark <= 5500 && headerSpace >= 3500 && headerSpace <= 5800 && count >= 20) {
    for (uint16_t i = 3; i < count - 1; i += 2) {
      decodedCode <<= 1;
      if (timings[i + 1] > 1000) {
        decodedCode |= 1;
      }
      decodedBits++;
      if (decodedBits >= 32) break;
    }
    strncpy(detectedProto, "Samsung", sizeof(detectedProto) - 1);
  }
  // 4. Sony SIRC (2400us mark, 600us space)
  else if (headerMark >= 1800 && headerMark <= 3000 && count >= 14) {
    for (uint16_t i = 2; i < count; i += 2) {
      if (timings[i] > 900) {
        decodedCode |= (1 << decodedBits);
      }
      decodedBits++;
      if (decodedBits >= 20) break;
    }
    strncpy(detectedProto, "Sony", sizeof(detectedProto) - 1);
  }
  // 5. Universal Pulse-Distance / Signature Fallback
  else {
    uint32_t signature = 0;
    for (uint16_t i = 1; i < count; i++) {
      signature = (signature << 1) ^ timings[i];
    }
    decodedCode = signature;
    decodedBits = (uint8_t)min((int)32, (int)(count - 2) / 2);
    strncpy(detectedProto, "Universal", sizeof(detectedProto) - 1);
  }

  if (decodedBits > 0 || decodedCode > 0) {
    latestIR.id = (uint32_t)random(100000, 999999);
    strncpy(latestIR.protocol, detectedProto, sizeof(latestIR.protocol) - 1);
    latestIR.code = decodedCode;
    latestIR.bitLength = decodedBits > 0 ? decodedBits : 32;
    snprintf(latestIR.hexCode, sizeof(latestIR.hexCode), "0x%08X", decodedCode);
    latestIR.timestamp = millis();

    latestIR.rawCount = 0;
    // Index 1 is the first MARK. Store marks at even indices and spaces at odd indices
    for (uint16_t i = 1; i < count && latestIR.rawCount < 256; i++) {
      latestIR.rawTimings[latestIR.rawCount++] = timings[i];
    }

    signalAvailable = true;
    lastRxTime = millis();
    Serial.printf("[IR RX] Decoded %s: 0x%08X (%d bits, %d edges)\n", detectedProto, decodedCode, decodedBits, count);
  }
}

bool IRManager::isSignalAvailable() const {
  return signalAvailable;
}

RecordedIR IRManager::getLatestSignal() const {
  return latestIR;
}

void IRManager::clearSignal() {
  signalAvailable = false;
  memset(&latestIR, 0, sizeof(latestIR));
}
