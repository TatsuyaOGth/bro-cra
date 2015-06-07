#pragma once

#include "common.h"
#include "ofxState.h"

class MovieSlicer : public BaseState
{
    ofVideoPlayer mMov;
    int mNumFrame;
    
public:
    MovieSlicer(string path)
    {
        
        assert(mMov.loadMovie(path));
        mNumFrame = mMov.getTotalNumFrames();
        mMov.setLoopState(OF_LOOP_NORMAL);
        mMov.play();
    }
    
    void stateEnter()
    {
        mMov.play();
    }
    
    void stateExit()
    {
        mMov.stop();
    }
    
    GET_NAME
    
    void update()
    {
        mMov.update();
    }
    
    void draw()
    {
        mMov.draw(0, 0, WIDTH, HEIGHT);
    }
    
    void bang()
    {
        randomPosition();
    }
    
    void randomPosition()
    {
        int pos = ofRandom(0, mNumFrame);
        mMov.setFrame(pos);
    }
    
};
