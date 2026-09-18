#pragma once

#include <lvgl.h>

void lvgl_port_init();
void lvgl_create_main_ui();

void lvgl_update_radio_state(uint64_t freqHz,
                             CivMode mode,
                             int smeter);

void lvgl_set_waiting_for_icom();
