#include "StorageManager.h"

StorageManager storage;

StorageManager::StorageManager() : presetCount(0) {
  memset(&settings, 0, sizeof(settings));
  strncpy(settings.apSsid, DEFAULT_AP_SSID, sizeof(settings.apSsid) - 1);
  strncpy(settings.apPass, DEFAULT_AP_PASS, sizeof(settings.apPass) - 1);
  settings.rfFreq = DEFAULT_RF_FREQ;
  settings.rfPower = DEFAULT_RF_POWER;
  settings.rfRepeats = DEFAULT_REPEAT_COUNT;
  settings.pinEnabled = false;
  strncpy(settings.pinCode, "1234", sizeof(settings.pinCode) - 1);
}

bool StorageManager::begin() {
  if (!prefs.begin("gaterep", false)) {
    Serial.println("[Storage] Failed to open Preferences namespace");
    return false;
  }
  loadSettings();
  loadPresets();
  return true;
}

DeviceSettings& StorageManager::getSettings() {
  return settings;
}

void StorageManager::loadSettings() {
  String wSsid = prefs.getString("w_ssid", "");
  String wPass = prefs.getString("w_pass", "");
  String aSsid = prefs.getString("a_ssid", DEFAULT_AP_SSID);
  String aPass = prefs.getString("a_pass", DEFAULT_AP_PASS);
  float freq = prefs.getFloat("rf_freq", DEFAULT_RF_FREQ);
  int8_t power = prefs.getChar("rf_pwr", DEFAULT_RF_POWER);
  uint8_t reps = prefs.getUChar("rf_rep", DEFAULT_REPEAT_COUNT);
  bool pEn = prefs.getBool("p_en", false);
  String pCode = prefs.getString("p_code", "1234");

  strncpy(settings.wifiSsid, wSsid.c_str(), sizeof(settings.wifiSsid) - 1);
  strncpy(settings.wifiPass, wPass.c_str(), sizeof(settings.wifiPass) - 1);
  strncpy(settings.apSsid, aSsid.c_str(), sizeof(settings.apSsid) - 1);
  strncpy(settings.apPass, aPass.c_str(), sizeof(settings.apPass) - 1);
  settings.rfFreq = (freq > 200.0f && freq < 1000.0f) ? freq : DEFAULT_RF_FREQ;
  settings.rfPower = power;
  settings.rfRepeats = reps > 0 ? reps : DEFAULT_REPEAT_COUNT;
  settings.pinEnabled = pEn;
  strncpy(settings.pinCode, pCode.c_str(), sizeof(settings.pinCode) - 1);
}

void StorageManager::saveSettings() {
  prefs.putString("w_ssid", settings.wifiSsid);
  prefs.putString("w_pass", settings.wifiPass);
  prefs.putString("a_ssid", settings.apSsid);
  prefs.putString("a_pass", settings.apPass);
  prefs.putFloat("rf_freq", settings.rfFreq);
  prefs.putChar("rf_pwr", settings.rfPower);
  prefs.putUChar("rf_rep", settings.rfRepeats);
  prefs.putBool("p_en", settings.pinEnabled);
  prefs.putString("p_code", settings.pinCode);
  Serial.println("[Storage] Settings saved to NVS");
}

void StorageManager::loadPresets() {
  presetCount = 0;
  String jsonStr = prefs.getString("presets", "");
  if (jsonStr.length() == 0) return;

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, jsonStr);
  if (error) return;

  JsonArray arr = doc.as<JsonArray>();
  for (JsonObject obj : arr) {
    if (presetCount >= MAX_PRESETS) break;
    GatePreset& p = presets[presetCount];
    p.id = obj["id"] | (uint32_t)random(100000, 999999);
    strncpy(p.name, obj["name"] | "Remote", sizeof(p.name) - 1);
    strncpy(p.category, obj["cat"] | "Gate", sizeof(p.category) - 1);
    p.type = obj["type"] | 0; // 0 = RF, 1 = IR
    p.code = obj["code"] | 0;
    p.bitLength = obj["bits"] | DEFAULT_BIT_LENGTH;
    p.protocol = obj["proto"] | DEFAULT_PROTOCOL;
    p.pulseLength = obj["pulse"] | DEFAULT_PULSE_LENGTH;
    p.frequency = obj["freq"] | DEFAULT_RF_FREQ;
    p.repeats = obj["reps"] | DEFAULT_REPEAT_COUNT;
    p.isToggle = obj["toggle"] | false;
    p.codeOff = obj["code_off"] | 0;
    p.state = obj["state"] | false;
    p.isRaw = obj["raw"] | false;
    strncpy(p.rawData, obj["raw_data"] | "", sizeof(p.rawData) - 1);
    presetCount++;
  }
}

void StorageManager::savePresets() {
  JsonDocument doc;
  JsonArray arr = doc.to<JsonArray>();

  for (int i = 0; i < presetCount; i++) {
    JsonObject obj = arr.add<JsonObject>();
    obj["id"] = presets[i].id;
    obj["name"] = presets[i].name;
    obj["cat"] = presets[i].category;
    obj["type"] = presets[i].type;
    obj["code"] = presets[i].code;
    obj["bits"] = presets[i].bitLength;
    obj["proto"] = presets[i].protocol;
    obj["pulse"] = presets[i].pulseLength;
    obj["freq"] = presets[i].frequency;
    obj["reps"] = presets[i].repeats;
    obj["toggle"] = presets[i].isToggle;
    obj["code_off"] = presets[i].codeOff;
    obj["state"] = presets[i].state;
    obj["raw"] = presets[i].isRaw;
    if (presets[i].isRaw) {
      obj["raw_data"] = presets[i].rawData;
    }
  }

  String jsonStr;
  serializeJson(doc, jsonStr);
  prefs.putString("presets", jsonStr);
}

int StorageManager::getPresetCount() {
  return presetCount;
}

GatePreset* StorageManager::getPreset(int index) {
  if (index >= 0 && index < presetCount) {
    return &presets[index];
  }
  return nullptr;
}

GatePreset* StorageManager::getPresetById(uint32_t id) {
  for (int i = 0; i < presetCount; i++) {
    if (presets[i].id == id) {
      return &presets[i];
    }
  }
  return nullptr;
}

bool StorageManager::addPreset(const GatePreset& preset) {
  if (presetCount >= MAX_PRESETS) return false;
  presets[presetCount] = preset;
  if (presets[presetCount].id == 0) {
    presets[presetCount].id = (uint32_t)random(100000, 999999);
  }
  presetCount++;
  savePresets();
  return true;
}

bool StorageManager::updatePreset(const GatePreset& preset) {
  for (int i = 0; i < presetCount; i++) {
    if (presets[i].id == preset.id) {
      presets[i] = preset;
      savePresets();
      return true;
    }
  }
  return false;
}

bool StorageManager::deletePreset(uint32_t id) {
  int foundIdx = -1;
  for (int i = 0; i < presetCount; i++) {
    if (presets[i].id == id) {
      foundIdx = i;
      break;
    }
  }
  if (foundIdx < 0) return false;

  for (int i = foundIdx; i < presetCount - 1; i++) {
    presets[i] = presets[i + 1];
  }
  presetCount--;
  savePresets();
  return true;
}

void StorageManager::clearWifiCredentials() {
  settings.wifiSsid[0] = '\0';
  settings.wifiPass[0] = '\0';
  prefs.remove("w_ssid");
  prefs.remove("w_pass");
}

void StorageManager::resetToDefaults() {
  prefs.clear();
  loadSettings();
  presetCount = 0;
}
