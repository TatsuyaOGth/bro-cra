#pragma once

#include "common.h"
#include "ofxState.h"

class GeometWave : public BaseState
{
    list<shared_ptr<BaseAnimationUnit> > mPerticle;
    
    int mode;
    
    ofColor mColor;
    
    vector<shared_ptr<RotationArc> > mArcs;
    
public:
    
    GeometWave()
    {
        mode = 0;
        for (int i = 0; i < WAVE_SIZE; i++) {
            mArcs.push_back(shared_ptr<RotationArc>(new RotationArc(300, 600, ofRandom(30, 180), 0, ofRandom(5, 20))));
        }
    }
    
    GET_NAME
    
    void update()
    {
        for (anm_it it = mPerticle.begin(); it != mPerticle.end(); it++) {
            (*it)->update();
            if ((*it)->isDied()) mPerticle.erase(it);
        }
        
        for (vector<shared_ptr<RotationArc> >::iterator it = mArcs.begin(); it != mArcs.end(); it++) {
            (*it)->update();
        }
    }
    
    void draw()
    {
        if (TOGGLE[T0])
        {
            ofBackground(190);
            mColor.set(0, 0, 0);
            ofSetColor(0);
        }
        else {
            ofBackground(0);
            mColor.set(255, 255, 255);
            ofSetColor(255);
        }
        ofDisableAntiAliasing();
        
        ofPushStyle();
        ofSetRectMode(OF_RECTMODE_CENTER);
        const int size = WAVE_SIZE * (LEVEL * 0.1 + 0.05);
        const float level = LEVEL;
        ofSetLineWidth(1);
        ofNoFill();
        ofBeginShape();
        for (unsigned int i = 0; i < size; i++) {
            float x = ofMap(i, 0, size-1, 0, WIDTH);
            float y = (HEIGHT * 0.5) - WAVE[i] * (HEIGHT * 0.5);
            ofVertex(x, y);
        }
        ofEndShape(false);
        for (unsigned int i = 0; i < size; i++) {
            float x = ofMap(i, 0, size, 0, WIDTH);
            float y = (HEIGHT * 0.5) - WAVE[i] * (HEIGHT * 0.5);
            ofSetColor(mColor);
            drawPoint(x, y, LEVEL, i);
        }
        ofPopStyle();
    }
    
    void bang()
    {
        randamPattern();
    }
    
    void randamPattern()
    {
        mode = (int)ofRandom(5);
    }
    
    void drawPoint(int x, int y, float size, int i)
    {
        if (TOGGLE[T1])
        {
            ofColor col;
            col.setHsb(ofRandom(255), 255, 255);
            ofSetColor(col);
        }
        switch (mode) {
            default:
            case 0:
                ofFill();
                ofCircle(x, y, size * 10);
                ofNoFill();
                ofCircle(x, y, size * 20);
                ofCircle(x, y, size * 30);
                break;
                
            case 1:
                ofFill();
                ofRect(x, y, size * 7, size * 7);
                ofNoFill();
                ofRect(x, y, size * 30, size * 10);
                ofRect(x, y, size * 10, size * 30);
                break;
                
            case 2:
                ofFill();
                mArcs[i]->pos.set(x, y);
                mArcs[i]->size = size * 70;
                mArcs[i]->draw();
                break;
                
            case 3:
                ofNoFill();
                ofSetLineWidth(2);
                mArcs[i]->pos.set(x, y);
                mArcs[i]->size = size * 100;
                mArcs[i]->draw();
                break;
                
            case 4:
                ofNoFill();
                ofSetLineWidth(1);
                ofLine(x, y, x, HEIGHT * 0.5);
                ofLine(x-5, HEIGHT * 0.5, x+5, HEIGHT * 0.5);
                ofLine(x-5, y, x+5, y);
                break;
                
        }
    }
    
};