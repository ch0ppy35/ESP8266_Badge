#include <Wire.h>
#include "SSD1306Wire.h"
#include "ESP8266WiFi.h"
#include "images.h"

SSD1306Wire display(0x3c, 2, 0);  //OLED on GPIO2 and GPIO0
int n; //number of nets found
int y; //scroll index into nets list
int buttonState = 0;

void setup()
{
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);

  drawImageDemo();

}

void scroll_nets()
{
  int outindex, row, ndisp;
  display.clear();
  ndisp = min(6, n); //num of rows (nets) to display
  for (row = 0; row < ndisp; row++)
  {
    outindex = row + y;
    if (outindex >= n)
      outindex -= n;
    display.drawString(00, 10 * row, WiFi.SSID(outindex));
    //display.drawString(100, 10*row, String(WiFi.RSSI(outindex)));
    display.drawString(100, 10 * row, String(WiFi.channel(outindex)));
  }
  //update scroll pointer
  if (n > 6)
  {
    y++;
    if (y == n)
      y = 0;
  }
  display.display();
}
void drawImageDemo() {
  display.clear();
  display.drawXbm(0, 0, xbm_width, xbm_height, xbm_bits);
  display.display();
  delay(800);
}

void loop()
{
  // every 12 scroll steps (re)scan for nets
  n = WiFi.scanNetworks();
  y = 0; //reset scroll pointer
  // scroll found nets to OLED for 12 steps
  for (int z = 0; z < 24; z++)
  {
    scroll_nets();
    delay(600);
  }
}
