#include <FastTouch.h>
#include <Bounce.h>

// fast touch test


int ccPin01 = 23;
int notePin01 = 14;
//
int cc = 1;
//
int low = 11;
int high = 50; // 27

void setup() {
  SerialUSB.begin(9600);
}

elapsedMillis msec = 0;
int ccPrev = 0;
bool note01state = false;
void loop() {

  if (msec >= 20) {
    msec = 0;
    int ccRead = constrain(map(fastTouchRead(ccPin01), low, high, 0, 127), 0, 127);
    Serial.print(ccRead);
    for (int i = 0; i < ccRead; i++) {
      Serial.print("#");
    }
    Serial.println("");
    
    // MIDI

    int noteRead = fastTouchRead(notePin01);
    if (note01state == false && noteRead > 20) {
      usbMIDI.sendNoteOn(60, 100, 1);
      note01state = true;
    } else if (note01state && noteRead < 20) {
      usbMIDI.sendNoteOff(60, 0, 1);
      note01state = false;
    }
    
    if (ccRead != ccPrev) {
      usbMIDI.sendControlChange(cc, ccRead, 1);
      ccPrev = ccRead;
    }

    while (usbMIDI.read()) {}
  }
}
