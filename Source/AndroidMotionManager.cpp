/*
  ==============================================================================

    AndroidMotionManager.cpp
    Created: 20 Jun 2022 1:51:24pm
    Author:  jiang

  ==============================================================================
*/
#if JUCE_ANDROID
#include "AndroidMotionManager.h"
#include <math.h>

MotionManager::MotionManager()
{
    sensorManager       = ASensorManager_getInstance();
    accelerometer       = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_ACCELEROMETER );
    magnetometer        = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_MAGNETIC_FIELD);
    looper              = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    motionEventQueue    = ASensorManager_createEventQueue(sensorManager, looper, LOOPER_ID_USER, NULL, NULL);
}

MotionManager::~MotionManager()
{
    ASensorManager_destroyEventQueue(sensorManager, motionEventQueue);
}

void MotionManager::start()
{
    ASensorEventQueue_enableSensor(motionEventQueue, accelerometer);
    ASensorEventQueue_setEventRate(motionEventQueue, accelerometer, SENSOR_REFRESH_PERIOD_US);
    ASensorEventQueue_enableSensor(motionEventQueue, magnetometer);
    ASensorEventQueue_setEventRate(motionEventQueue, magnetometer, SENSOR_REFRESH_PERIOD_US);
}

void MotionManager::stop()
{
    ASensorEventQueue_disableSensor(motionEventQueue, accelerometer);
    ASensorEventQueue_disableSensor(motionEventQueue, magnetometer);
}

void MotionManager::Update()
{
    ALooper_pollAll(0, NULL, NULL, NULL);
    ASensorEvent event;

    while (ASensorEventQueue_getEvents(motionEventQueue, &event, 1) > 0)
    {
        if (event.type == ASENSOR_TYPE_ACCELEROMETER )
        {
            accelerationX = event.acceleration.x ;
            accelerationY = event.acceleration.y ;
            accelerationZ = event.acceleration.z ;
        }

        if (event.type == ASENSOR_TYPE_MAGNETIC_FIELD)
        {
            fieldX = event.vector.y * 1;
            fieldY = event.vector.x * 1;
            fieldZ = event.vector.z * 1;
        }
    }
}

//COMPASS SETTINGS

//Calculating Magnetic Heading

float MotionManager::calculateHeading() {
    float Ax = oldAccelerationX;
    float Ay = oldAccelerationY;
    float Az = oldAccelerationZ;

    float Ex = oldFieldX;
    float Ey = oldFieldY;
    float Ez = oldFieldZ;

    //cross product of the magnetic field vector and the gravity vector
    float Hx = Ey * Az - Ez * Ay;
    float Hy = Ez * Ax - Ex * Az;
    float Hz = Ex * Ay - Ey * Ax;

    //normalize the values of resulting vector
    float invH = 1.0f / (float) sqrt(Hx * Hx + Hy * Hy + Hz * Hz);
    Hx *= invH;
    Hy *= invH;
    Hz *= invH;

    //normalize the values of gravity vector
    float invA = 1.0f / (float) sqrt(Ax * Ax + Ay * Ay + Az * Az);
    Ax *= invA;
    Ay *= invA;
    Az *= invA;

    //cross product of the gravity vector and the new vector H
    float Mx = Ay * Hz - Az * Hy;
    float My = Az * Hx - Ax * Hz;
    float Mz = Ax * Hy - Ay * Hx;

    //Result is in radians
    return (float) atan2(Hy, My);
}

void MotionManager::lowPassFilter() {
    float ALPHA = 0.15;
    oldAccelerationX = oldAccelerationX + ALPHA * (accelerationX - oldAccelerationX);
    oldAccelerationY = oldAccelerationY + ALPHA * (accelerationY - oldAccelerationY);
    oldAccelerationZ = oldAccelerationZ + ALPHA * (accelerationZ - oldAccelerationZ);

    oldFieldX = oldFieldX + ALPHA * (fieldX - oldFieldX);
    oldFieldY = oldFieldY + ALPHA * (fieldY - oldFieldY);
    oldFieldZ = oldFieldZ + ALPHA * (fieldZ - oldFieldZ);
}


#endif