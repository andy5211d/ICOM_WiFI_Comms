#include "lvgl_v8_port.h"
#include "CIV_Handler.h"

// You will wire this to Waveshare's LVGL + panel init.
// Here we just sketch the UI logic.

static lv_obj_t* freq_label = nullptr;
static lv_obj_t* mode_label = nullptr;
static lv_obj_t* smeter_label = nullptr;
static lv_obj_t* status_label = nullptr;

void lvgl_port_init() {
  lv_init();
  // Panel + display driver init is done in panel_init() (see esp_panel_board_custom_conf.h)
  // and the display driver must be registered with LVGL.
}

void lvgl_create_main_ui() {
  lv_obj_t* scr = lv_scr_act();

  freq_label = lv_label_create(scr);
  lv_obj_align(freq_label, LV_ALIGN_TOP_LEFT, 10, 10);
  lv_label_set_text(freq_label, "Freq: -----.--- MHz");

  mode_label = lv_label_create(scr);
  lv_obj_align(mode_label, LV_ALIGN_TOP_LEFT, 10, 40);
  lv_label_set_text(mode_label, "Mode: ---");

  smeter_label = lv_label_create(scr);
  lv_obj_align(smeter_label, LV_ALIGN_TOP_LEFT, 10, 70);
  lv_label_set_text(smeter_label, "S: --");

  status_label = lv_label_create(scr);
  lv_obj_align(status_label, LV_ALIGN_BOTTOM_MID, 0, -10);
  lv_label_set_text(status_label, "Waiting for Connection to ICOM");
}

static const char* modeToStr(CivMode m) {
  switch (m) {
    case CIV_MODE_LSB: return "LSB";
    case CIV_MODE_USB: return "USB";
    case CIV_MODE_AM:  return "AM";
    case CIV_MODE_FM:  return "FM";
    case CIV_MODE_CW:  return "CW";
    default:           return "---";
  }
}

void lvgl_update_radio_state(uint64_t freqHz,
                             CivMode mode,
                             int smeter) {
  if (!freq_label || !mode_label || !smeter_label || !status_label) return;

  char buf[64];

  double mhz = freqHz / 1e6;
  snprintf(buf, sizeof(buf), "Freq: %.6f MHz", mhz);
  lv_label_set_text(freq_label, buf);

  snprintf(buf, sizeof(buf), "Mode: %s", modeToStr(mode));
  lv_label_set_text(mode_label, buf);

  snprintf(buf, sizeof(buf), "S: %d", smeter);
  lv_label_set_text(smeter_label, buf);

  lv_label_set_text(status_label, "Connected to ICOM");
}

void lvgl_set_waiting_for_icom() {
  if (!status_label) return;
  lv_label_set_text(status_label, "Waiting for Connection to ICOM");
}
