/*
 * IRremoteESP8266: IRsendDemo - demonstrates sending IR codes with IRsend
 * An IR LED must be connected to ESP8266 pin 0.
 * Version 0.1 June, 2015
 * Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009, Copyright 2009 Ken Shirriff, http://arcfn.com
 */

#include <IRremoteESP8266.h>

IRsend irsend(0); //an IR led is connected to GPIO pin 0
unsigned int  rawData[45] = {950,750, 350,300, 300,800, 350,300, 900,250, 300,300, 950,250, 300,800, 950,750, 950,750, 950,250, 950,250, 300,300, 950,250, 950,250, 300,300, 950,250, 300,300, 300,800, 300,300, 300,800, 950,750, 300};  // UNKNOWN 165974ED
unsigned int  rawData[45] = {950,800, 350,350, 350,850, 350,350, 900,300, 350,300, 1000,250, 350,850, 950,800, 1000,800, 1000,300, 1000,250, 350,250, 1050,250, 1000,250, 350,300, 950,300, 350,350, 350,850, 350,350, 350,850, 950,800, 350};  // UNKNOWN 165974ED

void setup()
{
  irsend.begin();
  Serial.begin(9600);
  Serial.println("Ready");
}

void loop() {
  delay(500);
  Serial.println("Power");
  irsend.sendRaw(rawData, 45, 38); // 38 HZ
}
