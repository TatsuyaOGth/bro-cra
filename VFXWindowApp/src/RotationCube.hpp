#pragma once

#include "BaseContentsInterface.h"

class RotationCube : public BaseContentsInterface
{
    ofLight light; // creates a light and enables lighting
    
    ofCamera mCam;
    ofVec3f mCamLook;
    ofVec3f mCamPosition;
    float mCamSpeed;
    ofVec3f mTargerPos;
    
    float mRotationSpeed;
    float mBoxSize;
    ofVec3f mPos;
    
    ofBoxPrimitive mBox;
    
//    ofImage mImg;
    ofTexture & mTex;
    
    ofParameter<ofVec3f> mLigPos;
    ofParameter<float> mRotSpeed;
    ofParameter<float> mSize;
    
    float mVecRot;
    
public:
    RotationCube(ofTexture & tex):
    mTex(tex),
    mRotationSpeed(0.1),
    mBoxSize(100)
    {
        base::mParamGroup.add(mLigPos.set("lig_pos", ofVec3f(0,0,0), ofVec3f(-600,-600,-600), ofVec3f(600,600,600)));
        base::mParamGroup.add(mRotSpeed.set("rot_speed", 0.5, 0, 1));
        base::mParamGroup.add(mSize.set("size", 1, 0, 2));
    }
    
    
    void setup()
    {
        mCam.setPosition(0, 0, 150);
        mCamLook.set(0, 0, 0);
        mCamPosition.set(0, 0, 150);
        mCamSpeed = 0.001;
        
        mBox.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        mBox.setResolution(8);
        
//        light.setup();
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
        
        ofEnableDepthTest();
        
        ofEnableLighting();
        ofEnableNormalizedTexCoords();
        light.enable();
        light.setPosition(mLigPos);
        
        
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
        
        mBox.setScale((base::getLevel() * -1 + 1) + 1);

        // frame
        ofSetBoxResolution(8);
        if (mTex.isAllocated()) {
            mTex.bind();
            mBox.draw();
            mTex.unbind();
        } else {
            mBox.draw();
        }
        
        ofPopMatrix();
        
        //	mCam.end();
        
        ofDisableNormalizedTexCoords();
        light.disable();
        ofDisableLighting();
        ofDisableDepthTest();
        
    }
    
    void getBang()
    {
    }

    
    
};