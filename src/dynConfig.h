#ifndef DYN_CONFIG_H
#define DYN_CONFIG_H

typedef struct {
  char mode;
  char pattern_num;

  uint8_t brightness;
  uint32_t sec_color;
  uint16_t move_speed;
  uint32_t wave_color;
  float wave_freq;
  int8_t wave_duty;
  uint32_t fw_colors[5];
  uint8_t fw_increment;

  char effect_num;

  uint32_t eff_sl_color;
  uint8_t eff_sl_step;
  uint8_t eff_sl_length;
  uint16_t eff_sl_interval;

  uint16_t param1;
  uint16_t param2;
  uint16_t param3;
} dyn_config_t;

extern dyn_config_t config;

#endif
