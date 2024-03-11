#include "graphics.h"
#include <Arduino.h>

void graphics::setPixel(uint8_t x, uint8_t y, Color color) {
  x--;         // normal start is 0 but 1 is better
  y = 64 - y;  // normally starts at bottom but top better

  if ((x < 0) || (x >= WIDTH) || (y < 0) || (y >= HEIGHT)) {
    return;  // outside screen bound
  } else {
    switch (color) {
      case WHITE:
        buffer[x + (y / 8) * WIDTH] |= _BV(y % 8);  // Bitwise OR operator
        break;
      case BLACK:
        buffer[x + (y / 8) * WIDTH] &= ~_BV(y % 8);  // Bitwise AND / NOT operator
        break;
      case INVERSE:
        buffer[x + (y / 8) * WIDTH] ^= _BV(y % 8);  // Bitwise XOR operator
        break;
    }
  }
}

bool graphics::getPixel(uint8_t x, uint8_t y) {
  x--;         // normal start is 0 but 1 is better
  y = 64 - y;  // normally starts at bottom but top better

  if ((x < 0) || (x >= WIDTH) || (y < 0) || (y >= HEIGHT))
    return 0;  // outside screen bound
  if (buffer) {
    uint8_t byte = buffer[x + (y / 8) * WIDTH];

    return ((byte & (1 << y % 8)) >> y % 8) != 0;
  }
  return 0;
}

bool graphics::getBit(uint8_t byte, uint8_t bit) {
  return ((byte >> bit)  & 0x01);
}

void graphics::lineH(int16_t ox, int16_t oy, int16_t l, Color color) {
  l--;
  for (l; l >= 0; l--) {
    setPixel(ox + l, oy, color);
  }
}

void graphics::lineV(int16_t ox, int16_t oy, int16_t l, Color color) {
  l--;
  for (l; l >= 0; l--) {
    setPixel(ox, oy + l, color);
  }
}

void graphics::lineD(int16_t ox, int16_t oy, int16_t ex, int16_t ey, Color color) {
  int16_t dx = abs(ox - ex);
  int16_t dy = abs(oy - ey);
  int16_t sx = (ox < ex) ? 1 : -1;
  int16_t sy = (oy < ey) ? 1 : -1;
  int16_t err = dx - dy;

  for (;;) {
    setPixel(ox, oy, color);
    if (ox == ex && oy == ey) break;

    int16_t tmp = err * 2;
    if (tmp > -dy) {
      err -= dy;
      ox += sx;
    }
    if (tmp < dx) {
      err += dx;
      oy += sy;
    }
  }
}

void graphics::line(int16_t ox, int16_t oy, int16_t ex, int16_t ey, Color color) {
  if (ox == ex) {
    lineH(ox, oy, abs(ox - ex) + 1, color);  // Horizontal
  } else if (oy == ey) {
    lineV(ox, oy, abs(oy - ey) + 1, color);  // Vertical
  } else {
    lineD(ox, oy, ex, ey, color);  // Diagonal
  }
}

void graphics::rectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, Color color) {
  lineH(x, y, w, color);              // top
  lineH(x, y + h - 1, w, color);          // bottom
  lineV(x, y + 1, h - 2, color);      // left
  lineV(x + w - 1, y + 1, h - 2, color);  // right
}

void graphics::rectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, Color color, FillType fill) {
  switch (fill) {
    case NOFILL:
      rectangle(x, y, w, h, color);
      break;
    case FILL:
      for (uint8_t i = 0; i < h; i++) {
        lineH(x, y + i, w, color);
      }
      break;
  }
}

void graphics::circle(uint8_t x, uint8_t y, uint8_t r, Color color) {
  uint8_t offsetX, offsetY, setX, setY;

  for (uint16_t i = 0; i < 360; i++) {
    offsetX = round(r * cos(i * PI / 180));
    offsetY = round(r * sin(i * PI / 180));

    if ((setX == x + offsetX) && (setY == y + offsetY)) continue;  //already had this pixel

    setX = x + offsetX;
    setY = y + offsetY;
    setPixel(setX, setY, color);
  }
}

void graphics::circle(uint8_t x, uint8_t y, uint8_t r, Color color, FillType fill) {
  switch (fill) {
    case NOFILL:
      circle(x, y, r, color);
      break;
    case FILL:
      for (uint8_t f = 0; f <= r; f++) {
        circle(x, y, f, color);
      }
      break;
  }
}

void graphics::bitmap(uint8_t x, uint8_t y, const uint8_t Bitmap[], uint8_t w, uint8_t h, Color color) {
  uint8_t byteWidth = (w + 7) / 8;
  uint8_t b = 0;

  for (uint8_t j = 0; j < h; j++, y++) {
    for (uint8_t i = 0; i < w; i++) {
      if (i & 7)
        b <<= 1;
      else
        b = pgm_read_byte(&Bitmap[j * byteWidth + i / 8]);
      if (b & 0x80)
        setPixel(x + i, y, color);
    }
  }
}

void graphics::textColor(Color color) {
  TextColor = color;
}

void graphics::cursor(uint8_t x, uint8_t y) {
  CursorX = x;
  CursorY = y;
}

inline size_t graphics::write(uint8_t c) {

}
