#pragma once
#include <Arduino.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "Config.h"
#include "StorageManager.h"
#include "RFManager.h"
#include "IRManager.h"
#include "WiFiPortal.h"
#include "WebPage.h"

class WebHandler {
public:
  WebHandler();
  void begin();
  void update();

private:
  WebServer server;

  void setupRoutes();
  void handleRoot();
  void handleCaptiveRedirect();
  void handleGetStatus();
  void handleGetSignals();
  void handleClearSignals();
  void handleTransmit();
  void handleGetPresets();
  void handleAddPreset();
  void handleUpdatePreset();
  void handleTriggerPreset();
  void handleDeletePreset();
  void handleStreamStart();
  void handleStreamStop();
  void handleFuzzerStart();
  void handleFuzzerStop();
  void handleFuzzerStatus();
  void handleIRTransmit();
  void handleIRLatest();
  void handleIRClear();
  void handleIRListenStart();
  void handleIRListenStop();
  void handleRFListenStart();
  void handleRFListenStop();
  void handleRfConfig();
  void handleWifiScan();
  void handleWifiConnect();
  void handleWifiReset();
  void handleSecurity();

  void sendJson(int code, const String& jsonStr);
  void sendJsonResponse(bool success, const String& message = "");
};

extern WebHandler webHandler;
