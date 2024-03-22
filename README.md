# [ssd1306]()

<br/>

## library
To use this library:
```cpp
  #include "ssd1306.h"

  ssd1306 display(width, height, addr, sda, scl, rst);
```

##
<br/>

to begin the display use:
```cpp
  void setup() {
    Serial.begin(9600);

    if (!display.begin()) { // begins display
      Serial.println(display.error()); // shows display error if failed
    }
  }
```

<br/>

## functions
