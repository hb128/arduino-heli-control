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
int debugIndex = 0;

// http://forum.arduino.cc/index.php?topic=288234.msg2459466#msg2459466
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use by strtok() function
// variables to hold the parsed data
char messageFromPC[numChars] = {0};
int integerFromPC = 0;
float floatFromPC = 0.0;
boolean newData = false;

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
  throttleValue = mapToRange(throttle / 7, 0 , 131);
  fwdBwdValue = mapToRange(fwdBwd, 0, 35);
  turnValue = mapToRange(turn, 0, 35);
  generateRawData(throttleValue, fwdBwdValue, turnValue);
}


void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;
  
  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
            ndx = numChars - 1;
        }
      }
      else {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }
    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

bool parseData() {

      // split the data into its parts
    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    if(!strtokIndx)
      return false;
    strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    if(!strtokIndx)
      return false;
    integerFromPC = atoi(strtokIndx);     // convert this part to an integer
    strtokIndx = strtok(NULL, ",");
    if(!strtokIndx)
      return false;
    floatFromPC = atof(strtokIndx);     // convert this part to a float
    return true;
}

void showParsedData() {
    //Serial.print("Message");
    Serial.println(messageFromPC);
    //Serial.print("Integer");
    Serial.println(integerFromPC);
    //Serial.print("Float ");
    //Serial.println(floatFromPC);
}


void setup() {
  Serial.begin(115200);
  irsend.begin();
  Serial.println("READY TO RUMBLE!!!!");
}

void loop() {
  irsend.sendRaw(rawData, 45, 38);
  // send data only when you receive data:
  throttle = analogRead(A0);
  recvWithStartEndMarkers();
  if (newData == true) {
    strcpy(tempChars, receivedChars);
        // this temporary copy is necessary to protect the original data
        //   because strtok() replaces the commas with \0
    if(parseData()){
      showParsedData();
      newData = false;
      if(messageFromPC[0] == 'T') {
        turn = integerFromPC;
      }
      else if(messageFromPC[0] == 'F') {
        fwdBwd = integerFromPC;
      }
    }
    else {
      Serial.println("Error when parsing data");
    }
  }
  mapToHeliCodec(throttle, fwdBwd, turn);
  if(debugIndex%2 == 0){
    Serial.print("Send ");
    Serial.print(throttle);
    Serial.print(" ");
    Serial.print(fwdBwd);
    Serial.print(" ");
    Serial.print(turn);
    Serial.println(" ");
    debugIndex=0;
  }
  debugIndex += 1;
  delay(50);
}
