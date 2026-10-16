#pragma once
#include <Arduino.h>
#include <SmartRC_CC1101.h>
#include <RCSwitch.h>
#include "Config.h"

struct RecordedSignal {
  uint32_t id;
  uint32_t code;
  char hexCode[12];
  char binCode[36];
  uint8_t bitLength;
  uint8_t protocol;
  uint16_t pulseLength;
  float frequency;
  int16_t rssi;
  uint32_t timestamp;
  uint16_t repeatCount;
  bool isRaw;
  uint16_t rawCount;
  uint16_t rawTimings[128];
};

enum FuzzerMode {
  FUZZER_IDLE = 0,
  FUZZER_12BIT,
  FUZZER_10BIT,
  FUZZER_PROTO_SWEEP,
  FUZZER_FREQ_SWEEP,
  FUZZER_CUSTOM
};

struct FuzzerStatus {
  FuzzerMode mode;
  bool running;
  uint32_t currentCode;
  uint32_t startCode;
  uint32_t endCode;
  uint32_t step;
  uint8_t protocol;
  uint8_t bitLength;
  uint16_t pulseLength;
  float currentFreq;
  float startFreq;
  float endFreq;
  float freqStep;
  uint8_t repeatsPerStep;
  uint16_t stepDelayMs;
  uint32_t totalSteps;
  uint32_t completedSteps;
};

struct StreamState {
  bool active;
  uint32_t code;
  uint8_t bitLength;
  uint8_t protocol;
  uint16_t pulseLength;
  float frequency;
  uint32_t startTime;
  uint32_t lastBurstTime;
};

class RFManager {
public:
  RFManager();
  bool begin(float freq = DEFAULT_RF_FREQ, int8_t power = DEFAULT_RF_POWER);
  void update();

  bool transmitCode(uint32_t code, uint8_t bitLength, uint8_t protocol,
                    uint16_t pulseLength, uint8_t repeats, float freq = 0.0f);
  bool transmitRaw(const uint16_t* timings, uint16_t count,
                   uint8_t repeats, float freq = 0.0f);

  // Stream Mode (indefinite repeat while button is pressed)
  bool startStream(uint32_t code, uint8_t bitLength, uint8_t protocol,
                   uint16_t pulseLength, float freq = 0.0f);
  void stopStream();
  bool isStreaming() const;

  // Flipper Zero style Fast Fuzzer / Test Mode
  bool startFuzzer(FuzzerMode mode, uint32_t startVal, uint32_t endVal, uint32_t step,
                   uint8_t proto, uint8_t bits, uint16_t pulse, float freq,
                   float startF = 0, float endF = 0, float stepF = 0,
                   uint8_t repsPerCode = 2, uint16_t delayMs = 30);
  void stopFuzzer();
  FuzzerStatus getFuzzerStatus() const;

  void setFrequency(float mhz);
  float getFrequency() const;
  void setTxPower(int8_t power);
  int8_t getTxPower() const;

  int16_t getLiveRSSI();
  bool isConnected();

  int getSignalCount() const;
  RecordedSignal* getSignal(int index);
  RecordedSignal* getLatestSignal();
  void clearSignals();

  void startListening();
  void stopListening();
  bool isListening() const;

private:
  RCSwitch rcSwitch;
  bool listeningActive;
  float currentFreq;
  int8_t currentPower;
  bool cc1101Ok;
  uint32_t lastHealthCheckTime;
  uint8_t healthyCount;
  uint8_t unhealthyCount;

  RecordedSignal signalBuffer[MAX_RECORDED_SIGNALS];
  int signalCount;
  uint32_t lastReceiveTime;
  uint32_t lastReceivedCode;

  StreamState streamState;
  FuzzerStatus fuzzerState;
  uint32_t lastFuzzerStepTime;

  void processReceivedPacket();
  void serviceStreaming();
  void serviceFuzzer();
  void uintToBinaryStr(uint32_t val, uint8_t bits, char* outStr, size_t maxLen);
  void blinkLed(int times = 1, int durationMs = 30);
};

extern RFManager rfManager;
