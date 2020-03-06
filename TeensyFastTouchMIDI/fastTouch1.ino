#include <FastTouch.h>

// fast touch test


int testpin = 23;
int low = 0;
int high = 127; // 27

void setup() {
  SerialUSB.begin(9600);
}

void loop() {
  int pinread = fastTouchRead(testpin);
  //float perc = ((float) (pinread - low) / (float) (high - low)) * 100.0;
  float perc = map(pinread, low, high, 0, 100);
  Serial.print(perc);
  for (int i = 0; i < perc; i++) {
    Serial.print("#");
  }
  Serial.println("");
  delay(10);
}
