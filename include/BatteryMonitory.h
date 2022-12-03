#ifndef BATTERYMONITOR_H
#define BATTERYMONITOR_H

class BatteryMonitor
{
    public:
        BatteryMonitor(int vbattAdcPin, float vRef);
        void Init();
        float MeasureBatteryVoltage();
        float GetLastBatteryVoltage() const {return _batteryVoltage;}
    private:
        static float CalculateVoltage(unsigned int adcCounts, float vRef);

        const int _vBattAdcPin;
        const float _vRef;
        float _batteryVoltage;

};

#endif