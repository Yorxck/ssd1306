#include <splash.h>
#include "ssd1306.h"

ssd1306 display(128, 64);

void setup() {
  Serial.begin(9600);
  
  if (!display.begin()) {
    Serial.println("Failed to allocate");
  }
 
  display.println("BABABOEY");
}

void loop() {}