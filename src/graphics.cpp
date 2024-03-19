#include <Arduino.h>
#include <math.h>
#include "graphics.h"
#include "font.h"


void graphics::setPixel(uint8_t x, uint8_t y, Color color) {
  x--;         // normal start is 0 but 1 is better
  y = 64 - y;  // normally starts at bottom but top better

  if ((x < 0) || (x >= WIDTH) || (y < 0) || (y >= HEIGHT)) return; // outside screen bound

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

bool graphics::getPixel(uint8_t x, uint8_t y) {
  x--;         // normal start is 0 but 1 is better
  y = 64 - y;  // normally starts at bottom but top better
  
  if ((x < 0) || (x >= WIDTH) || (y < 0) || (y >= HEIGHT)) return false;  // outside screen bound
  
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
    lineV(ox, oy, abs(oy - ey), color);  // Horizontal
  } else if (oy == ey) {
    lineH(ox, oy, abs(ox-ex), color);  // Vertical
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
  uint16_t px, py;

  for (uint16_t angle = 0; angle < 360; angle++) {
    uint16_t xr = round(r * cos(angle * PI / 180));
    uint16_t yr = round(r * sin(angle * PI / 180));

    if (px == (x + xr) && py == (y + yr)) continue;

    px = x + xr;
    py = y + yr;
    setPixel(px, py, color);
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

void graphics::triangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, Color color) {
  line(x1, y1, x2, y2, color);
  line(x1, y1, x3, y3, color);
  line(x3, y3, x2, y2, color);
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

void graphics::textSize(uint8_t size) {
  TextSize = size;
}

void graphics::textWrap(bool wrap) {
  TextWrap = wrap;
}

void graphics::cursor(uint8_t x, uint8_t y) {
  CursorX = x;
  CursorY = y;
}

void graphics::charBounds(unsigned char c, uint16_t x, uint16_t y, Bounds* bounds) {
  if (c == '\n') {
    (*bounds).cursorX = 1;
    (*bounds).cursorY += 8 * TextSize;
  } else if (c != '\r') {
    if (TextWrap && (CursorX + TextSize * 6) > WIDTH) { // wrap text to next line
      (*bounds).cursorX = 1;
      (*bounds).cursorY += 8 * TextSize;
    }
    (*bounds).cursorX += 6 * TextSize;
    (*bounds).height = (bounds->cursorY + TextSize * 8 - 1) - y;
    (*bounds).width = (bounds->cursorX + TextSize * 6 - 1) - x;
  }
}

Bounds graphics::textBounds(const char* str, uint16_t x, uint16_t y) {
  uint8_t* c;
  Bounds bounds(0, 0, x, y); // w, h, x, y

  for (c = str; *c; c++) { //for every char
    charBounds(*c, x, y, &bounds);
  }

  return bounds;
}

void graphics::drawChar(uint8_t x, uint8_t y, char c, Color color, uint8_t Size) {
  if ((x < 1) || (x > WIDTH) || (y < 1) || (y > HEIGHT)) return;

  for (uint8_t i = 0; i < 5; i++) { // char 5 colomns
    uint8_t charbit = pgm_read_byte(&default_font[c * 5 + i]);
    for (uint8_t j = 0; j < 8; j++, charbit >>= 1) {
      if (charbit & 1) {
        if (TextSize == 1)
          setPixel(x + i, y + j, color);
        else
          rectangle(x + i * TextSize, y + j * TextSize, TextSize, TextSize, color, FILL);
      }
    }
  }
}

size_t graphics::write(uint8_t c) {
  if (c == '\n') {
    CursorX = 1;
    CursorY += 8 * TextSize;
  } else if (c != '\r') {
    if (TextWrap && (CursorX + TextSize * 6) > WIDTH) { // wrap text to next line
      CursorX = 1;
      CursorY += 8 * TextSize;
    }
    drawChar(CursorX, CursorY, c, TextColor, TextSize);
    CursorX += 6 * TextSize;
  }

  return 1;
}
