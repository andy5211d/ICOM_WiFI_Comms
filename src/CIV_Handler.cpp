#include "CIV_Handler.h"

// Simple CI-V framing assumptions for R8600-like radios
// This is intentionally minimal; you can refine with your actual spec.

CIV_Handler::CIV_Handler()
: _freqHz(0),
  _mode(CIV_MODE_UNKNOWN),
  _smeter(0)
{}

void CIV_Handler::handleFrame(const uint8_t* data, size_t len) {
  if (len < 5) return;

  // Basic CI-V: 0xFE 0xFE [to] [from] [cmd] ...
  if (data[0] != 0xFE || data[1] != 0xFE) return;

  uint8_t cmd = data[4];
  const uint8_t* payload = data + 5;
  size_t payloadLen = len - 5;

  switch (cmd) {
    case 0x03: // example: frequency data
      parseFrequency(payload, payloadLen);
      break;
    case 0x01: // example: mode data
      parseMode(payload, payloadLen);
      break;
    case 0x15: // example: S-meter
      parseSmeter(payload, payloadLen);
      break;
    default:
      // ignore others for now
      break;
  }
}

uint64_t CIV_Handler::getFrequencyHz() const {
  return _freqHz;
}

CivMode CIV_Handler::getMode() const {
  return _mode;
}

int CIV_Handler::getSmeter() const {
  return _smeter;
}

size_t CIV_Handler::buildFrequencyPoll(uint8_t* out, size_t maxLen) {
  // Minimal CI-V poll frame: FE FE [to] [from] 03 FD
  if (maxLen < 6) return 0;

  out[0] = 0xFE;
  out[1] = 0xFE;
  out[2] = 0xA4; // to: R8600 address (example)
  out[3] = 0xE0; // from: controller
  out[4] = 0x03; // command: frequency read
  out[5] = 0xFD; // terminator

  return 6;
}

void CIV_Handler::parseFrequency(const uint8_t* payload, size_t len) {
  // Example: BCD frequency, 5 bytes, little-endian
  if (len < 5) return;

  uint64_t freqHz = 0;
  for (size_t i = 0; i < 5; ++i) {
    uint8_t b = payload[i];
    uint8_t lo = b & 0x0F;
    uint8_t hi = (b >> 4) & 0x0F;
    freqHz = freqHz * 10 + hi;
    freqHz = freqHz * 10 + lo;
  }
  _freqHz = freqHz;
}

void CIV_Handler::parseMode(const uint8_t* payload, size_t len) {
  if (len < 1) return;
  switch (payload[0]) {
    case 0x00: _mode = CIV_MODE_LSB; break;
    case 0x01: _mode = CIV_MODE_USB; break;
    case 0x02: _mode = CIV_MODE_AM;  break;
    case 0x03: _mode = CIV_MODE_FM;  break;
    case 0x05: _mode = CIV_MODE_CW;  break;
    default:   _mode = CIV_MODE_UNKNOWN; break;
  }
}

void CIV_Handler::parseSmeter(const uint8_t* payload, size_t len) {
  if (len < 1) return;
  _smeter = payload[0]; // simple 0–255 scale
}
