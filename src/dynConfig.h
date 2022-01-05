#ifndef DYN_CONFIG_H
#define DYN_CONFIG_H

#include "drawWaves.h"

typedef struct {
  char mode;
  uint16_t auto_interval;
  uint16_t fade_duration;
  uint8_t fade_blend;

  char pattern_num;

  uint8_t brightness;
  uint32_t base_color;
  uint32_t sec_color;
  uint16_t move_speed;

  wave_params_t waves[5];

  uint32_t fw_colors[5];
  uint8_t fw_increment;

  char effect_num;

  uint32_t eff_sl_color;
  uint8_t eff_sl_step;
  uint8_t eff_sl_length;
  uint16_t eff_sl_interval;
  uint8_t eff_sl_fade;

  uint32_t eff_rs_color;
  uint16_t eff_rs_duration;
  uint8_t eff_rs_chance;
  uint8_t eff_rs_fade;
  uint8_t eff_rs_length;

  uint32_t eff_line_color;
  uint32_t eff_line_speed;
  uint16_t eff_line_duration;
  uint8_t eff_line_period;
  uint8_t eff_line_duty;
  uint16_t eff_line_fade_dur;

  uint32_t eff_sline_color;
  uint16_t eff_sline_duration;
  uint16_t eff_sline_speed;
  uint8_t eff_sline_fade;

  uint32_t eff_caps_color;
  uint16_t eff_caps_dur;
  uint16_t eff_caps_slew;

  uint16_t param1;
  uint16_t param2;
  uint16_t param3;

  uint8_t patt_triggers[4];

  uint16_t lox_min[4];
  uint16_t lox_max[4];
} dyn_config_t;

extern dyn_config_t config;

#endif
