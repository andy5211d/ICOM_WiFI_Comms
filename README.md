# ICOM_WiFi_Comms

A touch screen controller for the ICOM R8600.  This is intended to be used with the RC-28 connected to the Touch Screen controller (Waveshare ESP32-S3-Touch-LCD-5),but is unlikely to be working yet! 

(These files may not be correct as not yet tested.  This is a reconstruction of work done some months ago with CoPilot trying to re-generate the files.  I did not make good notes as at the time I intended to continue the following day! Silly me:-)  I did get this to work reliably, just need to find the correct code :-)

ESP32-S3-Touch-LCD-5 (Waveshare) controller for ICOM R8600 over LAN.

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
