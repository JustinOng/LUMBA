#ifndef DYN_CONFIG_H
#define DYN_CONFIG_H

#include "drawWaves.h"

typedef struct {
  char mode;
  uint16_t auto_interval;
  uint16_t fade_duration;

  char pattern_num;

  uint8_t brightness;
  uint32_t base_color;
  uint32_t sec_color;
  uint16_t move_speed;

  wave_params_t waves[5];

  uint32_t meteors_color;
  uint16_t meteors_speed;
  uint8_t meteors_period;
  uint8_t meteors_fade;
  uint8_t meteors_length;

  uint32_t sl_color;
  uint8_t sl_led_step;
  uint16_t sl_step_time;
  uint16_t sl_fade_time;
  uint16_t sl_cycle_time;
  uint16_t sl_star_len;

  uint32_t fw_colors[5];
  uint8_t fw_increment;

  uint32_t eff_caps_color;
  uint16_t eff_caps_dur;
  uint16_t eff_caps_slew;

  uint16_t lox_min[4];
  uint16_t lox_max[4];

  bool led_pos_display;
} dyn_config_t;

extern dyn_config_t config;

#endif
