#pragma once
#include <Arduino.h>
#include "Config.h"

struct RecordedIR {
  uint32_t id;
  char protocol[16]; // "NEC", "Sony", "Raw", "IR Remote"
  uint32_t code;
  uint8_t bitLength;
  char hexCode[12];
  uint32_t timestamp;
  uint16_t rawCount;
  uint16_t rawTimings[256];
};

class IRManager {
public:
  IRManager();
  void begin();
  void update();

  void startListening();
  void stopListening();
  bool isListening() const;

  bool transmitNEC(uint32_t code, uint8_t bits = 32, uint8_t repeats = 3);
  bool transmitSony(uint32_t code, uint8_t bits = 12, uint8_t repeats = 3);
  bool transmitPanasonic(uint32_t code, uint8_t bits = 32, uint8_t repeats = 2);
  bool transmitRaw(const uint16_t* timings, uint16_t count, uint8_t repeats = 2);

  bool isSignalAvailable() const;
  RecordedIR getLatestSignal() const;
  void clearSignal();

private:
  RecordedIR latestIR;
  bool signalAvailable;
  bool listeningActive;
  uint32_t lastRxTime;

  void send38kHzBurst(uint32_t durationUs);
  void sendSpace(uint32_t durationUs);
  void decodeCapturedPulses();
};

extern IRManager irManager;
