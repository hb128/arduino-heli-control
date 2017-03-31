#include <IRremoteESP8266.h>

#define MAX_THROTTLE   6
#define MIN            -3
#define MAX            3
#define LONG           900
#define SHORT          300

IRsend irsend(4);

int throttle = 0;
int fwdBwd = 0;
int turn = 0;
int b = 0;
int debugIndex = 0;

unsigned int rawData[45];
unsigned char words[7];

int mapToRange(int n, int low, int high) {
  return max(min(n, high), low);
}


void addRawData(unsigned int* rawData, unsigned int index, unsigned value, unsigned int pair) {
  unsigned shiftedIndex = index * 2 - 1;
  unsigned shiftedValue = (value >> (pair * 2)) & 0b11;
  switch (shiftedValue) {
    case 0:
      rawData[shiftedIndex] = SHORT;
      rawData[shiftedIndex + 1] = SHORT;
      break;
    case 1:
      rawData[shiftedIndex] = SHORT;
      rawData[shiftedIndex + 1] = LONG;
      break;
    case 2:
      rawData[shiftedIndex] = LONG;
      rawData[shiftedIndex + 1] = SHORT;
      break;
    case 3:
      rawData[shiftedIndex] = LONG;
      rawData[shiftedIndex + 1] = LONG;
      break;
  }
  return;
}

void generateRawData(int throttleValue, int fwdBwdValue, int turnValue) {
  words[0] |= 0b00100000;
  words[1] = turnValue | 0;
  words[2] = throttleValue | 0;
  words[3] = fwdBwdValue | 0;
  words[4] = 0b00010001;
  words[5] = 0b00000010;
  words[6] = words[0] ^ words[1] ^ words[2] ^ words[3] ^ words[4] ^ words[5];

  rawData[0] = LONG;
  addRawData(rawData, 1, words[0], 2);
  addRawData(rawData, 2, words[0], 1);
  addRawData(rawData, 3, words[0], 0);

  addRawData(rawData, 4, words[1], 2);
  addRawData(rawData, 5, words[1], 1);
  addRawData(rawData, 6, words[1], 0);

  addRawData(rawData, 7, words[2], 3);
  addRawData(rawData, 8, words[2], 2);
  addRawData(rawData, 9, words[2], 1);
  addRawData(rawData, 10, words[2], 0);

  addRawData(rawData, 11, words[3], 2);
  addRawData(rawData, 12, words[3], 1);
  addRawData(rawData, 13, words[3], 0);

  addRawData(rawData, 14, words[4], 2);
  addRawData(rawData, 15, words[4], 1);
  addRawData(rawData, 16, words[4], 0);

  addRawData(rawData, 17, words[5], 2);
  addRawData(rawData, 18, words[5], 1);
  addRawData(rawData, 19, words[5], 0);

  addRawData(rawData, 20, words[6], 2);
  addRawData(rawData, 21, words[6], 1);
  addRawData(rawData, 22, words[6], 0);
  return;
}


void mapToHeliCodec(int throttle, int fwdBwd, int turn) {
  int throttleValue, fwdBwdValue, turnValue;
  throttleValue = mapToRange(throttle * 22, 0 , 131);
  fwdBwdValue = mapToRange(fwdBwd * fwdBwd * fwdBwd + 17 , 0, 35);
  turnValue = mapToRange(turn * turn * turn + 17 , 0, 35);
  generateRawData(throttleValue, fwdBwdValue, turnValue);
}



void setup() {
  Serial.begin(115200);
  irsend.begin();
  Serial.println("READY TO RUMBLE!!!!");
}

void loop() {
  irsend.sendRaw(rawData, 45, 38);
  // send data only when you receive data:
  if (Serial.available() > 0) {
    b = Serial.read();
    if (b == '0')
      throttle = mapToRange(throttle + 1, 0, MAX_THROTTLE);
    else if ( b == '9')
      throttle = mapToRange(throttle - 1, 0, MAX_THROTTLE);
    else if ( b == 'a')
      turn = (turn <= 0) ? mapToRange(turn - 1, MIN, MAX) : 0;
    else if ( b == 'd')
      turn = (turn >= 0) ? mapToRange(turn + 1, MIN, MAX) : 0;
    else if ( b == 's')
      fwdBwd = (fwdBwd <= 0) ? mapToRange(fwdBwd - 1, MIN, MAX) : 0;
    else if ( b == 'w')
      fwdBwd = (fwdBwd >= 0) ? mapToRange(fwdBwd + 1, MIN, MAX) : 0;
    else
      Serial.println("Malicious byte recieved!");
    if (!Serial.available())
      mapToHeliCodec(throttle, fwdBwd, turn);
  }
  if(debugIndex%10){
    Serial.print("Send ");
    Serial.print(throttle);
    Serial.print(" ");
    Serial.print(fwdBwd);
    Serial.print(" ");
    Serial.print(turn);
    Serial.println("");
    debugIndex=0;
  }
  debugIndex += 1;
  delay(50);
}
