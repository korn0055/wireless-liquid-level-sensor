#ifndef LIQUIDLEVELSENSOR_H
#define LIQUIDLEVELSENSOR_H

class LiquidLevelSensor
{
    public:
        LiquidLevelSensor(int sensePin, int powerPin, int samplesToAverage, int warmUpTimeMs=50);
        void Init();
        float MeasureDepthMm();
        void LoadCalibration();
    private:
        static float CalculateDepth(float periodUs, float K0, float K1);

        unsigned long MeasurePeriodUs();
        int _sensePin;
        int _powerPin;
        int _samplesToAverage;
        float _K0;
        float _K1;
        int _warmUpTimeMs;
};

#endif