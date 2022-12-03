#include "Feed.h"
#include "Credentials.h"

AdafruitIoFeed::AdafruitIoFeed() :
    _io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS)
{
    _pDepthMmFeed = _io.feed("depth_mm");
    _pBatteryMonitorFeed = _io.feed("battery_monitor_V");
}

void AdafruitIoFeed::Setup()
{
    // connect to io.adafruit.com
    Serial.println("Connecting to AdafruitIO...");
    _io.connect();
    // wait for a connection
    while(_io.status() < AIO_CONNECTED) {
        Serial.println(_io.statusText());
        delay(500);
    }
    Serial.println();
    Serial.println(_io.statusText());
    // Serial.println("Sleep val=" + String(secondsToSleep));
}

void AdafruitIoFeed::Loop()
{
    _io.run();
}

void AdafruitIoFeed::PublishDepth(float depthMm)
{
    _pDepthMmFeed->save(depthMm);
}

void AdafruitIoFeed::PublishBattery(float batteryVoltage)
{
    _pBatteryMonitorFeed->save(batteryVoltage);
}