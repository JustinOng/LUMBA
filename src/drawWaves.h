#ifndef DRAW_WAVES_H
#define DRAW_WAVES_H

#include "dynConfig.h"
#include "helpers.h"

typedef struct {
  CRGB color;
  float freq;
  int8_t duty;
  uint16_t speed;
} wave_params_t;

void drawWaves(CRGB *leds, wave_params_t params, uint8_t delta, segment_t &segment) {
  CRGBPalette16 p = CRGB(params.color);
  uint16_t segment_length = getSegmentLength(segment);

  // shift all LEDs one space next
  for (int16_t i = segment_length - 1; i > 0; i--) {
    leds[getPixelIndex(i, segment)] = leds[getPixelIndex(i - 1, segment)];
  }

  // calculate and draw new pixel at index 0

  // triwave8 creates the variations, wave_freq scales frequency (same scaling as how sine works)
  // delta shifts the pattern
  uint8_t base = triwave8(params.freq * delta);
  // Shift up/down based on wave_duty. Rescale it to max 255
  int16_t scaled = (base + params.duty) * 255 / (255 + params.duty);

  uint8_t bri = 0;
  if (scaled >= 0 && scaled <= 255) {
    bri = scaled;
  } else if (scaled > 255) {
    bri = 255;
  }

  leds[getPixelIndex(0, segment)] = ColorFromPalette(p, 0, bri, LINEARBLEND);
}

#endif
