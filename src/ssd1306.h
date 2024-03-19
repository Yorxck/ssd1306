#ifndef SSD1306_h
#define SSD1306_H

#include "graphics.h"
#include <Wire.h>
#include <Arduino.h>

class ssd1306 : public graphics {
private:
  uint8_t addr;
  void exe_cmd(uint8_t cmd1);
  void exe_cmd(uint8_t cmd1, uint8_t cmd2);
  void exe_cmd(uint8_t cmd1, uint8_t cmd2, uint8_t cmd3);
  char* errMsg;

public:
  ssd1306(uint8_t __width = 128, uint8_t __height = 64, uint8_t __addr = 0x3C);
  bool begin();
  void update();
  void clear();
  uint8_t width();
  uint8_t height();
  void inverse(bool state);
  char* error();
};

#endif // SSD1306_H