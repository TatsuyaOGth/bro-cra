#include "ofMain.h"
#include "../../SharedData.h"
#include "common.h"
#include "ofxStateMachine.h"
#include "ofxOsc.h"
#include "ofxSharedMemory.h"

#include "GeometWave.hpp"

class ofApp : public ofBaseApp
{
    int mID;
    itg::ofxStateMachine<> * mVfx;
    SharedData * mSharedData;
    ofxSharedMemory<SharedData *> mSharedMem;
    bool bConnected;
    
public:
    ofApp(int argc, char** argv)
    {
        if (argc != 3)
        {
            string arg(argv[1]);
            mID = ofToInt(arg);
            ofLogNotice() << "ID: " << arg;
            
            mSharedMem.setup(SHARED_DATA_KEY, sizeof(SharedData), false);
            bConnected = mSharedMem.connect();
            mSharedData = 0;
            
            LEVEL = 0;
            for (int i = 0; i < NUM_TOGGLE; ++i) TOGGLE[i] = false;
            
            mVfx = new itg::ofxStateMachine<>();
            mVfx->addState(new GeometWave);
            mVfx->changeState("GeometWave");
            
        }
        else {
            ofLogError() << "missing args" << endl;
            OF_EXIT_APP(-1);
        }
    }
    
    void update()
    {
        if (!bConnected)
        {
            if (ofGetFrameNum() % 300 == 0) bConnected = mSharedMem.connect();
        }
        else {
            mSharedData = mSharedMem.getData();
            setLevel(mSharedData->level);
            memcpy(WAVE, mSharedData->wave, sizeof(mSharedData->wave));
        }
        
        mVfx->update();
        
        stringstream s;
        s << "VFX ID: " << mID << " | METER: ";
        for (int i = 0; i < 30; ++i)
        {
            ((float)i / 30) <= LEVEL ? (s << "#") : (s << "-");
        }
        ofSetWindowTitle(s.str());
    }
    
    void draw()
    {
        mVfx->draw();
    }
    
    void setLevel(float level)
    {
        LEVEL = level;
    }
    
    void sendBang()
    {
        itg::ofxStateMachine<>::StatePtr state = mVfx->getCurrentState();
        BaseState * base = dynamic_cast<BaseState*>(state.get());
        if (base != NULL)
        {
            base->bang();
        } else ofLogError() << "faild send bang";
    }
    
    void keyPressed(int key)
    {
        switch (key) {
            case 'b': sendBang(); break;
        }
    }
    
};

int main(int argc, char** argv)
{
	ofSetupOpenGL(DEFAULT_WINDOW_W, DEFAULT_WINDOW_H, OF_WINDOW);
	ofRunApp(new ofApp(argc, argv));
}
