#include "SSD1306SPIDisplay.h"

bool SSD1306SPIDisplay::begin() {
  if (!_isOn) {
    _isOn = true;
  }
  // Initialize shared SPI bus with correct pins if provided
  if (_sclk >= 0) {
    _spi->begin(_sclk, _miso, _mosi);
  }
#ifdef DISPLAY_ROTATION
  display.setRotation(DISPLAY_ROTATION);
#endif
  // periphBegin=false: we already initialized SPI above with correct pins
  return display.begin(SSD1306_SWITCHCAPVCC, 0, true, false);
}

void SSD1306SPIDisplay::turnOn() {
  if (!_isOn) {
    _isOn = true;
  }
  display.ssd1306_command(SSD1306_DISPLAYON);
}

void SSD1306SPIDisplay::turnOff() {
  display.ssd1306_command(SSD1306_DISPLAYOFF);
  _isOn = false;
}

void SSD1306SPIDisplay::clear() {
  display.clearDisplay();
  display.display();
}

void SSD1306SPIDisplay::startFrame(Color bkg) {
  display.clearDisplay();
  _color = SSD1306_WHITE;
  display.setTextColor(_color);
  display.setTextSize(1);
  display.cp437(true);
}

void SSD1306SPIDisplay::setTextSize(int sz) {
  display.setTextSize(sz);
}

void SSD1306SPIDisplay::setColor(Color c) {
  _color = (c != 0) ? SSD1306_WHITE : SSD1306_BLACK;
  display.setTextColor(_color);
}

void SSD1306SPIDisplay::setCursor(int x, int y) {
  display.setCursor(x, y);
}

void SSD1306SPIDisplay::print(const char* str) {
  display.print(str);
}

void SSD1306SPIDisplay::fillRect(int x, int y, int w, int h) {
  display.fillRect(x, y, w, h, _color);
}

void SSD1306SPIDisplay::drawRect(int x, int y, int w, int h) {
  display.drawRect(x, y, w, h, _color);
}

void SSD1306SPIDisplay::drawXbm(int x, int y, const uint8_t* bits, int w, int h) {
  display.drawBitmap(x, y, bits, w, h, SSD1306_WHITE);
}

uint16_t SSD1306SPIDisplay::getTextWidth(const char* str) {
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(str, 0, 0, &x1, &y1, &w, &h);
  return w;
}

void SSD1306SPIDisplay::endFrame() {
  display.display();
}
