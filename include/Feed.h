#ifndef FEED_H
#define FEED_H

#include <AdafruitIO.h>
#include <AdafruitIO_WiFi.h>


class AdafruitIoFeed
{
    public:
        AdafruitIoFeed();
        void Setup();
        void Loop();
        void PublishDepth(float depthMm);

    private:
        AdafruitIO_WiFi _io;
        AdafruitIO_Feed* _pDepthMmFeed;
};

#endif