#include "ssd1306.h"

ssd1306 display(128, 64);

void setup() {
  Serial.begin(9600);
  
  if (!display.begin()) {
    Serial.println("Failed to allocate");
  }
  
  display.triangle(1, 1, 128, 1, 64, 64, WHITE);
  display.textColor(INVERSE);
  display.write("Skill issue");
  display.update();
}

void loop() {}
