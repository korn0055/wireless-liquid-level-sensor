#include "Feed.h"
#include "Credentials.h"

AdafruitIoFeed::AdafruitIoFeed() :
    _io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS)
{
    _pDepthMmFeed = _io.feed("depth_mm");
}

void AdafruitIoFeed::Setup()
{
    // connect to io.adafruit.com
    _io.connect();
    // wait for a connection
    while(_io.status() < AIO_CONNECTED) {
        Serial.println(_io.statusText());
        delay(500);
    }

    Serial.println();
    Serial.println(_io.statusText());
    // Serial.println("Sleep val=" + String(secondsToSleep));
    Serial.println("AdafruitIoFeed::Setup");
}

void AdafruitIoFeed::Loop()
{
    _io.run();
}

void AdafruitIoFeed::PublishDepth(float depthMm)
{
    _pDepthMmFeed->save(depthMm);
}