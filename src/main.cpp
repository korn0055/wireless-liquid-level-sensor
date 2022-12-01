#include <Arduino.h>
#include "LiquidLevelSensor.h"
int sense_555_pin = 14;
int power_555_pin = 32;
LiquidLevelSensor sensor(sense_555_pin, power_555_pin, 10, 50);

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  sensor.Init();
}

// the loop function runs over and over again forever
void loop() {
  float depthMm;
  
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  depthMm = sensor.MeasureDepthMm();
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);
}

 