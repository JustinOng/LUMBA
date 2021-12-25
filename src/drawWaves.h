#ifndef DRAW_WAVES_H
#define DRAW_WAVES_H

#include "patternConfig.h"

void drawWaves(CRGB *leds, uint16_t start, uint16_t end, uint8_t delta, bool invert) {
  CRGBPalette16 p = CRGB(pattern.wave_color);
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

    uint8_t base = triwave8(pattern.wave_freq * pos);
    // Shift up/down based on wave_duty. Rescale it to max 255
    int16_t scaled = (base + pattern.wave_duty) * 255 / (255 + pattern.wave_duty);

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
