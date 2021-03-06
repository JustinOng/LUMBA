#ifndef DRAW_FIREWORKS_H
#define DRAW_FIREWORKS_H

#include "dynConfig.h"

void drawFireworks(CRGB *leds, CRGBPalette16 &fw_palette, uint16_t start, uint16_t end, uint8_t delta, bool invert) {
  uint8_t pos = delta;
  int16_t i = start;
  while ((start < end && i <= end) || (start > end && i >= end)) {
    if (!invert) {
      pos += config.fw_increment;
    } else {
      pos -= config.fw_increment;
    }

    leds[i] = ColorFromPalette(fw_palette, pos, 255, LINEARBLEND);

    if (start < end) {
      i++;
    } else {
      i--;
    }
  }
}

#endif
