#include "RFManager.h"

RFManager rfManager;

RFManager::RFManager()
  : currentFreq(DEFAULT_RF_FREQ),
    currentPower(DEFAULT_RF_POWER),
    cc1101Ok(false),
    listeningActive(true),
    signalCount(0),
    lastReceiveTime(0),
    lastReceivedCode(0),
    lastFuzzerStepTime(0) {
  memset(signalBuffer, 0, sizeof(signalBuffer));
  memset(&streamState, 0, sizeof(streamState));
  memset(&fuzzerState, 0, sizeof(fuzzerState));
}

bool RFManager::begin(float freq, int8_t power) {
  currentFreq = freq;
  currentPower = power;

  pinMode(PIN_LED_STATUS, OUTPUT);
  digitalWrite(PIN_LED_STATUS, HIGH); // Turn off LED (Active LOW)

  Serial.println("[RF] Configuring CC1101 pins...");
  ELECHOUSE_cc1101.setSpiPin(PIN_CC1101_SCK, PIN_CC1101_MISO, PIN_CC1101_MOSI, PIN_CC1101_CSN);
  ELECHOUSE_cc1101.setGDO(PIN_CC1101_GDO0, PIN_CC1101_GDO2);

  // Initialize CC1101 SPI registers and calibrate
  ELECHOUSE_cc1101.Init();

  byte version = ELECHOUSE_cc1101.SpiReadStatus(CC1101_VERSION);
  byte part = ELECHOUSE_cc1101.SpiReadStatus(CC1101_PARTNUM);
  Serial.printf("[RF] Read CC1101 Partnum: 0x%02X, Version: 0x%02X\n", part, version);

  if (ELECHOUSE_cc1101.getCC1101()) {
    Serial.println("[RF] CC1101 SPI Communication Verified (Chip detected)");
    cc1101Ok = true;
  } else {
    Serial.println("[RF] WARNING: CC1101 not detected on SPI! Check wiring and 3.3V power!");
    cc1101Ok = false;
  }

  ELECHOUSE_cc1101.setMHZ(currentFreq);
  ELECHOUSE_cc1101.setModulation(2); // ASK / OOK
  ELECHOUSE_cc1101.setPA(currentPower);
  ELECHOUSE_cc1101.setRxBW(812.50);  // Wide filter bandwidth for gate remotes
  ELECHOUSE_cc1101.SetRx();

  // Attach RCSwitch to GDO2 (demodulated digital RX output from CC1101)
  rcSwitch.enableReceive(PIN_CC1101_GDO2);

  blinkLed(2, 60);
  Serial.printf("[RF] Receiver active at %.2f MHz (Power: %d dBm)\n", currentFreq, currentPower);
  return cc1101Ok;
}

void RFManager::update() {
  if (streamState.active) {
    serviceStreaming();
    return;
  }

  if (fuzzerState.running) {
    serviceFuzzer();
    return;
  }

  if (rcSwitch.available()) {
    processReceivedPacket();
    rcSwitch.resetAvailable();
  }
}

void RFManager::processReceivedPacket() {
  uint32_t value = rcSwitch.getReceivedValue();
  if (value == 0) return;

  uint8_t bitLength = rcSwitch.getReceivedBitlength();
  uint16_t delay = rcSwitch.getReceivedDelay();
  uint8_t protocol = rcSwitch.getReceivedProtocol();
  int16_t rssi = ELECHOUSE_cc1101.getRssi();
  uint32_t now = millis();

  // De-bounce repeated transmissions within 1.5s
  if (signalCount > 0 && value == lastReceivedCode && (now - lastReceiveTime < 1500)) {
    signalBuffer[0].repeatCount++;
    signalBuffer[0].rssi = rssi;
    signalBuffer[0].timestamp = now;
    lastReceiveTime = now;
    digitalWrite(PIN_LED_STATUS, LOW);
    delayMicroseconds(500);
    digitalWrite(PIN_LED_STATUS, HIGH);
    return;
  }

  lastReceiveTime = now;
  lastReceivedCode = value;

  int maxIndex = (signalCount < MAX_RECORDED_SIGNALS) ? signalCount : (MAX_RECORDED_SIGNALS - 1);
  for (int i = maxIndex; i > 0; i--) {
    signalBuffer[i] = signalBuffer[i - 1];
  }
  if (signalCount < MAX_RECORDED_SIGNALS) {
    signalCount++;
  }

  RecordedSignal& sig = signalBuffer[0];
  sig.id = (uint32_t)random(100000, 999999);
  sig.code = value;
  snprintf(sig.hexCode, sizeof(sig.hexCode), "0x%06X", value);
  uintToBinaryStr(value, bitLength, sig.binCode, sizeof(sig.binCode));
  sig.bitLength = bitLength;
  sig.protocol = protocol;
  sig.pulseLength = delay;
  sig.frequency = currentFreq;
  sig.rssi = rssi;
  sig.timestamp = now;
  sig.repeatCount = 1;
  sig.isRaw = false;

  unsigned int* raw = rcSwitch.getReceivedRawdata();
  if (raw != nullptr) {
    int count = 2 * bitLength + 2;
    if (count > 128) count = 128;
    sig.rawCount = count;
    for (int i = 0; i < count; i++) {
      sig.rawTimings[i] = (uint16_t)raw[i];
    }
  } else {
    sig.rawCount = 0;
  }

  blinkLed(1, 40);
  Serial.printf("[RF] Received Code: %s (%lu) | Bits: %d | Proto: %d | Delay: %u us | RSSI: %d dBm\n",
                sig.hexCode, (unsigned long)value, bitLength, protocol, delay, rssi);
}

bool RFManager::transmitCode(uint32_t code, uint8_t bitLength, uint8_t protocol,
                             uint16_t pulseLength, uint8_t repeats, float freq) {
  if (code == 0) return false;

  rcSwitch.disableReceive();
  digitalWrite(PIN_LED_STATUS, LOW); // LED ON

  float targetFreq = (freq > 200.0f && freq < 1000.0f) ? freq : currentFreq;
  if (abs(targetFreq - currentFreq) > 0.005f) {
    ELECHOUSE_cc1101.setMHZ(targetFreq);
    currentFreq = targetFreq;
  }

  ELECHOUSE_cc1101.SetTx();
  rcSwitch.enableTransmit(PIN_CC1101_GDO0);
  rcSwitch.setProtocol(protocol > 0 ? protocol : 1);
  if (pulseLength > 0) {
    rcSwitch.setPulseLength(pulseLength);
  }
  rcSwitch.setRepeatTransmit(repeats > 0 ? repeats : DEFAULT_REPEAT_COUNT);

  rcSwitch.send(code, bitLength > 0 ? bitLength : DEFAULT_BIT_LENGTH);
  rcSwitch.disableTransmit();

  ELECHOUSE_cc1101.SetRx();
  rcSwitch.enableReceive(PIN_CC1101_GDO2);
  digitalWrite(PIN_LED_STATUS, HIGH); // LED OFF
  return true;
}

bool RFManager::transmitRaw(const uint16_t* timings, uint16_t count,
                            uint8_t repeats, float freq) {
  if (timings == nullptr || count <= 1) return false;

  rcSwitch.disableReceive();
  digitalWrite(PIN_LED_STATUS, LOW);

  float targetFreq = (freq > 200.0f && freq < 1000.0f) ? freq : currentFreq;
  if (abs(targetFreq - currentFreq) > 0.005f) {
    ELECHOUSE_cc1101.setMHZ(targetFreq);
    currentFreq = targetFreq;
  }

  ELECHOUSE_cc1101.SetTx();
  pinMode(PIN_CC1101_GDO0, OUTPUT);

  uint8_t repCount = (repeats > 0) ? repeats : DEFAULT_REPEAT_COUNT;
  for (uint8_t r = 0; r < repCount; r++) {
    bool level = HIGH;
    for (uint16_t i = 1; i < count; i++) {
      digitalWrite(PIN_CC1101_GDO0, level ? HIGH : LOW);
      delayMicroseconds(timings[i]);
      level = !level;
    }
    digitalWrite(PIN_CC1101_GDO0, LOW);
    delayMicroseconds(10000);
  }

  ELECHOUSE_cc1101.SetRx();
  rcSwitch.enableReceive(PIN_CC1101_GDO2);
  digitalWrite(PIN_LED_STATUS, HIGH);
  return true;
}

// ==============================================================================
// STREAM MODE (Continuous Transmission while button is held)
// ==============================================================================

bool RFManager::startStream(uint32_t code, uint8_t bitLength, uint8_t protocol,
                            uint16_t pulseLength, float freq) {
  if (code == 0) return false;

  stopFuzzer(); // Stop fuzzer if active

  streamState.active = true;
  streamState.code = code;
  streamState.bitLength = bitLength > 0 ? bitLength : DEFAULT_BIT_LENGTH;
  streamState.protocol = protocol > 0 ? protocol : DEFAULT_PROTOCOL;
  streamState.pulseLength = pulseLength > 0 ? pulseLength : DEFAULT_PULSE_LENGTH;
  streamState.frequency = (freq > 200.0f && freq < 1000.0f) ? freq : currentFreq;
  streamState.startTime = millis();
  streamState.lastBurstTime = 0;

  rcSwitch.disableReceive();
  if (abs(streamState.frequency - currentFreq) > 0.005f) {
    ELECHOUSE_cc1101.setMHZ(streamState.frequency);
    currentFreq = streamState.frequency;
  }
  ELECHOUSE_cc1101.SetTx();
  rcSwitch.enableTransmit(PIN_CC1101_GDO0);
  rcSwitch.setProtocol(streamState.protocol);
  rcSwitch.setPulseLength(streamState.pulseLength);
  rcSwitch.setRepeatTransmit(2);

  digitalWrite(PIN_LED_STATUS, LOW); // LED ON
  Serial.printf("[RF STREAM] Started stream for code 0x%06X (Hold mode active)\n", code);
  return true;
}

void RFManager::stopStream() {
  if (!streamState.active) return;
  streamState.active = false;
  rcSwitch.disableTransmit();
  ELECHOUSE_cc1101.SetRx();
  rcSwitch.enableReceive(PIN_CC1101_GDO2);
  digitalWrite(PIN_LED_STATUS, HIGH);
  Serial.println("[RF STREAM] Stopped stream. Returned to RX.");
}

bool RFManager::isStreaming() const {
  return streamState.active;
}

void RFManager::serviceStreaming() {
  uint32_t now = millis();
  // Safety Watchdog: auto-stop after 15 seconds continuous transmission
  if (now - streamState.startTime > 15000) {
    Serial.println("[RF STREAM] Safety watchdog timeout reached (15s). Auto-stopping.");
    stopStream();
    return;
  }

  // Send small repeated bursts continuously
  if (now - streamState.lastBurstTime >= 35) {
    streamState.lastBurstTime = now;
    rcSwitch.send(streamState.code, streamState.bitLength);
  }
}

// ==============================================================================
// FLIPPER ZERO STYLE FAST FUZZER / TEST MODE
// ==============================================================================

bool RFManager::startFuzzer(FuzzerMode mode, uint32_t startVal, uint32_t endVal, uint32_t step,
                           uint8_t proto, uint8_t bits, uint16_t pulse, float freq,
                           float startF, float endF, float stepF,
                           uint8_t repsPerCode, uint16_t delayMs) {
  stopStream(); // Ensure stream is off

  fuzzerState.mode = mode;
  fuzzerState.running = true;
  fuzzerState.startCode = startVal;
  fuzzerState.endCode = endVal;
  fuzzerState.currentCode = startVal;
  fuzzerState.step = step > 0 ? step : 1;
  fuzzerState.protocol = proto > 0 ? proto : 1;
  fuzzerState.bitLength = bits > 0 ? bits : 24;
  fuzzerState.pulseLength = pulse > 0 ? pulse : 350;
  fuzzerState.currentFreq = (freq > 200.0f) ? freq : currentFreq;
  fuzzerState.startFreq = startF > 200.0f ? startF : 433.00f;
  fuzzerState.endFreq = endF > 200.0f ? endF : 434.50f;
  fuzzerState.freqStep = stepF > 0.001f ? stepF : 0.05f;
  fuzzerState.repeatsPerStep = repsPerCode > 0 ? repsPerCode : 2;
  fuzzerState.stepDelayMs = delayMs > 5 ? delayMs : 25;
  fuzzerState.completedSteps = 0;

  if (mode == FUZZER_FREQ_SWEEP) {
    fuzzerState.currentFreq = fuzzerState.startFreq;
    fuzzerState.totalSteps = (uint32_t)((fuzzerState.endFreq - fuzzerState.startFreq) / fuzzerState.freqStep) + 1;
  } else if (mode == FUZZER_PROTO_SWEEP) {
    fuzzerState.totalSteps = 12;
    fuzzerState.protocol = 1;
  } else {
    fuzzerState.totalSteps = ((endVal - startVal) / fuzzerState.step) + 1;
  }

  rcSwitch.disableReceive();
  if (mode != FUZZER_FREQ_SWEEP && abs(fuzzerState.currentFreq - currentFreq) > 0.005f) {
    ELECHOUSE_cc1101.setMHZ(fuzzerState.currentFreq);
    currentFreq = fuzzerState.currentFreq;
  }
  ELECHOUSE_cc1101.SetTx();
  rcSwitch.enableTransmit(PIN_CC1101_GDO0);
  rcSwitch.setProtocol(fuzzerState.protocol);
  rcSwitch.setPulseLength(fuzzerState.pulseLength);
  rcSwitch.setRepeatTransmit(fuzzerState.repeatsPerStep);

  lastFuzzerStepTime = 0;
  Serial.printf("[RF FUZZER] Started Fuzzer Mode %d | Total Steps: %lu\n", mode, fuzzerState.totalSteps);
  return true;
}

void RFManager::stopFuzzer() {
  if (!fuzzerState.running) return;
  fuzzerState.running = false;
  fuzzerState.mode = FUZZER_IDLE;
  rcSwitch.disableTransmit();
  ELECHOUSE_cc1101.SetRx();
  rcSwitch.enableReceive(PIN_CC1101_GDO2);
  digitalWrite(PIN_LED_STATUS, HIGH);
  Serial.println("[RF FUZZER] Stopped Fuzzer. Returned to RX mode.");
}

FuzzerStatus RFManager::getFuzzerStatus() const {
  return fuzzerState;
}

void RFManager::serviceFuzzer() {
  uint32_t now = millis();
  if (now - lastFuzzerStepTime < fuzzerState.stepDelayMs) return;
  lastFuzzerStepTime = now;

  digitalWrite(PIN_LED_STATUS, LOW);

  if (fuzzerState.mode == FUZZER_FREQ_SWEEP) {
    ELECHOUSE_cc1101.setMHZ(fuzzerState.currentFreq);
    currentFreq = fuzzerState.currentFreq;
    rcSwitch.send(fuzzerState.currentCode > 0 ? fuzzerState.currentCode : 0xAAAAAA, fuzzerState.bitLength);

    fuzzerState.completedSteps++;
    fuzzerState.currentFreq += fuzzerState.freqStep;
    if (fuzzerState.currentFreq > fuzzerState.endFreq) {
      Serial.println("[RF FUZZER] Frequency sweep complete!");
      stopFuzzer();
      return;
    }
  } else if (fuzzerState.mode == FUZZER_PROTO_SWEEP) {
    rcSwitch.setProtocol(fuzzerState.protocol);
    rcSwitch.send(fuzzerState.currentCode, fuzzerState.bitLength);

    fuzzerState.completedSteps++;
    fuzzerState.protocol++;
    if (fuzzerState.protocol > 12) {
      Serial.println("[RF FUZZER] Protocol sweep complete!");
      stopFuzzer();
      return;
    }
  } else {
    // Standard Code / Brute-Force Sweep (12-bit, 10-bit, or custom)
    rcSwitch.send(fuzzerState.currentCode, fuzzerState.bitLength);

    fuzzerState.completedSteps++;
    if (fuzzerState.currentCode >= fuzzerState.endCode) {
      Serial.println("[RF FUZZER] Brute-force sweep complete!");
      stopFuzzer();
      return;
    }
    fuzzerState.currentCode += fuzzerState.step;
  }

  digitalWrite(PIN_LED_STATUS, HIGH);
}

void RFManager::setFrequency(float mhz) {
  if (mhz >= 300.0f && mhz <= 928.0f) {
    currentFreq = mhz;
    ELECHOUSE_cc1101.setMHZ(currentFreq);
    ELECHOUSE_cc1101.SetRx();
    Serial.printf("[RF] Frequency changed to %.2f MHz\n", currentFreq);
  }
}

float RFManager::getFrequency() const {
  return currentFreq;
}

void RFManager::setTxPower(int8_t power) {
  currentPower = power;
  ELECHOUSE_cc1101.setPA(currentPower);
  Serial.printf("[RF] TX Power set to %d dBm\n", currentPower);
}

int8_t RFManager::getTxPower() const {
  return currentPower;
}

int16_t RFManager::getLiveRSSI() {
  return ELECHOUSE_cc1101.getRssi();
}

bool RFManager::isConnected() {
  return ELECHOUSE_cc1101.getCC1101();
}

int RFManager::getSignalCount() const {
  return signalCount;
}

RecordedSignal* RFManager::getSignal(int index) {
  if (index >= 0 && index < signalCount) {
    return &signalBuffer[index];
  }
  return nullptr;
}

RecordedSignal* RFManager::getLatestSignal() {
  if (signalCount > 0) {
    return &signalBuffer[0];
  }
  return nullptr;
}

void RFManager::clearSignals() {
  signalCount = 0;
  memset(signalBuffer, 0, sizeof(signalBuffer));
  Serial.println("[RF] Cleared signal history buffer");
}

void RFManager::uintToBinaryStr(uint32_t val, uint8_t bits, char* outStr, size_t maxLen) {
  if (bits == 0 || bits > 32) bits = 24;
  if (maxLen <= (size_t)bits) return;

  for (int i = bits - 1; i >= 0; i--) {
    outStr[bits - 1 - i] = ((val >> i) & 1) ? '1' : '0';
  }
  outStr[bits] = '\0';
}

void RFManager::blinkLed(int times, int durationMs) {
  for (int i = 0; i < times; i++) {
    digitalWrite(PIN_LED_STATUS, LOW); // ON
    delay(durationMs);
    digitalWrite(PIN_LED_STATUS, HIGH); // OFF
    if (i < times - 1) delay(durationMs);
  }
}

void RFManager::startListening() {
  if (!listeningActive) {
    rcSwitch.enableReceive(PIN_CC1101_GDO2);
    listeningActive = true;
    Serial.println("[RF] Receiver enabled (Listening active)");
  }
}

void RFManager::stopListening() {
  if (listeningActive) {
    rcSwitch.disableReceive();
    listeningActive = false;
    Serial.println("[RF] Receiver disabled (Standby mode)");
  }
}

bool RFManager::isListening() const {
  return listeningActive;
}
