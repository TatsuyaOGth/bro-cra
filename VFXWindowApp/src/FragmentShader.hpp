#pragma once

#include "common.h"
#include "ofxState.h"

class FragmentShader : public BaseState
{
    ofShader mShader;
    ofFbo fbo;
    ofColor mCol;
    
public:
    FragmentShader(string shaderPath)
    {
        assert(mShader.load("shader/vert.vert", shaderPath));
        fbo.allocate(640, 480, GL_RGB);
        mCol.set(ofColor::fromHsb(ofRandom(255), 255, 255));
    }
    
    GET_NAME
    
    void update()
    {
        
    }
    
    
    void draw()
    {
        fbo.begin();
        ofBackground(0, 0, 0);
        ofSetColor(255, 255, 255);
        ofFill();
        
        const float w = fbo.getWidth();
        const float h = fbo.getHeight();
        
        mShader.begin();
        
        float resolution[2];
        resolution[0] = w;
        resolution[1] = h;
        float time = ofGetElapsedTimef();
        float mousePos[2];
        mousePos[0] = LEVEL;
        mousePos[1] = 0.5;
        
        mShader.setUniform1f("time", time);
        mShader.setUniform2fv("resolution", resolution);
        mShader.setUniform2fv("mouse", mousePos);
        
        glEnable(GL_DEPTH_TEST);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex2f(0, 0);
        glVertex2f(w, 0);
        glVertex2f(0, h);
        glVertex2f(w, h);
        glEnd();
        mShader.end();
        fbo.end();
        
        TOGGLE[T0] ? ofSetColor(mCol) : ofSetColor(255, 255, 255, 255);
        fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    
    void bang()
    {
        mCol.set(ofColor::fromHsb(ofRandom(255), 255, 255));
    }
    
};
