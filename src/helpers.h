#ifndef HELPERS_H
#define HELPERS_H

typedef struct {
  uint16_t start;
  uint16_t end;
  bool invert;
} segment_t;

uint16_t getPixelIndex(uint16_t i, segment_t &segment) {
  if (segment.start < segment.end) {
    return segment.start + i;
  } else {
    return segment.start - i;
  }
}

bool withinSegment(uint16_t i, segment_t &segment) {
  if (segment.start < segment.end) {
    return (segment.start + i) <= segment.end;
  } else {
    return (segment.start - i) >= segment.end;
  }
}

uint16_t getSegmentLength(segment_t &segment) {
  if (segment.start < segment.end) {
    return (segment.end - segment.start) + 1;
  } else {
    return (segment.start - segment.end) + 1;
  }
}

#endif
