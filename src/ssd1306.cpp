#include "ssd1306.h"
#include "graphics.h"
#include <Arduino.h>
#include <Wire.h>

void ssd1306::exe_cmd(uint8_t cmd1) {
  Wire.beginTransmission(addr);
  Wire.write(0x00);
  Wire.write(cmd1);
  Wire.endTransmission();
}

void ssd1306::exe_cmd(uint8_t cmd1, uint8_t cmd2) {
  Wire.beginTransmission(addr);
  Wire.write(0x00);
  Wire.write(cmd1);
  Wire.write(cmd2);
  Wire.endTransmission();
}

void ssd1306::exe_cmd(uint8_t cmd1, uint8_t cmd2, uint8_t cmd3) {
  Wire.beginTransmission(addr);
  Wire.write(0x00);
  Wire.write(cmd1);
  Wire.write(cmd2);
  Wire.write(cmd3);
  Wire.endTransmission();
}

bool ssd1306::begin() {
  buffer = (uint8_t *)malloc(WIDTH * HEIGHT / 8);
  if (buffer == nullptr) return false;  // malloc failed :(
  clear();

  Wire.begin();
  Wire.beginTransmission(addr);
  byte err = Wire.endTransmission();
  if (err != 0) return;  // didnt find device

  // set display properties
  exe_cmd(0xAE);              //Set display Off
  exe_cmd(0xD5, 0x80);        //Set display clock divide ratio/oscillator frequency
  exe_cmd(0xA8, HEIGHT - 1);  //Set multiplex ratio
  exe_cmd(0xD3, 0x00);        //Set display offset
  exe_cmd(0x40);              //Set display start line
  exe_cmd(0x8D);              //Set charge pump
  exe_cmd(0x14);              //VCC generated by internal DC/DC circuit
  exe_cmd(0xA1);              //Set segment re-map
  exe_cmd(0xC0);              //Set COM output scan direction
  exe_cmd(0xDA, 0x12);        //Set COM pins hardware configuration
  exe_cmd(0x81, 0xCF);        //Set contrast control
  exe_cmd(0xD9, 0xF1);        //Set pre-changed period
  exe_cmd(0xDB, 0x40);        //Set VCOMH Deselected level
  exe_cmd(0xA4);              //Set entire display on/off
  exe_cmd(0xA6);              //Set normal/inverse display
  exe_cmd(0x20);              //Set memory address mode
  exe_cmd(0x00);              //Horizontal
  exe_cmd(0xAF);              //Set display on

  return true;
}

void ssd1306::update() {
  exe_cmd(0x22, 0, 0xFF);
  exe_cmd(0x21, 0, WIDTH-1);

  for (unsigned short q = 0; q < (WIDTH * HEIGHT / 8);) {
    Wire.beginTransmission(addr);
    Wire.write(0x40);
    for (uint8_t w = 0; w < 16; w++) {
      Wire.write(buffer[q]);
      q++;
    }
    Wire.endTransmission();
  }
}

void ssd1306::clear() {
  memset(buffer, 0, WIDTH * HEIGHT / 8);
}

uint8_t ssd1306::width() {
  return WIDTH;
}

uint8_t ssd1306::height() {
  return HEIGHT;
}

void ssd1306::inverse(bool state) {
  exe_cmd(state ? 0xA7 : 0xA6);
}