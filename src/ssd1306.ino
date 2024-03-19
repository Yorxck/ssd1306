#include "ssd1306.h"

ssd1306 display;

void setup() {
  Serial.begin(9600);
  
  if (!display.begin()) {
    Serial.println("Failed to allocate");
  }
  
  display.circle(64, 32, 30, INVERSE);  // fix filled
  display.triangle(50, 20, 30, 60, 100, 32, INVERSE);
  display.textColor(WHITE);

  Bounds bounds = display.textBounds("Hello");

  Serial.println(bounds.height);
  Serial.println(bounds.width);

  display.cursor(display.width() / 2 - bounds.width / 2, display.height() / 2 - bounds.height / 2); // center cursor
  display.print("Hello"); // char bounds
  display.update();
}

void loop() {
  
}