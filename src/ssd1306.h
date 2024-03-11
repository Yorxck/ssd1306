#ifndef SSD1306_h
#define SSD1306_H

#include "graphics.h"
#include <Wire.h>
#include <Arduino.h>

class ssd1306 : public graphics {
private:
  uint8_t addr = 0x3C;
  uint8_t WIDTH;
  uint8_t HEIGHT;
  void exe_cmd(uint8_t cmd1);
  void exe_cmd(uint8_t cmd1, uint8_t cmd2);
  void exe_cmd(uint8_t cmd1, uint8_t cmd2, uint8_t cmd3);

public:
  ssd1306(uint8_t __width, uint8_t __height) : WIDTH(__width), HEIGHT(__height) {}
  ssd1306(uint8_t __width, uint8_t __height, uint8_t __addr)  : WIDTH(__width), HEIGHT(__height), addr(__addr) {}
  bool begin();
  void update();
  void clear();
  uint8_t width();
  uint8_t height();
  void inverse(bool state);
};

#endif // SSD1306_H