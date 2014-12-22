#include "ofMain.h"
#include "../../SharedData.h"
#include "common.h"
#include "ofxStateMachine.h"
#include "ofxOsc.h"
#include "ofxSharedMemory.h"

#include "GeometWave.hpp"

class ofApp : public ofBaseApp
{
    ofxOscReceiver mReceiver;
    itg::ofxStateMachine<> * mVfx;
    
    
    float getAsFloat(ofxOscMessage & m, const int index)
    {
        switch(m.getArgType(index))
        {
            case OFXOSC_TYPE_FLOAT: return m.getArgAsFloat(index);
            case OFXOSC_TYPE_INT32: return (float)m.getArgAsInt32(index);
            default: return 0;
        }
    }
    
public:
    ofApp(int argc, char** argv)
    {
        if (argc != 3)
        {
            string arg(argv[1]);
            ofLogNotice() << "listen port: " << arg;
            mReceiver.setup(ofToInt(arg));
            
            LEVEL = 0;
            WAVE.resize(WAVE_SIZE, 0);
            for (int i = 0; i < TOGGLE.size(); ++i) TOGGLE[i] = false;
            
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
        if (mReceiver.hasWaitingMessages())
        {
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            if (m.getAddress() == "/level") setLevel(getAsFloat(m, 0));
            if (m.getAddress() == "/bang") sendBang();
            if (m.getAddress() == "/exit") exit();
        }
        
        // dummy wave
        for (int i = 0; i < WAVE.size(); ++i)
        {
            WAVE[i] = ofRandomf();
        }
        
        mVfx->update();
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
