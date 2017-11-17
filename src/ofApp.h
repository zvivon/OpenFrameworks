#pragma once


#include "ofMain.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void update() override;
    void draw() override;
    void processButton();
    void processSensors();
    void resetTracking();
    void keyPressed(int key) override;

    // Timerange where we track things. 
    const int minTime = 2000; // 2 seconds
    const int medianTime = 6000; // 6 seconds
    const int maxTimeout = 10000; // 10 seconds
    
    // An instance of the serial object. One instance is needed for each active
    // serial connection.
    ofSerial serial;
    // A buffer to store our buffered data.
    std::string buffer;
    
    // Value from the serial port.
    bool buttonValue_0 = false;
    bool curSensorVal1 = false;
    bool curSensorVal2 = false;
    
    // Previous values
    bool prevSensorVal1 = false;
    bool prevSensorVal2 = false;
    
    // Sound we need to play.
    ofSoundPlayer   mySound;
    
    // Elapsed time between when we started tracking and
    // when we finish tracking.
    unsigned long int elapsedTime;
    
    // Are we tracking?
    bool tracking = false;
    
    // A queue to keep a track of num of times the audio should
    // be played based on number of people who have entered the
    // space.
    unsigned int pendingQueue = 0;
};
