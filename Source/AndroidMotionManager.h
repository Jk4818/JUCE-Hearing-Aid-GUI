/*
  ==============================================================================

    AndroidMotionManager.h
    Created: 20 Jun 2022 1:51:24pm
    Author:  jiang

  ==============================================================================
*/

#pragma once

#if JUCE_ANDROID

#include <android/sensor.h>
class MotionManager
{
public:
    MotionManager();
    ~MotionManager();

    void start();
    void stop();

    void Update();

    double oldAccelerationX = 0;
    double oldAccelerationY = 0;
    double oldAccelerationZ = 0;
    double oldFieldX = 0;
    double oldFieldY = 0;
    double oldFieldZ = 0;

    double accelerationX = 0;
    double accelerationY = 0;
    double accelerationZ = 0;
    double fieldX = 0;
    double fieldY = 0;
    double fieldZ = 0;

    float calculateHeading();
    void lowPassFilter();

private:
    const int LOOPER_ID_USER = 3;
    int SENSOR_REFRESH_RATE_HZ = 100;
    int SENSOR_REFRESH_PERIOD_US = 1000000 / SENSOR_REFRESH_RATE_HZ;

    ASensorManager* sensorManager;
    ASensorEventQueue* motionEventQueue;
    ALooper* looper;

    const ASensor* accelerometer;
    const ASensor* magnetometer;
};
#endif