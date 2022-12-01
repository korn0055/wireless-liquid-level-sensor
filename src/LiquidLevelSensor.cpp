#include "LiquidLevelSensor.h"
#include "Arduino.h"

LiquidLevelSensor::LiquidLevelSensor(int sensePin, int powerPin, int samplesToAverage, int warmUpTimeMs)
{
    _sensePin = sensePin;
    _powerPin = powerPin;
    _samplesToAverage = samplesToAverage;
    _warmUpTimeMs = warmUpTimeMs;
}

void LiquidLevelSensor::Init()
{
    pinMode(_sensePin, INPUT);
    pinMode(_powerPin, OUTPUT);
    digitalWrite(_powerPin, LOW);
    LoadCalibration();
}

void LiquidLevelSensor::LoadCalibration()
{
    // Calibration (2-point linear fit - assumes first point is dry/0 depth)
    const float t0 = 44.0;
    const float t1 = 463.5;
    const float d1 = 100.0;
    _K0 = -(t0 * d1)/(t1 - t0);
    _K1 = d1 / (t1 - t0);
}

unsigned long LiquidLevelSensor::MeasurePeriodUs()
{
    unsigned long periodMs, pulseHighMs, pulseLowMs;
    pulseHighMs = pulseIn(_sensePin, HIGH);
    pulseLowMs = pulseIn(_sensePin, LOW);
    periodMs = pulseHighMs + pulseLowMs;

    String s = "T=";
    s += periodMs;
    Serial.println(s);
    return periodMs;
}

float LiquidLevelSensor::MeasureDepthMm()
{
    unsigned long sum = 0;
    float averagePeriodUs;
    float depthMm;

    digitalWrite(_powerPin, HIGH);
    delay(_warmUpTimeMs);

    for(int i=0; i < _samplesToAverage; i++)
    {
        sum += MeasurePeriodUs();
    }

    digitalWrite(_powerPin, LOW);
    averagePeriodUs = sum / _samplesToAverage;
    depthMm = CalculateDepth(averagePeriodUs, _K0, _K1);
    
    String s = "Tavg=";
    s += averagePeriodUs;
    s += " us, d=";
    s += depthMm;
    s += " mm";
    s += " K0=";
    s += _K0;
    s += " K1=";
    s += _K1;
    Serial.println(s);
    return depthMm;
}

float LiquidLevelSensor::CalculateDepth(float periodUs, float K0, float K1)
{
  float depthMm = K1*periodUs+K0;

  return depthMm;
}