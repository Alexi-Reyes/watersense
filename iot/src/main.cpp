#include <Arduino.h>

const int analogPin = A0;    // pin that the signal of the water sensor is attached to
const int relayPin = 8;       // pin that the relay is attached to
const int threshold = 10;   // an arbitrary threshold level that's in the range of the analog input

void setup() {
  // initialize the relay pin as an output:
  pinMode(relayPin, OUTPUT);
  // initialize serial communications:
  Serial.begin(9600);
}

void loop() {

  int analogValue = analogRead(analogPin);

  // if the analog value is high enough, turn on the relay:
  if (analogValue > threshold) {
    digitalWrite(relayPin, HIGH);
  } else {
    digitalWrite(relayPin, LOW);
  }

  // print the analog value:
  Serial.println(analogValue);
  delay(1000);        // delay in between reads for stability
}