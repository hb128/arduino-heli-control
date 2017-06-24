#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

volatile int value  = 0;

namespace tacho{

void setup()   {
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  display.clearDisplay();
  display.display();
}

void drawBar(uint16_t value, uint16_t x0)
{
  value = value* 28 / 1023;
  display.drawRect(x0,2,7,28, WHITE);
  display.drawLine(x0-1,value+1,x0+7,value+1, WHITE);
  display.drawLine(x0-1,value+2,x0+7,value+2, WHITE);
  display.drawLine(x0-1,value+3,x0+7,value+3, WHITE);
}

void drawValues(uint16_t height, uint16_t vel, uint16_t rot)
{

  display.clearDisplay();

  display.drawCircle(64, 0, 24, WHITE);
  float angle = rot * 3.141 / 1023;

  uint16_t x_0, y_0, x_1, y_1;

  x_0 = 64;
  y_0 = 0;
  x_1 = x_0  - 22 * cos(angle);
  y_1 = y_0 +  22 * sin(angle);

  display.fillTriangle(x_0+2, y_0,x_0-2,y_0, x_1, y_1, WHITE);
  
  drawBar(vel,10);
  drawBar(height,111);
  display.display();
  delay(10);
}


};
