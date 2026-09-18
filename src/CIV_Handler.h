#pragma once
#include <stdint.h>
#include <stddef.h>

enum CivMode {
  CIV_MODE_UNKNOWN = 0,
  CIV_MODE_AM,
  CIV_MODE_FM,
  CIV_MODE_USB,
  CIV_MODE_LSB,
  CIV_MODE_CW
};

class CIV_Handler {
public:
  CIV_Handler();

  void handleFrame(const uint8_t* data, size_t len);

  uint64_t getFrequencyHz() const;
  CivMode  getMode() const;
  int      getSmeter() const;

  // Build a simple frequency poll CI-V frame
  size_t buildFrequencyPoll(uint8_t* out, size_t maxLen);

private:
  uint64_t _freqHz;
  CivMode  _mode;
  int      _smeter;

  void parseFrequency(const uint8_t* payload, size_t len);
  void parseMode(const uint8_t* payload, size_t len);
  void parseSmeter(const uint8_t* payload, size_t len);
};
