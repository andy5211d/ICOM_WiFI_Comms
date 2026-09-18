#include <WiFi.h>
#include <AsyncUDP.h>

#include "CIV_Handler.h"
#include "lvgl_v8_port.h"
#include "esp_panel_board_custom_conf.h"

// ---- Wi-Fi + CI-V config ----
const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// R8600 CI-V over LAN (example)
IPAddress icomHost(192, 168, 1, 100);   // change to your R8600/WFView host
const uint16_t icomPort = 50001;        // CI-V UDP port

// ---- State ----
AsyncUDP udp;
unsigned long lastCivRxMs   = 0;
unsigned long lastCivTxMs   = 0;
const unsigned long civKeepaliveIntervalMs = 1000;   // 1s polling
const unsigned long civWatchdogTimeoutMs   = 3000;   // 3s no RX -> watchdog

CIV_Handler civ;
bool icomConnected = false;

void setup() {
  Serial.begin(115200);
  delay(200);

  // Init display + LVGL
  panel_init();        // from esp_panel_board_custom_conf.h
  lvgl_port_init();    // from lvgl_v8_port.cpp
  lvgl_create_main_ui(); // your UI entry point (freq, status, etc.)

  // Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // UDP CI-V
  if (udp.listen(icomPort)) {
    Serial.print("Listening UDP on port ");
    Serial.println(icomPort);

    udp.onPacket([](AsyncUDPPacket packet) {
      size_t len = packet.length();
      if (len == 0) return;

      const uint8_t* data = packet.data();
      civ.handleFrame(data, len);

      lastCivRxMs = millis();
      icomConnected = true;

      // Update UI with latest radio state
      lvgl_update_radio_state(civ.getFrequencyHz(),
                              civ.getMode(),
                              civ.getSmeter());
    });
  } else {
    Serial.println("Failed to listen on UDP port");
  }

  lastCivRxMs = millis();
  lastCivTxMs = millis();
}

void loop() {
  // LVGL housekeeping
  lv_timer_handler();   // must be called regularly
  delay(5);

  unsigned long now = millis();

  // CI-V keepalive / polling
  if (now - lastCivTxMs >= civKeepaliveIntervalMs) {
    uint8_t pollBuf[16];
    size_t pollLen = civ.buildFrequencyPoll(pollBuf, sizeof(pollBuf));
    if (pollLen > 0) {
      udp.writeTo(pollBuf, pollLen, icomHost, icomPort);
      lastCivTxMs = now;
    }
  }

  // Watchdog: if no RX for a while, mark as disconnected
  if (icomConnected && (now - lastCivRxMs >= civWatchdogTimeoutMs)) {
    icomConnected = false;
    lvgl_set_waiting_for_icom();   // show "Waiting for Connection to ICOM"
  }
}
