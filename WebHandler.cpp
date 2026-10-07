#include "WebHandler.h"

WebHandler webHandler;

WebHandler::WebHandler() : server(80) {}

void WebHandler::begin() {
  setupRoutes();
  server.begin();
  Serial.println("[Web] HTTP Server started on port 80");
}

void WebHandler::update() {
  server.handleClient();
}

void WebHandler::setupRoutes() {
  server.on("/", HTTP_GET, [this]() { handleRoot(); });

  // Captive Portal
  server.on("/generate_204", HTTP_GET, [this]() { handleCaptiveRedirect(); });
  server.on("/gen_204", HTTP_GET, [this]() { handleCaptiveRedirect(); });
  server.on("/hotspot-detect.html", HTTP_GET, [this]() { handleCaptiveRedirect(); });
  server.on("/ncsi.txt", HTTP_GET, [this]() { handleCaptiveRedirect(); });
  server.on("/connecttest.txt", HTTP_GET, [this]() { handleCaptiveRedirect(); });
  server.on("/canonical.html", HTTP_GET, [this]() { handleCaptiveRedirect(); });
  server.on("/success.txt", HTTP_GET, [this]() { handleCaptiveRedirect(); });

  // REST API Routes
  server.on("/api/status", HTTP_GET, [this]() { handleGetStatus(); });
  server.on("/api/signals", HTTP_GET, [this]() { handleGetSignals(); });
  server.on("/api/clear_signals", HTTP_POST, [this]() { handleClearSignals(); });
  server.on("/api/transmit", HTTP_POST, [this]() { handleTransmit(); });
  server.on("/api/presets", HTTP_GET, [this]() { handleGetPresets(); });
  server.on("/api/preset/add", HTTP_POST, [this]() { handleAddPreset(); });
  server.on("/api/preset/update", HTTP_POST, [this]() { handleUpdatePreset(); });
  server.on("/api/preset/trigger", HTTP_GET, [this]() { handleTriggerPreset(); });
  server.on("/api/preset/delete", HTTP_POST, [this]() { handleDeletePreset(); });
  server.on("/api/stream/start", HTTP_POST, [this]() { handleStreamStart(); });
  server.on("/api/stream/stop", HTTP_POST, [this]() { handleStreamStop(); });
  server.on("/api/fuzzer/start", HTTP_POST, [this]() { handleFuzzerStart(); });
  server.on("/api/fuzzer/stop", HTTP_POST, [this]() { handleFuzzerStop(); });
  server.on("/api/fuzzer/status", HTTP_GET, [this]() { handleFuzzerStatus(); });

  // IR API Routes
  server.on("/api/ir/transmit", HTTP_POST, [this]() { handleIRTransmit(); });
  server.on("/api/ir/latest", HTTP_GET, [this]() { handleIRLatest(); });
  server.on("/api/ir/clear", HTTP_POST, [this]() { handleIRClear(); });
  server.on("/api/ir/listen/start", HTTP_POST, [this]() { handleIRListenStart(); });
  server.on("/api/ir/listen/stop", HTTP_POST, [this]() { handleIRListenStop(); });
  server.on("/api/rf/listen/start", HTTP_POST, [this]() { handleRFListenStart(); });
  server.on("/api/rf/listen/stop", HTTP_POST, [this]() { handleRFListenStop(); });

  // Settings & WiFi
  server.on("/api/rf/config", HTTP_POST, [this]() { handleRfConfig(); });
  server.on("/api/wifi/scan", HTTP_GET, [this]() { handleWifiScan(); });
  server.on("/api/wifi/connect", HTTP_POST, [this]() { handleWifiConnect(); });
  server.on("/api/wifi/reset", HTTP_POST, [this]() { handleWifiReset(); });
  server.on("/api/security", HTTP_POST, [this]() { handleSecurity(); });

  server.onNotFound([this]() {
    if (!server.uri().startsWith("/api/")) {
      handleCaptiveRedirect();
    } else {
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(404, "text/plain", "API Not Found");
    }
  });
}

void WebHandler::handleRoot() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.send_P(200, "text/html", PAGE_HTML);
}

void WebHandler::handleCaptiveRedirect() {
  String redirectUrl = "http://" + WiFi.softAPIP().toString() + "/";
  server.sendHeader("Location", redirectUrl, true);
  server.send(302, "text/plain", "Redirecting...");
}

void WebHandler::sendJson(int code, const String& jsonStr) {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(code, "application/json", jsonStr);
}

void WebHandler::sendJsonResponse(bool success, const String& message) {
  JsonDocument doc;
  doc["success"] = success;
  if (message.length() > 0) {
    if (success) doc["message"] = message;
    else doc["error"] = message;
  }
  String out;
  serializeJson(doc, out);
  sendJson(success ? 200 : 400, out);
}

void WebHandler::handleGetStatus() {
  JsonDocument doc;
  DeviceSettings& cfg = storage.getSettings();

  doc["cc1101_ok"] = rfManager.isConnected();
  doc["rf_freq"] = rfManager.getFrequency();
  doc["rf_power"] = rfManager.getTxPower();
  doc["sta_connected"] = wifiPortal.isStaConnected();
  doc["sta_ssid"] = wifiPortal.getStaSSID();
  doc["sta_ip"] = wifiPortal.getStaIP();
  doc["sta_rssi"] = wifiPortal.getStaRSSI();
  doc["ap_ip"] = wifiPortal.getApIP();
  doc["ap_ssid"] = cfg.apSsid;
  doc["ap_pass"] = cfg.apPass;
  doc["preset_count"] = storage.getPresetCount();
  doc["signal_count"] = rfManager.getSignalCount();
  doc["pin_enabled"] = cfg.pinEnabled;
  doc["is_streaming"] = rfManager.isStreaming();
  doc["is_fuzzing"] = rfManager.getFuzzerStatus().running;
  doc["rf_listening"] = rfManager.isListening();
  doc["ir_listening"] = irManager.isListening();
  doc["ir_signal_ready"] = irManager.isSignalAvailable();

  String out;
  serializeJson(doc, out);
  sendJson(200, out);
}

void WebHandler::handleGetSignals() {
  JsonDocument doc;
  JsonArray arr = doc.to<JsonArray>();

  int count = rfManager.getSignalCount();
  for (int i = 0; i < count; i++) {
    RecordedSignal* s = rfManager.getSignal(i);
    if (!s) continue;
    JsonObject obj = arr.add<JsonObject>();
    obj["id"] = s->id;
    obj["code"] = s->code;
    obj["hex"] = s->hexCode;
    obj["bin"] = s->binCode;
    obj["bits"] = s->bitLength;
    obj["proto"] = s->protocol;
    obj["pulse"] = s->pulseLength;
    obj["freq"] = s->frequency;
    obj["rssi"] = s->rssi;
    obj["repeats"] = s->repeatCount;
    obj["timestamp"] = s->timestamp;
  }

  String out;
  serializeJson(doc, out);
  sendJson(200, out);
}

void WebHandler::handleClearSignals() {
  rfManager.clearSignals();
  sendJsonResponse(true, "Signal history cleared");
}

void WebHandler::handleTransmit() {
  if (!server.hasArg("plain")) {
    sendJsonResponse(false, "Missing JSON payload");
    return;
  }

  JsonDocument doc;
  deserializeJson(doc, server.arg("plain"));

  uint32_t code = doc["code"] | 0;
  uint8_t bits = doc["bits"] | DEFAULT_BIT_LENGTH;
  uint8_t proto = doc["proto"] | DEFAULT_PROTOCOL;
  uint16_t pulse = doc["pulse"] | DEFAULT_PULSE_LENGTH;
  uint8_t reps = doc["reps"] | DEFAULT_REPEAT_COUNT;
  float freq = doc["freq"] | rfManager.getFrequency();

  if (code == 0) {
    sendJsonResponse(false, "Code cannot be 0");
    return;
  }

  bool ok = rfManager.transmitCode(code, bits, proto, pulse, reps, freq);
  sendJsonResponse(ok, ok ? "Transmitted successfully" : "Transmission failed");
}

void WebHandler::handleGetPresets() {
  JsonDocument doc;
  JsonArray arr = doc.to<JsonArray>();

  int count = storage.getPresetCount();
  for (int i = 0; i < count; i++) {
    GatePreset* p = storage.getPreset(i);
    if (!p) continue;
    JsonObject obj = arr.add<JsonObject>();
    obj["id"] = p->id;
    obj["name"] = p->name;
    obj["cat"] = p->category;
    obj["type"] = p->type;
    obj["code"] = p->code;
    obj["bits"] = p->bitLength;
    obj["proto"] = p->protocol;
    obj["pulse"] = p->pulseLength;
    obj["freq"] = p->frequency;
    obj["reps"] = p->repeats;
    obj["toggle"] = p->isToggle;
    obj["code_off"] = p->codeOff;
    obj["state"] = p->state;
    obj["raw"] = p->isRaw;
    obj["raw_data"] = p->rawData;
  }

  String out;
  serializeJson(doc, out);
  sendJson(200, out);
}

void WebHandler::handleAddPreset() {
  if (!server.hasArg("plain")) {
    sendJsonResponse(false, "Missing JSON payload");
    return;
  }

  JsonDocument doc;
  deserializeJson(doc, server.arg("plain"));

  GatePreset p;
  memset(&p, 0, sizeof(p));
  p.id = doc["id"] | (uint32_t)random(100000, 999999);
  const char* nameStr = doc["name"] | "Automation";
  strncpy(p.name, nameStr, sizeof(p.name) - 1);
  const char* catStr = doc["cat"] | "Gate";
  strncpy(p.category, catStr, sizeof(p.category) - 1);
  p.type = doc["type"] | 0; // 0 = RF, 1 = IR
  p.code = doc["code"] | 0;
  p.bitLength = doc["bits"] | DEFAULT_BIT_LENGTH;
  p.protocol = doc["proto"] | DEFAULT_PROTOCOL;
  p.pulseLength = doc["pulse"] | DEFAULT_PULSE_LENGTH;
  p.frequency = doc["freq"] | (p.type == 1 ? 38.0f : rfManager.getFrequency());
  p.repeats = doc["reps"] | DEFAULT_REPEAT_COUNT;
  p.isToggle = doc["toggle"] | false;
  p.codeOff = doc["code_off"] | 0;
  p.state = false;
  p.isRaw = doc["raw"] | false;
  if (doc.containsKey("raw_data")) {
    const char* rd = doc["raw_data"];
    if (rd) strncpy(p.rawData, rd, sizeof(p.rawData) - 1);
  }

  bool ok = storage.addPreset(p);
  sendJsonResponse(ok, ok ? "Preset saved" : "Max presets reached");
}

void WebHandler::handleUpdatePreset() {
  if (!server.hasArg("plain")) {
    sendJsonResponse(false, "Missing JSON payload");
    return;
  }

  JsonDocument doc;
  deserializeJson(doc, server.arg("plain"));

  uint32_t id = doc["id"] | 0;
  if (id == 0) {
    sendJsonResponse(false, "Invalid preset ID");
    return;
  }

  GatePreset* existing = storage.getPresetById(id);
  if (!existing) {
    sendJsonResponse(false, "Preset not found");
    return;
  }

  GatePreset p = *existing;
  if (doc.containsKey("name")) strncpy(p.name, doc["name"] | p.name, sizeof(p.name) - 1);
  if (doc.containsKey("cat")) strncpy(p.category, doc["cat"] | p.category, sizeof(p.category) - 1);
  if (doc.containsKey("type")) p.type = doc["type"];
  if (doc.containsKey("code")) p.code = doc["code"];
  if (doc.containsKey("bits")) p.bitLength = doc["bits"];
  if (doc.containsKey("proto")) p.protocol = doc["proto"];
  if (doc.containsKey("pulse")) p.pulseLength = doc["pulse"];
  if (doc.containsKey("freq")) p.frequency = doc["freq"];
  if (doc.containsKey("reps")) p.repeats = doc["reps"];
  if (doc.containsKey("toggle")) p.isToggle = doc["toggle"];
  if (doc.containsKey("code_off")) p.codeOff = doc["code_off"];
  if (doc.containsKey("raw")) p.isRaw = doc["raw"];
  if (doc.containsKey("raw_data")) {
    const char* rd = doc["raw_data"];
    if (rd) strncpy(p.rawData, rd, sizeof(p.rawData) - 1);
  }

  bool ok = storage.updatePreset(p);
  sendJsonResponse(ok, ok ? "Preset updated" : "Failed to update preset");
}

void WebHandler::handleTriggerPreset() {
  if (!server.hasArg("id")) {
    sendJsonResponse(false, "Missing preset ID");
    return;
  }

  uint32_t id = server.arg("id").toInt();
  GatePreset* p = storage.getPresetById(id);
  if (!p) {
    sendJsonResponse(false, "Preset not found");
    return;
  }

  bool ok = false;
  uint32_t codeToTransmit = p->code;

  if (p->isToggle) {
    p->state = !p->state;
    codeToTransmit = p->state ? p->code : (p->codeOff > 0 ? p->codeOff : p->code);
    storage.updatePreset(*p);
  }

  if (p->type == 1) { // IR transmission
    uint8_t reps = p->repeats > 0 ? p->repeats : 3;
    if (p->isRaw && strlen(p->rawData) > 0) {
      uint16_t rawBuf[128];
      uint16_t count = 0;
      char copy[256];
      strncpy(copy, p->rawData, sizeof(copy) - 1);
      copy[sizeof(copy) - 1] = '\0';
      char* token = strtok(copy, ",");
      while (token != NULL && count < 128) {
        rawBuf[count++] = atoi(token);
        token = strtok(NULL, ",");
      }
      ok = irManager.transmitRaw(rawBuf, count, reps);
    } else if (p->protocol == 2) {
      ok = irManager.transmitSony(codeToTransmit, p->bitLength > 0 ? p->bitLength : 12, reps > 2 ? reps : 3);
    } else if (p->protocol == 3) {
      ok = irManager.transmitPanasonic(codeToTransmit, p->bitLength > 0 ? p->bitLength : 32, reps);
    } else {
      ok = irManager.transmitNEC(codeToTransmit, p->bitLength > 0 ? p->bitLength : 32, reps);
    }
  } else { // RF transmission
    ok = rfManager.transmitCode(codeToTransmit, p->bitLength, p->protocol,
                                 p->pulseLength, p->repeats, p->frequency);
  }

  JsonDocument doc;
  doc["success"] = ok;
  doc["state"] = p->state;
  doc["code_sent"] = codeToTransmit;
  String out;
  serializeJson(doc, out);
  sendJson(ok ? 200 : 400, out);
}

void WebHandler::handleDeletePreset() {
  uint32_t id = 0;
  if (server.hasArg("id")) {
    id = server.arg("id").toInt();
  } else if (server.hasArg("plain")) {
    JsonDocument doc;
    deserializeJson(doc, server.arg("plain"));
    id = doc["id"] | 0;
  }

  if (id == 0) {
    sendJsonResponse(false, "Invalid ID");
    return;
  }

  bool ok = storage.deletePreset(id);
  sendJsonResponse(ok, ok ? "Preset deleted" : "Preset not found");
}

void WebHandler::handleStreamStart() {
  uint32_t code = 0;
  uint8_t bits = DEFAULT_BIT_LENGTH;
  uint8_t proto = DEFAULT_PROTOCOL;
  uint16_t pulse = DEFAULT_PULSE_LENGTH;
  float freq = rfManager.getFrequency();

  if (server.hasArg("id")) {
    uint32_t id = server.arg("id").toInt();
    GatePreset* p = storage.getPresetById(id);
    if (p) {
      code = p->code;
      bits = p->bitLength;
      proto = p->protocol;
      pulse = p->pulseLength;
      freq = p->frequency;
    }
  } else if (server.hasArg("plain")) {
    JsonDocument doc;
    deserializeJson(doc, server.arg("plain"));
    code = doc["code"] | 0;
    bits = doc["bits"] | DEFAULT_BIT_LENGTH;
    proto = doc["proto"] | DEFAULT_PROTOCOL;
    pulse = doc["pulse"] | DEFAULT_PULSE_LENGTH;
    freq = doc["freq"] | rfManager.getFrequency();
  }

  if (code == 0) {
    sendJsonResponse(false, "Invalid code for streaming");
    return;
  }

  bool ok = rfManager.startStream(code, bits, proto, pulse, freq);
  sendJsonResponse(ok, ok ? "Streaming started" : "Failed to start streaming");
}

void WebHandler::handleStreamStop() {
  rfManager.stopStream();
  sendJsonResponse(true, "Streaming stopped");
}

void WebHandler::handleFuzzerStart() {
  if (!server.hasArg("plain")) {
    sendJsonResponse(false, "Missing payload");
    return;
  }

  JsonDocument doc;
  deserializeJson(doc, server.arg("plain"));

  int modeInt = doc["mode"] | 1;
  FuzzerMode mode = (FuzzerMode)modeInt;
  uint32_t startVal = doc["start"] | 0;
  uint32_t endVal = doc["end"] | 4095;
  uint32_t step = doc["step"] | 1;
  uint8_t proto = doc["proto"] | 1;
  uint8_t bits = doc["bits"] | 12;
  uint16_t pulse = doc["pulse"] | 350;
  float freq = doc["freq"] | rfManager.getFrequency();
  float startF = doc["start_freq"] | 433.00f;
  float endF = doc["end_freq"] | 434.50f;
  float stepF = doc["step_freq"] | 0.05f;
  uint8_t reps = doc["reps"] | 2;
  uint16_t delayMs = doc["delay"] | 25;

  bool ok = rfManager.startFuzzer(mode, startVal, endVal, step, proto, bits, pulse,
                                  freq, startF, endF, stepF, reps, delayMs);
  sendJsonResponse(ok, ok ? "Fuzzer started" : "Failed to start fuzzer");
}

void WebHandler::handleFuzzerStop() {
  rfManager.stopFuzzer();
  sendJsonResponse(true, "Fuzzer stopped");
}

void WebHandler::handleFuzzerStatus() {
  FuzzerStatus st = rfManager.getFuzzerStatus();
  JsonDocument doc;

  doc["running"] = st.running;
  doc["mode"] = (int)st.mode;
  doc["current_code"] = st.currentCode;
  char hexBuf[16];
  snprintf(hexBuf, sizeof(hexBuf), "0x%X", st.currentCode);
  doc["current_hex"] = hexBuf;
  doc["current_freq"] = st.currentFreq;
  doc["protocol"] = st.protocol;
  doc["completed"] = st.completedSteps;
  doc["total"] = st.totalSteps;
  float pct = (st.totalSteps > 0) ? ((float)st.completedSteps / (float)st.totalSteps) * 100.0f : 0.0f;
  if (pct > 100.0f) pct = 100.0f;
  doc["percent"] = pct;

  String out;
  serializeJson(doc, out);
  sendJson(200, out);
}

// Infrared Handlers
void WebHandler::handleIRTransmit() {
  if (!server.hasArg("plain")) {
    sendJsonResponse(false, "Missing payload");
    return;
  }

  JsonDocument doc;
  deserializeJson(doc, server.arg("plain"));

  uint8_t reps = doc["reps"] | 3;
  if (reps < 2) reps = 2; // Always send at least 2 repeats for appliance reliability!

  bool ok = false;
  if (doc.containsKey("raw") && doc["raw"].is<JsonArray>()) {
    JsonArray arr = doc["raw"];
    uint16_t count = arr.size();
    if (count > 0 && count <= 256) {
      uint16_t rawBuf[256];
      for (uint16_t i = 0; i < count; i++) {
        rawBuf[i] = arr[i];
      }
      ok = irManager.transmitRaw(rawBuf, count, reps);
      sendJsonResponse(ok, ok ? "Raw IR transmitted" : "Raw IR transmission failed");
      return;
    }
  }

  const char* proto = doc["proto"] | "NEC";
  if (strcasecmp(proto, "Raw") == 0) {
    RecordedIR latest = irManager.getLatestSignal();
    ok = irManager.transmitRaw(latest.rawTimings, latest.rawCount, reps);
    sendJsonResponse(ok, ok ? "Raw IR transmitted" : "Raw IR transmission failed");
    return;
  }

  uint32_t code = doc["code"] | 0;
  uint8_t bits = doc["bits"] | 32;

  if (strcasecmp(proto, "Sony") == 0) {
    ok = irManager.transmitSony(code, bits, reps > 2 ? reps : 3);
  } else if (strcasecmp(proto, "Panasonic") == 0 || strcasecmp(proto, "Kaseikyo") == 0) {
    ok = irManager.transmitPanasonic(code, bits, reps);
  } else {
    ok = irManager.transmitNEC(code, bits, reps);
  }

  sendJsonResponse(ok, ok ? "IR code transmitted" : "Transmission failed");
}

void WebHandler::handleIRLatest() {
  RecordedIR ir = irManager.getLatestSignal();
  JsonDocument doc;

  doc["ready"] = irManager.isSignalAvailable();
  doc["code"] = ir.code;
  doc["hex"] = ir.hexCode;
  doc["proto"] = ir.protocol;
  doc["bits"] = ir.bitLength;
  doc["timestamp"] = ir.timestamp;
  doc["raw_count"] = ir.rawCount;

  JsonArray rawArr = doc["raw"].to<JsonArray>();
  for (uint16_t i = 0; i < ir.rawCount && i < 150; i++) {
    rawArr.add(ir.rawTimings[i]);
  }

  String out;
  serializeJson(doc, out);
  sendJson(200, out);
}

void WebHandler::handleIRClear() {
  irManager.clearSignal();
  sendJsonResponse(true, "IR signal cleared");
}

void WebHandler::handleIRListenStart() {
  irManager.startListening();
  sendJsonResponse(true, "IR listening started");
}

void WebHandler::handleIRListenStop() {
  irManager.stopListening();
  sendJsonResponse(true, "IR listening stopped");
}

void WebHandler::handleRFListenStart() {
  rfManager.startListening();
  sendJsonResponse(true, "RF listening started");
}

void WebHandler::handleRFListenStop() {
  rfManager.stopListening();
  sendJsonResponse(true, "RF listening stopped");
}

void WebHandler::handleRfConfig() {
  if (!server.hasArg("plain")) {
    sendJsonResponse(false, "Missing payload");
    return;
  }

  JsonDocument doc;
  deserializeJson(doc, server.arg("plain"));

  float freq = doc["freq"] | rfManager.getFrequency();
  int8_t power = doc["power"] | rfManager.getTxPower();
  uint8_t reps = doc["repeats"] | DEFAULT_REPEAT_COUNT;

  rfManager.setFrequency(freq);
  rfManager.setTxPower(power);

  DeviceSettings& cfg = storage.getSettings();
  cfg.rfFreq = freq;
  cfg.rfPower = power;
  cfg.rfRepeats = reps;
  storage.saveSettings();

  sendJsonResponse(true, "RF configuration updated");
}

void WebHandler::handleWifiScan() {
  String jsonStr = wifiPortal.scanNetworksJson();
  sendJson(200, jsonStr);
}

void WebHandler::handleWifiConnect() {
  if (!server.hasArg("plain")) {
    sendJsonResponse(false, "Missing payload");
    return;
  }

  JsonDocument doc;
  deserializeJson(doc, server.arg("plain"));

  const char* ssid = doc["ssid"];
  const char* pass = doc["pass"] | "";

  if (!ssid || strlen(ssid) == 0) {
    sendJsonResponse(false, "SSID cannot be empty");
    return;
  }

  bool ok = wifiPortal.connectStation(ssid, pass);
  JsonDocument resp;
  resp["success"] = ok;
  if (ok) {
    resp["ip"] = wifiPortal.getStaIP();
    resp["message"] = "Connected to Wi-Fi successfully";
  } else {
    resp["error"] = "Failed to connect to Wi-Fi";
  }

  String out;
  serializeJson(resp, out);
  sendJson(ok ? 200 : 400, out);
}

void WebHandler::handleWifiReset() {
  wifiPortal.forgetStation();
  sendJsonResponse(true, "Wi-Fi credentials reset to Hotspot mode");
}

void WebHandler::handleSecurity() {
  if (!server.hasArg("plain")) {
    sendJsonResponse(false, "Missing payload");
    return;
  }

  JsonDocument doc;
  deserializeJson(doc, server.arg("plain"));

  DeviceSettings& cfg = storage.getSettings();
  cfg.pinEnabled = doc["pin_enable"] | false;
  const char* pin = doc["pin"] | "1234";
  strncpy(cfg.pinCode, pin, sizeof(cfg.pinCode) - 1);
  storage.saveSettings();

  sendJsonResponse(true, "Security settings updated");
}
