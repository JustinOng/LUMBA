#ifndef DRAW_WAVES_H
#define DRAW_WAVES_H

#include "dynConfig.h"

typedef struct {
  CRGB color;
  float freq;
  int8_t duty;
  uint8_t speed;
} wave_params_t;

void drawWaves(CRGB *leds, wave_params_t params, uint16_t start, uint16_t end, uint8_t delta, bool invert) {
  CRGBPalette16 p = CRGB(params.color);
  uint8_t pos = delta;
  int16_t i = start;
  while ((start < end && i <= end) || (start > end && i >= end)) {
    // triwave8 creates the variations, wave_freq scales frequency (same scaling as how sine works)
    // delta shifts the pattern

    if (!invert) {
      pos++;
    } else {
      pos--;
    }

    uint8_t base = triwave8(params.freq * pos);
    // Shift up/down based on wave_duty. Rescale it to max 255
    int16_t scaled = (base + params.duty) * 255 / (255 + params.duty);

    uint8_t bri = 0;
    if (scaled >= 0 && scaled <= 255) {
      bri = scaled;
    } else if (scaled > 255) {
      bri = 255;
    }

    leds[i] = ColorFromPalette(p, i, bri, LINEARBLEND);

    if (start < end) {
      i++;
    } else {
      i--;
    }
  }
}

#endif
