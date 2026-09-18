# ICOM_WiFi_Comms (v0.8-style reconstruction)

ESP32-S3-Touch-LCD-5 (Waveshare) controller for ICOM R8600 over LAN CI-V.

## Hardware

- Waveshare ESP32-S3-Touch-LCD-5 rev1.1

## Toolchain

- Arduino IDE 2.x
- ESP32 Arduino core 3.0.2
- Waveshare LVGL + ESP32_Display_Panel stack

## Build

1. Install ESP32 core 3.0.2.
2. Install Waveshare ESP32-S3-Touch-LCD-5 board support and LVGL example libs.
3. Select board: `ESP32-S3-Touch-LCD-5`.
4. Open `ICOM_WiFi_Comms.ino`.
5. Set `ssid`, `password`, `icomHost`, `icomPort`.
6. Upload to the board.

## Behaviour

- Shows "Waiting for Connection to ICOM" until CI-V frames are received.
- Sends periodic CI-V frequency polls (keepalive).
- Updates frequency, mode, and S-meter on the display.
- Uses a watchdog to revert to "Waiting..." if CI-V RX stops.
