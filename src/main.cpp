#include <Arduino.h>
#include "LiquidLevelSensor.h"
#include "BatteryMonitory.h"
#include "Feed.h"

int sense_555_pin = 14;
int power_555_pin = 32;
int analog_vbatt_pin = A13;
int measurement_period_ms = 10000;

LiquidLevelSensor sensor(sense_555_pin, power_555_pin, 10, 50);
BatteryMonitor batteryMonitor(analog_vbatt_pin, 3.3);
AdafruitIoFeed feed;

bool ProcessDepthSensing();

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  sensor.Init();
  batteryMonitor.Init();
  feed.Setup();
}

// the loop function runs over and over again forever
void loop() {
  feed.Loop();
  if(ProcessDepthSensing())
  {
    feed.PublishDepth(sensor.GetLastDepthMm());
    feed.PublishBattery(batteryMonitor.GetLastBatteryVoltage());
  }
}

bool ProcessDepthSensing()
{
  static unsigned long lastMeasurementTimeMs = 0;
  bool newMeasurement = false;

  if(millis() - lastMeasurementTimeMs > measurement_period_ms)
  {
    lastMeasurementTimeMs = millis();

    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    sensor.MeasureDepthMm();
    batteryMonitor.MeasureBatteryVoltage();

    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    newMeasurement = true;
  }

  return newMeasurement;
}



 