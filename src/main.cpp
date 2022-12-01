#include <Arduino.h>
int sense_555_pin = 14;
int power_555_pin = 32;

int val = 0;
const float t0 = 44.0;
const float t1 = 463.5;
const float d1 = 100.0;

const float K0 = -(t0 * d1)/(t1 - t0);
const float K1 = d1 / (t1 - t0);

float CalculateDepth(unsigned long periodUs, float K0, float K1);
unsigned long MeasurePeriodUs(int pin);

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  pinMode(sense_555_pin, INPUT);
  pinMode(power_555_pin, OUTPUT);
  digitalWrite(power_555_pin, LOW);

  String s = "K0=";
  s += K0;
  s += " K1=";
  s += K1;


}

// the loop function runs over and over again forever
void loop() {
  float sum = 0;
  float avg = 0;
  int N = 10;
  float depthMm = -999;

  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(power_555_pin, HIGH);
  // warm up delay
  delay(50);

  for(int i=0; i < N; i++)
  {
    sum += MeasurePeriodUs(sense_555_pin);
  }

  digitalWrite(power_555_pin, LOW);

  avg = sum / N;
  depthMm = CalculateDepth(avg, K0, K1);

  String s = "Tavg=";
  s += avg;
  s += " us, d=";
  s += depthMm;
  s += " mm";
  s += " K0=";
  s += K0;
  s += " K1=";
  s += K1;
  Serial.println(s);
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW

  delay(1000);
}

unsigned long MeasurePeriodUs(int pin)
{
  unsigned long period, pulseHigh, pulseLow;
  pulseHigh = pulseIn(pin, HIGH);
  pulseLow = pulseIn(pin, LOW);
  period = pulseHigh + pulseLow;

  String s = "T=";
  s += period;
  Serial.println(s);
  return period;
}

float CalculateDepth(unsigned long periodUs, float K0, float K1)
{
  float depthMm = K1*periodUs+K0;

  return depthMm;
}
 