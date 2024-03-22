# [ssd1306]()

<br/>

## library
To use this library:
```cpp
  #include "ssd1306.h"

  ssd1306 display(width, height, addr, sda, scl, rst);
```

## Basics

### `begin()`

this function will initialize the display and reserve memory for the buffer.

#### Syntax
```c++
  display.begin(); // initialize display
```

#### Parameters
None

#### Returns
* `bool`: it will return true if the initialization was successful.

#### See also
* [`error()`](#error)

## 
<br/>

### `error()`

will return the error message (if error was caught at begin function).

#### Syntax
```c++
  if (!display.begin()) { // initialize display
      Serial.println(display.error()); // shows display error if failed
      for (;;) {}
  }
```

#### Parameters
None

#### Returns
* `char*`: will return the error message (if error was caught at begin function).

#### See also
* [`begin()`](#begin)
