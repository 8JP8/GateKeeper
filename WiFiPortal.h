#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <DNSServer.h>
#include <ArduinoJson.h>
#include "Config.h"
#include "StorageManager.h"

class WiFiPortal {
public:
  WiFiPortal();
  void begin();
  void update();

  String scanNetworksJson();
  bool connectStation(const char* ssid, const char* pass);
  void forgetStation();

  bool isStaConnected() const;
  String getStaIP() const;
  String getApIP() const;
  String getStaSSID() const;
  int getStaRSSI() const;

private:
  DNSServer dnsServer;
  bool staConnected;
  bool dnsRunning;
  uint32_t lastCheckTime;
  void startAP();
};

extern WiFiPortal wifiPortal;
