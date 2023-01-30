// Origins/Author of the source code unfortunatelly not known anymore.
// Please let me know if you recognize it, so I can reference it.

#include <Joystick.h>
#include <HID_Buttons.h>

const int ROW_COUNT = 6;
const int COLUMN_COUNT = 5;

const int KEYFREE = 0;
const int KEYDOWN = 1;
const int KEYUP = 2;
const int KEYHOLD = 3;

int rowPins [] = {21, 18, 19, 20, 15, 14};
int columnPins [] = {6, 5, 4, 3, 2};
int keyStatus [ ROW_COUNT ][ COLUMN_COUNT ] = { };

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
  30, // Button Count
  0,                        // Hat Switch Count
  false, false, false,      // No X, Y, or Z axes
  false, false, false,      // No Rx, Ry, or Rz
  false, false,             // No rudder or throttle
  false, false, false);     // No accelerator, brake, or steering

void setup() {
  
  //Serial.begin(9600);
  
  // Set all key status in matrix as FREE or not pressed
  for (int i = 0; i < ROW_COUNT; i++) {
    for (int j = 0; j < COLUMN_COUNT; j++) {
      keyStatus [ i ][ j ] = KEYFREE;
    }
  }
  
  // Configure pin column as an input and enable the internal pull-up resistor
  for (int i = 0; i < COLUMN_COUNT; i++) {
    pinMode(columnPins [i], INPUT_PULLUP);
  }
  
  // Configure all row pins as input
  for (int i = 0; i < ROW_COUNT; i++) {
    pinMode(rowPins [i], INPUT);
  }
  
  Joystick.begin();
}

void loop() {
  int currentButton = 0;
  for (int i = 0; i < ROW_COUNT; i++) {
    pinMode(rowPins [i], OUTPUT);
    digitalWrite(rowPins [i], LOW);
    for (int j = 0; j < COLUMN_COUNT; j++) {
      int sensorVal = digitalRead(columnPins [j]);
      if (sensorVal == LOW && keyStatus[i][j] == KEYFREE) {
        //Serial.print((String)"Button " + currentButton + " :"); 
        //Serial.println("KEYDOWN");  
        keyStatus[i][j] = KEYDOWN;
        Joystick.setButton(currentButton, true);
      } else if (sensorVal == LOW && keyStatus[i][j] == KEYDOWN) {
        //Serial.print((String)"Button " + currentButton + " :"); 
        //Serial.println("KEYHOLD");
        keyStatus[i][j] = KEYHOLD;
        Joystick.setButton(currentButton, true);
      } else if (sensorVal == HIGH && (keyStatus[i][j] == KEYDOWN || keyStatus[i][j] == KEYHOLD)) {
        //Serial.print((String)"Button " + currentButton + " :"); 
        //Serial.println("KEYUP");
        keyStatus[i][j] = KEYUP;
        Joystick.setButton(currentButton, false);
      } else if (sensorVal == HIGH && keyStatus[i][j] == KEYUP) {
        //Serial.print((String)"Button " + currentButton + " :"); 
        //Serial.println("KEYFREE"); 
        keyStatus[i][j] = KEYFREE;
        Joystick.setButton(currentButton, false);
      }
      currentButton++;
    }
    pinMode(rowPins [i], INPUT);
  }
  delay(5);
}
