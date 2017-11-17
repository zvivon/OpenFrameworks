#include "ofApp.h"


void ofApp::setup()
{
    ofBackground(ofColor::black);
    ////////// Runthis on terminal ls -ls /dev/cu* - This command lets you see all the serial devices on your computer.
    ////////// Copy the name of your ardunio in here, like the red port downhere
    serial.setup("/dev/cu.usbmodem1411", 9600); // MAC
    //serial.setup("/dev/ttyACM0", 9600); // Raspberry Pi
    mySound.load("LISE_VO_DEADHORSE_2.wav");
    mySound.setVolume(0.5f);
    buttonValue_0 = true;
}

void ofApp::update()
{
    // Play the audio from the queue till the pendingQueue is
    // empty.
    if (!mySound.isPlaying() && pendingQueue > 0) {
        mySound.play();
        pendingQueue = pendingQueue - 1;
    }
    
    // As long as there is serial data available to read, repeat.
    while (serial.available() > 0)
    {
        // Read the byte.
        char b = serial.readByte();
        
        // End of line character.
        if (b == '\n')
        {
            // Split the buffer on the commas.
            std::vector<std::string> tokens = ofSplitString(buffer, ",");
            
            // The number of tokens in our packet is 3, here we check to make
            // sure that our packet is correctly formed.
            if (tokens.size() == 3)
            {
                buttonValue_0 = ofToBool(tokens[0]);
                curSensorVal1 = ofToBool(tokens[1]); // Since we reversed the sensors.
                curSensorVal2 = ofToBool(tokens[2]);
                
                // Process button click.
                processButton();
                
                // Process sensors.
                processSensors();
            }
            buffer = "";
        }
        else
        {
            // If it's not the line feed character, add it to the buffer.
            buffer += b;
        }
    }
}

void ofApp::draw()
{
    ofSetColor(ofColor::green);
    ofFill();
}

void ofApp::processButton() {
    // Is button pressed?
    if (buttonValue_0 != true)
    {
        // Is sound playing?
        if (!mySound.isPlaying()) {
            mySound.play();
            // Loop the sound.
            mySound.setLoop(true);
        } else {
            mySound.stop();
            // Stop looping the sound.
            mySound.setLoop(false);
        }
    }
}

void ofApp::processSensors() {
    // Do something with sensor 1 value.
    if (curSensorVal1 == true) {
        if (tracking == false) {
            // Start tracking.
            prevSensorVal1 = curSensorVal1;
            tracking = true;
            
            // Reset timer.
            ofResetElapsedTimeCounter();
            
            cout << "Start tracking" << endl;
        }
    }
    
    // Do something with sensor 2 value.
    if (curSensorVal2 == true) {
        if (prevSensorVal1 == true) {
            // Calculate the elapsed time.
            elapsedTime = ofGetElapsedTimeMillis();
            
            if (elapsedTime < minTime) {
                // Somebody is trying to exit as soon as somebody began entering.
                // Ignore. Ignore.
                // We won't do anything here.
                // This also handles any errors in both the sensors receiving same values.
            } else if (elapsedTime < medianTime) {
                // Average case. If this time has passed, I can be confident that
                // a person has probably entered. If they haven't entered and current sensor value
                // 2 still triggered, let's presume they should have entered by now.
                
                cout << "Somebody entered" << endl;
                cout << "Elapsed Time is " << elapsedTime << endl;
                
                // Reset tracking of the sensor.
                resetTracking();
                
                // Increment the queue for number of times audio should be played.
                pendingQueue = pendingQueue + 1;
            } else if (elapsedTime < maxTimeout){
                // Unexpected. Person has taken too long to come inside the room.
                // We will reset tracking.
                // Don't increment the pending queue.
                resetTracking();
            }
        }
    }
}

void ofApp::resetTracking() {
    prevSensorVal1 = false;
    tracking = false;
}

void ofApp::keyPressed(int key) {
    // Restart tracking.
    if (key == 49) {
        resetTracking();
    }
}

