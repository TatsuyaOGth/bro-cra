#pragma once

#include "common.h"
#include "ofxState.h"
#include "ofxPostGlitch.h"

class VerticalWave : public BaseState
{
    ofFbo fbo;
    ofxPostGlitch glitch;
    
public:
    
    VerticalWave()
    {
        fbo.allocate(640, 480, GL_RGB);
        assert(glitch.setup(&fbo, "glitch"));
    }
    
    GET_NAME
    
    void update()
    {
    }
    
    void draw()
    {
        fbo.begin();
        ofBackground(0);
        ofSetColor(255);
        
        ofPushStyle();
        ofDisableAntiAliasing();
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofSetLineWidth(2);
        const float level = LEVEL;
        const float w = fbo.getWidth();
        const float h = fbo.getHeight();
        
        TOGGLE[T2] ? ofFill() : ofNoFill();
        if (TOGGLE[T0])
        {
            ofPushMatrix();
            ofTranslate(ofMap(LEVEL, 0, 1, -(h*0.5), h*0.5), 0);
        }
        
        if (TOGGLE[T1])
        {
            // center
            ofBeginShape();
            ofVertex(w * 0.5, 0);
            for (unsigned int i = 0; i < WAVE_SIZE; i++)
            {
                float x = (w * 0.5) + (WAVE[i] * (h*0.5));
                float y = ofMap(i, 0, WAVE_SIZE, 0, h);
                ofVertex(x, y);
            }
            ofVertex(w * 0.5, h);
            ofEndShape(false);
        }
        else {
            // side
            for (int n = 0; n < 2; ++n)
            {
                ofBeginShape();
                ofVertex(w * n, 0);
                for (unsigned int i = 0; i < WAVE_SIZE; i++)
                {
                    float x = (w * n) + (WAVE[i] * (h*0.5));
                    float y = ofMap(i, 0, WAVE_SIZE, 0, h);
                    ofVertex(x, y);
                }
                ofVertex(w * n, h);
                ofEndShape(false);
            }
        }
        
        if (TOGGLE[T0])
        {
            ofPopMatrix();
        }
        ofPopStyle();
        fbo.end();
        
        glitch.generateFx();
        ofSetColor(255, 255, 255);
        fbo.getTextureReference().draw(0, 0, WIDTH, HEIGHT);
    }
    
    void bang()
    {
        glitch.setFxTo(OFXPOSTGLITCH_SLITSCAN, 0.5);
    }
    
};