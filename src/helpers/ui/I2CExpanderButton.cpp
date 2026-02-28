#include "I2CExpanderButton.h"

// PI4IOE5V6408 registers
#define PI4IO_REG_IRQ_STA  0x13
#define PI4IO_REG_IN_STA   0x0F

#define MULTI_CLICK_WINDOW_MS  280

I2CExpanderButton::I2CExpanderButton(TwoWire& wire, uint8_t addr, uint8_t buttonBit, int long_press_millis)
  : _wire(wire), _addr(addr), _buttonBit(buttonBit), _long_millis(long_press_millis)
{
  down_at = 0;
  prev = 0;  // not pressed
  cancel = 0;
  _click_count = 0;
  _last_click_time = 0;
  _multi_click_window = MULTI_CLICK_WINDOW_MS;
  _pending_click = false;
}

void I2CExpanderButton::begin() {
  // Clear any pending IRQ from the expander
  _wire.beginTransmission(_addr);
  _wire.write(PI4IO_REG_IRQ_STA);
  _wire.endTransmission();
  _wire.requestFrom(_addr, (uint8_t)1);
  if (_wire.available()) _wire.read();
}

bool I2CExpanderButton::readButton() const {
  // Read input status register - buttons are active LOW
  _wire.beginTransmission(_addr);
  _wire.write(PI4IO_REG_IN_STA);
  _wire.endTransmission();
  _wire.requestFrom(_addr, (uint8_t)1);
  if (_wire.available()) {
    uint8_t val = _wire.read();
    return !(val & (1 << _buttonBit));  // active LOW: pressed when bit is 0
  }
  return false;
}

bool I2CExpanderButton::isPressed() const {
  return readButton();
}

void I2CExpanderButton::cancelClick() {
  cancel = 1;
  down_at = 0;
  _click_count = 0;
  _last_click_time = 0;
  _pending_click = false;
}

int I2CExpanderButton::check(bool repeat_click) {
  int event = BUTTON_EVENT_NONE;
  int btn = readButton() ? 1 : 0;

  if (btn != prev) {
    if (btn) {
      // button DOWN
      down_at = millis();
    } else {
      // button UP
      if (_long_millis > 0) {
        if (down_at > 0 && (unsigned long)(millis() - down_at) < (unsigned long)_long_millis) {
          _click_count++;
          _last_click_time = millis();
          _pending_click = true;
        }
      } else {
        _click_count++;
        _last_click_time = millis();
        _pending_click = true;
      }
      if (event == BUTTON_EVENT_CLICK && cancel) {
        event = BUTTON_EVENT_NONE;
        _click_count = 0;
        _last_click_time = 0;
        _pending_click = false;
      }
      down_at = 0;
    }
    prev = btn;
  }
  if (!btn && cancel) {
    cancel = 0;
  }

  if (_long_millis > 0 && down_at > 0 && (unsigned long)(millis() - down_at) >= (unsigned long)_long_millis) {
    if (_pending_click) {
      cancelClick();
    } else {
      event = BUTTON_EVENT_LONG_PRESS;
      down_at = 0;
      _click_count = 0;
      _last_click_time = 0;
      _pending_click = false;
    }
  }
  if (down_at > 0 && repeat_click) {
    unsigned long diff = (unsigned long)(millis() - down_at);
    if (diff >= 700) {
      event = BUTTON_EVENT_CLICK;
    }
  }

  if (_pending_click && (millis() - _last_click_time) >= (unsigned long)_multi_click_window) {
    if (down_at > 0) {
      return event;
    }
    switch (_click_count) {
      case 1:
        event = BUTTON_EVENT_CLICK;
        break;
      case 2:
        event = BUTTON_EVENT_DOUBLE_CLICK;
        break;
      case 3:
      default:
        event = BUTTON_EVENT_TRIPLE_CLICK;
        break;
    }
    _click_count = 0;
    _last_click_time = 0;
    _pending_click = false;
  }

  return event;
}
