#include "BatteryMonitory.h"
#include "Arduino.h"

BatteryMonitor::BatteryMonitor(int vBattAdcPin, float vRef) :
    _vBattAdcPin(vBattAdcPin),
    _vRef(vRef),
    _batteryVoltage(NAN)
{}

void BatteryMonitor::Init()
{
    pinMode(_vBattAdcPin, INPUT);
    Serial.println("BatteryMonitor initialized.");
}

float BatteryMonitor::CalculateVoltage(unsigned int adcCount, float vRef)
{
    return 2 * adcCount * vRef * 1.1 / 4095.0;
}

float BatteryMonitor::MeasureBatteryVoltage()
{
    unsigned int adcCount = analogRead(_vBattAdcPin);
    _batteryVoltage = CalculateVoltage(adcCount, _vRef);

    String s = "Vbatt=" + String(_batteryVoltage) + " V";
    Serial.println(s);

    return _batteryVoltage;
}