#pragma once

#include "BaseContentsInterface.h"
#include "ofxEasingCam.h"

class Orientation : public BaseContentsInterface
{
    ofVec3f previous, current;
    ofxEasingCam cam;
    
    ofLight lig;
    ofParameter<float> mSpeed;
    ofParameter<ofVec3f> mLigPos;
    ofParameter<ofVec3f> mLigDirection;
    
    
public:
    Orientation()
    {
        base::mParamGroup.setName(getName());
        base::mParamGroup.add(mSpeed.set("speed", 0.05, 0.001, 0.1));
        base::mParamGroup.add(mLigPos.set("light_pos", ofVec3f(0,0,0), ofVec3f(-600,-600,-600), ofVec3f(600,600,600)));
        base::mParamGroup.add(mLigDirection.set("light_direction", ofVec3f(0,0,0), ofVec3f(-360,-360,-360), ofVec3f(360,360,360)));
    }
    
    void setup()
    {
        cam.setup(WIDTH, HEIGHT);
        lig.setup();
//        lig.setSpotlight();
        lig.setDirectional();
        
    }
    
    void update()
    {
        previous = current;
        
        // generate a noisy 3d position over time
        float t = (2 + share::elapsedTime) * mSpeed;
        current.x = ofSignedNoise(t, 0, 0);
        current.y = ofSignedNoise(0, t, 0);
        current.z = ofSignedNoise(0, 0, t);
        current *= 1200; // scale from -1,+1 range to -400,+400
        
        cam.setPos(current);
        cam.update();
        
        lig.setPosition(mLigPos);
        lig.setOrientation(mLigDirection);
    }
    
    void draw()
    {
        
        
        ofPushStyle();
        ofNoFill();
        ofEnableLighting();
        ofEnableDepthTest();
        cam.begin();
        lig.enable();
        
        ofSetColor(255, 255, 255, LEVEL * 200 + 55);
        drawGrid(1500, 20, true, false, true, false);
        lig.draw();
        lig.disable();
        cam.end();
        ofDisableLighting();
        ofDisableDepthTest();
        
        
        drawRaderWindowMarks(6, (abs(sin(share::elapsedTime))) * 20 + 5);
        ofPopStyle();
    }
    
    void getBang()
    {
        
    }
    
    void drawGrid(float scale, float ticks, bool labels, bool x, bool y, bool z)
    {
        ofPushStyle();
        
        if (x) {
            drawGridPlane(scale, ticks, labels);
        }
        if (y) {
            ofPushMatrix();
            ofRotate(90, 0, 0, -1);
            drawGridPlane(scale, ticks, labels);
            ofPopMatrix();
        }
        if (z) {
            ofPushMatrix();
            ofRotate(90, 0, 1, 0);
            drawGridPlane(scale, ticks, labels);
            ofPopMatrix();
        }
        
        if (labels) {
            ofPushStyle();
            ofSetColor(255, 0, 0);
            float labelPos = scale * (1.0f + 0.5f / ticks);
            ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
            ofDrawBitmapString("x", labelPos, 0, 0);
            ofDrawBitmapString("y", 0, labelPos, 0);
            ofDrawBitmapString("z", 0, 0, labelPos);
            ofPopStyle();
        }
        ofPopStyle();
    }
    
    void drawGridPlane(float scale, float ticks, bool labels)
    {
        
        float minor = scale / ticks;
        float major =  minor * 2.0f;
        
        ofPushStyle();
        ofFill();
        for (int iDimension=0; iDimension<2; iDimension++)
        {
            for (float yz=-scale; yz<=scale; yz+= minor)
            {
                
                //major major
                if (fabs(yz) == scale || yz == 0)
                    ofSetLineWidth(2);
                
                //major
                else if (yz / major == floor(yz / major) )
                    ofSetLineWidth(1.5);
                
                //minor
                else
                    ofSetLineWidth(1);
                if (iDimension==0) {
                    ofLine(0, yz, -scale, 0, yz, scale);
                }
                else {
                    ofLine(0, -scale, yz, 0, scale, yz);
                }

            }
        }
        ofPopStyle();
        
        if (labels) {
            //draw numbers on axes
            ofPushStyle();
            ofSetColor(190, 190, 200);
            
            float accuracy = ceil(-log(scale/ticks)/log(10.0f));
            
            ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
            for (float yz = -scale; yz<=scale; yz+=minor)
            {
                ofDrawBitmapString(ofToString(yz, accuracy), 0, yz, 0);
                ofDrawBitmapString(ofToString(yz, accuracy), 0, 0, yz);		
            }
            ofPopStyle();
        }
    }
    
    void drawRaderWindowMarks(int res, float size)
    {
        int xstep = (int)(WIDTH  / res);
        int ystep = xstep;
        
        for (int y = 0; y <= HEIGHT; y += ystep) {
            for (int x = 0; x <= WIDTH; x += xstep) {
                ofLine(x    - (size * 0.5), y, x + (size * 0.5), y);
                ofLine(x, y - (size * 0.5), x, y + (size * 0.5)   );
            }
        }
    }
    
};
