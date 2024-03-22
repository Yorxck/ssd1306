#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Arduino.h>

/* [[ Enums ]] */
enum Color {
  BLACK,
  WHITE,
  INVERSE
};

enum FillType {
  NOFILL,
  FILL
};

/* [[ Structures ]] */
struct TextBounds {
  uint16_t width = 0;
  uint16_t height = 0;
  uint16_t cursorX = 0;
  uint16_t cursorY = 0;

  TextBounds(uint16_t __width, uint16_t __height, uint16_t __cursorX, uint16_t __cursorY) : width(__width), height(__height), cursorX(__cursorX), cursorY(__cursorY) {}
};

/* [[ Typedef ]] */
typedef const uint8_t Bitmap;

/* [[ Class ]] */
class graphics : public Print {
protected:
  uint8_t* buffer;
  uint8_t WIDTH;
  uint8_t HEIGHT;
  uint8_t CursorX = 1;
  uint8_t CursorY = 1;
  Color TextColor = WHITE;
  uint8_t TextSize = 1;
  bool TextWrap = true;
  
  bool getBit(uint8_t byte, uint8_t bit);
  void lineD(int16_t ox, int16_t oy, int16_t ex, int16_t ey, Color color);
  void lineH(int16_t ox, int16_t oy, int16_t l, Color color);
  void lineV(int16_t ox, int16_t oy, int16_t l, Color color);
  void fillCircle(uint8_t x, uint8_t y, uint8_t r, Color color);
  void drawChar(uint8_t x, uint8_t y, char c, Color color, uint8_t Size);
  void charBounds(unsigned char c, uint16_t* x, uint16_t* y, int16_t* minx, int16_t* miny, int16_t* maxx, int16_t* maxy);

public:
  // Methods
  void setPixel(uint8_t x, uint8_t y, Color color);
  bool getPixel(uint8_t x, uint8_t y);
  void line(int16_t ox, int16_t oy, int16_t ex, int16_t ey, Color color);
  void circle(uint8_t x, uint8_t y, uint8_t r, Color color);
  void circle(uint8_t x, uint8_t y, uint8_t r, Color color, FillType fill);
  void rectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, Color color);
  void rectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, Color color, FillType fill);
  void triangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, Color color);
  void triangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, Color color, FillType fill);
  void bitmap(uint8_t x, uint8_t y, Bitmap Bitmap[], uint8_t w, uint8_t h, Color color);
  void textColor(Color color);
  void cursor(uint8_t x, uint8_t y);
  void textSize(uint8_t size);
  void textWrap(bool wrap);
  TextBounds textBounds(char* str, uint16_t x = 1, uint16_t y = 1);

  // inherited
  using Print::write;
  virtual size_t write(uint8_t);
};

#endif // GRAPHICS_H
