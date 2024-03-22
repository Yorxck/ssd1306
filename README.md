# [ssd1306]()

<br/>

## library
To use this library:
```cpp
  #include "ssd1306.h"

  ssd1306 display(width, height, addr, sda, scl, rst);
```
<br/>
to startup the library:
```cpp
  void setup() {
    Serial.begin(9600);

    if (!display.begin()) {
      Serial.println(display.error());
    }
  }
```

<br/>

## functions
