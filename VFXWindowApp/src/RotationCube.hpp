#pragma once

#include "common.h"
#include "ofxState.h"

class RotationCube : public BaseState
{
    
    ofCamera mCam;
    ofVec3f mCamLook;
    ofVec3f mCamPosition;
    float mCamSpeed;
    ofVec3f mTargerPos;
    
    float mRotationSpeed;
    float mBoxSize;
    ofVec3f mPos;
    
    ofBoxPrimitive mBox;
    
    ofImage mImg;
    
    float mRotSpeed;
    float mSize;
    
    float mVecRot;
    
public:
    RotationCube():
    mRotationSpeed(0.1),
    mBoxSize(100)
    {
        mRotSpeed = 0.5;
        mSize = 0.5;
        mImg.loadImage("tmp.jpg");
    }
    GET_NAME
    
    void bang()
    {
        
    }
    
    void setup()
    {
        mCam.setPosition(0, 0, 150);
        mCamLook.set(0, 0, 0);
        mCamPosition.set(0, 0, 150);
        mCamSpeed = 0.001;
        
        mBox.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        mBox.setResolution(8);
        
        mVecRot = 0;
    }
    
    void update()
    {
        float movementSpeed = mRotSpeed;
        float cloudSize = WIDTH / 2;
        float maxBoxSize = 100;
        float spacing = 0.5;
        
        float t = (ofGetElapsedTimef() * spacing) * movementSpeed;
        mPos.set(
                 ofSignedNoise(t, 0, 0),
                 ofSignedNoise(0, t, 0),
                 ofSignedNoise(0, 0, t));
        mBoxSize = maxBoxSize * ofNoise(mPos.x, mPos.y, mPos.z) * mSize;
        
        mPos *= cloudSize;
        
        
    }
    
    void draw()
    {
        ofBackground(ofRandom(20, 190), ofRandom(20, 190), ofRandom(20, 190));
        ofSetColor(255,255,255);
        
        ofEnableDepthTest();
        ofEnableNormalizedTexCoords();
        
        //	mCam.begin();
        
        ofPushMatrix();
        ofTranslate(WIDTH / 2, HEIGHT / 2);
        
        ofRotateX(mPos.x);
        ofRotateY(mPos.y);
        ofRotateZ(mPos.z);
        
        mBox.set(mBoxSize * 10);
        //    mBox.rotate(mPos.x, 1, 0, 0);
        //    mBox.rotate(mPos.y, 0, 1, 0);
        //    mBox.rotate(mPos.z, 0, 0, 1);
        
        // image
        
        mBox.setScale((LEVEL * -1 + 1) + 1);

        // frame
        ofSetBoxResolution(8);
        if (mImg.isAllocated()) {
            mImg.bind();
            mBox.draw();
            mImg.unbind();
        } else {
            mBox.draw();
        }
        
        ofPopMatrix();
        
        //	mCam.end();
        
        ofDisableDepthTest();
        
    }
    
    
    
};