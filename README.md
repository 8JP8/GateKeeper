# 🛡️ GateKeeper — Universal RF & IR Smart Home Controller

An all-in-one Sub-GHz (CC1101) and Infrared (IR) smart automation hub built for the **ESP32-C3 Super Mini**. GateKeeper turns an ultra-compact microcontroller into a versatile RF remote emulator, signal sniffer, brute-force fuzzer, and universal IR controller for TVs, air conditioners, soundbars, gates, and LED strips — completely self-contained with a responsive web dashboard, on-device AP/STA Wi-Fi, and zero cloud dependency.

---

## ⚡ Features

### 📡 Sub-GHz Radio Transceiver (CC1101)
* **Frequency Agility**: Configurable from 300 MHz to 928 MHz (default **433.92 MHz**).
* **Live RF Sniffer**: Real-time interrupt-based packet capture with protocol, bit length, pulse duration, and frequency logging.
* **Continuous Streaming & Burst**: Stream signals continuously on button-hold or fire rapid single bursts.
* **Flipper Zero-Style Sub-GHz Fuzzer**: Brute-force gates, garage doors, and RF toggles with customizable range, protocol, and step delays.
* **Power & Modulation**: Adjustable TX power up to +10 dBm with ASK/OOK modulation via SmartRC CC1101 driver.

### 🔴 Infrared (IR) Engine
* **Crystal-Locked 38.000 kHz Carrier**: Generated using ESP32-C3 hardware PWM (LEDC timer) at 1/3 duty cycle (85/255) for maximum optical power and TSOP/VS1838 bandpass compatibility.
* **Multi-Protocol Decoder**: Automatically recognizes:
  * **NEC** (32-bit standard)
  * **Kaseikyo / Panasonic / Midea** (3500µs / 1750µs header)
  * **Samsung** (4500µs / 4500µs)
  * **Sony SIRC** (12-bit / 15-bit / 20-bit)
* **Raw Pulse-Train Capture & Exact Replay**: Records full raw microsecond timings (up to 350 transitions with a 65ms frame window). Retransmits the exact physical signal with accurate polarity and inter-frame repeats.
* **Multi-Set Appliance Database**: Over 40 brands pre-programmed with code set pagination:
  * **Portuguese & Iberian Niche Brands**: Kunft (Worten), Mitsai (Worten), Silver (Portugal), TD Systems, Electronia (Rádio Popular), Selecline (Auchan), Cecotec, Denver, Engel/Axil, Vestel Universal (RC1910, RC4848, RC5).
  * **Major Televisions**: Panasonic Viera, Samsung, LG, Sony Bravia, Philips, TCL, Hisense, Xiaomi, Toshiba, Sharp, Pioneer, Vizio, JVC, Hitachi, Grundig, Telefunken, Thomson, Blaupunkt.
  * **Sound Systems & Audio**: Samsung Soundbar, LG Soundbar, Sony AV/Soundbar, JBL (Bar 2.1/5.1/MultiBeam), Bose (Solo, Wave, Soundbar 500/700), Yamaha, Denon, Marantz, Onkyo, Harman Kardon, Philips Audio, Pioneer VSX, Offbrand Chinese soundbars.
  * **RGB Lighting**: Ubiquitous 24-Key Magic RGB LED remote (all 16 colors + effects), 44-Key DIY RGB controller, 17-Key Mini Strip, Tuya Smart Life IR, Govee RGB/Dreamcolor, Mini USB controllers.
  * **Air Conditioners (AC)**: Gree, Midea, Daikin, Mitsubishi, LG Dual Inverter, Samsung WindFree, AUX, Haier, Carrier, Panasonic Inverter.
  * **Fans & Projectors**: Dyson Cool, Xiaomi Smart Fan, Midea, Honeywell, Epson, BenQ, Optoma, ViewSonic.
* **Rapid Code Tester & Auto-Scanner**: Cycle through code variants or auto-test Power codes every 2 seconds to instantly discover working signals for unknown appliances.

### 🌐 Web Interface & Smart Home Dashboard
* **Unified Dashboard**: Categorized tiles for Gate, Garage, Lighting, Blinds, TV, AC, Audio, and Custom Automations.
* **Mobile & Desktop Responsive**: Adaptable layouts with sticky header/footer navigation.
* **Instant Smartphone Connect**: Built-in QR code generator to connect directly to the Wi-Fi AP or launch the browser dashboard.
* **Bilingual Support**: Instant toggle between English (EN) and Portuguese (PT).
* **Fully Self-Contained**: HTML, CSS, JavaScript, and SVG icons embedded in PROGMEM (no external CDNs or internet access required).

---

## 🛠️ Hardware & Pinout

### ESP32-C3 Super Mini Pin Mapping

| Component | Pin | ESP32-C3 Pin | Header Position | Notes |
| :--- | :--- | :--- | :--- | :--- |
| **CC1101 VCC** | VCC | **3.3V** | Right Header, Pin 3 | ⚠️ **3.3V ONLY!** Do NOT use 5V |
| **CC1101 GND** | GND | **GND** | Right Header, Pin 2 | Common Ground |
| **CC1101 SCK** | SCLK | **GPIO 4** | Right Header, Pin 4 | Hardware SPI SCK |
| **CC1101 MISO**| SO   | **GPIO 5** | Left Header, Pin 1  | Hardware SPI MISO |
| **CC1101 MOSI**| SI   | **GPIO 6** | Left Header, Pin 2  | Hardware SPI MOSI |
| **CC1101 CSN** | CSN  | **GPIO 7** | Left Header, Pin 3  | SPI Chip Select |
| **CC1101 GDO0**| GDO0 | **GPIO 3** | Right Header, Pin 5 | RF TX Modulation |
| **CC1101 GDO2**| GDO2 | **GPIO 2** | Right Header, Pin 6 | RF RX Demodulation |
| **IR TX** | S / Signal | **GPIO 10** | Left Header, Pin 6 | Hardware 38kHz PWM (LEDC) |
| **IR RX** | Out / Signal | **GPIO 0** | Right Header, Pin 8 | INPUT_PULLUP Active-LOW (VS1838B) |
| **Status LED** | Blue LED | **GPIO 8** | Left Header, Pin 4 | Onboard LED (Active LOW) |

### Power Supply Wiring
* **5V Power Input**: Connect USB-C or external 5V to the ESP32-C3 `5V` / `VBUS` pin.
* **IR Receiver (VS1838B / KY-022)**: Powered from `3.3V` or `5V`, with Ground connected to common `GND` and Output signal to `GPIO0`.
* **IR Transmitter LED**:
  * If using a 3-pin module with an onboard transistor: `VCC` -> `5V` (or `3.3V`), `GND` -> `GND`, `S` -> `GPIO10`.
  * If using a bare IR LED: Anode (+) -> `GPIO10` through a 100Ω–220Ω current-limiting resistor, Cathode (-) -> `GND`.

---

## 🚀 Getting Started

### 1. Requirements
* **Board**: ESP32-C3 Super Mini (`nologo_esp32c3_super_mini` or generic `esp32c3`).
* **Arduino Core**: `esp32` by Espressif Systems (v3.0.0 or higher recommended).
* **Libraries**:
  * `SmartRC-CC1101-Driver-Lib` (v3.0.0+)
  * `rc-switch` (v2.6.4+)
  * `ArduinoJson` (v7.x)
  * `WiFi`, `WebServer`, `DNSServer`, `ESPmDNS`, `Preferences` (built into ESP32 core)

### 2. Compilation & Upload (using `arduino-cli`)
```bash
# Compile firmware
arduino-cli compile --fqbn esp32:esp32:nologo_esp32c3_super_mini .

# Flash to ESP32-C3
arduino-cli upload -p COM12 --fqbn esp32:esp32:nologo_esp32c3_super_mini .
```

### 3. Connecting to the Web Portal
1. Connect to the Wi-Fi Access Point:
   * **SSID:** `ESP32-Gate-Remote`
   * **Password:** `GatePass1234`
2. Open your browser:
   * **Direct AP URL:** `http://192.168.4.1/`
   * **Local mDNS:** `http://gatekeeper.local`
3. In **Settings**, optionally connect the board to your home Wi-Fi network for LAN access.

---

## 📂 Project Architecture

```
GateKeeper/
├── Config.h             # Pin assignments, default RF/IR frequencies & limits
├── RadioRepeater.ino    # Main application setup, loop, and component initialization
├── RFManager.h/.cpp     # CC1101 driver abstraction, streaming, sniffing & fuzzer engine
├── IRManager.h/.cpp     # 38kHz hardware PWM, raw pulse capture & multi-protocol decoder
├── StorageManager.h/.cpp# Non-volatile NVS storage for presets and device configuration
├── WebHandler.h/.cpp    # REST API endpoints (/api/presets, /api/ir, /api/fuzzer, /api/rf)
├── WebPage.h            # Self-contained HTML5/CSS3/JS portal with appliance database
├── WiFiPortal.h/.cpp    # SoftAP + Station Wi-Fi manager with Captive Portal DNS
└── README.md            # Documentation
```

---

## 📜 License
This project is open-source and released under the MIT License.
