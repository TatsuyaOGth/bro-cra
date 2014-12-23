#pragma once

#include "common.h"
#include "ofxState.h"

class Orientation : public BaseState
{
    float mVol;
    int mPeak;
    
    ofVec3f previous, current;
    ofEasyCam easyCam;
    ofCamera cam;
    
    deque<ofVec3f> pathVertices;
    
    
    void rotateToNormal(ofVec3f normal)
    {
        normal.normalize();
        
        float rotationAmount;
        ofVec3f rotationAngle;
        ofQuaternion rotation;
        
        ofVec3f axis(0, 0, 1);
        rotation.makeRotate(axis, normal);
        rotation.getRotate(rotationAmount, rotationAngle);
        ofRotate(rotationAmount, rotationAngle.x, rotationAngle.y, rotationAngle.z);
    }
    
public:
    
    Orientation()
    {
        cam.resetTransform();
        cam.setFov(60);
        cam.clearParent();
    }
    
    GET_NAME
    
    void update()
    {
        mVol = LEVEL;
        
        previous = current;
        
        // generate a noisy 3d position over time
        float t = (2 + ofGetElapsedTimef()) * .2;
        current.x = ofSignedNoise(t, 0, 0);
        current.y = ofSignedNoise(0, t, 0);
        current.z = ofSignedNoise(0, 0, t);
        current *= 1200; // scale from -1,+1 range to -400,+400
        
        cam.setGlobalPosition(current);
    }
    
    
    void draw()
    {
        
        ofBackground(0);
        ofNoFill();
        
        //	easyCam.begin();
        cam.begin();
        cam.lookAt(ofVec3f(0, 0, 0));
        ofRotateX(15);
        
        ofSetColor(0);
        ofDrawGrid(500 * LEVEL + 250, 5, false, TOGGLE[T0], TOGGLE[T1], TOGGLE[T2]);
        
        ofDrawRotationAxes(500, 1, 16);
        
        
        // translate and rotate to the current position and orientation
        ofTranslate(current.x, current.y, current.z);
        rotateToNormal(current - previous);
        
        cam.end();
        //	easyCam.end();
    }
    
    void bang()
    {
        
    }

};