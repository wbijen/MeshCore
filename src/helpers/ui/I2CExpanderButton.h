#pragma once

#include <Arduino.h>
#include <Wire.h>

#define BUTTON_EVENT_NONE        0
#define BUTTON_EVENT_CLICK       1
#define BUTTON_EVENT_LONG_PRESS  2
#define BUTTON_EVENT_DOUBLE_CLICK 3
#define BUTTON_EVENT_TRIPLE_CLICK 4

class I2CExpanderButton {
  TwoWire& _wire;
  uint8_t _addr;
  uint8_t _buttonBit;
  int _long_millis;
  int prev, cancel;
  unsigned long down_at;
  uint8_t _click_count;
  unsigned long _last_click_time;
  int _multi_click_window;
  bool _pending_click;

  bool readButton() const;

public:
  I2CExpanderButton(TwoWire& wire, uint8_t addr, uint8_t buttonBit, int long_press_millis = 1000);
  void begin();
  int check(bool repeat_click = false);
  void cancelClick();
  uint8_t getPin() { return 0xFF; }
  bool isPressed() const;
};
