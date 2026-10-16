#pragma once
#include <Arduino.h>

const char PAGE_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no, viewport-fit=cover">
<meta name="apple-mobile-web-app-capable" content="yes">
<meta name="apple-mobile-web-app-status-bar-style" content="black-translucent">
<meta name="theme-color" content="#090d16">
<title>GateKeeper CC1101 + IR</title>
<style>
:root {
  --bg: #090d16;
  --panel: #111827;
  --card: #151f32;
  --border: #202d44;
  --text: #f8fafc;
  --muted: #94a3b8;
  --primary: #06b6d4;
  --primary-hover: #0891b2;
  --accent: #6366f1;
  --success: #10b981;
  --danger: #ef4444;
  --amber: #f59e0b;
}
* { box-sizing: border-box; margin: 0; padding: 0; font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, monospace, sans-serif; -webkit-tap-highlight-color: transparent; }
body {
  background: var(--bg);
  color: var(--text);
  min-height: 100vh;
  display: flex;
  flex-direction: column;
  padding-bottom: calc(75px + env(safe-area-inset-bottom, 0px));
}

/* Header */
.header {
  background: var(--panel);
  border-bottom: 1px solid var(--border);
  padding: 12px 16px;
  display: flex;
  justify-content: space-between;
  align-items: center;
  position: sticky;
  top: 0;
  z-index: 100;
}
.brand {
  display: flex;
  align-items: center;
  gap: 8px;
  font-weight: 800;
  font-size: 1.15rem;
  color: #fff;
}
.header-actions {
  display: flex;
  align-items: center;
  gap: 8px;
}
.status-pill {
  display: flex;
  gap: 6px;
}
.badge {
  font-size: 0.72rem;
  padding: 3px 8px;
  border-radius: 999px;
  background: #1e293b;
  color: var(--muted);
  border: 1px solid var(--border);
  display: flex;
  align-items: center;
  gap: 5px;
}
.badge.online { background: #064e3b; color: #a7f3d0; border-color: #059669; }
.badge.warn { background: #78350f; color: #fde68a; border-color: #d97706; }
.badge.live { background: #831843; color: #fbcfe8; border-color: #db2777; animation: blink 1s infinite; }
@keyframes blink { 50% { opacity: 0.5; } }

/* Desktop Navigation */
@media (min-width: 800px) {
  body { padding-bottom: 0; }
  .nav-bar-mobile { display: none !important; }
  .nav-bar-desktop {
    display: flex;
    background: var(--panel);
    border-bottom: 1px solid var(--border);
    padding: 0 16px;
    overflow-x: auto;
    position: sticky;
    top: 53px;
    z-index: 99;
  }
  .tab-btn {
    flex: initial;
    padding: 12px 16px;
    font-size: 0.85rem;
    flex-direction: row;
    gap: 8px;
    border-bottom: 2px solid transparent;
  }
  .tab-btn.active {
    border-bottom-color: var(--primary);
    background: rgba(6,182,212,0.08);
  }
  .grid-presets {
    display: grid;
    grid-template-columns: repeat(auto-fill, minmax(280px, 1fr));
    gap: 14px;
  }
}

/* Mobile Navigation */
@media (max-width: 799px) {
  .nav-bar-desktop { display: none !important; }
  .nav-bar-mobile {
    display: flex;
    position: fixed;
    bottom: 0;
    left: 0;
    right: 0;
    background: var(--panel);
    border-top: 1px solid var(--border);
    padding-bottom: env(safe-area-inset-bottom, 0px);
    z-index: 100;
    box-shadow: 0 -4px 20px rgba(0,0,0,0.5);
  }
  .grid-presets {
    display: flex;
    flex-direction: column;
    gap: 12px;
  }
}

.tab-btn {
  flex: 1;
  padding: 10px 4px;
  background: none;
  border: none;
  color: var(--muted);
  font-size: 0.72rem;
  font-weight: 600;
  cursor: pointer;
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 4px;
  transition: all 0.15s;
  white-space: nowrap;
}
.tab-btn .icon { font-size: 1.2rem; }
.tab-btn.active { color: var(--primary); }

.container {
  max-width: 960px;
  margin: 14px auto;
  padding: 0 12px;
  flex: 1 0 auto;
  width: 100%;
}
.tab-content { display: none; }
.tab-content.active { display: block; }

.card {
  background: var(--card);
  border: 1px solid var(--border);
  border-radius: 14px;
  padding: 16px;
  margin-bottom: 14px;
  box-shadow: 0 4px 16px rgba(0,0,0,0.25);
}
.card-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 12px;
  padding-bottom: 10px;
  border-bottom: 1px solid var(--border);
}
.card-title {
  font-size: 1rem;
  font-weight: 700;
  color: #fff;
  display: flex;
  align-items: center;
  gap: 8px;
}

/* Category Filter Chips */
.cat-chips {
  display: flex;
  gap: 6px;
  overflow-x: auto;
  padding-bottom: 8px;
  margin-bottom: 12px;
  scrollbar-width: none;
}
.cat-chips::-webkit-scrollbar { display: none; }
.cat-chip {
  padding: 6px 12px;
  border-radius: 20px;
  font-size: 0.75rem;
  font-weight: 600;
  background: #0c1220;
  color: var(--muted);
  border: 1px solid var(--border);
  cursor: pointer;
  white-space: nowrap;
  transition: all 0.15s;
}
.cat-chip.active {
  background: var(--primary);
  color: #000;
  border-color: var(--primary);
  font-weight: 700;
}

/* Preset & Device Cards */
.preset-box {
  background: #0f172a;
  border: 1px solid var(--border);
  border-radius: 14px;
  padding: 14px;
  display: flex;
  flex-direction: column;
  gap: 8px;
}
.preset-meta-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
}
.cat-badge {
  font-size: 0.68rem;
  padding: 2px 7px;
  border-radius: 6px;
  background: #1e293b;
  color: #38bdf8;
  border: 1px solid #334155;
  font-weight: 600;
}
.type-badge {
  font-size: 0.68rem;
  padding: 2px 7px;
  border-radius: 6px;
  background: #312e81;
  color: #c7d2fe;
  border: 1px solid #4338ca;
  font-weight: 600;
}

/* Action Buttons */
.btn-gate {
  width: 100%;
  padding: 14px 10px;
  background: linear-gradient(135deg, #0284c7, #0369a1);
  color: #fff;
  border: none;
  border-radius: 10px;
  font-size: 1rem;
  font-weight: 700;
  cursor: pointer;
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 4px;
  box-shadow: 0 4px 12px rgba(2,132,199,0.3);
  transition: transform 0.1s, filter 0.15s;
}
.btn-gate:active { transform: scale(0.98); filter: brightness(1.2); }

.btn-toggle {
  width: 100%;
  padding: 14px 12px;
  border-radius: 10px;
  font-size: 0.95rem;
  font-weight: 700;
  cursor: pointer;
  display: flex;
  justify-content: space-between;
  align-items: center;
  transition: all 0.2s;
  border: 1px solid var(--border);
}
.btn-toggle.on {
  background: linear-gradient(135deg, #059669, #047857);
  color: #fff;
  box-shadow: 0 4px 12px rgba(16,185,129,0.3);
}
.btn-toggle.off {
  background: #1e293b;
  color: var(--muted);
}
.toggle-switch-pill {
  padding: 3px 10px;
  border-radius: 12px;
  background: rgba(0,0,0,0.3);
  font-size: 0.75rem;
  font-weight: 800;
}

.btn-stream {
  width: 100%;
  padding: 9px;
  background: #1e1b4b;
  color: #a5b4fc;
  border: 1px solid #4338ca;
  border-radius: 8px;
  font-size: 0.76rem;
  font-weight: 700;
  cursor: pointer;
  transition: all 0.15s;
}
.btn-stream:active, .btn-stream.active {
  background: #dc2626 !important;
  color: #fff !important;
  border-color: #ef4444 !important;
}

.preset-card-footer {
  display: flex;
  justify-content: space-between;
  align-items: center;
  font-size: 0.7rem;
  color: var(--muted);
  margin-top: 4px;
  padding-top: 6px;
  border-top: 1px dashed #1e293b;
}

/* Common Buttons */
.btn {
  padding: 8px 14px;
  border-radius: 8px;
  font-size: 0.82rem;
  font-weight: 600;
  cursor: pointer;
  border: none;
  display: inline-flex;
  align-items: center;
  justify-content: center;
  gap: 6px;
  transition: all 0.15s;
}
.btn:active { transform: scale(0.97); }
.btn-primary { background: var(--primary); color: #000; }
.btn-success { background: var(--success); color: #fff; }
.btn-danger { background: var(--danger); color: #fff; }
.btn-warning { background: var(--amber); color: #000; }
.btn-accent { background: var(--accent); color: #fff; }
.btn-outline { background: transparent; border: 1px solid var(--border); color: var(--text); }
.btn-sm { padding: 4px 10px; font-size: 0.74rem; }

/* Form Controls */
.form-group { margin-bottom: 12px; }
.form-label { display: block; font-size: 0.75rem; font-weight: 600; color: var(--muted); margin-bottom: 5px; }
.form-control {
  width: 100%;
  padding: 9px 12px;
  background: #0c1220;
  border: 1px solid var(--border);
  border-radius: 8px;
  color: var(--text);
  font-size: 0.85rem;
  outline: none;
}
.form-control:focus { border-color: var(--primary); }
.grid-2 { display: grid; grid-template-columns: 1fr 1fr; gap: 10px; }
.grid-3 { display: grid; grid-template-columns: 1fr 1fr 1fr; gap: 8px; }

/* Sniffer Signals List */
.sig-card {
  background: #0f172a;
  border: 1px solid var(--border);
  border-radius: 10px;
  padding: 12px;
  margin-bottom: 10px;
}
.sig-header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 6px; }
.sig-val { font-family: monospace; font-size: 1.05rem; font-weight: 800; color: var(--primary); }
.sig-meta { display: flex; flex-wrap: wrap; gap: 8px 12px; font-size: 0.72rem; color: var(--muted); margin-bottom: 6px; }
.sig-meta strong { color: var(--text); }
.sig-bin { font-family: monospace; font-size: 0.65rem; color: #64748b; word-break: break-all; margin-bottom: 8px; }
.sig-actions-3 { display: grid; grid-template-columns: 1fr 1fr 1fr; gap: 6px; }
.btn-stream-sm {
  background: #1e1b4b;
  color: #a5b4fc;
  border: 1px solid #4338ca;
  border-radius: 8px;
  font-size: 0.74rem;
  font-weight: 700;
  cursor: pointer;
  padding: 5px;
}
.btn-stream-sm:active, .btn-stream-sm.active {
  background: #dc2626 !important;
  color: #fff !important;
  border-color: #ef4444 !important;
}

/* Fuzzer Progress */
.terminal-card {
  background: #030712;
  border: 1px solid var(--border);
  border-radius: 10px;
  padding: 12px;
  font-family: monospace;
  font-size: 0.8rem;
  margin-top: 10px;
}
.term-line { display: flex; justify-content: space-between; margin-bottom: 4px; }
.term-progress { height: 10px; background: #111827; border-radius: 5px; overflow: hidden; margin-top: 8px; }
.term-bar { height: 100%; width: 0%; background: linear-gradient(90deg, var(--primary), var(--accent)); transition: width 0.2s; }

/* IR Appliance Remote Keypad */
.appliance-selector-card {
  background: #0c1220;
  border: 1px solid var(--border);
  border-radius: 12px;
  padding: 14px;
  margin-bottom: 14px;
}
.remote-grid {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(135px, 1fr));
  gap: 8px;
  margin-top: 12px;
}
.ir-key-box {
  background: #111827;
  border: 1px solid var(--border);
  border-radius: 8px;
  padding: 8px;
  display: flex;
  flex-direction: column;
  justify-content: space-between;
  gap: 4px;
  text-align: center;
}
.ir-key-name { font-size: 0.78rem; font-weight: 700; color: #fff; }
.ir-key-hex { font-family: monospace; font-size: 0.65rem; color: var(--muted); }
.ir-key-actions { display: grid; grid-template-columns: 1fr auto; gap: 4px; margin-top: 2px; }

/* Pinout & Wiring Tables */
.wiring-table { width: 100%; border-collapse: collapse; margin-top: 8px; font-size: 0.82rem; }
.wiring-table th, .wiring-table td { padding: 9px 12px; border: 1px solid var(--border); text-align: left; }
.wiring-table th { background: #0c1220; color: var(--primary); font-weight: 700; }
.wiring-table tr:nth-child(even) { background: #0a0f1d; }
.pin-badge { font-family: monospace; font-size: 0.75rem; font-weight: 800; padding: 2px 7px; border-radius: 5px; color: #fff; display: inline-block; }
.pin-gpio { background: #0284c7; }
.pin-power { background: #dc2626; }
.pin-gnd { background: #334155; }

/* Modals */
.modal { display: none; position: fixed; top: 0; left: 0; right: 0; bottom: 0; background: rgba(0,0,0,0.8); z-index: 200; align-items: center; justify-content: center; padding: 16px; }
.modal.open { display: flex; }
.modal-content { background: var(--panel); border: 1px solid var(--border); border-radius: 16px; padding: 18px; max-width: 480px; width: 100%; max-height: 90vh; overflow-y: auto; }

/* QR Modal Specific */
.qr-container { display: flex; flex-direction: column; align-items: center; justify-content: center; padding: 14px; background: #fff; border-radius: 12px; margin: 14px auto; width: 220px; height: 220px; }
.qr-tabs { display: flex; gap: 6px; margin-bottom: 12px; }

/* Toast */
.toast { position: fixed; top: 15px; left: 50%; transform: translateX(-50%) translateY(-100px); background: #1e293b; border: 1px solid var(--primary); color: #fff; padding: 10px 18px; border-radius: 20px; box-shadow: 0 8px 30px rgba(0,0,0,0.6); font-size: 0.85rem; font-weight: 600; z-index: 1000; opacity: 0; transition: all 0.25s; pointer-events: none; width: max-content; max-width: 90%; text-align: center; }
.toast.show { transform: translateX(-50%) translateY(0); opacity: 1; }

.pulse-dot { width: 9px; height: 9px; border-radius: 50%; background: var(--success); display: inline-block; animation: pulse 1.5s infinite; }
@keyframes pulse { 0% { box-shadow: 0 0 0 0 rgba(16, 185, 129, 0.7); } 70% { box-shadow: 0 0 0 8px rgba(16, 185, 129, 0); } 100% { box-shadow: 0 0 0 0 rgba(16, 185, 129, 0); } }

/* Footer */
.footer {
  flex-shrink: 0;
  width: 100%;
  text-align: center;
  padding: 14px 16px;
  font-size: 0.72rem;
  color: var(--muted);
  border-top: 1px solid var(--border);
  background: var(--panel);
  margin-top: auto;
}
@media (orientation: landscape) and (max-height: 500px) {
  body { padding-bottom: 20px; }
  .footer { margin-top: auto; }
}
</style>
</head>
<body>

<!-- Header -->
<header class="header">
  <div class="brand">
    <span>🛡️</span>
    <span>GateKeeper CC1101 + IR</span>
  </div>
  <div class="header-actions">
    <button id="btn-lang" class="btn btn-outline btn-sm" onclick="toggleLanguage()">PT</button>
    <button class="btn btn-outline btn-sm" onclick="openQrModal()">📱 QR CODE</button>
    <div class="status-pill">
      <div class="badge" id="b-cc1101"><span>●</span> CC1101...</div>
      <div class="badge" id="b-freq">433.92 MHz</div>
      <div class="badge" id="b-stream" style="display:none;">LIVE STREAM</div>
    </div>
  </div>
</header>

<!-- Desktop Navigation Bar -->
<nav class="nav-bar-desktop">
  <button class="tab-btn active" onclick="switchTab('tab-dash', this)">
    <span class="icon">🏠</span>
    <span data-i18n="tab_dash">Dashboard</span>
  </button>
  <button class="tab-btn" onclick="switchTab('tab-ir', this)">
    <span class="icon">🔦</span>
    <span data-i18n="tab_ir">IR Remote</span>
  </button>
  <button class="tab-btn" onclick="switchTab('tab-sniff', this)">
    <span class="icon">📡</span>
    <span data-i18n="tab_sniff">Sniffer</span>
  </button>
  <button class="tab-btn" onclick="switchTab('tab-manual', this)">
    <span class="icon">⌨</span>
    <span data-i18n="tab_manual">Manual</span>
  </button>
  <button class="tab-btn" onclick="switchTab('tab-fuzzer', this)">
    <span class="icon">⚡</span>
    <span data-i18n="tab_fuzzer">Fuzzer</span>
  </button>
  <button class="tab-btn" onclick="switchTab('tab-wiring', this)">
    <span class="icon">🔌</span>
    <span data-i18n="tab_wiring">Wiring & About</span>
  </button>
  <button class="tab-btn" onclick="switchTab('tab-settings', this)">
    <span class="icon">⚙</span>
    <span data-i18n="tab_settings">Settings</span>
  </button>
</nav>

<div class="container">

  <!-- TAB: DASHBOARD (Unified Smart Home Hub) -->
  <div id="tab-dash" class="tab-content active">
    <div class="card">
      <div class="card-header">
        <span class="card-title"><span>🏠</span> <span data-i18n="dash_title">Smart Home Dashboard</span></span>
        <div style="display:flex; gap:6px;">
          <button class="btn btn-primary btn-sm" onclick="openAddDeviceModal()">➕ <span data-i18n="add_device">Add Device</span></button>
          <button class="btn btn-outline btn-sm" onclick="haptic(); fetchPresets()">↻</button>
        </div>
      </div>

      <!-- Category Filter Chips -->
      <div class="cat-chips">
        <button class="cat-chip active" onclick="filterCategory('All', this)" data-i18n="cat_all">All</button>
        <button class="cat-chip" onclick="filterCategory('Gate', this)">🚪 <span data-i18n="cat_gate">Gate</span></button>
        <button class="cat-chip" onclick="filterCategory('Garage', this)">🚘 <span data-i18n="cat_garage">Garage</span></button>
        <button class="cat-chip" onclick="filterCategory('Lighting', this)">💡 <span data-i18n="cat_lighting">Lighting</span></button>
        <button class="cat-chip" onclick="filterCategory('Blinds', this)">🪟 <span data-i18n="cat_blinds">Blinds</span></button>
        <button class="cat-chip" onclick="filterCategory('AC', this)">❄️ <span data-i18n="cat_ac">AC</span></button>
        <button class="cat-chip" onclick="filterCategory('TV', this)">📺 <span data-i18n="cat_tv">TV</span></button>
        <button class="cat-chip" onclick="filterCategory('Audio', this)">🔊 <span data-i18n="cat_audio">Audio</span></button>
        <button class="cat-chip" onclick="filterCategory('Fan', this)">🌀 <span data-i18n="cat_fan">Fan</span></button>
        <button class="cat-chip" onclick="filterCategory('Other', this)">⚡ <span data-i18n="cat_other">Other</span></button>
      </div>

      <div id="quick-presets-container" class="grid-presets">
        <p style="color:var(--muted); text-align:center; padding: 20px;" data-i18n="no_presets">Loading devices...</p>
      </div>
    </div>
  </div>

  <!-- TAB: IR REMOTES -->
  <div id="tab-ir" class="tab-content">
    <!-- On-Demand IR Sniffer / Learn Card -->
    <div class="card">
      <div class="card-header">
        <span class="card-title">
          <span id="ir-rx-dot" class="pulse-dot" style="background:#64748b;"></span>
          <span data-i18n="ir_sniffer_title">IR Signal Sniffer / Learn</span>
        </span>
        <span id="badge-ir-listen" class="badge">SLEEPING (OFF)</span>
      </div>
      <p style="color:var(--muted); font-size:0.8rem; margin-bottom:12px;" data-i18n="ir_sniffer_desc">
        Interrupt capture on GPIO0 is idle to save CPU. Click below to start listening for incoming signals.
      </p>

      <div style="display:flex; gap:8px; margin-bottom:12px;">
        <button id="btn-ir-listen-toggle" class="btn btn-warning btn-sm" onclick="toggleIRListening()" data-i18n="ir_btn_record">
          🔴 Start Recording IR
        </button>
        <button class="btn btn-outline btn-sm" onclick="haptic(); clearIRSignal()">
          🗑 <span data-i18n="clear_btn">Clear</span>
        </button>
      </div>

      <div id="ir-latest-container" style="background:#0c1220; border:1px solid var(--border); border-radius:10px; padding:14px;">
        <p style="color:var(--muted); text-align:center;" data-i18n="ir_waiting">Press 'Start Recording IR' then aim your remote at GPIO0.</p>
      </div>
    </div>

    <!-- Appliance Presets (Xiaomi Remote / LG Remote style) -->
    <div class="card">
      <div class="card-header">
        <span class="card-title"><span>📚</span> <span data-i18n="ir_brand_lookup">Appliance Brand Presets</span></span>
      </div>

      <div class="appliance-selector-card">
        <div class="grid-2">
          <div class="form-group">
            <label class="form-label" data-i18n="appliance_type">Appliance Type</label>
            <select class="form-control" id="ir-appliance-type" onchange="onApplianceTypeChanged(this.value)">
              <option value="tv">📺 Television (TV)</option>
              <option value="audio">🔊 Audio & Soundbar</option>
              <option value="rgb">💡 RGB LED Strip</option>
              <option value="ac">❄️ Air Conditioner (AC)</option>
              <option value="fan">🌀 Fan & Air</option>
              <option value="projector">📽 Projector</option>
              <option value="media">🎬 Streaming & Media</option>
            </select>
          </div>
          <div class="form-group">
            <label class="form-label" data-i18n="brand_name">Brand & Model</label>
            <select class="form-control" id="ir-brand-select" onchange="onBrandChanged()">
              <!-- Populated via JS -->
            </select>
          </div>
        </div>

        <!-- CODE SET / VARIANT PAGER & RAPID TESTER -->
        <div style="background:var(--card); border:1px solid var(--border); border-radius:10px; padding:12px; margin-bottom:14px;">
          <div style="display:flex; justify-content:space-between; align-items:center; flex-wrap:wrap; gap:8px; margin-bottom:10px;">
            <div style="display:flex; align-items:center; gap:8px; flex-wrap:wrap;">
              <span style="font-size:0.8rem; font-weight:700; color:var(--text);" data-i18n="code_set_label">Code Set:</span>
              <button class="btn btn-outline btn-sm" onclick="prevCodeSet()" title="Previous Code Set">◀ Prev</button>
              <select class="form-control form-control-sm" id="ir-codeset-select" onchange="onCodeSetSelected(parseInt(this.value))" style="width:auto; min-width:180px; padding:4px 8px; font-size:0.8rem;">
                <!-- Populated via JS -->
              </select>
              <button class="btn btn-outline btn-sm" onclick="nextCodeSet()" title="Next Code Set">Next ▶</button>
            </div>
            <span id="codeset-indicator" class="badge online" style="font-size:0.75rem;">Set 1 of 4</span>
          </div>
          
          <div style="display:flex; gap:8px; flex-wrap:wrap; align-items:center;">
            <button class="btn btn-primary btn-sm" onclick="testActivePowerCode()" style="flex:1; min-width:120px;">⚡ <span data-i18n="test_power">Test Power</span></button>
            <button class="btn btn-amber btn-sm" onclick="cycleNextSetAndTest()" style="flex:1; min-width:130px;">⏭ <span data-i18n="cycle_next">Next Set & Test</span></button>
            <button id="btn-auto-scanner" class="btn btn-outline btn-sm" onclick="toggleAutoScanner()" style="flex:1; min-width:130px;">🔄 <span data-i18n="auto_scan">Auto-Scan Sets</span></button>
          </div>
          <p id="scanner-status" style="font-size:0.72rem; color:var(--muted); margin-top:6px; display:none;"></p>
        </div>

        <div id="appliance-keypad-container" class="remote-grid">
          <!-- Populated via JS -->
        </div>
      </div>
    </div>
  </div>

  <!-- TAB: SNIFFER / RECORD -->
  <div id="tab-sniff" class="tab-content">
    <div class="card">
      <div class="card-header">
        <span class="card-title">
          <span class="pulse-dot"></span>
          <span data-i18n="live_sniffer">Live RF Sniffer</span>
        </span>
        <div style="display:flex; gap:6px;">
          <span id="badge-rf-listen" class="badge online">RECEIVER ACTIVE</span>
          <button class="btn btn-outline btn-sm" onclick="haptic(); clearSignals()">🗑 <span data-i18n="clear_btn">Clear</span></button>
        </div>
      </div>
      <p style="color:var(--muted); font-size:0.8rem; margin-bottom:12px;">
        <span data-i18n="sniff_desc">Listening continuously on</span> <strong id="sniff-freq-txt" style="color:var(--primary)">433.92 MHz</strong>.
      </p>
      <div id="signals-container">
        <p style="color:var(--muted); text-align:center; padding: 20px;" data-i18n="waiting_sig">Waiting for RF remote signal...</p>
      </div>
    </div>
  </div>

  <!-- TAB: MANUAL TRANSMITTER -->
  <div id="tab-manual" class="tab-content">
    <div class="card">
      <div class="card-header">
        <span class="card-title"><span>⌨</span> <span data-i18n="man_title">Manual RF Emulation</span></span>
      </div>
      <form id="manual-tx-form" onsubmit="transmitManual(event)">
        <div class="grid-2">
          <div class="form-group">
            <label class="form-label" data-i18n="man_code">Radio Code (Hex or Decimal)</label>
            <input class="form-control" id="man-code" placeholder="0x1A2B3C or 1715004" required>
          </div>
          <div class="form-group">
            <label class="form-label" data-i18n="man_bits">Bit Length</label>
            <input class="form-control" type="number" id="man-bits" value="24" min="1" max="32" required>
          </div>
        </div>
        <div class="grid-2">
          <div class="form-group">
            <label class="form-label" data-i18n="man_proto">Protocol (1 to 12)</label>
            <input class="form-control" type="number" id="man-proto" value="1" min="1" max="12" required>
          </div>
          <div class="form-group">
            <label class="form-label" data-i18n="man_pulse">Pulse Length (µs)</label>
            <input class="form-control" type="number" id="man-pulse" value="350" min="50" max="2000" required>
          </div>
        </div>
        <div class="grid-2">
          <div class="form-group">
            <label class="form-label" data-i18n="man_freq">Frequency (MHz)</label>
            <input class="form-control" type="number" step="0.01" id="man-freq" value="433.92" required>
          </div>
          <div class="form-group">
            <label class="form-label" data-i18n="man_reps">Repeat Transmissions</label>
            <input class="form-control" type="number" id="man-repeats" value="15" min="1" max="50" required>
          </div>
        </div>
        <div style="display:grid; grid-template-columns: 1fr 1fr; gap:10px; margin-top:8px;">
          <button type="submit" class="btn btn-primary" data-i18n="single_burst">🚀 Single Burst</button>
          <button type="button" class="btn btn-accent" onclick="saveManualAsPreset()" data-i18n="save_preset">💾 Save to Dashboard</button>
        </div>
        <button type="button" id="btn-manual-stream" class="btn btn-stream" style="margin-top:10px;">
          🔴 HOLD TO STREAM (Continuous TX)
        </button>
      </form>
    </div>
  </div>

  <!-- TAB: FUZZER -->
  <div id="tab-fuzzer" class="tab-content">
    <div class="card">
      <div class="card-header">
        <span class="card-title"><span>⚡</span> <span data-i18n="fuzzer_title">Sub-GHz Fuzzer / Brute-Force</span></span>
        <span id="fuzzer-badge" class="badge">IDLE</span>
      </div>
      <p style="color:var(--muted); font-size:0.8rem; margin-bottom:12px;" data-i18n="fuzzer_desc">
        Fast Sub-GHz protocol sweep, 12-bit / 10-bit gate brute-forcing, and frequency range fuzzer (Flipper Zero style).
      </p>

      <form onsubmit="startFuzzerAction(event)">
        <div class="form-group">
          <label class="form-label" data-i18n="fuzz_profile">Fuzzer Profile</label>
          <select class="form-control" id="fuzz-mode" onchange="adjustFuzzerMode(this.value)">
            <option value="1">12-Bit Brute-Force (Came TOP / Nice FLO: 0 - 4095)</option>
            <option value="2">10-Bit Brute-Force (Linear / Multi-Code: 0 - 1023)</option>
            <option value="3">Protocol Sweep (Test Protocols 1 to 12 with selected code)</option>
            <option value="4">Frequency Range Sweep (433.00 - 434.50 MHz)</option>
            <option value="5">Custom Code Range Sweep</option>
          </select>
        </div>

        <div class="grid-2" id="grp-fuzz-range">
          <div class="form-group">
            <label class="form-label">Start Code</label>
            <input class="form-control" id="fuzz-start" value="0">
          </div>
          <div class="form-group">
            <label class="form-label">End Code</label>
            <input class="form-control" id="fuzz-end" value="4095">
          </div>
        </div>

        <div class="grid-2" id="grp-fuzz-freq-range" style="display:none;">
          <div class="form-group">
            <label class="form-label">Start Freq (MHz)</label>
            <input class="form-control" type="number" step="0.01" id="fuzz-start-freq" value="433.00">
          </div>
          <div class="form-group">
            <label class="form-label">End Freq (MHz)</label>
            <input class="form-control" type="number" step="0.01" id="fuzz-end-freq" value="434.50">
          </div>
        </div>

        <div class="grid-2">
          <div class="form-group">
            <label class="form-label">Speed / Repeats per Code</label>
            <select class="form-control" id="fuzz-speed">
              <option value="1">⚡ Ultra Fast (1 repeat, 15ms gap)</option>
              <option value="2" selected>🚀 Fast (2 repeats, 25ms gap)</option>
              <option value="3">🎯 Reliable (3 repeats, 45ms gap)</option>
            </select>
          </div>
          <div class="form-group">
            <label class="form-label">Carrier Frequency (MHz)</label>
            <input class="form-control" type="number" step="0.01" id="fuzz-base-freq" value="433.92">
          </div>
        </div>

        <div style="display:grid; grid-template-columns: 1fr 1fr; gap:10px; margin-top:8px;">
          <button type="submit" id="btn-fuzz-start" class="btn btn-warning" data-i18n="fuzz_start">▶ START FUZZER</button>
          <button type="button" id="btn-fuzz-stop" class="btn btn-danger" onclick="stopFuzzerAction()" data-i18n="fuzz_stop">⏹ STOP</button>
        </div>
      </form>
    </div>

    <!-- Live Fuzzer Progress Card -->
    <div class="terminal-card">
      <div class="term-line">
        <span style="color:#38bdf8;">FUZZER STATUS:</span>
        <span id="term-status" style="color:var(--muted)">STANDBY</span>
      </div>
      <div class="term-line">
        <span>CURRENT CODE:</span>
        <strong id="term-code" style="color:var(--primary); font-size:1.1rem;">0x000</strong>
      </div>
      <div class="term-line">
        <span>FREQUENCY:</span>
        <span id="term-freq" style="color:#a7f3d0;">433.92 MHz</span>
      </div>
      <div class="term-line">
        <span>PROGRESS:</span>
        <span id="term-count">0 / 0 (0%)</span>
      </div>
      <div class="term-progress">
        <div id="term-bar" class="term-bar"></div>
      </div>
    </div>
  </div>

  <!-- TAB: WIRING & ABOUT -->
  <div id="tab-wiring" class="tab-content">
    <div class="card">
      <div class="card-header">
        <span class="card-title"><span>🔌</span> <span data-i18n="tab_wiring">Hardware Pinout & Wiring Guide</span></span>
      </div>
      <p style="color:var(--muted); font-size:0.85rem; line-height:1.5; margin-bottom:14px;" data-i18n="wiring_intro">
        All hardware components have dedicated, non-overlapping GPIOs on the ESP32-C3 Super Mini. They share common GND and 3.3V power rails, allowing CC1101 RF, IR Transmitter, and IR Receiver to remain wired simultaneously!
      </p>

      <!-- CC1101 Radio Pinout -->
      <h3 style="color:#38bdf8; font-size:0.95rem; margin:14px 0 6px 0;">1. 433MHz CC1101 Wireless Transceiver</h3>
      <table class="wiring-table">
        <thead>
          <tr><th>CC1101 Pin</th><th>ESP32-C3 Super Mini</th><th>Function & Description</th></tr>
        </thead>
        <tbody>
          <tr><td><strong>Pin 1 (VCC)</strong></td><td><span class="pin-badge pin-power">3.3V</span> (Right Pin 3)</td><td>Power Supply (3.3V ONLY, never 5V)</td></tr>
          <tr><td><strong>Pin 2 (GND)</strong></td><td><span class="pin-badge pin-gnd">GND</span> (Right Pin 2)</td><td>Ground Return</td></tr>
          <tr><td><strong>Pin 3 (SI / MOSI)</strong></td><td><span class="pin-badge pin-gpio">GPIO 6</span></td><td>SPI Data In</td></tr>
          <tr><td><strong>Pin 4 (SCLK)</strong></td><td><span class="pin-badge pin-gpio">GPIO 4</span></td><td>SPI Clock</td></tr>
          <tr><td><strong>Pin 5 (SO / MISO)</strong></td><td><span class="pin-badge pin-gpio">GPIO 5</span></td><td>SPI Data Out</td></tr>
          <tr><td><strong>Pin 6 (GDO2)</strong></td><td><span class="pin-badge pin-gpio">GPIO 2</span></td><td>RF Demodulated Data Input (RX)</td></tr>
          <tr><td><strong>Pin 7 (GDO0)</strong></td><td><span class="pin-badge pin-gpio">GPIO 3</span></td><td>RF Modulated Data Output (TX)</td></tr>
          <tr><td><strong>Pin 8 (CSN)</strong></td><td><span class="pin-badge pin-gpio">GPIO 7</span></td><td>SPI Chip Select</td></tr>
        </tbody>
      </table>

      <!-- IR Transmitter Pinout -->
      <h3 style="color:#38bdf8; font-size:0.95rem; margin:18px 0 6px 0;">2. Infrared (IR) Transmitter (KY-005 3-Pin Module)</h3>
      <p style="color:var(--muted); font-size:0.8rem; margin-bottom:6px;">
        Module with 3 pins: <strong>"-"</strong>, <strong>middle (blank)</strong>, and <strong>"S"</strong>.
      </p>
      <table class="wiring-table">
        <thead>
          <tr><th>Module Pin</th><th>ESP32-C3 Super Mini</th><th>Function & Wiring</th></tr>
        </thead>
        <tbody>
          <tr><td><strong>"-"</strong></td><td><span class="pin-badge pin-gnd">GND</span></td><td>Connect to Ground (Shared GND)</td></tr>
          <tr><td><strong>Middle (Blank)</strong></td><td><span class="pin-badge pin-gnd">NC</span></td><td>No Connection (Leave empty, not internally connected)</td></tr>
          <tr><td><strong>"S" (Signal)</strong></td><td><span class="pin-badge pin-gpio">GPIO 10</span></td><td>38kHz Modulated Carrier Output</td></tr>
        </tbody>
      </table>
      <div style="background:#0c1220; border-left:3px solid var(--amber); padding:10px; margin-top:8px; font-size:0.78rem; line-height:1.4;">
        <strong style="color:var(--amber);">⚠️ 5V Safety Note:</strong> The ESP32-C3 GPIO pins operate strictly at <strong>3.3V</strong> and are <strong>NOT 5V tolerant</strong>.
        Connecting KY-005 "S" pin directly to GPIO10 at 3.3V is 100% safe and effective for 2-4m.
        For high-power 5V driving, use an NPN transistor (2N2222): GPIO10 -> 1kΩ -> Base, Emitter -> GND, Collector -> LED cathode, LED anode -> 5V rail through 47Ω resistor.
      </div>

      <!-- IR Receiver Pinout -->
      <h3 style="color:#38bdf8; font-size:0.95rem; margin:18px 0 6px 0;">3. Infrared (IR) Receiver (VS1838B / KY-022 38kHz Demodulator)</h3>
      <p style="color:var(--muted); font-size:0.8rem; margin-bottom:6px;">
        IR receivers are <strong>digital demodulators</strong>. They filter the 38kHz optical bursts and output clean digital active-low pulses.
      </p>
      <table class="wiring-table">
        <thead>
          <tr><th>Receiver Pin</th><th>ESP32-C3 Super Mini</th><th>Function & Wiring</th></tr>
        </thead>
        <tbody>
          <tr><td><strong>OUT / S</strong></td><td><span class="pin-badge pin-gpio">GPIO 0</span></td><td>Digital IR Pulse Input (Interrupt-Driven on GPIO0)</td></tr>
          <tr><td><strong>VCC / +</strong></td><td><span class="pin-badge pin-power">3.3V</span></td><td>Power Supply (3.3V)</td></tr>
          <tr><td><strong>GND / -</strong></td><td><span class="pin-badge pin-gnd">GND</span></td><td>Ground (Shared)</td></tr>
        </tbody>
      </table>

      <!-- Status LED -->
      <h3 style="color:#38bdf8; font-size:0.95rem; margin:18px 0 6px 0;">4. Onboard Blue Status LED</h3>
      <p style="color:var(--muted); font-size:0.8rem;">
        Onboard ESP32-C3 Super Mini LED is on <strong>GPIO 8</strong> (Active LOW: blinks on RF and IR reception/transmission).
      </p>
    </div>
  </div>

  <!-- TAB: SETTINGS -->
  <div id="tab-settings" class="tab-content">
    <!-- WIFI CONFIG -->
    <div class="card">
      <div class="card-header">
        <span class="card-title"><span>📶</span> <span data-i18n="wifi_title">Wi-Fi Network</span></span>
        <button class="btn btn-outline btn-sm" onclick="openQrModal()">QR</button>
      </div>
      <div style="background:#0c1220; padding:10px; border-radius:8px; margin-bottom:12px; font-size:0.8rem; line-height:1.5;">
        <div><strong>Hotspot IP:</strong> <span id="txt-ap-ip" style="color:var(--primary)">192.168.4.1</span></div>
        <div><strong>Home LAN IP:</strong> <span id="txt-sta-ip" style="color:#38bdf8">Not Connected</span></div>
        <div><strong>Direct URL:</strong> <a href="http://gatekeeper.local" target="_blank" style="color:var(--accent)">http://gatekeeper.local</a></div>
      </div>
      <form onsubmit="connectWifi(event)">
        <div class="form-group">
          <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:4px;">
            <label class="form-label" style="margin:0;">Nearby Networks</label>
            <button type="button" class="btn btn-outline btn-sm" onclick="scanWifi()" data-i18n="wifi_scan">🔍 Scan</button>
          </div>
          <select class="form-control" id="wifi-ssid-select" onchange="document.getElementById('wifi-ssid-input').value = this.value">
            <option value="">-- Choose network or type below --</option>
          </select>
        </div>
        <div class="form-group">
          <label class="form-label">SSID</label>
          <input class="form-control" id="wifi-ssid-input" placeholder="Home Wi-Fi Name" required>
        </div>
        <div class="form-group">
          <label class="form-label">Password</label>
          <input class="form-control" type="password" id="wifi-pass-input" placeholder="Password">
        </div>
        <div style="display:grid; grid-template-columns: 1fr 1fr; gap:8px;">
          <button type="submit" class="btn btn-primary" data-i18n="wifi_connect">Connect & Save</button>
          <button type="button" class="btn btn-danger" onclick="forgetWifi()" data-i18n="wifi_forget">Forget Wi-Fi</button>
        </div>
      </form>
    </div>

    <!-- RF CONFIG -->
    <div class="card">
      <div class="card-header">
        <span class="card-title"><span>📡</span> <span data-i18n="rf_title">Radio Frequency</span></span>
      </div>
      <form onsubmit="saveRfConfig(event)">
        <div class="grid-2">
          <div class="form-group">
            <label class="form-label">Preset Band</label>
            <select class="form-control" id="cfg-freq-sel" onchange="toggleCustomFreq(this.value)">
              <option value="433.92">433.92 MHz (Standard EU)</option>
              <option value="315.00">315.00 MHz (US Standard)</option>
              <option value="433.42">433.42 MHz (Somfy RTS)</option>
              <option value="868.35">868.35 MHz (EU 868 Band)</option>
              <option value="custom">Custom Frequency...</option>
            </select>
          </div>
          <div class="form-group" id="grp-custom-freq" style="display:none;">
            <label class="form-label">Custom Frequency (MHz)</label>
            <input class="form-control" type="number" step="0.01" id="cfg-custom-freq" value="433.92">
          </div>
        </div>
        <div class="grid-2">
          <div class="form-group">
            <label class="form-label">TX Power</label>
            <select class="form-control" id="cfg-power">
              <option value="10">+10 dBm (Max Range)</option>
              <option value="7">+7 dBm</option>
              <option value="0">0 dBm</option>
            </select>
          </div>
          <div class="form-group">
            <label class="form-label">Default Repeats</label>
            <input class="form-control" type="number" id="cfg-repeats" value="15" min="1" max="50">
          </div>
        </div>
        <button type="submit" class="btn btn-primary" style="width:100%;" data-i18n="rf_save">Save RF Settings</button>
      </form>
    </div>
  </div>

</div>

<!-- FOOTER -->
<footer class="footer">
  <span data-i18n="footer_text">GateKeeper CC1101 + IR IoT Portal • ESP32 Super Mini</span>
</footer>

<!-- ADD DEVICE MODAL -->
<div id="add-device-modal" class="modal">
  <div class="modal-content">
    <div class="card-header">
      <span class="card-title"><span>➕</span> <span data-i18n="modal_add_title">Add Device to Dashboard</span></span>
      <button class="btn btn-outline btn-sm" onclick="closeAddDeviceModal()">✕</button>
    </div>
    <form onsubmit="submitAddDevice(event)">
      <div class="grid-2">
        <div class="form-group">
          <label class="form-label">Name</label>
          <input class="form-control" id="add-name" placeholder="e.g. Living Room TV, Driveway Gate" required>
        </div>
        <div class="form-group">
          <label class="form-label">Category</label>
          <select class="form-control" id="add-cat">
            <option value="Gate">🚪 Gate</option>
            <option value="Garage">🚘 Garage</option>
            <option value="Lighting">💡 Lighting</option>
            <option value="Blinds">🪟 Blinds</option>
            <option value="AC">❄️ AC</option>
            <option value="TV">📺 TV</option>
            <option value="Audio">🔊 Audio</option>
            <option value="Fan">🌀 Fan</option>
            <option value="Other">⚡ Other</option>
          </select>
        </div>
      </div>
      <div class="grid-2">
        <div class="form-group">
          <label class="form-label">Signal Type</label>
          <select class="form-control" id="add-type" onchange="toggleAddDeviceType(this.value)">
            <option value="0">📡 RF (433.92 MHz Sub-GHz)</option>
            <option value="1">🔦 IR (38 kHz Infrared)</option>
          </select>
        </div>
        <div class="form-group">
          <label class="form-label">Operation Mode</label>
          <select class="form-control" id="add-toggle" onchange="toggleAddCodeOffInput(this.value === '1')">
            <option value="0">Momentary (Single Action / Burst)</option>
            <option value="1">Toggle Switch (ON / OFF)</option>
          </select>
        </div>
      </div>
      <div class="grid-2">
        <div class="form-group">
          <label class="form-label">Code ON / Primary (Hex or Dec)</label>
          <input class="form-control" id="add-code" placeholder="0x1A2B3C or 1715004" required>
        </div>
        <div class="form-group" id="grp-add-code-off" style="display:none;">
          <label class="form-label">Code OFF (Hex or Dec)</label>
          <input class="form-control" id="add-code-off" placeholder="Optional alternate code">
        </div>
      </div>
      <div class="grid-2" id="grp-add-rf-props">
        <div class="form-group">
          <label class="form-label">Bit Length</label>
          <input class="form-control" type="number" id="add-bits" value="24" min="1" max="32">
        </div>
        <div class="form-group">
          <label class="form-label">Protocol (1 to 12)</label>
          <input class="form-control" type="number" id="add-proto" value="1" min="1" max="12">
        </div>
        <div class="form-group">
          <label class="form-label">Pulse (µs)</label>
          <input class="form-control" type="number" id="add-pulse" value="350">
        </div>
        <div class="form-group">
          <label class="form-label">Frequency (MHz)</label>
          <input class="form-control" type="number" step="0.01" id="add-freq" value="433.92">
        </div>
      </div>
      <div class="form-group">
        <label class="form-label">Repeats</label>
        <input class="form-control" type="number" id="add-repeats" value="15" min="1" max="50">
      </div>
      <div style="display:grid; grid-template-columns: 1fr 1fr; gap:10px; margin-top:10px;">
        <button type="submit" class="btn btn-success">➕ Save to Dashboard</button>
        <button type="button" class="btn btn-outline" onclick="closeAddDeviceModal()">Cancel</button>
      </div>
    </form>
  </div>
</div>

<!-- EDIT PRESET MODAL -->
<div id="edit-modal" class="modal">
  <div class="modal-content">
    <div class="card-header">
      <span class="card-title"><span>✏</span> Edit Remote / Automation</span>
      <button class="btn btn-outline btn-sm" onclick="closeEditModal()">✕</button>
    </div>
    <form onsubmit="submitEditPreset(event)">
      <input type="hidden" id="edit-id">
      <div class="grid-2">
        <div class="form-group">
          <label class="form-label">Name</label>
          <input class="form-control" id="edit-name" required>
        </div>
        <div class="form-group">
          <label class="form-label">Category</label>
          <select class="form-control" id="edit-cat">
            <option value="Gate">🚪 Gate</option>
            <option value="Garage">🚘 Garage</option>
            <option value="Lighting">💡 Lighting</option>
            <option value="Blinds">🪟 Blinds</option>
            <option value="AC">❄️ AC</option>
            <option value="TV">📺 TV</option>
            <option value="Audio">🔊 Audio</option>
            <option value="Fan">🌀 Fan</option>
            <option value="Other">⚡ Other</option>
          </select>
        </div>
      </div>
      <div class="grid-2">
        <div class="form-group">
          <label class="form-label">Type</label>
          <select class="form-control" id="edit-type">
            <option value="0">📡 RF (433.92 MHz)</option>
            <option value="1">🔦 IR (38 kHz Infrared)</option>
          </select>
        </div>
        <div class="form-group">
          <label class="form-label">Operation Mode</label>
          <select class="form-control" id="edit-toggle" onchange="toggleCodeOffInput(this.value === '1')">
            <option value="0">Momentary (Single Trigger)</option>
            <option value="1">Toggle Switch (ON / OFF)</option>
          </select>
        </div>
      </div>
      <div class="grid-2">
        <div class="form-group">
          <label class="form-label">Code ON / Primary (Hex or Dec)</label>
          <input class="form-control" id="edit-code" required>
        </div>
        <div class="form-group" id="grp-edit-code-off" style="display:none;">
          <label class="form-label">Code OFF (Hex or Dec)</label>
          <input class="form-control" id="edit-code-off" placeholder="Optional alternate code">
        </div>
      </div>
      <div class="grid-2">
        <div class="form-group">
          <label class="form-label">Repeats</label>
          <input class="form-control" type="number" id="edit-repeats" min="1" max="100" required>
        </div>
        <div class="form-group">
          <label class="form-label">Bit Length</label>
          <input class="form-control" type="number" id="edit-bits" min="1" max="32" required>
        </div>
      </div>
      <div class="grid-2">
        <div class="form-group">
          <label class="form-label">Protocol</label>
          <input class="form-control" type="number" id="edit-proto" min="1" max="12" required>
        </div>
        <div class="form-group">
          <label class="form-label">Pulse (µs) / Freq (MHz)</label>
          <div style="display:grid; grid-template-columns: 1fr 1fr; gap:6px;">
            <input class="form-control" type="number" id="edit-pulse" placeholder="Pulse" required>
            <input class="form-control" type="number" step="0.01" id="edit-freq" placeholder="Freq" required>
          </div>
        </div>
      </div>
      <div style="display:grid; grid-template-columns: 1fr 1fr; gap:10px; margin-top:10px;">
        <button type="submit" class="btn btn-success">Save Changes</button>
        <button type="button" class="btn btn-outline" onclick="closeEditModal()">Cancel</button>
      </div>
    </form>
  </div>
</div>

<!-- QR CODE MODAL -->
<div id="qr-modal" class="modal">
  <div class="modal-content" style="text-align:center;">
    <div class="card-header">
      <span class="card-title"><span>📱</span> <span data-i18n="qr_title">Quick Connect QR Code</span></span>
      <button class="btn btn-outline btn-sm" onclick="closeQrModal()">✕</button>
    </div>
    <div class="qr-tabs" style="justify-content:center;">
      <button class="btn btn-primary btn-sm" id="btn-qr-tab-wifi" onclick="switchQrMode('wifi')">📶 Wi-Fi Hotspot</button>
      <button class="btn btn-outline btn-sm" id="btn-qr-tab-url" onclick="switchQrMode('url')">🌐 Web URL</button>
    </div>
    <p id="qr-desc-txt" style="color:var(--muted); font-size:0.8rem; margin-bottom:8px;">
      Scan with your phone's camera to connect directly to Wi-Fi without typing the password!
    </p>
    <div class="qr-container">
      <div id="qrcode-box"></div>
    </div>
    <div style="margin-top:12px;">
      <button class="btn btn-outline btn-sm" style="width:100%;" onclick="closeQrModal()">Close</button>
    </div>
  </div>
</div>

<!-- Mobile Bottom Navigation Dock -->
<nav class="nav-bar-mobile">
  <button class="tab-btn active" onclick="switchTab('tab-dash', this)">
    <span class="icon">🏠</span>
    <span data-i18n="tab_dash">Dashboard</span>
  </button>
  <button class="tab-btn" onclick="switchTab('tab-ir', this)">
    <span class="icon">🔦</span>
    <span data-i18n="tab_ir">IR</span>
  </button>
  <button class="tab-btn" onclick="switchTab('tab-sniff', this)">
    <span class="icon">📡</span>
    <span data-i18n="tab_sniff">Sniffer</span>
  </button>
  <button class="tab-btn" onclick="switchTab('tab-manual', this)">
    <span class="icon">⌨</span>
    <span data-i18n="tab_manual">Manual</span>
  </button>
  <button class="tab-btn" onclick="switchTab('tab-fuzzer', this)">
    <span class="icon">⚡</span>
    <span data-i18n="tab_fuzzer">Fuzzer</span>
  </button>
  <button class="tab-btn" onclick="switchTab('tab-wiring', this)">
    <span class="icon">🔌</span>
    <span data-i18n="tab_wiring">Wiring</span>
  </button>
  <button class="tab-btn" onclick="switchTab('tab-settings', this)">
    <span class="icon">⚙</span>
    <span data-i18n="tab_settings">Settings</span>
  </button>
</nav>

<div id="toast" class="toast">Action completed</div>

<script>
var QRCode;!function(){function a(a){this.mode=c.MODE_8BIT_BYTE,this.data=a,this.parsedData=[];for(var b=[],d=0,e=this.data.length;e>d;d++){var f=this.data.charCodeAt(d);f>65536?(b[0]=240|(1835008&f)>>>18,b[1]=128|(258048&f)>>>12,b[2]=128|(4032&f)>>>6,b[3]=128|63&f):f>2048?(b[0]=224|(61440&f)>>>12,b[1]=128|(4032&f)>>>6,b[2]=128|63&f):f>128?(b[0]=192|(1984&f)>>>6,b[1]=128|63&f):b[0]=f,this.parsedData=this.parsedData.concat(b)}this.parsedData.length!=this.data.length&&(this.parsedData.unshift(191),this.parsedData.unshift(187),this.parsedData.unshift(239))}function b(a,b){this.typeNumber=a,this.errorCorrectLevel=b,this.modules=null,this.moduleCount=0,this.dataCache=null,this.dataList=[]}function i(a,b){if(void 0==a.length)throw new Error(a.length+"/"+b);for(var c=0;c<a.length&&0==a[c];)c++;this.num=new Array(a.length-c+b);for(var d=0;d<a.length-c;d++)this.num[d]=a[d+c]}function j(a,b){this.totalCount=a,this.dataCount=b}function k(){this.buffer=[],this.length=0}function m(){return"undefined"!=typeof CanvasRenderingContext2D}function n(){var a=!1,b=navigator.userAgent;return/android/i.test(b)&&(a=!0,aMat=b.toString().match(/android ([0-9]\.[0-9])/i),aMat&&aMat[1]&&(a=parseFloat(aMat[1]))),a}function r(a,b){for(var c=1,e=s(a),f=0,g=l.length;g>=f;f++){var h=0;switch(b){case d.L:h=l[f][0];break;case d.M:h=l[f][1];break;case d.Q:h=l[f][2];break;case d.H:h=l[f][3]}if(h>=e)break;c++}if(c>l.length)throw new Error("Too long data");return c}function s(a){var b=encodeURI(a).toString().replace(/\%[0-9a-fA-F]{2}/g,"a");return b.length+(b.length!=a?3:0)}a.prototype={getLength:function(){return this.parsedData.length},write:function(a){for(var b=0,c=this.parsedData.length;c>b;b++)a.put(this.parsedData[b],8)}},b.prototype={addData:function(b){var c=new a(b);this.dataList.push(c),this.dataCache=null},isDark:function(a,b){if(0>a||this.moduleCount<=a||0>b||this.moduleCount<=b)throw new Error(a+","+b);return this.modules[a][b]},getModuleCount:function(){return this.moduleCount},make:function(){this.makeImpl(!1,this.getBestMaskPattern())},makeImpl:function(a,c){this.moduleCount=4*this.typeNumber+17,this.modules=new Array(this.moduleCount);for(var d=0;d<this.moduleCount;d++){this.modules[d]=new Array(this.moduleCount);for(var e=0;e<this.moduleCount;e++)this.modules[d][e]=null}this.setupPositionProbePattern(0,0),this.setupPositionProbePattern(this.moduleCount-7,0),this.setupPositionProbePattern(0,this.moduleCount-7),this.setupPositionAdjustPattern(),this.setupTimingPattern(),this.setupTypeInfo(a,c),this.typeNumber>=7&&this.setupTypeNumber(a),null==this.dataCache&&(this.dataCache=b.createData(this.typeNumber,this.errorCorrectLevel,this.dataList)),this.mapData(this.dataCache,c)},setupPositionProbePattern:function(a,b){for(var c=-1;7>=c;c++)if(!(-1>=a+c||this.moduleCount<=a+c))for(var d=-1;7>=d;d++)-1>=b+d||this.moduleCount<=b+d||(this.modules[a+c][b+d]=c>=0&&6>=c&&(0==d||6==d)||d>=0&&6>=d&&(0==c||6==c)||c>=2&&4>=c&&d>=2&&4>=d?!0:!1)},getBestMaskPattern:function(){for(var a=0,b=0,c=0;8>c;c++){this.makeImpl(!0,c);var d=f.getLostPoint(this);(0==c||a>d)&&(a=d,b=c)}return b},createMovieClip:function(a,b,c){var d=a.createEmptyMovieClip(b,c),e=1;this.make();for(var f=0;f<this.modules.length;f++)for(var g=f*e,h=0;h<this.modules[f].length;h++){var i=h*e,j=this.modules[f][h];j&&(d.beginFill(0,100),d.moveTo(i,g),d.lineTo(i+e,g),d.lineTo(i+e,g+e),d.lineTo(i,g+e),d.endFill())}return d},setupTimingPattern:function(){for(var a=8;a<this.moduleCount-8;a++)null==this.modules[a][6]&&(this.modules[a][6]=0==a%2);for(var b=8;b<this.moduleCount-8;b++)null==this.modules[6][b]&&(this.modules[6][b]=0==b%2)},setupPositionAdjustPattern:function(){for(var a=f.getPatternPosition(this.typeNumber),b=0;b<a.length;b++)for(var c=0;c<a.length;c++){var d=a[b],e=a[c];if(null==this.modules[d][e])for(var g=-2;2>=g;g++)for(var h=-2;2>=h;h++)this.modules[d+g][e+h]=-2==g||2==g||-2==h||2==h||0==g&&0==h?!0:!1}},setupTypeNumber:function(a){for(var b=f.getBCHTypeNumber(this.typeNumber),c=0;18>c;c++){var d=!a&&1==(1&b>>c);this.modules[Math.floor(c/3)][c%3+this.moduleCount-8-3]=d}for(var c=0;18>c;c++){var d=!a&&1==(1&b>>c);this.modules[c%3+this.moduleCount-8-3][Math.floor(c/3)]=d}},setupTypeInfo:function(a,b){for(var c=this.errorCorrectLevel<<3|b,d=f.getBCHTypeInfo(c),e=0;15>e;e++){var g=!a&&1==(1&d>>e);6>e?this.modules[e][8]=g:8>e?this.modules[e+1][8]=g:this.modules[this.moduleCount-15+e][8]=g}for(var e=0;15>e;e++){var g=!a&&1==(1&d>>e);8>e?this.modules[8][this.moduleCount-e-1]=g:9>e?this.modules[8][15-e-1+1]=g:this.modules[8][15-e-1]=g}this.modules[this.moduleCount-8][8]=!a},mapData:function(a,b){for(var c=-1,d=this.moduleCount-1,e=7,g=0,h=this.moduleCount-1;h>0;h-=2)for(6==h&&h--;;){for(var i=0;2>i;i++)if(null==this.modules[d][h-i]){var j=!1;g<a.length&&(j=1==(1&a[g]>>>e));var k=f.getMask(b,d,h-i);k&&(j=!j),this.modules[d][h-i]=j,e--,-1==e&&(g++,e=7)}if(d+=c,0>d||this.moduleCount<=d){d-=c,c=-c;break}}}},b.PAD0=236,b.PAD1=17,b.createData=function(a,c,d){for(var e=j.getRSBlocks(a,c),g=new k,h=0;h<d.length;h++){var i=d[h];g.put(i.mode,4),g.put(i.getLength(),f.getLengthInBits(i.mode,a)),i.write(g)}for(var l=0,h=0;h<e.length;h++)l+=e[h].dataCount;if(g.getLengthInBits()>8*l)throw new Error("code length overflow. ("+g.getLengthInBits()+">"+8*l+")");for(g.getLengthInBits()+4<=8*l&&g.put(0,4);0!=g.getLengthInBits()%8;)g.putBit(!1);for(;;){if(g.getLengthInBits()>=8*l)break;if(g.put(b.PAD0,8),g.getLengthInBits()>=8*l)break;g.put(b.PAD1,8)}return b.createBytes(g,e)},b.createBytes=function(a,b){for(var c=0,d=0,e=0,g=new Array(b.length),h=new Array(b.length),j=0;j<b.length;j++){var k=b[j].dataCount,l=b[j].totalCount-k;d=Math.max(d,k),e=Math.max(e,l),g[j]=new Array(k);for(var m=0;m<g[j].length;m++)g[j][m]=255&a.buffer[m+c];c+=k;var n=f.getErrorCorrectPolynomial(l),o=new i(g[j],n.getLength()-1),p=o.mod(n);h[j]=new Array(n.getLength()-1);for(var m=0;m<h[j].length;m++){var q=m+p.getLength()-h[j].length;h[j][m]=q>=0?p.get(q):0}}for(var r=0,m=0;m<b.length;m++)r+=b[m].totalCount;for(var s=new Array(r),t=0,m=0;d>m;m++)for(var j=0;j<b.length;j++)m<g[j].length&&(s[t++]=g[j][m]);for(var m=0;e>m;m++)for(var j=0;j<b.length;j++)m<h[j].length&&(s[t++]=h[j][m]);return s};for(var c={MODE_NUMBER:1,MODE_ALPHA_NUM:2,MODE_8BIT_BYTE:4,MODE_KANJI:8},d={L:1,M:0,Q:3,H:2},e={PATTERN000:0,PATTERN001:1,PATTERN010:2,PATTERN011:3,PATTERN100:4,PATTERN101:5,PATTERN110:6,PATTERN111:7},f={PATTERN_POSITION_TABLE:[[],[6,18],[6,22],[6,26],[6,30],[6,34],[6,22,38],[6,24,42],[6,26,46],[6,28,50],[6,30,54],[6,32,58],[6,34,62],[6,26,46,66],[6,26,48,70],[6,26,50,74],[6,30,54,78],[6,30,56,82],[6,30,58,86],[6,34,62,90],[6,28,50,72,94],[6,26,50,74,98],[6,30,54,78,102],[6,28,54,80,106],[6,32,58,84,110],[6,30,58,86,114],[6,34,62,90,118],[6,26,50,74,98,122],[6,30,54,78,102,126],[6,26,52,78,104,130],[6,30,56,82,108,134],[6,34,60,86,112,138],[6,30,58,86,114,142],[6,34,62,90,118,146],[6,30,54,78,102,126,150],[6,24,50,76,102,128,154],[6,28,54,80,106,132,158],[6,32,58,84,110,136,162],[6,26,54,82,110,138,166],[6,30,58,86,114,142,170]],G15:1335,G18:7973,G15_MASK:21522,getBCHTypeInfo:function(a){for(var b=a<<10;f.getBCHDigit(b)-f.getBCHDigit(f.G15)>=0;)b^=f.G15<<f.getBCHDigit(b)-f.getBCHDigit(f.G15);return(a<<10|b)^f.G15_MASK},getBCHTypeNumber:function(a){for(var b=a<<12;f.getBCHDigit(b)-f.getBCHDigit(f.G18)>=0;)b^=f.G18<<f.getBCHDigit(b)-f.getBCHDigit(f.G18);return a<<12|b},getBCHDigit:function(a){for(var b=0;0!=a;)b++,a>>>=1;return b},getPatternPosition:function(a){return f.PATTERN_POSITION_TABLE[a-1]},getMask:function(a,b,c){switch(a){case e.PATTERN000:return 0==(b+c)%2;case e.PATTERN001:return 0==b%2;case e.PATTERN010:return 0==c%3;case e.PATTERN011:return 0==(b+c)%3;case e.PATTERN100:return 0==(Math.floor(b/2)+Math.floor(c/3))%2;case e.PATTERN101:return 0==b*c%2+b*c%3;case e.PATTERN110:return 0==(b*c%2+b*c%3)%2;case e.PATTERN111:return 0==(b*c%3+(b+c)%2)%2;default:throw new Error("bad maskPattern:"+a)}},getErrorCorrectPolynomial:function(a){for(var b=new i([1],0),c=0;a>c;c++)b=b.multiply(new i([1,g.gexp(c)],0));return b},getLengthInBits:function(a,b){if(b>=1&&10>b)switch(a){case c.MODE_NUMBER:return 10;case c.MODE_ALPHA_NUM:return 9;case c.MODE_8BIT_BYTE:return 8;case c.MODE_KANJI:return 8;default:throw new Error("mode:"+a)}else if(27>b)switch(a){case c.MODE_NUMBER:return 12;case c.MODE_ALPHA_NUM:return 11;case c.MODE_8BIT_BYTE:return 16;case c.MODE_KANJI:return 10;default:throw new Error("mode:"+a)}else{if(!(41>b))throw new Error("type:"+b);switch(a){case c.MODE_NUMBER:return 14;case c.MODE_ALPHA_NUM:return 13;case c.MODE_8BIT_BYTE:return 16;case c.MODE_KANJI:return 12;default:throw new Error("mode:"+a)}}},getLostPoint:function(a){for(var b=a.getModuleCount(),c=0,d=0;b>d;d++)for(var e=0;b>e;e++){for(var f=0,g=a.isDark(d,e),h=-1;1>=h;h++)if(!(0>d+h||d+h>=b))for(var i=-1;1>=i;i++)0>e+i||e+i>=b||(0!=h||0!=i)&&g==a.isDark(d+h,e+i)&&f++;f>5&&(c+=3+f-5)}for(var d=0;b-1>d;d++)for(var e=0;b-1>e;e++){var j=0;a.isDark(d,e)&&j++,a.isDark(d+1,e)&&j++,a.isDark(d,e+1)&&j++,a.isDark(d+1,e+1)&&j++,(0==j||4==j)&&(c+=3)}for(var d=0;b>d;d++)for(var e=0;b-6>e;e++)a.isDark(d,e)&&!a.isDark(d,e+1)&&a.isDark(d,e+2)&&a.isDark(d,e+3)&&a.isDark(d,e+4)&&!a.isDark(d,e+5)&&a.isDark(d,e+6)&&(c+=40);for(var e=0;b>e;e++)for(var d=0;b-6>d;d++)a.isDark(d,e)&&!a.isDark(d+1,e)&&a.isDark(d+2,e)&&a.isDark(d+3,e)&&a.isDark(d+4,e)&&!a.isDark(d+5,e)&&a.isDark(d+6,e)&&(c+=40);for(var k=0,e=0;b>e;e++)for(var d=0;b>d;d++)a.isDark(d,e)&&k++;var l=Math.abs(100*k/b/b-50)/5;return c+=10*l}},g={glog:function(a){if(1>a)throw new Error("glog("+a+")");return g.LOG_TABLE[a]},gexp:function(a){for(;0>a;)a+=255;for(;a>=256;)a-=255;return g.EXP_TABLE[a]},EXP_TABLE:new Array(256),LOG_TABLE:new Array(256)},h=0;8>h;h++)g.EXP_TABLE[h]=1<<h;for(var h=8;256>h;h++)g.EXP_TABLE[h]=g.EXP_TABLE[h-4]^g.EXP_TABLE[h-5]^g.EXP_TABLE[h-6]^g.EXP_TABLE[h-8];for(var h=0;255>h;h++)g.LOG_TABLE[g.EXP_TABLE[h]]=h;i.prototype={get:function(a){return this.num[a]},getLength:function(){return this.num.length},multiply:function(a){for(var b=new Array(this.getLength()+a.getLength()-1),c=0;c<this.getLength();c++)for(var d=0;d<a.getLength();d++)b[c+d]^=g.gexp(g.glog(this.get(c))+g.glog(a.get(d)));return new i(b,0)},mod:function(a){if(this.getLength()-a.getLength()<0)return this;for(var b=g.glog(this.get(0))-g.glog(a.get(0)),c=new Array(this.getLength()),d=0;d<this.getLength();d++)c[d]=this.get(d);for(var d=0;d<a.getLength();d++)c[d]^=g.gexp(g.glog(a.get(d))+b);return new i(c,0).mod(a)}},j.RS_BLOCK_TABLE=[[1,26,19],[1,26,16],[1,26,13],[1,26,9],[1,44,34],[1,44,28],[1,44,22],[1,44,16],[1,70,55],[1,70,44],[2,35,17],[2,35,13],[1,100,80],[2,50,32],[2,50,24],[4,25,9],[1,134,108],[2,67,43],[2,33,15,2,34,16],[2,33,11,2,34,12],[2,86,68],[4,43,27],[4,43,19],[4,43,15],[2,98,78],[4,49,31],[2,32,14,4,33,15],[4,39,13,1,40,14],[2,121,97],[2,60,38,2,61,39],[4,40,18,2,41,19],[4,40,14,2,41,15],[2,146,116],[3,58,36,2,59,37],[4,36,16,4,37,17],[4,36,12,4,37,13],[2,86,68,2,87,69],[4,69,43,1,70,44],[6,43,19,2,44,20],[6,43,15,2,44,16],[4,101,81],[1,80,50,4,81,51],[4,50,22,4,51,23],[3,36,12,8,37,13],[2,116,92,2,117,93],[6,58,36,2,59,37],[4,46,20,6,47,21],[7,42,14,4,43,15],[4,133,107],[8,59,37,1,60,38],[8,44,20,4,45,21],[12,33,11,4,34,12],[3,145,115,1,146,116],[4,64,40,5,65,41],[11,36,16,5,37,17],[11,36,12,5,37,13],[5,109,87,1,110,88],[5,65,41,5,66,42],[5,54,24,7,55,25],[11,36,12],[5,122,98,1,123,99],[7,73,45,3,74,46],[15,43,19,2,44,20],[3,45,15,13,46,16],[1,135,107,5,136,108],[10,74,46,1,75,47],[1,50,22,15,51,23],[2,42,14,17,43,15],[5,150,120,1,151,121],[9,69,43,4,70,44],[17,50,22,1,51,23],[2,42,14,19,43,15],[3,141,113,4,142,114],[3,70,44,11,71,45],[17,47,21,4,48,22],[9,39,13,16,40,14],[3,135,107,5,136,108],[3,67,41,13,68,42],[15,54,24,5,55,25],[15,43,15,10,44,16],[4,144,116,4,145,117],[17,68,42],[17,50,22,6,51,23],[19,46,16,6,47,17],[2,139,111,7,140,112],[17,74,46],[7,54,24,16,55,25],[34,37,13],[4,151,121,5,152,122],[4,75,47,14,76,48],[11,54,24,14,55,25],[16,45,15,14,46,16],[6,147,117,4,148,118],[6,73,45,14,74,46],[11,54,24,16,55,25],[30,46,16,2,47,17],[8,132,106,4,133,107],[8,75,47,13,76,48],[7,54,24,22,55,25],[22,45,15,13,46,16],[10,142,114,2,143,115],[19,74,46,4,75,47],[28,50,22,6,51,23],[33,46,16,4,47,17],[8,152,122,4,153,123],[22,73,45,3,74,46],[8,53,23,26,54,24],[12,45,15,28,46,16],[3,147,117,10,148,118],[3,73,45,23,74,46],[4,54,24,31,55,25],[11,45,15,31,46,16],[7,146,116,7,147,117],[21,73,45,7,74,46],[1,53,23,37,54,24],[19,45,15,26,46,16],[5,145,115,10,146,116],[19,75,47,10,76,48],[15,54,24,25,55,25],[23,45,15,25,46,16],[13,145,115,3,146,116],[2,74,46,29,75,47],[42,54,24,1,55,25],[23,45,15,28,46,16],[17,145,115],[10,74,46,23,75,47],[10,54,24,35,55,25],[19,45,15,35,46,16],[17,145,115,1,146,116],[14,74,46,21,75,47],[29,54,24,19,55,25],[11,45,15,46,46,16],[13,145,115,6,146,116],[14,74,46,23,75,47],[44,54,24,7,55,25],[59,46,16,1,47,17],[12,151,121,7,152,122],[12,75,47,26,76,48],[39,54,24,14,55,25],[22,45,15,41,46,16],[6,151,121,14,152,122],[6,75,47,34,76,48],[46,54,24,10,55,25],[2,45,15,64,46,16],[17,152,122,4,153,123],[29,74,46,14,75,47],[49,54,24,10,55,25],[24,45,15,46,46,16],[4,152,122,18,153,123],[13,74,46,32,75,47],[48,54,24,14,55,25],[42,45,15,32,46,16],[20,147,117,4,148,118],[40,75,47,7,76,48],[43,54,24,22,55,25],[10,45,15,67,46,16],[19,148,118,6,149,119],[18,75,47,31,76,48],[34,54,24,34,55,25],[20,45,15,61,46,16]],j.getRSBlocks=function(a,b){var c=j.getRsBlockTable(a,b);if(void 0==c)throw new Error("bad rs block @ typeNumber:"+a+"/errorCorrectLevel:"+b);for(var d=c.length/3,e=[],f=0;d>f;f++)for(var g=c[3*f+0],h=c[3*f+1],i=c[3*f+2],k=0;g>k;k++)e.push(new j(h,i));return e},j.getRsBlockTable=function(a,b){switch(b){case d.L:return j.RS_BLOCK_TABLE[4*(a-1)+0];case d.M:return j.RS_BLOCK_TABLE[4*(a-1)+1];case d.Q:return j.RS_BLOCK_TABLE[4*(a-1)+2];case d.H:return j.RS_BLOCK_TABLE[4*(a-1)+3];default:return void 0}},k.prototype={get:function(a){var b=Math.floor(a/8);return 1==(1&this.buffer[b]>>>7-a%8)},put:function(a,b){for(var c=0;b>c;c++)this.putBit(1==(1&a>>>b-c-1))},getLengthInBits:function(){return this.length},putBit:function(a){var b=Math.floor(this.length/8);this.buffer.length<=b&&this.buffer.push(0),a&&(this.buffer[b]|=128>>>this.length%8),this.length++}};var l=[[17,14,11,7],[32,26,20,14],[53,42,32,24],[78,62,46,34],[106,84,60,44],[134,106,74,58],[154,122,86,64],[192,152,108,84],[230,180,130,98],[271,213,151,119],[321,251,177,137],[367,287,203,155],[425,331,241,177],[458,362,258,194],[520,412,292,220],[586,450,322,250],[644,504,364,280],[718,560,394,310],[792,624,442,338],[858,666,482,382],[929,711,509,403],[1003,779,565,439],[1091,857,611,461],[1171,911,661,511],[1273,997,715,535],[1367,1059,751,593],[1465,1125,805,625],[1528,1190,868,658],[1628,1264,908,698],[1732,1370,982,742],[1840,1452,1030,790],[1952,1538,1112,842],[2068,1628,1168,898],[2188,1722,1228,958],[2303,1809,1283,983],[2431,1911,1351,1051],[2563,1989,1423,1093],[2699,2099,1499,1139],[2809,2213,1579,1219],[2953,2331,1663,1273]],o=function(){var a=function(a,b){this._el=a,this._htOption=b};return a.prototype.draw=function(a){function g(a,b){var c=document.createElementNS("http://www.w3.org/2000/svg",a);for(var d in b)b.hasOwnProperty(d)&&c.setAttribute(d,b[d]);return c}var b=this._htOption,c=this._el,d=a.getModuleCount();Math.floor(b.width/d),Math.floor(b.height/d),this.clear();var h=g("svg",{viewBox:"0 0 "+String(d)+" "+String(d),width:"100%",height:"100%",fill:b.colorLight});h.setAttributeNS("http://www.w3.org/2000/xmlns/","xmlns:xlink","http://www.w3.org/1999/xlink"),c.appendChild(h),h.appendChild(g("rect",{fill:b.colorDark,width:"1",height:"1",id:"template"}));for(var i=0;d>i;i++)for(var j=0;d>j;j++)if(a.isDark(i,j)){var k=g("use",{x:String(i),y:String(j)});k.setAttributeNS("http://www.w3.org/2000/xmlns/","xmlns:xlink","http://www.w3.org/1999/xlink"),h.appendChild(k)}},a.prototype.clear=function(){for(;this._el.hasChildNodes();)this._el.removeChild(this._el.lastChild)},a}(),p="svg"===document.documentElement.tagName.toLowerCase(),q=p?o:m()?function(){function a(){this._elImage.src=this._elCanvas.toDataURL("image/png"),this._elImage.style.display="block",this._elCanvas.style.display="none"}function d(a,b){var c=this;if(c._fFail=b,c._fSuccess=a,null===c._bSupportDataURI){var d=document.createElement("img"),e=function(){c._bSupportDataURI=!1,c._fFail&&_fFail.call(c)},f=function(){c._bSupportDataURI=!0,c._fSuccess&&c._fSuccess.call(c)};return d.onabort=e,d.onerror=e,d.onload=f,d.src="data:image/gif;base64,iVBORw0KGgoAAAANSUhEUgAAAAUAAAAFCAYAAACNbyblAAAAHElEQVQI12P4//8/w38GIAXDIBKE0DHxgljNBAAO9TXL0Y4OHwAAAABJRU5ErkJggg==",void 0}c._bSupportDataURI===!0&&c._fSuccess?c._fSuccess.call(c):c._bSupportDataURI===!1&&c._fFail&&c._fFail.call(c)}if(this._android&&this._android<=2.1){var b=1/window.devicePixelRatio,c=CanvasRenderingContext2D.prototype.drawImage;CanvasRenderingContext2D.prototype.drawImage=function(a,d,e,f,g,h,i,j){if("nodeName"in a&&/img/i.test(a.nodeName))for(var l=arguments.length-1;l>=1;l--)arguments[l]=arguments[l]*b;else"undefined"==typeof j&&(arguments[1]*=b,arguments[2]*=b,arguments[3]*=b,arguments[4]*=b);c.apply(this,arguments)}}var e=function(a,b){this._bIsPainted=!1,this._android=n(),this._htOption=b,this._elCanvas=document.createElement("canvas"),this._elCanvas.width=b.width,this._elCanvas.height=b.height,a.appendChild(this._elCanvas),this._el=a,this._oContext=this._elCanvas.getContext("2d"),this._bIsPainted=!1,this._elImage=document.createElement("img"),this._elImage.style.display="none",this._el.appendChild(this._elImage),this._bSupportDataURI=null};return e.prototype.draw=function(a){var b=this._elImage,c=this._oContext,d=this._htOption,e=a.getModuleCount(),f=d.width/e,g=d.height/e,h=Math.round(f),i=Math.round(g);b.style.display="none",this.clear();for(var j=0;e>j;j++)for(var k=0;e>k;k++){var l=a.isDark(j,k),m=k*f,n=j*g;c.strokeStyle=l?d.colorDark:d.colorLight,c.lineWidth=1,c.fillStyle=l?d.colorDark:d.colorLight,c.fillRect(m,n,f,g),c.strokeRect(Math.floor(m)+.5,Math.floor(n)+.5,h,i),c.strokeRect(Math.ceil(m)-.5,Math.ceil(n)-.5,h,i)}this._bIsPainted=!0},e.prototype.makeImage=function(){this._bIsPainted&&d.call(this,a)},e.prototype.isPainted=function(){return this._bIsPainted},e.prototype.clear=function(){this._oContext.clearRect(0,0,this._elCanvas.width,this._elCanvas.height),this._bIsPainted=!1},e.prototype.round=function(a){return a?Math.floor(1e3*a)/1e3:a},e}():function(){var a=function(a,b){this._el=a,this._htOption=b};return a.prototype.draw=function(a){for(var b=this._htOption,c=this._el,d=a.getModuleCount(),e=Math.floor(b.width/d),f=Math.floor(b.height/d),g=['<table style="border:0;border-collapse:collapse;">'],h=0;d>h;h++){g.push("<tr>");for(var i=0;d>i;i++)g.push('<td style="border:0;border-collapse:collapse;padding:0;margin:0;width:'+e+"px;height:"+f+"px;background-color:"+(a.isDark(h,i)?b.colorDark:b.colorLight)+';"></td>');g.push("</tr>")}g.push("</table>"),c.innerHTML=g.join("");var j=c.childNodes[0],k=(b.width-j.offsetWidth)/2,l=(b.height-j.offsetHeight)/2;k>0&&l>0&&(j.style.margin=l+"px "+k+"px")},a.prototype.clear=function(){this._el.innerHTML=""},a}();QRCode=function(a,b){if(this._htOption={width:256,height:256,typeNumber:4,colorDark:"#000000",colorLight:"#ffffff",correctLevel:d.H},"string"==typeof b&&(b={text:b}),b)for(var c in b)this._htOption[c]=b[c];"string"==typeof a&&(a=document.getElementById(a)),this._android=n(),this._el=a,this._oQRCode=null,this._oDrawing=new q(this._el,this._htOption),this._htOption.text&&this.makeCode(this._htOption.text)},QRCode.prototype.makeCode=function(a){this._oQRCode=new b(r(a,this._htOption.correctLevel),this._htOption.correctLevel),this._oQRCode.addData(a),this._oQRCode.make(),this._el.title=a,this._oDrawing.draw(this._oQRCode),this.makeImage()},QRCode.prototype.makeImage=function(){"function"==typeof this._oDrawing.makeImage&&(!this._android||this._android>=3)&&this._oDrawing.makeImage()},QRCode.prototype.clear=function(){this._oDrawing.clear()},QRCode.CorrectLevel=d}();

let currentTab = 'tab-dash';
let pollingTimer = null;
let fuzzerTimer = null;
let loadedPresets = [];
let selectedCatFilter = 'All';
let currentLang = localStorage.getItem('gk_lang') || 'en';
let qrMode = 'wifi';
let deviceStatus = {};
let isIRListeningActive = false;
let isRFListeningActive = false;

// Major Brands IR Appliance Database (RemoteCentral Codes)
const IR_APPLIANCES = {"tv":{"kunft":{"name":"Kunft TV (Worten)","sets":[{"name":"Set 1: Vestel RC1910 / Standard","proto":"NEC","bits":32,"keys":[["Power",12536246,1],["Vol +",12534206],["Vol -",12534461],["Mute",12534716],["Ch +",12534971],["Ch -",12535226],["Source",12535481],["Menu",12535736],["OK",12538796],["Exit",12539051],["Info",12539306],["Up",12539561],["Down",12539816],["Left",12540071],["Right",12540326]]},{"name":"Set 2: Vestel Variant (0x01FE)","proto":"NEC","bits":32,"keys":[["Power",33441975,1],["Vol +",33446055],["Vol -",33478695],["Mute",33454215],["Ch +",33437895],["Ch -",33470535],["Source",33429735],["OK",33464925],["Menu",33462375],["Return",33433815]]},{"name":"Set 3: Chinese Smart OEM (0x00FF)","proto":"NEC","bits":32,"keys":[["Power",16753245,1],["Vol +",16736925],["Vol -",16754775],["Mute",16769565],["Ch +",16720605],["Ch -",16712445],["Source",16748655],["Home",16761405],["OK",16712445],["Back",16732845]]},{"name":"Set 4: MStar Android Kunft (0x04FB)","proto":"NEC","bits":32,"keys":[["Power",83574975,1],["Vol +",83566815],["Vol -",83599455],["Mute",83583135],["Ch +",83615775],["Ch -",83562735],["Input",83595375],["OK",83568855],["Home",83579055]]}]},"mitsai":{"name":"Mitsai TV (Worten)","sets":[{"name":"Set 1: Vestel Standard","proto":"NEC","bits":32,"keys":[["Power",12536246,1],["Vol +",12534206],["Vol -",12534461],["Mute",12534716],["Ch +",12534971],["Ch -",12535226],["Source",12535481],["Menu",12535736],["OK",12538796]]},{"name":"Set 2: Konka / Mitsai Variant","proto":"NEC","bits":32,"keys":[["Power",150405885,1],["Vol +",150462495],["Vol -",150458415],["Mute",150466575],["Ch +",150423735],["Ch -",150407415],["AV",150438015],["OK",150410985]]},{"name":"Set 3: Mitsai Universal Chinese","proto":"NEC","bits":32,"keys":[["Power",16712445,1],["Vol +",16736415],["Vol -",16752735],["Mute",16769055],["Ch +",16720095],["Ch -",16711935],["Source",16728255],["OK",16748655]]}]},"silver":{"name":"Silver TV (Portugal)","sets":[{"name":"Set 1: Silver Smart TV (0x00FF)","proto":"NEC","bits":32,"keys":[["Power",16711935,1],["Vol +",16728255],["Vol -",16760895],["Mute",16720095],["Ch +",16744575],["Ch -",16712445],["Source",16736415],["Menu",16769055],["OK",16748655],["Back",16752735]]},{"name":"Set 2: Silver MStar (0x04FB)","proto":"NEC","bits":32,"keys":[["Power",83574975,1],["Vol +",83566815],["Vol -",83599455],["Mute",83583135],["Ch +",83615775],["Ch -",83562735],["Input",83595375],["OK",83568855]]},{"name":"Set 3: Silver Vestel Profile","proto":"NEC","bits":32,"keys":[["Power",12536246,1],["Vol +",12534206],["Vol -",12534461],["Mute",12534716],["Source",12535481],["OK",12538796]]}]},"tdsystems":{"name":"TD Systems (Iberia)","sets":[{"name":"Set 1: K-Series (0x00FF)","proto":"NEC","bits":32,"keys":[["Power",16711935,1],["Vol +",16728255],["Vol -",16760895],["Mute",16720095],["Ch +",16744575],["Ch -",16712445],["Source",16736415],["Menu",16769055],["OK",16748655]]},{"name":"Set 2: Vestel Profile","proto":"NEC","bits":32,"keys":[["Power",12536246,1],["Vol +",12534206],["Vol -",12534461],["Mute",12534716],["Source",12535481],["OK",12538796]]},{"name":"Set 3: Android Smart (0x00FFA25D)","proto":"NEC","bits":32,"keys":[["Power",16753245,1],["Vol +",16736925],["Vol -",16754775],["Mute",16769565],["Home",16761405],["OK",16712445]]}]},"electronia":{"name":"Electronia (R\u00e1dio Popular)","sets":[{"name":"Set 1: Vestel RC1910","proto":"NEC","bits":32,"keys":[["Power",12536246,1],["Vol +",12534206],["Vol -",12534461],["Mute",12534716],["Ch +",12534971],["Ch -",12535226],["Source",12535481],["OK",12538796]]},{"name":"Set 2: Chinese Smart (0x00FF)","proto":"NEC","bits":32,"keys":[["Power",16753245,1],["Vol +",16736925],["Vol -",16754775],["Mute",16769565],["Source",16748655],["OK",16712445]]}]},"selecline":{"name":"Selecline (Auchan)","sets":[{"name":"Set 1: Vestel Profile","proto":"NEC","bits":32,"keys":[["Power",12536246,1],["Vol +",12534206],["Vol -",12534461],["Mute",12534716],["Ch +",12534971],["Ch -",12535226],["Source",12535481],["OK",12538796]]},{"name":"Set 2: Skyworth Profile","proto":"NEC","bits":32,"keys":[["Power",33441975,1],["Vol +",33446055],["Vol -",33478695],["Mute",33454215],["Source",33429735],["OK",33464925]]}]},"cecotec":{"name":"Cecotec TV","sets":[{"name":"Set 1: Android Smart (0x00FF)","proto":"NEC","bits":32,"keys":[["Power",16753245,1],["Vol +",16736925],["Vol -",16754775],["Mute",16769565],["Ch +",16720605],["Ch -",16712445],["Source",16748655],["Home",16761405],["OK",16712445]]},{"name":"Set 2: MStar Variant (0x04FB)","proto":"NEC","bits":32,"keys":[["Power",83574975,1],["Vol +",83566815],["Vol -",83599455],["Mute",83583135],["Input",83595375],["OK",83568855]]}]},"denver":{"name":"Denver / Engel / Axil","sets":[{"name":"Set 1: Denver LED (0x00FF)","proto":"NEC","bits":32,"keys":[["Power",16711935,1],["Vol +",16728255],["Vol -",16760895],["Mute",16720095],["Source",16736415],["OK",16748655]]},{"name":"Set 2: Engel Axil DVB (0x01FE)","proto":"NEC","bits":32,"keys":[["Power",33441975,1],["Vol +",33446055],["Vol -",33478695],["Mute",33454215],["OK",33464925]]}]},"vestel":{"name":"Vestel Universal (OEM Euro)","sets":[{"name":"Set 1: RC1910 / RC4848 (Kunft/Mitsai/Sharp/Toshiba)","proto":"NEC","bits":32,"keys":[["Power",12536246,1],["Vol +",12534206],["Vol -",12534461],["Mute",12534716],["Ch +",12534971],["Ch -",12535226],["Source",12535481],["Menu",12535736],["OK",12538796],["Exit",12539051],["Info",12539306],["Guide",12540071]]},{"name":"Set 2: RC2040 / 3040 Variant","proto":"NEC","bits":32,"keys":[["Power",33441975,1],["Vol +",33446055],["Vol -",33478695],["Mute",33454215],["Ch +",33437895],["Ch -",33470535],["Source",33429735],["OK",33464925]]},{"name":"Set 3: RC5 Chassis","proto":"NEC","bits":32,"keys":[["Power",150405885,1],["Vol +",150462495],["Vol -",150458415],["Mute",150466575],["OK",150410985]]}]},"panasonic":{"name":"Panasonic Viera / TV","sets":[{"name":"Set 1: Viera Kaseikyo / Modern (Panasonic)","proto":"Panasonic","bits":32,"keys":[["Power",1074004224,1],["Vol +",1074004228],["Vol -",1074004356],["Mute",1074004300],["Ch +",1074004268],["Ch -",1074004396],["Input",1074004384],["Menu",1074004298],["OK",1074004370],["Return",1074004434]]},{"name":"Set 2: Viera NEC Standard (RemoteCentral)","proto":"NEC","bits":32,"keys":[["Power",16825533,1],["Vol +",16778245],["Vol -",16811141],["Mute",16796749],["Ch +",16788525],["Ch -",16821421],["Input",16818337],["OK",16814739],["Return",16831187]]},{"name":"Set 3: Panasonic Android TV / Vestel","proto":"NEC","bits":32,"keys":[["Power",12536246,1],["Vol +",12534206],["Vol -",12534461],["Mute",12534716],["Ch +",12534971],["Ch -",12535226],["Source",12535481],["Home",12538796],["OK",12538796]]},{"name":"Set 4: Discrete Power & Direct HDMI","proto":"Panasonic","bits":32,"keys":[["Power ON",1074004284,1],["Power OFF",1074004476,1],["HDMI 1",1074004448],["HDMI 2",1074004320],["HDMI 3",1074004322],["Component",1074004256]]}]},"samsung":{"name":"Samsung TV","sets":[{"name":"Set 1: Modern Smart TV","proto":"NEC","bits":32,"keys":[["Power",3772793023,1],["Vol +",3772833823],["Vol -",3772829743],["Mute",3772837903],["Ch +",3772795063],["Ch -",3772778743],["Source",3772809343],["Menu",3772799143],["Return",3772783333],["Tools",3772831273],["Info",3772839943],["OK",3772782313],["Up",3772778233],["Down",3772810873],["Left",3772819033],["Right",3772794553],["HDMI",3772815208]]},{"name":"Set 2: Discrete Power & Direct HDMI","proto":"NEC","bits":32,"keys":[["Power ON",3772815718,1],["Power OFF",3772783078,1],["HDMI 1",3772829998],["HDMI 2",3772834078],["HDMI 3",3772818268],["HDMI 4",3772800163],["AV",3772817758],["Component",3772788433],["SmartHub",3772816993]]},{"name":"Set 3: Legacy LCD / Plasma","proto":"NEC","bits":32,"keys":[["Power",551489775,1],["Vol +",551502015],["Vol -",551534655],["Mute",551522415],["Ch +",551485695],["Ch -",551518335],["Source",551538735],["OK",551494365]]},{"name":"Set 4: Older CRT / Variant","proto":"NEC","bits":32,"keys":[["Power",150421695,1],["Vol +",150462495],["Vol -",150458415],["Mute",150466575],["Source",150438015]]}]},"lg":{"name":"LG TV","sets":[{"name":"Set 1: WebOS & Standard LCD","proto":"NEC","bits":32,"keys":[["Power",551489775,1],["Vol +",551502015],["Vol -",551534655],["Mute",551522415],["Ch +",551485695],["Ch -",551518335],["Input",551538735],["Home",551501505],["Back",551490795],["Settings",551535165],["OK",551494365],["Up",551486205],["Down",551518845],["Left",551542815],["Right",551510175]]},{"name":"Set 2: Discrete Power & Direct Inputs","proto":"NEC","bits":32,"keys":[["Power ON",551494620,1],["Power OFF",551515020,1],["HDMI 1",551515020],["HDMI 2",551498700],["HDMI 3",551524200],["HDMI 4",551509410],["AV",551508645],["Component",551539755]]},{"name":"Set 3: Legacy Goldstar / LG CRT/LCD","proto":"NEC","bits":32,"keys":[["Power",2155811055,1],["Vol +",2155823295],["Vol -",2155855935],["Mute",2155843695],["Ch +",2155806975],["Ch -",2155839615],["Input",2155860015],["OK",2155815645]]},{"name":"Set 4: LG Service Menu & Diagnostics","proto":"NEC","bits":32,"keys":[["In-Start",551542560],["Ez-Adjust",551550720],["Power-Only",551550465],["Eye-Q",551531595]]}]},"sony":{"name":"Sony Bravia","sets":[{"name":"Set 1: Bravia 12-bit Standard","proto":"Sony","bits":12,"keys":[["Power",2704,1],["Vol +",1168],["Vol -",3216],["Mute",656],["Ch +",144],["Ch -",2192],["Input",2640],["Home",112],["Return",3184],["Wide",3304],["OK",2672],["Up",752],["Down",2800],["Left",720],["Right",3280]]},{"name":"Set 2: Discrete Power & Direct Video (15-bit)","proto":"Sony","bits":15,"keys":[["Power ON",1872,1],["Power OFF",3920,1],["HDMI 1",2608],["HDMI 2",2352],["HDMI 3",1328],["HDMI 4",816],["Video 1",48],["Video 2",2096]]},{"name":"Set 3: Bravia Android TV (15-bit)","proto":"Sony","bits":15,"keys":[["Power",1168,1],["Vol +",576],["Vol -",1600],["Mute",320],["Input",1344],["Home",112],["Action Menu",624],["OK",112]]}]},"philips":{"name":"Philips TV","sets":[{"name":"Set 1: Modern Android TV / TP Vision (0x00FF)","proto":"NEC","bits":32,"keys":[["Power",16711935,1],["Vol +",16728255],["Vol -",16760895],["Mute",16720095],["Ch +",16744575],["Ch -",16712445],["Source",16736415],["Home",16716015],["OK",16748655],["Back",16752735]]},{"name":"Set 2: Philips Smart TV Variant (0x02FD)","proto":"NEC","bits":32,"keys":[["Power",50153655,1],["Vol +",50157735],["Vol -",50190375],["Mute",50165895],["Ch +",50149575],["Ch -",50182215],["Source",50141415],["OK",50176605]]},{"name":"Set 3: Philips RC6 Mode 0","proto":"NEC","bits":32,"keys":[["Power",4108,1],["Vol +",4112],["Vol -",4113],["Mute",4109],["Ch +",4128],["Ch -",4129],["Source",4152]]},{"name":"Set 4: Philips RC5 Legacy","proto":"NEC","bits":32,"keys":[["Power",12,1],["Vol +",16],["Vol -",17],["Mute",13],["Ch +",32],["Ch -",33]]}]},"tcl":{"name":"TCL TV","sets":[{"name":"Set 1: Modern Android & Google TV","proto":"NEC","bits":32,"keys":[["Power",1086275775,1],["Vol +",1086316575],["Vol -",1086312495],["Mute",1086320655],["Ch +",1086277815],["Ch -",1086261495],["Source",1086292095],["Home",1086290055],["OK",1086265065],["Up",1086260985],["Down",1086293625],["Left",1086301785],["Right",1086277305],["Back",1086266085]]},{"name":"Set 2: TCL Roku TV","proto":"NEC","bits":32,"keys":[["Power",1474553879,1],["Vol +",1474511039],["Vol -",1474543679],["Mute",1474502879],["Home",1474551839],["OK",1474533479],["Back",1474520729],["Up",1474533989],["Down",1474517669]]},{"name":"Set 3: TCL Legacy / Thomson","proto":"NEC","bits":32,"keys":[["Power",12536246,1],["Vol +",12534206],["Vol -",12534461],["Mute",12534716],["OK",12538796]]}]},"hisense":{"name":"Hisense TV","sets":[{"name":"Set 1: VIDAA Smart TV","proto":"NEC","bits":32,"keys":[["Power",4244783295,1],["Vol +",4244824095],["Vol -",4244820015],["Mute",4244828175],["Ch +",4244785335],["Ch -",4244769015],["Source",4244799615],["Home",4244782785],["OK",4244772585],["Back",4244773605],["Menu",4244789415]]},{"name":"Set 2: Hisense Roku TV","proto":"NEC","bits":32,"keys":[["Power",1474553879,1],["Vol +",1474511039],["Vol -",1474543679],["Mute",1474502879],["Home",1474551839],["OK",1474533479]]},{"name":"Set 3: Legacy LCD (0x20DF)","proto":"NEC","bits":32,"keys":[["Power",551489775,1],["Vol +",551502015],["Vol -",551534655],["Mute",551522415],["Ch +",551485695],["Ch -",551518335],["Input",551538735],["OK",551494365]]},{"name":"Set 4: Chinese Variant (0x00FF)","proto":"NEC","bits":32,"keys":[["Power",16753245,1],["Vol +",16736925],["Vol -",16754775],["Mute",16769565],["OK",16712445]]}]},"xiaomi":{"name":"Xiaomi Mi TV","sets":[{"name":"Set 1: Mi TV Standard","proto":"NEC","bits":32,"keys":[["Power",2188198079,1],["Vol +",2188238879],["Vol -",2188234799],["Mute",2188242959],["Home",2188197569],["Back",2188186859],["Menu",2188204199],["OK",2188187369],["Up",2188183289],["Down",2188215929],["Left",2188224089],["Right",2188199609]]},{"name":"Set 2: Mi Box / TV Variant","proto":"NEC","bits":32,"keys":[["Power",16753245,1],["Vol +",16736925],["Vol -",16754775],["Home",16761405],["OK",16712445]]}]},"toshiba":{"name":"Toshiba TV","sets":[{"name":"Set 1: Regza Japanese Standard","proto":"NEC","bits":32,"keys":[["Power",50153655,1],["Vol +",50157735],["Vol -",50190375],["Mute",50165895],["Ch +",50149575],["Ch -",50182215],["Input",50141415],["OK",50176605]]},{"name":"Set 2: Toshiba Europe / Vestel","proto":"NEC","bits":32,"keys":[["Power",12536246,1],["Vol +",12534206],["Vol -",12534461],["Mute",12534716],["Source",12535481],["OK",12538796]]},{"name":"Set 3: Toshiba Android TV","proto":"NEC","bits":32,"keys":[["Power",16753245,1],["Vol +",16736925],["Vol -",16754775],["OK",16712445]]}]},"sharp":{"name":"Sharp Aquos","sets":[{"name":"Set 1: Aquos NEC","proto":"NEC","bits":32,"keys":[["Power",2857713855,1],["Vol +",2857754655],["Vol -",2857750575],["Mute",2857758735],["Input",2857730175],["OK",2857703145]]},{"name":"Set 2: Sharp Standard (0x55AA)","proto":"NEC","bits":32,"keys":[["Power",1437221055,1],["Vol +",1437261855],["Vol -",1437257775],["Mute",1437265935],["Input",1437237375],["OK",1437210345]]},{"name":"Set 3: Sharp Europe / Vestel","proto":"NEC","bits":32,"keys":[["Power",12536246,1],["Vol +",12534206],["Vol -",12534461],["Mute",12534716],["Source",12535481],["OK",12538796]]}]},"vizio":{"name":"Vizio TV","sets":[{"name":"Set 1: SmartCast","proto":"NEC","bits":32,"keys":[["Power",551489775,1],["Vol +",551502015],["Vol -",551534655],["Mute",551522415],["Input",551538735],["OK",551494365]]},{"name":"Set 2: Vizio Legacy","proto":"NEC","bits":32,"keys":[["Power",16711935,1],["Vol +",16728255],["Vol -",16760895],["Mute",16720095],["Input",16736415]]}]},"pioneer":{"name":"Pioneer TV","sets":[{"name":"Set 1: Kuro Plasma","proto":"NEC","bits":32,"keys":[["Power",2774153415,1],["Vol +",2774159535],["Vol -",2774192175],["Mute",2774157495],["Input",2774184015],["Menu",2774161575]]},{"name":"Set 2: Pioneer LCD","proto":"NEC","bits":32,"keys":[["Power",12536246,1],["Vol +",12534206],["Vol -",12534461],["Mute",12534716],["OK",12538796]]}]},"jvc":{"name":"JVC TV","sets":[{"name":"Set 1: JVC NEC","proto":"NEC","bits":32,"keys":[["Power",66865335,1],["Vol +",66869415],["Vol -",66902055],["Mute",66877575],["Input",66853095],["OK",66888285]]},{"name":"Set 2: JVC Europe / Vestel","proto":"NEC","bits":32,"keys":[["Power",12536246,1],["Vol +",12534206],["Vol -",12534461],["Mute",12534716],["Source",12535481],["OK",12538796]]}]},"hitachi":{"name":"Hitachi TV","sets":[{"name":"Set 1: Hitachi NEC","proto":"NEC","bits":32,"keys":[["Power",1353660615,1],["Vol +",1353666735],["Vol -",1353699375],["Mute",1353664695],["Input",1353691215],["OK",1353651945]]},{"name":"Set 2: Hitachi Europe / Vestel","proto":"NEC","bits":32,"keys":[["Power",12536246,1],["Vol +",12534206],["Vol -",12534461],["Mute",12534716],["Source",12535481],["OK",12538796]]}]},"grundig":{"name":"Grundig TV","sets":[{"name":"Set 1: TP6/TP7 Chassis","proto":"NEC","bits":32,"keys":[["Power",12536246,1],["Vol +",12534206],["Vol -",12534461],["Mute",12534716],["Source",12535481],["OK",12538796]]},{"name":"Set 2: Grundig Smart (0x01FE)","proto":"NEC","bits":32,"keys":[["Power",33441975,1],["Vol +",33446055],["Vol -",33478695],["Mute",33454215],["OK",33464925]]}]},"telefunken":{"name":"Telefunken TV","sets":[{"name":"Set 1: Vestel Chassis","proto":"NEC","bits":32,"keys":[["Power",12536246,1],["Vol +",12534206],["Vol -",12534461],["Mute",12534716],["Source",12535481],["OK",12538796]]},{"name":"Set 2: Thomson Chassis","proto":"NEC","bits":32,"keys":[["Power",1086275775,1],["Vol +",1086316575],["Vol -",1086312495],["Mute",1086320655],["OK",1086265065]]}]},"thomson":{"name":"Thomson TV","sets":[{"name":"Set 1: TCL/Thomson Modern","proto":"NEC","bits":32,"keys":[["Power",1086275775,1],["Vol +",1086316575],["Vol -",1086312495],["Mute",1086320655],["Source",1086292095],["OK",1086265065]]},{"name":"Set 2: Thomson RC5","proto":"NEC","bits":32,"keys":[["Power",12,1],["Vol +",16],["Vol -",17],["Mute",13],["Ch +",32],["Ch -",33]]}]},"blaupunkt":{"name":"Blaupunkt TV","sets":[{"name":"Set 1: UMC/Vestel Chassis","proto":"NEC","bits":32,"keys":[["Power",12536246,1],["Vol +",12534206],["Vol -",12534461],["Mute",12534716],["Source",12535481],["OK",12538796]]},{"name":"Set 2: Blaupunkt Smart (0x00FF)","proto":"NEC","bits":32,"keys":[["Power",16711935,1],["Vol +",16728255],["Vol -",16760895],["Mute",16720095],["OK",16748655]]}]}},"audio":{"samsung_audio":{"name":"Samsung Soundbar & Audio","sets":[{"name":"Set 1: HW-Q / HW-R Series","proto":"NEC","bits":32,"keys":[["Power",3268051071,1],["Vol +",3268022511],["Vol -",3268055151],["Mute",3268030671],["Source",3268038831],["Sound Mode",3268071471],["Bass +",3268046991],["Bass -",3268079631],["Bluetooth",3268034751]]},{"name":"Set 2: HW-M / HW-K Series","proto":"NEC","bits":32,"keys":[["Power",3827019855,1],["Vol +",3827036175],["Vol -",3827032095],["Mute",3827028015],["Source",3827003535],["Sound",3826987215]]},{"name":"Set 3: Soundbar Legacy (0xE0E0)","proto":"NEC","bits":32,"keys":[["Power",3772793023,1],["Vol +",3772833823],["Vol -",3772829743],["Mute",3772837903],["Source",3772809343]]}]},"lg_audio":{"name":"LG Soundbar & Audio","sets":[{"name":"Set 1: AKB Series / SK & SL","proto":"NEC","bits":32,"keys":[["Power",2289500925,1],["Vol +",2289502965],["Vol -",2289502455],["Mute",2289501945],["Function",2289501435],["Sound Effect",2289503985],["Optical",2289505515],["Bluetooth",2289506025]]},{"name":"Set 2: LG Hi-Fi Variant (0x20DF)","proto":"NEC","bits":32,"keys":[["Power",551507370,1],["Vol +",551502015],["Vol -",551534655],["Mute",551522415],["Function",551538735]]},{"name":"Set 3: LG Soundbar Legacy","proto":"NEC","bits":32,"keys":[["Power",2289504495,1],["Vol +",2289516735],["Vol -",2289549375],["Mute",2289537135]]}]},"sony_audio":{"name":"Sony Soundbar & AV Receiver","sets":[{"name":"Set 1: Sony AV Receiver / Soundbar (12-bit)","proto":"Sony","bits":12,"keys":[["Power",1344,1],["Vol +",576],["Vol -",1600],["Mute",320],["Input",2624],["Bass",3648]]},{"name":"Set 2: Sony HT Soundbar (15-bit)","proto":"Sony","bits":15,"keys":[["Power",21632,1],["Vol +",9344],["Vol -",25728],["Mute",5248],["TV Input",1152],["Bluetooth",33920]]},{"name":"Set 3: Sony Discrete Power","proto":"Sony","bits":12,"keys":[["Power ON",1346,1],["Power OFF",1347,1],["Vol +",576],["Vol -",1600],["Mute",320]]}]},"jbl":{"name":"JBL Soundbar","sets":[{"name":"Set 1: Bar 2.1 / Bar 5.1 / Cinema SB","proto":"NEC","bits":32,"keys":[["Power",2155813095,1],["Vol +",2155829415],["Vol -",2155862055],["Mute",2155821255],["Bass +",2155837575],["Bass -",2155870215],["TV",2155845735],["Bluetooth",2155853895],["HDMI",2155849815]]},{"name":"Set 2: Bar 9.1 / MultiBeam","proto":"NEC","bits":32,"keys":[["Power",2155806975,1],["Vol +",2155823295],["Vol -",2155855935],["Mute",2155815135],["Source",2155831455]]},{"name":"Set 3: Cinema SB150/250 (0x00FF)","proto":"NEC","bits":32,"keys":[["Power",16712445,1],["Vol +",16736415],["Vol -",16752735],["Mute",16769055],["Optical",16744575],["Bluetooth",16728255]]}]},"bose":{"name":"Bose Soundbar & Audio","sets":[{"name":"Set 1: Solo & Soundbar 500/700","proto":"NEC","bits":32,"keys":[["Power",1587640455,1],["Vol +",1587632295],["Vol -",1587664935],["Mute",1587648615],["TV",1587656775],["Bluetooth",1587624135],["Bass",1587615975]]},{"name":"Set 2: Wave Music System","proto":"NEC","bits":32,"keys":[["Power",1587664935,1],["Vol +",1587656775],["Vol -",1587624135],["Mute",1587673095],["AUX",1587611895],["CD",1587644535],["FM/AM",1587628215]]},{"name":"Set 3: Lifestyle / Companion","proto":"NEC","bits":32,"keys":[["Power",1587609855,1],["Vol +",1587626175],["Vol -",1587658815],["Mute",1587618015]]}]},"yamaha":{"name":"Yamaha AV & Soundbar","sets":[{"name":"Set 1: AV Receiver / YAS Soundbar","proto":"NEC","bits":32,"keys":[["Power",2122405035,1],["Vol +",2122391265],["Vol -",2122440225],["Mute",2122399425],["Input +",2122407585],["Input -",2122440225],["Cinema DSP",2122395345],["Straight",2122423905]]},{"name":"Set 2: Discrete Power & Direct HDMI","proto":"NEC","bits":32,"keys":[["Power ON",2122415745,1],["Power OFF",2122448385,1],["Vol +",2122391265],["Vol -",2122440225],["Mute",2122399425],["HDMI 1",2122405545],["HDMI 2",2122421865],["Optical",2122433085],["Bluetooth",2122424925]]}]},"denon":{"name":"Denon AV Receiver","sets":[{"name":"Set 1: Denon AVR Remote","proto":"NEC","bits":32,"keys":[["Power",709624573,1],["Vol +",709634263],["Vol -",709666903],["Mute",709642423],["CBL/SAT",709630183],["Blu-ray",709662823],["Game",709646503],["Media Player",709679143]]},{"name":"Set 2: Discrete Power","proto":"NEC","bits":32,"keys":[["Power ON",709649053,1],["Power OFF",709681693,1],["Vol +",709634263],["Vol -",709666903],["Mute",709642423]]}]},"marantz":{"name":"Marantz Receiver","sets":[{"name":"Set 1: Slimline AVR","proto":"NEC","bits":32,"keys":[["Power",709624573,1],["Vol +",709634263],["Vol -",709666903],["Mute",709642423],["TV Audio",709658743]]}]},"onkyo":{"name":"Onkyo Receiver","sets":[{"name":"Set 1: TX-NR / HT-S","proto":"NEC","bits":32,"keys":[["Power",1261883694,1],["Vol +",1261846974],["Vol -",1261879614],["Mute",1261871454],["BD/DVD",1261838814],["TV/CD",1261887774]]}]},"harman":{"name":"Harman Kardon","sets":[{"name":"Set 1: Enchant / SB Series","proto":"NEC","bits":32,"keys":[["Power",33441975,1],["Vol +",33446055],["Vol -",33478695],["Mute",33454215],["HDMI",33429735],["Optical",33462375],["Bluetooth",33470535]]},{"name":"Set 2: AVR Classic","proto":"NEC","bits":32,"keys":[["Power",2155813095,1],["Vol +",2155829415],["Vol -",2155862055],["Mute",2155821255]]}]},"philips_audio":{"name":"Philips Soundbar & HTS","sets":[{"name":"Set 1: Soundbar / HTS (0x02FD)","proto":"NEC","bits":32,"keys":[["Power",50153655,1],["Vol +",50157735],["Vol -",50190375],["Mute",50165895],["Optical",50141415],["Bluetooth",50174055]]},{"name":"Set 2: Micro Hi-Fi (0x00FF)","proto":"NEC","bits":32,"keys":[["Power",16711935,1],["Vol +",16728255],["Vol -",16760895],["Mute",16720095]]}]},"pioneer_audio":{"name":"Pioneer VSX Receiver","sets":[{"name":"Set 1: VSX Audio","proto":"NEC","bits":32,"keys":[["Power",2774153415,1],["Vol +",2774159535],["Vol -",2774192175],["Mute",2774157495],["BD",2774184015],["TV",2774167695]]}]},"offbrand_audio":{"name":"Offbrand & Chinese Soundbar","sets":[{"name":"Set 1: Common Chinese Soundbar 0x00FF (Trevi/Silver/Denver)","proto":"NEC","bits":32,"keys":[["Power",16711935,1],["Vol +",16728255],["Vol -",16760895],["Mute",16720095],["Mode",16736415],["Play/Pause",16744575],["Next",16716015],["Prev",16748655],["EQ",16769055]]},{"name":"Set 2: Alternate Soundbar (0x807F)","proto":"NEC","bits":32,"keys":[["Power",2155813095,1],["Vol +",2155829415],["Vol -",2155862055],["Mute",2155821255],["Mode",2155837575],["Bluetooth",2155853895],["Optical",2155845735]]},{"name":"Set 3: Soundbar (0x04FB)","proto":"NEC","bits":32,"keys":[["Power",83574975,1],["Vol +",83566815],["Vol -",83599455],["Mute",83583135],["Mode",83595375],["Bluetooth",83562735]]}]}},"rgb":{"rgb24":{"name":"24-Key Magic RGB LED","sets":[{"name":"Set 1: Standard 24-Key (0x00F7)","proto":"NEC","bits":32,"keys":[["ON",16236607,1],["OFF",16203967,1],["Bright +",16187647],["Bright -",16220287],["Red",16195807],["Green",16228447],["Blue",16212127],["White",16244767],["Orange",16191727],["Pea Green",16224367],["Dark Blue",16208047],["Dark Yellow",16199887],["Cyan",16232527],["Brown",16216207],["Yellow",16189687],["Light Blue",16222327],["Pink",16206007],["Flash",16240687],["Strobe",16248847],["Fade",16238647],["Smooth",16246807]]},{"name":"Set 2: Variant 24-Key (0x00FF)","proto":"NEC","bits":32,"keys":[["ON",16712445,1],["OFF",16745085,1],["Bright +",16726725],["Bright -",16759365],["Red",16718565],["Green",16751205],["Blue",16753245],["White",16720605],["Flash",16722645],["Strobe",16755285],["Fade",16724685],["Smooth",16757325]]}]},"rgb44":{"name":"44-Key Multi-Color RGB LED","sets":[{"name":"Set 1: Full 44-Key DIY Controller","proto":"NEC","bits":32,"keys":[["ON",16712445,1],["OFF",16745085,1],["Bright +",16726725],["Bright -",16759365],["Play/Pause",16714485],["Red",16718565],["Green",16751205],["Blue",16753245],["White",16720605],["Red Up",16722135],["Red Down",16754775],["Green Up",16752735],["Green Down",16736415],["Blue Up",16738455],["Blue Down",16771095],["DIY 1",16724175],["DIY 2",16756815],["DIY 3",16740495],["DIY 4",16773135],["DIY 5",16713975],["DIY 6",16746615],["Auto",16730295],["Flash",16764975],["Speed +",16769055],["Speed -",16736415],["Jump 3",16720095],["Jump 7",16752735],["Fade 3",16726215],["Fade 7",16758855]]}]},"rgb17":{"name":"17-Key Mini RF/IR Strip","sets":[{"name":"Set 1: Standard 17-Key (0x00F7)","proto":"NEC","bits":32,"keys":[["Power",16203967,1],["Light",16187647],["Bright 100%",16220287],["Bright 50%",16195807],["Bright 25%",16228447],["Mode +",16212127],["Mode -",16244767],["Speed +",16191727],["Speed -",16224367]]}]},"tuya_rgb":{"name":"Tuya Smart Life IR LED","sets":[{"name":"Set 1: Tuya RGBW (0x00BF)","proto":"NEC","bits":32,"keys":[["Power",12574751,1],["Mode",12542111],["Bright +",12554351],["Bright -",12521711],["Red",12525791],["Green",12558431],["Blue",12542111],["White",12574751],["Music",12538031]]},{"name":"Set 2: Tuya Strip Variant (0x807F)","proto":"NEC","bits":32,"keys":[["Power",2155864095,1],["Bright +",2155843695],["Bright -",2155811055],["Color",2155815135],["Scene",2155827375]]}]},"govee":{"name":"Govee RGB & Dreamcolor","sets":[{"name":"Set 1: Govee 44/24-Key (0x01FE)","proto":"NEC","bits":32,"keys":[["Power",33441975,1],["Bright +",33446055],["Bright -",33478695],["Warm",33454215],["Red",33429735],["Green",33462375],["Blue",33470535],["Music",33437895]]},{"name":"Set 2: Govee USB Strip (0x00FF)","proto":"NEC","bits":32,"keys":[["Power",16711935,1],["Mode",16728255],["Speed",16760895],["Color",16720095]]}]},"mini_usb_rgb":{"name":"Mini USB LED Controller","sets":[{"name":"Set 1: Mini 6-Key (0x00FF)","proto":"NEC","bits":32,"keys":[["Power",16712445,1],["Bright +",16736415],["Bright -",16752735],["Mode",16728255],["Color",16744575],["Speed",16720095]]}]}},"ac":{"gree":{"name":"Gree AC","sets":[{"name":"Set 1: Standard Gree","proto":"NEC","bits":32,"keys":[["Power",2122405035,1],["Temp +",2122391265],["Temp -",2122440225],["Mode Cool",2122407585],["Mode Heat",2122423905],["Fan Speed",2122399425],["Swing",2122395345],["Turbo",2122415745]]}]},"midea":{"name":"Midea AC","sets":[{"name":"Set 1: Standard Midea / Kaseikyo","proto":"Panasonic","bits":32,"keys":[["Power",2072305695,1],["Temp +",2072273055],["Temp -",2072289375],["Cool 24C",2072252655],["Heat 22C",2072285295],["Fan Auto",2072260815],["Swing",2072268975]]}]},"daikin":{"name":"Daikin AC","sets":[{"name":"Set 1: Standard Daikin","proto":"NEC","bits":32,"keys":[["Power",286146751,1],["Temp +",286187551],["Temp -",286183471],["Cool Mode",286163071],["Fan Auto",286152871],["Swing",286137061]]}]},"mitsubishi":{"name":"Mitsubishi AC","sets":[{"name":"Set 1: Standard Mitsubishi","proto":"NEC","bits":32,"keys":[["Power",600516313,1],["Temp +",600548953],["Temp -",600532633],["Cool Mode",600514273],["Heat Mode",600546913],["Fan Speed",600522433]]}]},"lg_ac":{"name":"LG Dual Inverter AC","sets":[{"name":"Set 1: Standard LG AC","proto":"NEC","bits":32,"keys":[["Power",143392849,1],["Temp +",142641999],["Temp -",142646111],["Jet Cool",142672025],["Fan Speed",142737576]]}]},"samsung_ac":{"name":"Samsung WindFree AC","sets":[{"name":"Set 1: Samsung AC","proto":"NEC","bits":32,"keys":[["Power",3772809343,1],["Temp +",3772793023],["Temp -",3772825663],["Mode Cool",3772784863],["WindFree",3772817503]]}]},"aux":{"name":"AUX AC","sets":[{"name":"Set 1: AUX Standard","proto":"NEC","bits":32,"keys":[["Power",3276587071,1],["Temp +",3276554431],["Temp -",3276570751],["Cool Mode",3276546271],["Fan Speed",3276578911]]}]},"haier":{"name":"Haier AC","sets":[{"name":"Set 1: Haier Standard","proto":"NEC","bits":32,"keys":[["Power",2707308735,1],["Temp +",2707349535],["Temp -",2707345455],["Cool Mode",2707325055],["Fan Speed",2707314855]]}]},"carrier":{"name":"Carrier AC","sets":[{"name":"Set 1: Carrier Standard","proto":"NEC","bits":32,"keys":[["Power",1303527615,1],["Temp +",1303568415],["Temp -",1303564335],["Cool Mode",1303543935],["Fan Auto",1303533735]]}]},"panasonic_ac":{"name":"Panasonic Inverter AC","sets":[{"name":"Set 1: Panasonic Inverter","proto":"Panasonic","bits":32,"keys":[["Power",1074004224,1],["Temp +",1074004228],["Temp -",1074004356],["Mode Cool",1074004300],["Fan Speed",1074004268]]}]}},"fan":{"dyson":{"name":"Dyson Pure / Cool Fan","sets":[{"name":"Set 1: Dyson Standard","proto":"NEC","bits":32,"keys":[["Power",1270145055,1],["Speed +",1270112415],["Speed -",1270128735],["Oscillate",1270096095],["Night Mode",1270145565],["Timer",1270092015]]}]},"xiaomi_fan":{"name":"Xiaomi Smart Fan","sets":[{"name":"Set 1: Xiaomi Fan","proto":"NEC","bits":32,"keys":[["Power",2188198079,1],["Speed +",2188238879],["Speed -",2188234799],["Oscillate",2188214399],["Natural Wind",2188187369]]}]},"midea_fan":{"name":"Midea Standing Fan","sets":[{"name":"Set 1: Midea Fan","proto":"NEC","bits":32,"keys":[["Power",12536246,1],["Speed",12534206],["Oscillate",12534461],["Timer",12534716],["Mode",12534971]]}]},"honeywell":{"name":"Honeywell Tower Fan","sets":[{"name":"Set 1: Honeywell Fan","proto":"NEC","bits":32,"keys":[["Power",16711935,1],["Speed",16728255],["Oscillate",16760895],["Timer",16720095],["Breeze",16744575]]}]},"lasko":{"name":"Lasko Wind Machine","sets":[{"name":"Set 1: Lasko Fan","proto":"NEC","bits":32,"keys":[["Power",16753245,1],["Speed",16736925],["Oscillate",16754775],["Timer",16769565]]}]}},"projector":{"epson":{"name":"Epson 3LCD Projector","sets":[{"name":"Set 1: Epson Standard","proto":"NEC","bits":32,"keys":[["Power",1102987455,1],["Source Search",1103003775],["Menu",1102993575],["Enter / OK",1102976745],["Esc",1102977765],["Keystone +",1102972665],["Keystone -",1103005305]]}]},"benq":{"name":"BenQ DLP Projector","sets":[{"name":"Set 1: BenQ Standard","proto":"NEC","bits":32,"keys":[["Power",16728255,1],["Source",16744575],["Menu",16769055],["OK",16748655],["Eco Blank",16736415],["Auto",16716015]]}]},"optoma":{"name":"Optoma Home Projector","sets":[{"name":"Set 1: Optoma Standard","proto":"NEC","bits":32,"keys":[["Power ON",284115135,1],["Power OFF",284147775,1],["Source",284131455],["Menu",284155935],["OK",284104425],["Freeze",284106975]]}]},"viewsonic":{"name":"ViewSonic Projector","sets":[{"name":"Set 1: ViewSonic Standard","proto":"NEC","bits":32,"keys":[["Power",2205958335,1],["Source",2205974655],["Menu",2205964455],["Enter",2205947625],["Keystone",2205943545]]}]}},"media":{"appletv":{"name":"Apple TV Remote","sets":[{"name":"Set 1: Apple TV Silver Remote","proto":"NEC","bits":32,"keys":[["Menu",2011250879],["Play/Pause",2011242719],["Up",2011287599],["Down",2011279439],["Left",2011238639],["Right",2011291679],["Select / OK",2011281989]]}]},"roku":{"name":"Roku Streaming Box","sets":[{"name":"Set 1: Roku IR Remote","proto":"NEC","bits":32,"keys":[["Home",1474551839],["Back",1474520729],["OK",1474533479],["Up",1474533989],["Down",1474517669],["Left",1474501349],["Right",1474550309],["Play/Pause",1474535519],["Replay",1474503389]]}]},"mibox":{"name":"Xiaomi Mi Box S","sets":[{"name":"Set 1: Mi Box Standard","proto":"NEC","bits":32,"keys":[["Power",2188198079,1],["Home",2188197569],["Back",2188186859],["OK",2188187369],["Up",2188183289],["Down",2188215929],["Left",2188224089],["Right",2188199609],["Vol +",2188238879],["Vol -",2188234799]]}]},"sony_bd":{"name":"Sony Blu-ray Player","sets":[{"name":"Set 1: Sony BD Player (15-bit)","proto":"Sony","bits":15,"keys":[["Power",21688,1],["Play",11448],["Stop",7352],["Pause",19640],["Home",1464],["Top Menu",15800],["OK",23992]]}]}}};

const i18n = {
  en: {
    tab_dash: "Dashboard",
    tab_ir: "IR Remote",
    tab_sniff: "Sniffer",
    tab_manual: "Manual",
    tab_fuzzer: "Fuzzer",
    tab_wiring: "Wiring & About",
    tab_settings: "Settings",
    cat_all: "All",
    cat_gate: "Gate",
    cat_garage: "Garage",
    cat_lighting: "Lighting",
    cat_blinds: "Blinds",
    cat_ac: "AC",
    cat_tv: "TV",
    cat_audio: "Audio",
    cat_fan: "Fan",
    cat_other: "Other",
    dash_title: "Smart Home Dashboard",
    add_device: "Add Device",
    refresh: "Refresh",
    no_presets: "No devices configured yet. Click 'Add Device' or sniff in Sniffer / IR!",
    live_sniffer: "Live RF Sniffer",
    sniff_desc: "Listening continuously on",
    waiting_sig: "Waiting for RF remote signal...",
    clear_btn: "Clear",
    man_title: "Manual RF Emulation",
    man_code: "Radio Code (Hex or Decimal)",
    man_bits: "Bit Length",
    man_proto: "Protocol (1 to 12)",
    man_pulse: "Pulse Length (µs)",
    man_freq: "Frequency (MHz)",
    man_reps: "Repeat Transmissions",
    single_burst: "🚀 Single Burst",
    save_preset: "💾 Save to Dashboard",
    fuzzer_title: "Sub-GHz Fuzzer / Brute-Force",
    fuzzer_desc: "Fast Sub-GHz protocol sweep, 12-bit / 10-bit gate brute-forcing, and frequency range fuzzer (Flipper Zero style).",
    fuzz_profile: "Fuzzer Profile",
    fuzz_start: "▶ START FUZZER",
    fuzz_stop: "⏹ STOP",
    ir_brand_lookup: "Appliance Brand Presets",
    appliance_type: "Appliance Type",
    brand_name: "Brand & Model",
    ir_sniffer_title: "IR Signal Sniffer / Learn",
    ir_sniffer_desc: "Interrupt capture on GPIO0 is idle to save CPU. Click below to start listening for incoming signals.",
    ir_waiting: "Press 'Start Recording IR' then aim your remote at GPIO0.",
    ir_btn_record: "🔴 Start Recording IR",
    ir_btn_stop: "⏹ Stop Recording",
    qr_title: "Quick Connect QR Code",
    wifi_title: "Wi-Fi Network",
    wifi_scan: "Scan",
    wifi_connect: "Connect & Save",
    wifi_forget: "Forget Wi-Fi",
    rf_title: "Radio Frequency",
    rf_save: "Save RF Settings",
    footer_text: "GateKeeper CC1101 + IR IoT Portal • ESP32 Super Mini",
    modal_add_title: "Add Device to Dashboard",
    code_set_label: "Code Set",
    test_power: "Test Power",
    cycle_next: "Next Set & Test",
    auto_scan: "Auto-Scan Sets"
  },
  pt: {
    tab_dash: "Painel",
    tab_ir: "Comando IR",
    tab_sniff: "Rastreador",
    tab_manual: "Manual",
    tab_fuzzer: "Fuzzer",
    tab_wiring: "Ligações e Sobre",
    tab_settings: "Definições",
    cat_all: "Todos",
    cat_gate: "Portão",
    cat_garage: "Garagem",
    cat_lighting: "Iluminação",
    cat_blinds: "Estores",
    cat_ac: "AC",
    cat_tv: "TV",
    cat_audio: "Áudio",
    cat_fan: "Ventoinha",
    cat_other: "Outro",
    dash_title: "Painel Domótica e Comandos",
    add_device: "Adicionar Equipamento",
    refresh: "Atualizar",
    no_presets: "Nenhum equipamento configurado. Prima 'Adicionar Equipamento' ou grave via Rastreador / IR!",
    live_sniffer: "Rastreador RF em Direto",
    sniff_desc: "Escutando continuamente em",
    waiting_sig: "Aguardando sinal RF de comando...",
    clear_btn: "Limpar",
    man_title: "Emulação RF Manual",
    man_code: "Código de Rádio (Hex ou Decimal)",
    man_bits: "Tamanho de Bits",
    man_proto: "Protocolo (1 a 12)",
    man_pulse: "Comprimento de Pulso (µs)",
    man_freq: "Frequência (MHz)",
    man_reps: "Repetições",
    single_burst: "🚀 Disparo Único",
    save_preset: "💾 Guardar no Painel",
    fuzzer_title: "Fuzzer Sub-GHz / Força Bruta",
    fuzzer_desc: "Varrimento rápido de protocolos, força bruta de portões e scanner de frequências (estilo Flipper Zero).",
    fuzz_profile: "Perfil do Fuzzer",
    fuzz_start: "▶ INICIAR FUZZER",
    fuzz_stop: "⏹ PARAR",
    ir_brand_lookup: "Modelos de Eletrodomésticos",
    appliance_type: "Tipo de Equipamento",
    brand_name: "Marca e Modelo",
    ir_sniffer_title: "Rastreador / Leitor de Sinal IR",
    ir_sniffer_desc: "A captura por interrupção no GPIO0 está desativada para poupar CPU. Prima abaixo para começar a escutar.",
    ir_waiting: "Prima 'Gravar Sinal IR' e aponte o comando para o GPIO0.",
    ir_btn_record: "🔴 Gravar Sinal IR",
    ir_btn_stop: "⏹ Parar Gravação",
    qr_title: "Código QR de Ligação Rápida",
    wifi_title: "Rede Wi-Fi",
    wifi_scan: "Procurar",
    wifi_connect: "Ligar e Guardar",
    wifi_forget: "Esquecer Wi-Fi",
    rf_title: "Frequência de Rádio",
    rf_save: "Guardar Definições RF",
    footer_text: "GateKeeper CC1101 + IR Portal IoT • ESP32 Super Mini",
    modal_add_title: "Adicionar ao Painel",
    code_set_label: "Conjunto",
    test_power: "Testar Ligar/Desl.",
    cycle_next: "Próximo & Testar",
    auto_scan: "Varrimento Auto"
  }
};

function toggleLanguage() {
  haptic();
  currentLang = (currentLang === 'en') ? 'pt' : 'en';
  localStorage.setItem('gk_lang', currentLang);
  applyLanguage();
}

function applyLanguage() {
  const t = i18n[currentLang];
  document.getElementById('btn-lang').innerText = (currentLang === 'en') ? 'PT' : 'EN';
  document.querySelectorAll('[data-i18n]').forEach(el => {
    const k = el.getAttribute('data-i18n');
    if (t[k]) el.innerText = t[k];
  });
  renderQuickPresets(loadedPresets);
}

function haptic(ms=35) {
  if (navigator.vibrate) navigator.vibrate(ms);
}

function showToast(msg, duration=2500) {
  const t = document.getElementById('toast');
  t.innerText = msg;
  t.classList.add('show');
  setTimeout(() => t.classList.remove('show'), duration);
}

async function switchTab(tabId, el) {
  haptic();
  const prevTab = currentTab;
  currentTab = tabId;
  document.querySelectorAll('.tab-btn').forEach(b => b.classList.remove('active'));
  document.querySelectorAll(`.tab-btn[onclick*="${tabId}"]`).forEach(b => b.classList.add('active'));
  document.querySelectorAll('.tab-content').forEach(c => c.classList.remove('active'));
  document.getElementById(tabId).classList.add('active');
  window.scrollTo({ top: 0, behavior: 'smooth' });

  // On-demand Sniffer management
  if (prevTab === 'tab-sniff' && tabId !== 'tab-sniff') {
    await stopRFListening();
  }
  if (tabId === 'tab-sniff') {
    await startRFListening();
    fetchSignals();
  }

  // On-demand IR management
  if (prevTab === 'tab-ir' && tabId !== 'tab-ir') {
    if (isIRListeningActive) {
      await stopIRListening();
    }
  }

  if (tabId === 'tab-dash') fetchPresets();
  if (tabId === 'tab-fuzzer') fetchFuzzerStatus();
}

function filterCategory(cat, btnEl) {
  haptic();
  selectedCatFilter = cat;
  document.querySelectorAll('.cat-chip').forEach(c => c.classList.remove('active'));
  btnEl.classList.add('active');
  renderQuickPresets(loadedPresets);
}

function toggleCustomFreq(val) {
  document.getElementById('grp-custom-freq').style.display = (val === 'custom') ? 'block' : 'none';
}

function toggleCodeOffInput(show) {
  document.getElementById('grp-edit-code-off').style.display = show ? 'block' : 'none';
}

function toggleAddCodeOffInput(show) {
  document.getElementById('grp-add-code-off').style.display = show ? 'block' : 'none';
}

function toggleAddDeviceType(typeVal) {
  const isRF = (parseInt(typeVal) === 0);
  document.getElementById('grp-add-rf-props').style.display = isRF ? 'grid' : 'none';
  document.getElementById('add-repeats').value = isRF ? 15 : 1;
}

function parseCode(str) {
  str = String(str).trim();
  if (str.startsWith('0x') || str.startsWith('0X')) {
    return parseInt(str, 16);
  }
  return parseInt(str, 10);
}

async function fetchStatus() {
  try {
    const res = await fetch('/api/status');
    const data = await res.json();
    deviceStatus = data;

    const ccBadge = document.getElementById('b-cc1101');
    if (data.cc1101_ok) {
      ccBadge.className = 'badge online';
      ccBadge.innerHTML = '<span>●</span> CC1101: OK';
    } else {
      ccBadge.className = 'badge warn';
      ccBadge.innerHTML = '<span>●</span> CC1101: Offline';
    }

    document.getElementById('b-freq').innerText = data.rf_freq.toFixed(2) + ' MHz';
    document.getElementById('sniff-freq-txt').innerText = data.rf_freq.toFixed(2) + ' MHz';

    const stBadge = document.getElementById('b-stream');
    if (data.is_streaming) {
      stBadge.style.display = 'flex';
      stBadge.className = 'badge live';
    } else {
      stBadge.style.display = 'none';
    }

    if (data.sta_connected) {
      document.getElementById('txt-sta-ip').innerText = data.sta_ip + ' (' + data.sta_ssid + ')';
    } else {
      document.getElementById('txt-sta-ip').innerText = 'Not Connected';
    }
    document.getElementById('txt-ap-ip').innerText = data.ap_ip;

    if (data.ir_signal_ready && currentTab === 'tab-ir') {
      fetchIRLatest();
    }
  } catch(e) {}
}

async function fetchPresets() {
  try {
    const res = await fetch('/api/presets');
    loadedPresets = await res.json();
    renderQuickPresets(loadedPresets);
  } catch(e) {}
}

function getCatIcon(cat) {
  switch(cat) {
    case 'Gate': return '🚪';
    case 'Garage': return '🚘';
    case 'Lighting': return '💡';
    case 'Blinds': return '🪟';
    case 'AC': return '❄️';
    case 'TV': return '📺';
    case 'Audio': return '🔊';
    case 'Fan': return '🌀';
    default: return '⚡';
  }
}

function renderQuickPresets(presets) {
  const container = document.getElementById('quick-presets-container');
  if (!presets || presets.length === 0) {
    container.innerHTML = `<p style="color:var(--muted); text-align:center; padding: 20px;">${i18n[currentLang].no_presets}</p>`;
    return;
  }

  const filtered = (selectedCatFilter === 'All') 
    ? presets 
    : presets.filter(p => (p.cat || 'Gate') === selectedCatFilter);

  if (filtered.length === 0) {
    container.innerHTML = `<p style="color:var(--muted); text-align:center; padding: 20px;">No items in category "${selectedCatFilter}".</p>`;
    return;
  }

  let html = '';
  filtered.forEach(p => {
    const icon = getCatIcon(p.cat);
    const isIR = (p.type === 1);
    const typeLabel = isIR ? '🔦 IR 38kHz' : '📡 RF 433MHz';
    const hexCode = '0x' + p.code.toString(16).toUpperCase();

    if (p.toggle) {
      const isOn = p.state;
      const stateTxt = isOn ? 'ON' : 'OFF';
      html += `
        <div class="preset-box">
          <div class="preset-meta-header">
            <span class="cat-badge">${icon} ${p.cat || 'Automation'}</span>
            <span class="type-badge">${typeLabel}</span>
          </div>
          <button class="btn-toggle ${isOn ? 'on' : 'off'}" onclick="triggerPreset(${p.id}, '${p.name}')">
            <span>${p.name.toUpperCase()}</span>
            <span class="toggle-switch-pill">${stateTxt}</span>
          </button>
          <div class="preset-card-footer">
            <span>${hexCode} • ${p.reps || 1}x</span>
            <div style="display:flex; gap:6px;">
              <button class="btn btn-outline btn-sm" onclick="openEditModal(${p.id})">✏</button>
              <button class="btn btn-danger btn-sm" onclick="deletePreset(${p.id})">🗑</button>
            </div>
          </div>
        </div>
      `;
    } else {
      html += `
        <div class="preset-box">
          <div class="preset-meta-header">
            <span class="cat-badge">${icon} ${p.cat || 'Gate'}</span>
            <span class="type-badge">${typeLabel}</span>
          </div>
          <button class="btn btn-gate" onclick="triggerPreset(${p.id}, '${p.name}')">
            <span>${icon} ${currentLang === 'pt' ? 'ACIONAR' : 'TRIGGER'} ${p.name.toUpperCase()}</span>
            <span style="font-size:0.72rem; font-weight:400; opacity:0.85;">
              ${hexCode} | ${isIR ? 'IR' : p.freq + ' MHz'} | ${p.reps}x
            </span>
          </button>
          <button class="btn btn-stream"
                  data-id="${p.id}"
                  onmousedown="startStreamPreset(${p.id}, this)"
                  onmouseup="stopStreamPreset(this)"
                  onmouseleave="stopStreamPreset(this)"
                  ontouchstart="startStreamPreset(${p.id}, this, event)"
                  ontouchend="stopStreamPreset(this, event)">
            🔴 HOLD TO STREAM (Continuous Burst)
          </button>
          <div class="preset-card-footer">
            <span>${isIR ? 'IR Infrared' : p.freq + ' MHz'}</span>
            <div style="display:flex; gap:6px;">
              <button class="btn btn-outline btn-sm" onclick="openEditModal(${p.id})">✏</button>
              <button class="btn btn-danger btn-sm" onclick="deletePreset(${p.id})">🗑</button>
            </div>
          </div>
        </div>
      `;
    }
  });
  container.innerHTML = html;
}

// Add Device Modal
function openAddDeviceModal() {
  haptic();
  document.getElementById('add-device-modal').classList.add('open');
  toggleAddDeviceType(document.getElementById('add-type').value);
}

function closeAddDeviceModal() {
  document.getElementById('add-device-modal').classList.remove('open');
}

async function submitAddDevice(e) {
  e.preventDefault();
  haptic();
  const name = document.getElementById('add-name').value;
  const cat = document.getElementById('add-cat').value;
  const type = parseInt(document.getElementById('add-type').value);
  const isToggle = (document.getElementById('add-toggle').value === '1');
  const code = parseCode(document.getElementById('add-code').value);
  const codeOff = parseCode(document.getElementById('add-code-off').value) || 0;
  const reps = parseInt(document.getElementById('add-repeats').value) || (type === 1 ? 1 : 15);
  const bits = parseInt(document.getElementById('add-bits').value) || 24;
  const proto = parseInt(document.getElementById('add-proto').value) || 1;
  const pulse = parseInt(document.getElementById('add-pulse').value) || 350;
  const freq = parseFloat(document.getElementById('add-freq').value) || 433.92;

  if (isNaN(code) || code === 0) {
    showToast('Invalid Code');
    return;
  }

  await savePresetObject({
    name: name,
    cat: cat,
    type: type,
    code: code,
    code_off: codeOff,
    toggle: isToggle,
    reps: reps,
    bits: bits,
    proto: proto,
    pulse: pulse,
    freq: freq
  });
  closeAddDeviceModal();
}

// Preset Edit Modal
function openEditModal(id) {
  const p = loadedPresets.find(x => x.id === id);
  if (!p) return;
  document.getElementById('edit-id').value = p.id;
  document.getElementById('edit-name').value = p.name;
  document.getElementById('edit-cat').value = p.cat || 'Gate';
  document.getElementById('edit-type').value = p.type || 0;
  document.getElementById('edit-toggle').value = p.toggle ? '1' : '0';
  document.getElementById('edit-code').value = '0x' + p.code.toString(16).toUpperCase();
  document.getElementById('edit-code-off').value = p.code_off ? ('0x' + p.code_off.toString(16).toUpperCase()) : '';
  toggleCodeOffInput(p.toggle);
  document.getElementById('edit-repeats').value = p.reps;
  document.getElementById('edit-bits').value = p.bits;
  document.getElementById('edit-proto').value = p.proto;
  document.getElementById('edit-pulse').value = p.pulse;
  document.getElementById('edit-freq').value = p.freq;
  document.getElementById('edit-modal').classList.add('open');
}

function closeEditModal() {
  document.getElementById('edit-modal').classList.remove('open');
}

async function submitEditPreset(e) {
  e.preventDefault();
  haptic();
  const id = parseInt(document.getElementById('edit-id').value);
  const name = document.getElementById('edit-name').value;
  const cat = document.getElementById('edit-cat').value;
  const type = parseInt(document.getElementById('edit-type').value);
  const isToggle = (document.getElementById('edit-toggle').value === '1');
  const code = parseCode(document.getElementById('edit-code').value);
  const codeOff = parseCode(document.getElementById('edit-code-off').value) || 0;
  const reps = parseInt(document.getElementById('edit-repeats').value);
  const bits = parseInt(document.getElementById('edit-bits').value);
  const proto = parseInt(document.getElementById('edit-proto').value);
  const pulse = parseInt(document.getElementById('edit-pulse').value);
  const freq = parseFloat(document.getElementById('edit-freq').value);

  try {
    const res = await fetch('/api/preset/update', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({
        id: id,
        name: name,
        cat: cat,
        type: type,
        toggle: isToggle,
        code: code,
        code_off: codeOff,
        reps: reps,
        bits: bits,
        proto: proto,
        pulse: pulse,
        freq: freq
      })
    });
    const data = await res.json();
    if (data.success) {
      showToast(`Device "${name}" updated!`);
      closeEditModal();
      fetchPresets();
    } else {
      showToast('Error updating');
    }
  } catch(e) {
    showToast('Network error');
  }
}

// Stream Mode
let isHoldingStream = false;

async function startStreamPreset(id, btnEl, evt) {
  if (evt) evt.preventDefault();
  if (isHoldingStream) return;
  isHoldingStream = true;
  haptic(60);
  btnEl.classList.add('active');
  btnEl.innerText = '⚡ TRANSMITTING (RELEASE TO STOP)';
  try {
    await fetch(`/api/stream/start?id=${id}`, { method: 'POST' });
  } catch(e) {}
}

async function stopStreamPreset(btnEl, evt) {
  if (evt) evt.preventDefault();
  if (!isHoldingStream) return;
  isHoldingStream = false;
  haptic(30);
  btnEl.classList.remove('active');
  btnEl.innerText = '🔴 HOLD TO STREAM (Continuous Burst)';
  try {
    await fetch('/api/stream/stop', { method: 'POST' });
  } catch(e) {}
}

// Manual Tab Stream
const manStreamBtn = document.getElementById('btn-manual-stream');
manStreamBtn.addEventListener('mousedown', () => startManualStream());
manStreamBtn.addEventListener('mouseup', () => stopManualStream());
manStreamBtn.addEventListener('mouseleave', () => stopManualStream());
manStreamBtn.addEventListener('touchstart', (e) => { e.preventDefault(); startManualStream(); });
manStreamBtn.addEventListener('touchend', (e) => { e.preventDefault(); stopManualStream(); });

async function startManualStream() {
  if (isHoldingStream) return;
  isHoldingStream = true;
  haptic(60);
  manStreamBtn.classList.add('active');
  manStreamBtn.innerText = '⚡ TRANSMITTING STREAM (RELEASE TO STOP)';
  const code = parseCode(document.getElementById('man-code').value);
  const bits = parseInt(document.getElementById('man-bits').value);
  const proto = parseInt(document.getElementById('man-proto').value);
  const pulse = parseInt(document.getElementById('man-pulse').value);
  const freq = parseFloat(document.getElementById('man-freq').value);

  try {
    await fetch('/api/stream/start', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ code: code, bits: bits, proto: proto, pulse: pulse, freq: freq })
    });
  } catch(e) {}
}

async function stopManualStream() {
  if (!isHoldingStream) return;
  isHoldingStream = false;
  haptic(30);
  manStreamBtn.classList.remove('active');
  manStreamBtn.innerText = '🔴 HOLD TO STREAM (Continuous TX)';
  try {
    await fetch('/api/stream/stop', { method: 'POST' });
  } catch(e) {}
}

// Sniffer Stream / Burst (Hold to Stream button right next to Fire button)
async function startSnifferStream(code, bits, proto, pulse, freq, btnEl, evt) {
  if (evt) evt.preventDefault();
  if (isHoldingStream) return;
  isHoldingStream = true;
  haptic(60);
  btnEl.classList.add('active');
  btnEl.innerText = '⚡ BURSTING';
  try {
    await fetch('/api/stream/start', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ code: code, bits: bits, proto: proto, pulse: pulse, freq: freq })
    });
  } catch(e) {}
}

async function stopSnifferStream(btnEl, evt) {
  if (evt) evt.preventDefault();
  if (!isHoldingStream) return;
  isHoldingStream = false;
  haptic(30);
  btnEl.classList.remove('active');
  btnEl.innerText = '⚡ Burst';
  try {
    await fetch('/api/stream/stop', { method: 'POST' });
  } catch(e) {}
}

async function triggerPreset(id, name) {
  haptic(60);
  showToast(`Emulating ${name}...`);
  try {
    const res = await fetch(`/api/preset/trigger?id=${id}`);
    const data = await res.json();
    if (data.success) {
      haptic(100);
      showToast(`Triggered: ${name}!`);
      fetchPresets();
    } else {
      showToast(`Error: ${data.error || 'Failed'}`);
    }
  } catch(e) {
    showToast('Network error triggering preset');
  }
}

async function deletePreset(id) {
  haptic();
  if (!confirm('Are you sure you want to delete this device?')) return;
  try {
    const res = await fetch(`/api/preset/delete?id=${id}`, { method: 'POST' });
    const data = await res.json();
    if (data.success) {
      showToast('Device deleted');
      fetchPresets();
    }
  } catch(e) {
    showToast('Failed to delete');
  }
}

// On-Demand RF Listening
async function startRFListening() {
  try {
    await fetch('/api/rf/listen/start', { method: 'POST' });
    isRFListeningActive = true;
    const b = document.getElementById('badge-rf-listen');
    if (b) { b.className = 'badge online'; b.innerText = 'RECEIVER ACTIVE'; }
  } catch(e) {}
}

async function stopRFListening() {
  try {
    await fetch('/api/rf/listen/stop', { method: 'POST' });
    isRFListeningActive = false;
    const b = document.getElementById('badge-rf-listen');
    if (b) { b.className = 'badge'; b.innerText = 'IDLE (OFF)'; }
  } catch(e) {}
}

// On-Demand IR Listening
async function toggleIRListening() {
  haptic();
  if (!isIRListeningActive) {
    await startIRListening();
  } else {
    await stopIRListening();
  }
}

async function startIRListening() {
  try {
    await fetch('/api/ir/listen/start', { method: 'POST' });
    isIRListeningActive = true;
    const btn = document.getElementById('btn-ir-listen-toggle');
    const badge = document.getElementById('badge-ir-listen');
    const dot = document.getElementById('ir-rx-dot');
    btn.className = 'btn btn-danger btn-sm';
    btn.innerText = (currentLang === 'pt') ? '⏹ Parar Gravação' : '⏹ Stop Recording';
    badge.className = 'badge live';
    badge.innerText = 'RECORDING...';
    dot.style.background = 'var(--danger)';
    showToast('IR Receiver listening on GPIO0');
    fetchIRLatest();
  } catch(e) {}
}

async function stopIRListening() {
  try {
    await fetch('/api/ir/listen/stop', { method: 'POST' });
    isIRListeningActive = false;
    const btn = document.getElementById('btn-ir-listen-toggle');
    const badge = document.getElementById('badge-ir-listen');
    const dot = document.getElementById('ir-rx-dot');
    btn.className = 'btn btn-warning btn-sm';
    btn.innerText = (currentLang === 'pt') ? '🔴 Gravar Sinal IR' : '🔴 Start Recording IR';
    badge.className = 'badge';
    badge.innerText = 'SLEEPING (OFF)';
    dot.style.background = '#64748b';
  } catch(e) {}
}

// Infrared Transmit & Learning
async function fireIR(code, proto="NEC", bits=32, reps=3) {
  haptic(60);
  showToast(`Emulating IR ${proto} 0x${code.toString(16).toUpperCase()} (${reps} reps)...`);
  try {
    const res = await fetch('/api/ir/transmit', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ code: code, proto: proto, bits: bits, reps: reps })
    });
    const data = await res.json();
    if (data.success) {
      showToast('IR Code Transmitted!');
    } else {
      showToast('IR transmission failed');
    }
  } catch(e) {
    showToast('Network error');
  }
}

async function fireRawIR(rawArr, reps=2) {
  if (!rawArr || rawArr.length === 0) {
    showToast('No raw signal available');
    return;
  }
  haptic(60);
  showToast(`Replaying Raw IR (${rawArr.length} pulses, ${reps} reps)...`);
  try {
    const res = await fetch('/api/ir/transmit', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ raw: rawArr, reps: reps })
    });
    const data = await res.json();
    if (data.success) {
      showToast('Raw IR Signal Transmitted!');
    } else {
      showToast('Raw IR transmission failed');
    }
  } catch(e) {
    showToast('Network error');
  }
}

async function quickSaveIRPreset(name, code, cat="Other", isToggle=false, codeOff=0, proto="NEC", bits=32, rawArr=null) {
  haptic();
  const isRaw = (rawArr && rawArr.length > 0);
  savePresetObject({
    name: name,
    cat: cat,
    type: 1, // IR
    code: code,
    bits: bits,
    proto: (proto === "Sony" ? 2 : (proto === "Panasonic" ? 3 : 1)),
    pulse: 560,
    freq: 38.0,
    reps: 2,
    toggle: isToggle,
    code_off: codeOff,
    raw: isRaw,
    raw_data: isRaw ? rawArr.slice(0, 150) : null
  });
}

let latestCapturedIR = null;

async function fetchIRLatest() {
  try {
    const res = await fetch('/api/ir/latest');
    const data = await res.json();
    const box = document.getElementById('ir-latest-container');
    if (data.ready) {
      latestCapturedIR = data;
      const rawCount = data.raw_count || (data.raw ? data.raw.length : 0);
      box.innerHTML = `
        <div style="display:flex; justify-content:space-between; align-items:center;">
          <span style="font-size:1.25rem; font-weight:800; color:var(--primary); font-family:monospace;">${data.hex}</span>
          <span class="type-badge">${data.proto} (${data.bits} bits • ${rawCount} pulses)</span>
        </div>
        <div style="font-size:0.75rem; color:var(--muted); margin:6px 0;">
          <span>Decimal: <strong>${data.code}</strong> | Pulses: <strong>${rawCount}</strong> (Hardware 38kHz)</span>
        </div>
        <div class="sig-actions-3" style="margin-top:8px; display:flex; gap:6px; flex-wrap:wrap;">
          <button class="btn btn-primary btn-sm" onclick="fireRawIR(latestCapturedIR.raw, 2)">▶ Replay Raw (Exact)</button>
          <button class="btn btn-outline btn-sm" onclick="fireIR(${data.code}, '${data.proto}', ${data.bits}, 3)">⚡ Fire Decoded</button>
          <button class="btn btn-accent btn-sm" onclick="promptSaveIRPreset(${data.code}, '${data.proto}', ${data.bits}, latestCapturedIR.raw)">💾 Save</button>
          <button class="btn btn-outline btn-sm" onclick="clearIRSignal()">🗑 Clear</button>
        </div>
      `;
    } else {
      box.innerHTML = `<p style="color:var(--muted); text-align:center;">${i18n[currentLang].ir_waiting}</p>`;
    }
  } catch(e) {}
}

async function clearIRSignal() {
  haptic();
  await fetch('/api/ir/clear', { method: 'POST' });
  fetchIRLatest();
  showToast('IR buffer cleared');
}

function promptSaveIRPreset(code, proto, bits, rawArr) {
  haptic();
  const name = prompt('Name for this IR device/button:', 'IR Remote Key');
  if (!name) return;
  quickSaveIRPreset(name, code, 'Other', false, 0, proto, bits, rawArr);
}

// Appliance Combobox & Keypad
let currentCodeSetIdx = 0;
let autoScanInterval = null;

function onApplianceTypeChanged(appType) {
  const brandSel = document.getElementById('ir-brand-select');
  brandSel.innerHTML = '';
  const brands = IR_APPLIANCES[appType] || {};
  for (const bKey in brands) {
    const opt = document.createElement('option');
    opt.value = bKey;
    opt.innerText = brands[bKey].name;
    brandSel.appendChild(opt);
  }
  currentCodeSetIdx = 0;
  stopAutoScanner();
  updateCodeSetOptions();
  renderApplianceKeypad();
}

function onBrandChanged() {
  currentCodeSetIdx = 0;
  stopAutoScanner();
  updateCodeSetOptions();
  renderApplianceKeypad();
}

function updateCodeSetOptions() {
  const appType = document.getElementById('ir-appliance-type').value;
  const brandKey = document.getElementById('ir-brand-select').value;
  const setSel = document.getElementById('ir-codeset-select');
  if (!setSel) return;
  setSel.innerHTML = '';
  const brandData = IR_APPLIANCES[appType]?.[brandKey];
  if (!brandData || !brandData.sets) return;

  if (currentCodeSetIdx >= brandData.sets.length) currentCodeSetIdx = 0;

  brandData.sets.forEach((s, idx) => {
    const opt = document.createElement('option');
    opt.value = idx;
    opt.innerText = s.name;
    if (idx === currentCodeSetIdx) opt.selected = true;
    setSel.appendChild(opt);
  });

  updateCodeSetIndicator();
}

function updateCodeSetIndicator() {
  const appType = document.getElementById('ir-appliance-type').value;
  const brandKey = document.getElementById('ir-brand-select').value;
  const brandData = IR_APPLIANCES[appType]?.[brandKey];
  const total = brandData?.sets?.length || 1;
  const ind = document.getElementById('codeset-indicator');
  if (ind) {
    ind.innerText = `Set ${currentCodeSetIdx + 1} of ${total}`;
  }
}

function onCodeSetSelected(idx) {
  currentCodeSetIdx = idx;
  updateCodeSetIndicator();
  renderApplianceKeypad();
}

function prevCodeSet() {
  haptic();
  const appType = document.getElementById('ir-appliance-type').value;
  const brandKey = document.getElementById('ir-brand-select').value;
  const brandData = IR_APPLIANCES[appType]?.[brandKey];
  if (!brandData || !brandData.sets) return;
  currentCodeSetIdx = (currentCodeSetIdx - 1 + brandData.sets.length) % brandData.sets.length;
  const setSel = document.getElementById('ir-codeset-select');
  if (setSel) setSel.value = currentCodeSetIdx;
  updateCodeSetIndicator();
  renderApplianceKeypad();
}

function nextCodeSet() {
  haptic();
  const appType = document.getElementById('ir-appliance-type').value;
  const brandKey = document.getElementById('ir-brand-select').value;
  const brandData = IR_APPLIANCES[appType]?.[brandKey];
  if (!brandData || !brandData.sets) return;
  currentCodeSetIdx = (currentCodeSetIdx + 1) % brandData.sets.length;
  const setSel = document.getElementById('ir-codeset-select');
  if (setSel) setSel.value = currentCodeSetIdx;
  updateCodeSetIndicator();
  renderApplianceKeypad();
}

function testActivePowerCode() {
  haptic();
  const appType = document.getElementById('ir-appliance-type').value;
  const brandKey = document.getElementById('ir-brand-select').value;
  const brandData = IR_APPLIANCES[appType]?.[brandKey];
  if (!brandData || !brandData.sets) return;
  const setObj = brandData.sets[currentCodeSetIdx] || brandData.sets[0];
  if (!setObj || !setObj.keys || setObj.keys.length === 0) return;

  let pKey = setObj.keys.find(k => k[0].toLowerCase().includes('power') || k[0].toLowerCase() === 'on');
  if (!pKey) pKey = setObj.keys[0];

  fireIR(pKey[1], setObj.proto, setObj.bits);
  showToast(`⚡ ${setObj.name}: ${pKey[0]} (0x${pKey[1].toString(16).toUpperCase()})`);
}

function cycleNextSetAndTest() {
  nextCodeSet();
  testActivePowerCode();
}

function toggleAutoScanner() {
  if (autoScanInterval) {
    stopAutoScanner();
  } else {
    startAutoScanner();
  }
}

function startAutoScanner() {
  haptic();
  const btn = document.getElementById('btn-auto-scanner');
  const statusEl = document.getElementById('scanner-status');
  if (btn) {
    btn.className = 'btn btn-danger btn-sm';
    btn.innerHTML = '⏹ ' + (currentLang === 'pt' ? 'Parar Varrimento' : 'Stop Scan');
  }
  if (statusEl) {
    statusEl.style.display = 'block';
    statusEl.innerText = (currentLang === 'pt' ? 'Varrimento ativo: disparando códigos Power de 2 em 2 segundos...' : 'Auto-scanning: firing Power codes every 2 seconds...');
  }

  testActivePowerCode();
  autoScanInterval = setInterval(() => {
    cycleNextSetAndTest();
  }, 2200);
}

function stopAutoScanner() {
  if (autoScanInterval) {
    clearInterval(autoScanInterval);
    autoScanInterval = null;
  }
  const btn = document.getElementById('btn-auto-scanner');
  if (btn) {
    btn.className = 'btn btn-outline btn-sm';
    btn.innerHTML = '🔄 <span data-i18n="auto_scan">' + (currentLang === 'pt' ? 'Varrimento Auto' : 'Auto-Scan Sets') + '</span>';
  }
  const statusEl = document.getElementById('scanner-status');
  if (statusEl) statusEl.style.display = 'none';
}

function renderApplianceKeypad() {
  const appType = document.getElementById('ir-appliance-type').value;
  const brandKey = document.getElementById('ir-brand-select').value;
  const container = document.getElementById('appliance-keypad-container');
  container.innerHTML = '';

  const brandData = IR_APPLIANCES[appType]?.[brandKey];
  if (!brandData || !brandData.sets) return;

  const setObj = brandData.sets[currentCodeSetIdx] || brandData.sets[0];
  if (!setObj) return;

  const categoryMap = {
    tv: 'TV',
    ac: 'AC',
    audio: 'Audio',
    projector: 'TV',
    fan: 'Fan',
    rgb: 'Lighting',
    media: 'TV'
  };
  const cat = categoryMap[appType] || 'Other';

  setObj.keys.forEach(k => {
    const kName = k[0];
    const kCode = k[1];
    const kToggle = k[2] === 1;
    const hexStr = '0x' + kCode.toString(16).toUpperCase();
    const keyBox = document.createElement('div');
    keyBox.className = 'ir-key-box';
    keyBox.innerHTML = `
      <span class="ir-key-name">${kName}</span>
      <span class="ir-key-hex">${setObj.proto} ${setObj.bits}b • ${hexStr}</span>
      <div class="ir-key-actions">
        <button class="btn btn-primary btn-sm" onclick="fireIR(${kCode}, '${setObj.proto}', ${setObj.bits})">▶</button>
        <button class="btn btn-outline btn-sm" title="Add to Dashboard" onclick="quickSaveIRPreset('${brandData.name} ${kName}', ${kCode}, '${cat}', ${kToggle ? 'true' : 'false'}, 0, '${setObj.proto}', ${setObj.bits})">➕</button>
      </div>
    `;
    container.appendChild(keyBox);
  });
}


// Sniffer / RF Signals
async function fetchSignals() {
  if (currentTab !== 'tab-sniff') return;
  try {
    const res = await fetch('/api/signals');
    const signals = await res.json();
    const container = document.getElementById('signals-container');
    if (!signals || signals.length === 0) {
      container.innerHTML = `<p style="color:var(--muted); text-align:center; padding: 20px;">${i18n[currentLang].waiting_sig}</p>`;
      return;
    }
    let html = '';
    signals.forEach(s => {
      html += `
        <div class="sig-card">
          <div class="sig-header">
            <div class="sig-val">${s.hex}</div>
            <span style="font-size:0.75rem; color:var(--muted);">${s.repeats}x burst</span>
          </div>
          <div class="sig-meta">
            <span>Dec: <strong>${s.code}</strong></span>
            <span>Bits: <strong>${s.bits}</strong></span>
            <span>Proto: <strong>${s.proto}</strong></span>
            <span>Pulse: <strong>${s.pulse}µs</strong></span>
            <span>RSSI: <strong>${s.rssi} dBm</strong></span>
          </div>
          <div class="sig-bin">${s.bin}</div>
          <div class="sig-actions-3">
            <button class="btn btn-success btn-sm" onclick="emulateSignal(${s.code}, ${s.bits}, ${s.proto}, ${s.pulse}, ${s.freq})">▶ Fire</button>
            <button class="btn-stream-sm"
                    onmousedown="startSnifferStream(${s.code}, ${s.bits}, ${s.proto}, ${s.pulse}, ${s.freq}, this)"
                    onmouseup="stopSnifferStream(this)"
                    onmouseleave="stopSnifferStream(this)"
                    ontouchstart="startSnifferStream(${s.code}, ${s.bits}, ${s.proto}, ${s.pulse}, ${s.freq}, this, event)"
                    ontouchend="stopSnifferStream(this, event)">
              ⚡ Burst
            </button>
            <button class="btn btn-accent btn-sm" onclick="promptSavePreset(${s.code}, ${s.bits}, ${s.proto}, ${s.pulse}, ${s.freq})">💾 Save</button>
          </div>
        </div>
      `;
    });
    container.innerHTML = html;
  } catch(e) {}
}

async function clearSignals() {
  await fetch('/api/clear_signals', { method: 'POST' });
  fetchSignals();
  showToast('Signal history cleared');
}

async function emulateSignal(code, bits, proto, pulse, freq) {
  haptic(50);
  showToast('Emulating captured signal...');
  try {
    const res = await fetch('/api/transmit', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ code: code, bits: bits, proto: proto, pulse: pulse, freq: freq, reps: 15 })
    });
    const data = await res.json();
    if (data.success) {
      haptic(100);
      showToast('Signal transmitted!');
    } else {
      showToast('Transmit error');
    }
  } catch(e) {
    showToast('Failed to transmit');
  }
}

function promptSavePreset(code, bits, proto, pulse, freq) {
  haptic();
  const name = prompt('Enter a name for this remote:', 'Front Gate');
  if (!name) return;
  savePresetObject({
    name: name,
    cat: 'Gate',
    type: 0,
    code: code,
    bits: bits,
    proto: proto,
    pulse: pulse,
    freq: freq,
    reps: 15,
    toggle: false,
    code_off: 0
  });
}

async function savePresetObject(obj) {
  try {
    const res = await fetch('/api/preset/add', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(obj)
    });
    const data = await res.json();
    if (data.success) {
      showToast(`Saved to Dashboard: "${obj.name}"!`);
      fetchPresets();
    } else {
      showToast(`Error: ${data.error}`);
    }
  } catch(e) {
    showToast('Failed to save');
  }
}

// Manual Emulation
async function transmitManual(e) {
  e.preventDefault();
  haptic(50);
  const code = parseCode(document.getElementById('man-code').value);
  const bits = parseInt(document.getElementById('man-bits').value);
  const proto = parseInt(document.getElementById('man-proto').value);
  const pulse = parseInt(document.getElementById('man-pulse').value);
  const freq = parseFloat(document.getElementById('man-freq').value);
  const reps = parseInt(document.getElementById('man-repeats').value);

  if (isNaN(code) || code === 0) {
    showToast('Invalid code entered');
    return;
  }
  showToast(`Emulating code 0x${code.toString(16).toUpperCase()}...`);
  try {
    const res = await fetch('/api/transmit', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ code: code, bits: bits, proto: proto, pulse: pulse, freq: freq, reps: reps })
    });
    const data = await res.json();
    if (data.success) {
      haptic(100);
      showToast('Emulation complete!');
    } else {
      showToast('Error transmitting');
    }
  } catch(e) {
    showToast('Network error during transmission');
  }
}

function saveManualAsPreset() {
  haptic();
  const code = parseCode(document.getElementById('man-code').value);
  if (isNaN(code) || code === 0) {
    showToast('Enter a valid code first');
    return;
  }
  const name = prompt('Name for this device:', 'My Automation');
  if (!name) return;
  savePresetObject({
    name: name,
    cat: 'Gate',
    type: 0,
    code: code,
    bits: parseInt(document.getElementById('man-bits').value),
    proto: parseInt(document.getElementById('man-proto').value),
    pulse: parseInt(document.getElementById('man-pulse').value),
    freq: parseFloat(document.getElementById('man-freq').value),
    reps: parseInt(document.getElementById('man-repeats').value),
    toggle: false,
    code_off: 0
  });
}

// Fuzzer
function adjustFuzzerMode(val) {
  const v = parseInt(val);
  const rangeGrp = document.getElementById('grp-fuzz-range');
  const freqGrp = document.getElementById('grp-fuzz-freq-range');

  if (v === 1) {
    rangeGrp.style.display = 'grid';
    freqGrp.style.display = 'none';
    document.getElementById('fuzz-start').value = '0';
    document.getElementById('fuzz-end').value = '4095';
  } else if (v === 2) {
    rangeGrp.style.display = 'grid';
    freqGrp.style.display = 'none';
    document.getElementById('fuzz-start').value = '0';
    document.getElementById('fuzz-end').value = '1023';
  } else if (v === 3) {
    rangeGrp.style.display = 'grid';
    freqGrp.style.display = 'none';
    document.getElementById('fuzz-start').value = '0x1A2B3C';
    document.getElementById('fuzz-end').value = '0x1A2B3C';
  } else if (v === 4) {
    rangeGrp.style.display = 'none';
    freqGrp.style.display = 'grid';
  } else {
    rangeGrp.style.display = 'grid';
    freqGrp.style.display = 'none';
  }
}

async function startFuzzerAction(e) {
  e.preventDefault();
  haptic(60);
  const mode = parseInt(document.getElementById('fuzz-mode').value);
  const startVal = parseCode(document.getElementById('fuzz-start').value);
  const endVal = parseCode(document.getElementById('fuzz-end').value);
  const startFreq = parseFloat(document.getElementById('fuzz-start-freq').value);
  const endFreq = parseFloat(document.getElementById('fuzz-end-freq').value);
  const baseFreq = parseFloat(document.getElementById('fuzz-base-freq').value);
  const reps = parseInt(document.getElementById('fuzz-speed').value);
  const delay = (reps === 1) ? 15 : ((reps === 2) ? 25 : 45);

  const payload = {
    mode: mode,
    start: startVal,
    end: endVal,
    step: 1,
    proto: 1,
    bits: (mode === 1 ? 12 : (mode === 2 ? 10 : 24)),
    pulse: 350,
    freq: baseFreq,
    start_freq: startFreq,
    end_freq: endFreq,
    step_freq: 0.05,
    reps: reps,
    delay: delay
  };

  showToast('Starting Sub-GHz Fuzzer...');
  try {
    const res = await fetch('/api/fuzzer/start', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(payload)
    });
    const data = await res.json();
    if (data.success) {
      showToast('Fuzzer active!');
      startFuzzerPolling();
    } else {
      showToast('Failed to start fuzzer');
    }
  } catch(e) {
    showToast('Network error');
  }
}

async function stopFuzzerAction() {
  haptic(40);
  try {
    await fetch('/api/fuzzer/stop', { method: 'POST' });
    showToast('Fuzzer stopped.');
    fetchFuzzerStatus();
  } catch(e) {}
}

async function fetchFuzzerStatus() {
  try {
    const res = await fetch('/api/fuzzer/status');
    const st = await res.json();

    const badge = document.getElementById('fuzzer-badge');
    const termStat = document.getElementById('term-status');
    if (st.running) {
      badge.className = 'badge live';
      badge.innerText = 'FUZZING...';
      termStat.innerText = 'TRANSMITTING ACTIVE';
      termStat.style.color = '#ef4444';
      document.getElementById('btn-fuzz-start').disabled = true;
    } else {
      badge.className = 'badge';
      badge.innerText = 'IDLE';
      termStat.innerText = 'STANDBY';
      termStat.style.color = 'var(--muted)';
      document.getElementById('btn-fuzz-start').disabled = false;
    }

    document.getElementById('term-code').innerText = st.current_hex + ` (${st.current_code})`;
    document.getElementById('term-freq').innerText = st.current_freq.toFixed(2) + ' MHz';
    document.getElementById('term-count').innerText = `${st.completed} / ${st.total} (${st.percent.toFixed(1)}%)`;
    document.getElementById('term-bar').style.width = st.percent + '%';
  } catch(e) {}
}

function startFuzzerPolling() {
  if (fuzzerTimer) clearInterval(fuzzerTimer);
  fuzzerTimer = setInterval(async () => {
    await fetchFuzzerStatus();
  }, 400);
}

// Settings & Wi-Fi
async function saveRfConfig(e) {
  e.preventDefault();
  haptic();
  let freq = parseFloat(document.getElementById('cfg-freq-sel').value);
  if (document.getElementById('cfg-freq-sel').value === 'custom') {
    freq = parseFloat(document.getElementById('cfg-custom-freq').value);
  }
  const power = parseInt(document.getElementById('cfg-power').value);
  const reps = parseInt(document.getElementById('cfg-repeats').value);

  showToast('Saving RF settings...');
  try {
    const res = await fetch('/api/rf/config', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ freq: freq, power: power, repeats: reps })
    });
    const data = await res.json();
    if (data.success) {
      showToast('RF settings saved!');
      fetchStatus();
    } else {
      showToast('Failed to save RF config');
    }
  } catch(e) {
    showToast('Failed to save RF config');
  }
}

async function scanWifi() {
  haptic();
  const sel = document.getElementById('wifi-ssid-select');
  sel.innerHTML = '<option value="">Scanning...</option>';
  try {
    const res = await fetch('/api/wifi/scan');
    const networks = await res.json();
    sel.innerHTML = '<option value="">-- Choose a network --</option>';
    networks.forEach(net => {
      const opt = document.createElement('option');
      opt.value = net.ssid;
      opt.innerText = `${net.ssid} (${net.rssi} dBm)${net.open ? ' [Open]' : ''}`;
      sel.appendChild(opt);
    });
  } catch(e) {
    sel.innerHTML = '<option value="">Scan failed</option>';
  }
}

async function connectWifi(e) {
  e.preventDefault();
  haptic();
  const ssid = document.getElementById('wifi-ssid-input').value;
  const pass = document.getElementById('wifi-pass-input').value;
  showToast(`Connecting to "${ssid}"...`);
  try {
    const res = await fetch('/api/wifi/connect', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ ssid: ssid, pass: pass })
    });
    const data = await res.json();
    if (data.success) {
      showToast(`Connected to ${ssid}! IP: ${data.ip}`);
      fetchStatus();
    } else {
      showToast('Failed to connect');
    }
  } catch(e) {
    showToast('Connection attempt sent');
  }
}

async function forgetWifi() {
  haptic();
  if (!confirm('Forget saved home Wi-Fi and revert to Hotspot only?')) return;
  try {
    await fetch('/api/wifi/reset', { method: 'POST' });
    showToast('Operating in Hotspot mode.');
    fetchStatus();
  } catch(e) {
    showToast('Failed to reset Wi-Fi');
  }
}

// QR Code Generator Modal
function openQrModal() {
  haptic();
  document.getElementById('qr-modal').classList.add('open');
  switchQrMode(qrMode);
}

function closeQrModal() {
  document.getElementById('qr-modal').classList.remove('open');
}

function switchQrMode(mode) {
  qrMode = mode;
  const box = document.getElementById('qrcode-box');
  const btnWifi = document.getElementById('btn-qr-tab-wifi');
  const btnUrl = document.getElementById('btn-qr-tab-url');
  const desc = document.getElementById('qr-desc-txt');

  btnWifi.className = (mode === 'wifi') ? 'btn btn-primary btn-sm' : 'btn btn-outline btn-sm';
  btnUrl.className = (mode === 'url') ? 'btn btn-primary btn-sm' : 'btn btn-outline btn-sm';

  let textToEncode = '';
  if (mode === 'wifi') {
    const apSsid = deviceStatus.ap_ssid || 'ESP32-Gate-Remote';
    const apPass = deviceStatus.ap_pass || 'GateKeeper123';
    textToEncode = `WIFI:T:WPA;S:${apSsid};P:${apPass};;`;
    desc.innerText = (currentLang === 'pt') 
      ? 'Aponte a câmara do telemóvel para ligar diretamente ao Wi-Fi sem escrever a palavra-passe!'
      : 'Scan with your phone camera to connect directly to Wi-Fi without typing the password!';
  } else {
    textToEncode = window.location.origin || 'http://192.168.4.1/';
    desc.innerText = (currentLang === 'pt')
      ? 'Aponte a câmara do telemóvel para abrir a página do GateKeeper no navegador!'
      : 'Scan with your phone to open the GateKeeper dashboard in your browser!';
  }

  box.innerHTML = '';
  try {
    new QRCode(box, {
      text: textToEncode,
      width: 190,
      height: 190,
      colorDark: "#000000",
      colorLight: "#ffffff",
      correctLevel: QRCode.CorrectLevel.M
    });
  } catch(e) {}
}

window.addEventListener('DOMContentLoaded', () => {
  applyLanguage();
  onApplianceTypeChanged('tv');
  fetchStatus();
  fetchPresets();
  pollingTimer = setInterval(() => {
    fetchStatus();
    if (currentTab === 'tab-sniff') fetchSignals();
    if (currentTab === 'tab-fuzzer') fetchFuzzerStatus();
    if (currentTab === 'tab-ir' && isIRListeningActive) fetchIRLatest();
  }, 600);
});

</script>
</body>
</html>
)rawliteral";
