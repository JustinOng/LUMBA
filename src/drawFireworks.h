#ifndef DRAW_FIREWORKS_H
#define DRAW_FIREWORKS_H

#include "patternConfig.h"

void drawFireworks(CRGB *leds, CRGBPalette16 &fw_palette, uint16_t start, uint16_t end, uint8_t delta, bool invert) {
  uint8_t pos = delta;
  uint16_t i = start;
  while (i != end) {
    if (!invert) {
      pos += pattern.fw_increment;
    } else {
      pos -= pattern.fw_increment;
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
