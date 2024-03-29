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
  for (int16_t rx = -r; rx <= (r * 2); rx++) {
    for (int16_t ry = -r; ry <= (r * 2); ry++) { // loop for square
      
      if (round(sqrt(pow(rx, 2) + pow(ry, 2))) == r) { // inside of radius
        setPixel(x + rx, y + ry, color);
      }
    }
  }
}

void graphics::fillCircle(uint8_t x, uint8_t y, uint8_t r, Color color) {
  for (int16_t rx = -r; rx <= (r * 2); rx++) {
    for (int16_t ry = -r; ry <= (r * 2); ry++) { // loop for square
      
      if (sqrt(pow(rx, 2) + pow(ry, 2)) <= r) { // inside of radius
        setPixel(x + rx, y + ry, color);
      }
    }
  }
}

void graphics::circle(uint8_t x, uint8_t y, uint8_t r, Color color, FillType fill) {
  switch (fill) {
    case NOFILL:
      circle(x, y, r, color);
      break;
    case FILL:
      fillCircle(x, y, r, color);
      break;
  }
}

void graphics::triangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, Color color) {
  line(x1, y1, x2, y2, color);
  line(x1, y1, x3, y3, color);
  line(x3, y3, x2, y2, color);
}

void graphics::bitmap(uint8_t x, uint8_t y, Bitmap bitmap[], uint8_t w, uint8_t h, Color color) {
  uint8_t byteWidth = (w + 7) / 8;
  uint8_t b = 0;

  for (uint8_t j = 0; j < h; j++, y++) {
    for (uint8_t i = 0; i < w; i++) {
      if (i & 7)
        b <<= 1;
      else
        b = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
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

void graphics::charBounds(unsigned char c, uint16_t* x, uint16_t* y, int16_t* minx, int16_t* miny, int16_t* maxx, int16_t* maxy) {
  if (c == '\n') {
    *x = 1;
    *y += 8 * TextSize;
  } else if (c != '\r') {
    if (TextWrap && (CursorX + TextSize * 6) >= WIDTH) { // wrap text to next line
      *x = 1;
      *y += 8 * TextSize;
    }
    int16_t px = (*x + TextSize * 6 - 1),
            py = (*y + TextSize * 8 - 1);

    if (px > *maxx)
      *maxx = px;
    if (*x < *minx)
      *minx = *x;
    if (py > *maxy)
      *maxy = py;
    if (*y < *miny)
      *miny = *y;

    *x += 6 * TextSize;
  }
}

TextBounds graphics::textBounds(char* str, uint16_t x, uint16_t y) {
  uint8_t c;
  int16_t minx = WIDTH + 1,
          miny = HEIGHT + 1,
          maxx = 0,
          maxy = 0;
  TextBounds bounds(0, 0, x, y); // w, h, x, y
  
  for (uint16_t i = 0; i < strlen(str); i++) { //for every char
    charBounds(str[i], &x, &y, &minx, &miny, &maxx, &maxy);
  }

  if (maxx >= minx) {
    bounds.cursorX = x;
    bounds.width = maxx - minx + 1;
    
  }

  if (maxy >= miny) {
    bounds.cursorY = y;
    bounds.height = maxy - miny + 1;
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
