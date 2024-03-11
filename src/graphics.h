#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Arduino.h>

enum Color {
  BLACK,
  WHITE,
  INVERSE
};

enum FillType {
  NOFILL,
  FILL
};

class graphics : public Print {
protected:
  uint8_t* buffer;
  uint8_t WIDTH;
  uint8_t HEIGHT;
  uint8_t CursorX, CursorY = 1;
  Color TextColor = WHITE;
  
  bool getBit(uint8_t byte, uint8_t bit);
  void lineD(int16_t ox, int16_t oy, int16_t ex, int16_t ey, Color color);
  void lineH(int16_t ox, int16_t oy, int16_t l, Color color);
  void lineV(int16_t ox, int16_t oy, int16_t l, Color color);

public:
  // Methods
  void setPixel(uint8_t x, uint8_t y, Color color);
  bool getPixel(uint8_t x, uint8_t y);
  void line(int16_t ox, int16_t oy, int16_t ex, int16_t ey, Color color);
  void circle(uint8_t x, uint8_t y, uint8_t r, Color color);
  void circle(uint8_t x, uint8_t y, uint8_t r, Color color, FillType fill);
  void rectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, Color color);
  void rectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, Color color, FillType fill);
  void bitmap(uint8_t x, uint8_t y, const uint8_t Bitmap[], uint8_t w, uint8_t h, Color color);
  void textColor(Color color);
  void cursor(uint8_t x, uint8_t y);

  // inherited
  virtual size_t write(uint8_t);
};

#endif // GRAPHICS_H
