#include "WiFiPortal.h"

WiFiPortal wifiPortal;

WiFiPortal::WiFiPortal() : staConnected(false), dnsRunning(false), lastCheckTime(0) {}

void WiFiPortal::begin() {
  DeviceSettings& cfg = storage.getSettings();

  Serial.println("[WiFi] Initializing Wi-Fi Manager...");

  if (strlen(cfg.wifiSsid) > 0) {
    WiFi.mode(WIFI_AP_STA);
    startAP();

    Serial.printf("[WiFi] Attempting connection to saved network: '%s'...\n", cfg.wifiSsid);
    WiFi.begin(cfg.wifiSsid, cfg.wifiPass);

    uint32_t startAttempt = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 10000) {
      delay(250);
      Serial.print(".");
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
      staConnected = true;
      Serial.printf("[WiFi] Connected to '%s'! Station IP: %s\n", cfg.wifiSsid, WiFi.localIP().toString().c_str());
    } else {
      staConnected = false;
      Serial.println("[WiFi] Connection to saved network timed out. Hotspot active.");
    }
  } else {
    WiFi.mode(WIFI_AP);
    startAP();
    Serial.println("[WiFi] Operating in standalone Hotspot mode.");
  }

  // Setup mDNS responder (gatekeeper.local)
  if (MDNS.begin(DEFAULT_MDNS_HOST)) {
    Serial.printf("[WiFi] mDNS responder started: http://%s.local\n", DEFAULT_MDNS_HOST);
    MDNS.addService("http", "tcp", 80);
  }
}

void WiFiPortal::startAP() {
  DeviceSettings& cfg = storage.getSettings();
  const char* ssid = strlen(cfg.apSsid) > 0 ? cfg.apSsid : DEFAULT_AP_SSID;
  const char* pass = strlen(cfg.apPass) >= 8 ? cfg.apPass : DEFAULT_AP_PASS;

  WiFi.softAP(ssid, pass);

  // Start captive portal DNS server answering all domain queries with AP IP (192.168.4.1)
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", WiFi.softAPIP());
  dnsRunning = true;

  Serial.printf("[WiFi] Hotspot active! SSID: '%s' | Password: '%s' | IP: %s\n",
                ssid, pass, WiFi.softAPIP().toString().c_str());
  Serial.println("[WiFi] Captive portal DNS server active on port 53 (all domains redirect here)");
}

void WiFiPortal::update() {
  if (dnsRunning) {
    dnsServer.processNextRequest();
  }

  uint32_t now = millis();
  if (now - lastCheckTime > 5000) {
    lastCheckTime = now;
    staConnected = (WiFi.status() == WL_CONNECTED);
  }
}

String WiFiPortal::scanNetworksJson() {
  Serial.println("[WiFi] Scanning available 2.4GHz networks...");
  int n = WiFi.scanNetworks();
  JsonDocument doc;
  JsonArray arr = doc.to<JsonArray>();

  for (int i = 0; i < n; i++) {
    JsonObject net = arr.add<JsonObject>();
    net["ssid"] = WiFi.SSID(i);
    net["rssi"] = WiFi.RSSI(i);
    net["open"] = (WiFi.encryptionType(i) == WIFI_AUTH_OPEN);
  }

  String output;
  serializeJson(doc, output);
  WiFi.scanDelete();
  return output;
}

bool WiFiPortal::connectStation(const char* ssid, const char* pass) {
  if (ssid == nullptr || strlen(ssid) == 0) return false;

  Serial.printf("[WiFi] Connecting to '%s'...\n", ssid);
  WiFi.mode(WIFI_AP_STA);
  startAP(); // Ensure SoftAP stays active!

  WiFi.disconnect();
  delay(100);
  WiFi.begin(ssid, pass);

  uint32_t startAttempt = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 15000) {
    delay(250);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    staConnected = true;
    DeviceSettings& cfg = storage.getSettings();
    strncpy(cfg.wifiSsid, ssid, sizeof(cfg.wifiSsid) - 1);
    strncpy(cfg.wifiPass, pass ? pass : "", sizeof(cfg.wifiPass) - 1);
    storage.saveSettings();
    Serial.printf("[WiFi] Successfully connected! IP: %s (Credentials saved)\n", WiFi.localIP().toString().c_str());
    return true;
  }

  staConnected = false;
  Serial.println("[WiFi] Connection failed.");
  return false;
}

void WiFiPortal::forgetStation() {
  storage.clearWifiCredentials();
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  startAP();
  staConnected = false;
  Serial.println("[WiFi] Reverted to Hotspot (SoftAP) only.");
}

bool WiFiPortal::isStaConnected() const {
  return staConnected;
}

String WiFiPortal::getStaIP() const {
  return staConnected ? WiFi.localIP().toString() : "Not Connected";
}

String WiFiPortal::getApIP() const {
  return WiFi.softAPIP().toString();
}

String WiFiPortal::getStaSSID() const {
  return staConnected ? WiFi.SSID() : "";
}

int WiFiPortal::getStaRSSI() const {
  return staConnected ? WiFi.RSSI() : 0;
}
