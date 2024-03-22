#ifndef SSD1306_h
#define SSD1306_H

#include "graphics.h"
#include <Wire.h>
#include <Arduino.h>

/* [[ Display Commands ]] */
#define DISPLAY_ON 0xAF
#define DISPLAY_OFF 0xAE
#define INVERTED 0xA7
#define NORMAL 0xA6
#define CONTRAST 0x81

/* [[ Class ]] */
class ssd1306 : public graphics {
private:
  uint8_t addr;
  uint8_t rst;
  void cmd(uint8_t cmd1);
  void cmd(uint8_t cmd1, uint8_t cmd2);
  void cmd(uint8_t cmd1, uint8_t cmd2, uint8_t cmd3);
  char* errMsg;

public:
  ssd1306(uint8_t __width = 128, uint8_t __height = 64, uint8_t __addr = 0x3C, uint8_t __sda = SDA, uint8_t __scl = A5, uint8_t __rst = -1);
  bool begin();
  void update();
  void clear();
  uint8_t width();
  uint8_t height();
  char* error();
  void inverse(bool state);
  void on();
  void off();
  void contrast(uint8_t c);
};

#endif // SSD1306_H