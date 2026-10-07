#pragma once
#include <Arduino.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include "Config.h"

struct GatePreset {
  uint32_t id;
  char name[32];
  char category[24]; // "Gate", "Garage", "Lighting", "Blinds", "AC", "TV", "Other"
  uint8_t type;      // 0 = RF, 1 = IR
  uint32_t code;
  uint8_t bitLength;
  uint8_t protocol;
  uint16_t pulseLength;
  float frequency;
  uint8_t repeats;
  bool isToggle;
  uint32_t codeOff;
  bool state;
  bool isRaw;
  char rawData[256];
};

struct DeviceSettings {
  char wifiSsid[33];
  char wifiPass[65];
  char apSsid[33];
  char apPass[65];
  float rfFreq;
  int8_t rfPower;
  uint8_t rfRepeats;
  bool pinEnabled;
  char pinCode[9];
};

class StorageManager {
public:
  StorageManager();
  bool begin();

  DeviceSettings& getSettings();
  void saveSettings();

  int getPresetCount();
  GatePreset* getPreset(int index);
  GatePreset* getPresetById(uint32_t id);
  bool addPreset(const GatePreset& preset);
  bool updatePreset(const GatePreset& preset);
  bool deletePreset(uint32_t id);
  void savePresets();

  void clearWifiCredentials();
  void resetToDefaults();

private:
  Preferences prefs;
  DeviceSettings settings;
  GatePreset presets[MAX_PRESETS];
  int presetCount;

  void loadSettings();
  void loadPresets();
};

extern StorageManager storage;
