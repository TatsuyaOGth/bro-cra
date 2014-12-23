#include "ofMain.h"
#include "../../SharedData.h"
#include "common.h"
#include "ofxStateMachine.h"
#include "ofxOsc.h"
#include "ofxSharedMemory.h"
#include "ofxPostGlitch.h"

#include "GeometWave.hpp"
#include "BoxPerticle.hpp"
#include "Orientation.hpp"
#include "RotationCube.hpp"
#include "Earth.hpp"

class ofApp : public ofBaseApp
{
    int mID;
    itg::ofxStateMachine<> * mVfx;
    SharedData * mSharedData;
    ofxSharedMemory<SharedData *> mSharedMem;
    bool bConnected;
    
    ofRectangle mCurrentWinRect;
    vector<string> mVfxNames;
    
    ofFbo mFbo;
    ofxPostGlitch mGlitch;

    
public:
    ofApp(int argc, char** argv)
    {
        if (argc != 3)
        {
            
            string arg(argv[1]);
            mID = ofToInt(arg) - 1;
            ofLogNotice() << "ID: " << arg;
            
            mSharedMem.setup(SHARED_DATA_KEY, sizeof(SharedData), false);
            bConnected = mSharedMem.connect();
            mSharedData = 0;
            
            LEVEL = 0;
            for (int i = 0; i < NUM_TOGGLE; ++i) TOGGLE[i] = false;
            
            mVfx = new itg::ofxStateMachine<>();
            mVfx->addState(new GeometWave);
            mVfx->addState(new BoxPerticle);
            mVfx->addState(new Orientation);
            mVfx->addState(new RotationCube);
            mVfx->addState(new Earth);
            
            mVfxNames.push_back("GeometWave");
            mVfxNames.push_back("BoxPerticle");
            mVfxNames.push_back("Orientation");
            mVfxNames.push_back("RotationCube");
            mVfxNames.push_back("Earth");
            
            mVfx->changeState(mVfxNames[0]);
            
            mCurrentWinRect.set(ofGetWindowRect());
            
        }
        else {
            ofLogError() << "missing args" << endl;
            OF_EXIT_APP(-1);
        }
    }
    
    void setup()
    {
        ofSetFrameRate(60);
        ofSetVerticalSync(true);
        ofSetCircleResolution(16);
    }
    
    void update()
    {
        if (!bConnected)
        {
            if (ofGetFrameNum() % 300 == 0) bConnected = mSharedMem.connect();
        }
        else {
            mSharedData = mSharedMem.getData();
            
            if (mSharedData->kill) exit();
            setLevel(mSharedData->level);
            memcpy(WAVE, mSharedData->wave, sizeof(mSharedData->wave));
            memcpy(TOGGLE, mSharedData->toggles[mID], sizeof(mSharedData->toggles[mID]));
            
            if (mSharedData->bang_switch != common::bang_switch)
            {
                sendBang();
                common::bang_switch = mSharedData->bang_switch;
            }
            
            int mode = mSharedData->mode[mID];
            if (mode < mVfxNames.size())
            {
                mVfx->changeState(mVfxNames[mode]);
            }
            
            // window move
            updateWindowPosition();
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
        
//        ofSetColor(255);
//        stringstream s;
//        s << "DISPLAY_INFO[ " << common::dispRect.x << ":" << common::dispRect.y
//          << " | " << common::dispRect.getWidth() << ":" << common::dispRect.getHeight() << " ]" << endl;
//        ofDrawBitmapString(s.str(), 20, 20);
    }
    
    void exit()
    {
        OF_EXIT_APP(0);
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
    
    void updateWindowPosition()
    {
        if (mID != 0)
        {
        int x = mSharedData->rect[mID].x;
        int y = mSharedData->rect[mID].y;
        int w = mSharedData->rect[mID].w;
        int h = mSharedData->rect[mID].h;
//        if (mCurrentWinRect != ofRectangle(x, y, w, h))
//        {
            ofSetWindowPosition(x, y);
            ofSetWindowShape(w, h);
            mCurrentWinRect.set(x, y, w, h);
//        }
        }
    }
    
    void resetWindowPosition()
    {
        if (mID != 0) {
            ofSetWindowPosition(80, 80);
            ofSetWindowShape(320, 240);
            mCurrentWinRect.set(ofGetWindowRect());
        }
    }
    
    void keyPressed(int key)
    {
        switch (key) {
            case 'f': ofToggleFullscreen(); break;
            case 'r': resetWindowPosition(); break;
            case 'b': sendBang(); break;
        }
    }
    
};

int main(int argc, char** argv)
{
	ofSetupOpenGL(DEFAULT_WINDOW_W, DEFAULT_WINDOW_H, OF_WINDOW);
	ofRunApp(new ofApp(argc, argv));
}
