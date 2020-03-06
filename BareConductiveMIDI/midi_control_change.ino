#include <MPR121.h>
//#include <MPR121_Datastream.h>
#include <Wire.h>
#define MPR121_ADDR 0x5C
#define MPR121_INT 4

// MIDI
#include <SoftwareSerial.h>

SoftwareSerial mySerial(12, 10); //Soft TX on 10, we don't use RX in this code


#define top 1000
#define bottom -4


void setup() {
  Serial.begin(9600);

  if(!MPR121.begin(MPR121_ADDR)) Serial.println("error setting up MPR121");
  MPR121.setInterruptPin(MPR121_INT);

  // MIDI
  // Setup soft serial for MIDI control
  mySerial.begin(31250);
}

void loop() {
  MPR121.updateAll();
  int pin0 = readPin(0);
  int pin11 = readPin(11);

  Serial.print("Pin 0: ");
  Serial.print(pin0);

  Serial.print("\t Pin 11: ");
  Serial.print(pin11);

  Serial.println("");

  
  // MIDI

  Serial.println(filter(pin0));
  
  sendControlChange(10, filter(pin0));
  sendControlChange(11, filter(pin11));

  //MIDIUSB.flush();

  delay(10);
}


int readPin(int pin) {
  return MPR121.getBaselineData(pin)-MPR121.getFilteredData(pin);
}


uint8_t filter(int input) {

  float tmp = sqrt((float) map(input, bottom, top, 0, 1000) / 1000);
  return tmp * 127;
  
}



// MIDI
void sendControlChange(uint8_t pos, uint8_t value) {
  // controlchange value B, 0, location, val
  //talkMIDI( (0xB0 | 0), pos, value );
  MIDIUSB.write({ 0x0B, 0xB0 | (((uint8_t) 0) & 0x0F), pos, value });
  
}

// note on/off works like this:
void noteOn(uint8_t channel, uint8_t pitch, uint8_t velocity) {
  MIDIUSB.write({0x09, 0x90 | (channel & 0x0F), pitch, velocity});
}

void noteOff(uint8_t channel, uint8_t pitch, uint8_t velocity) {
  MIDIUSB.write({0x08, 0x80 | (channel & 0x0F), pitch, velocity});
}
