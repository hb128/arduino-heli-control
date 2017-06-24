#include "lib/tacho.h"
//#include "lib/ircontrol.h"
//#include "lib/mpusensor.h"

#define Poti A0

volatile int throttle = 0;
volatile int yaw = 0;
volatile int pitch = 0;

int debugIndex = 0;

void setup() {
//  setup_mpu();
//  setup_ircontrol();
  tacho::setup();
}


void loop() {
  throttle = analogRead(Poti);
  //yaw = mpu::read_yaw();
  //pitch = mpu::read_pitch();
  //ir::sendControl(throttle, pitch, yaw);
  tacho::drawValues(throttle, pitch, yaw);
}
