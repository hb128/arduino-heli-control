#include "lib/tacho.h"
#include "lib/ircontrol.h"
//#include "lib/mpusensor.h"

#define Poti A0

volatile int throttle = 0;
volatile int yaw = 0;
volatile int pitch = 0;

int debugIndex = 0;

void setup() {
  //mpusensor::setup();
  ircontrol::setup();
  tacho::setup();
}


void loop() {
  throttle = analogRead(Poti);
  //yaw = mpusensor::read_yaw();
  //pitch = mpusensor::read_pitch();
  ircontrol::sendControl(throttle, pitch, yaw);
  tacho::drawValues(throttle, pitch, yaw);
  delay(50);
}
