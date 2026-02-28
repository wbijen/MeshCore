#pragma once

#include "DisplayDriver.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#define SSD1306_NO_SPLASH
#include <Adafruit_SSD1306.h>

class SSD1306SPIDisplay : public DisplayDriver {
  Adafruit_SSD1306 display;
  SPIClass* _spi;
  int8_t _sclk, _miso, _mosi;
  bool _isOn;
  uint8_t _color;

public:
  SSD1306SPIDisplay(SPIClass* spi, int8_t dc_pin, int8_t rst_pin, int8_t cs_pin,
                    int8_t sclk = -1, int8_t miso = -1, int8_t mosi = -1,
                    uint8_t w = 64, uint8_t h = 48)
    : DisplayDriver(w, h),
      display(w, h, spi, dc_pin, rst_pin, cs_pin, 8000000UL),
      _spi(spi), _sclk(sclk), _miso(miso), _mosi(mosi)
  {
    _isOn = false;
  }

  bool begin();

  bool isOn() override { return _isOn; }
  void turnOn() override;
  void turnOff() override;
  void clear() override;
  void startFrame(Color bkg = DARK) override;
  void setTextSize(int sz) override;
  void setColor(Color c) override;
  void setCursor(int x, int y) override;
  void print(const char* str) override;
  void fillRect(int x, int y, int w, int h) override;
  void drawRect(int x, int y, int w, int h) override;
  void drawXbm(int x, int y, const uint8_t* bits, int w, int h) override;
  uint16_t getTextWidth(const char* str) override;
  void endFrame() override;
};
