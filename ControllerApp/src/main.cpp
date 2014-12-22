#include "ofMain.h"

#include "../../SharedData.h"
#include "ofxSharedMemory.h"
#include "ofxUI.h"

#define SOUND_DEVICE_ID 0



class ofApp : public ofBaseApp
{
    SharedData * mSharedData;
    ofxSharedMemory<SharedData *> mSharedMem;
    bool bConnected;
    
    ofxUISuperCanvas * gui1;
    
    ofSoundStream soundStream;
    float mWaveL[WAVE_SIZE];
    float mWaveR[WAVE_SIZE];
    float mSmoothedVol;
    float mSmoothBalance;
    float mGain;
    float mBangTh;
    bool mBang;
    
public:
    ofApp()
    {
        ofSetFrameRate(60);

        //----------
        // setup shared memory
        //----------
        mSharedData = new SharedData();
        mSharedData->level = 0;
        for (int i = 0; i < WAVE_SIZE; ++i) mSharedData->wave[i] = 0;
        mSharedData->mode = 0;
        mSharedData->bang_switch = false;
        for (int i = 0; i < NUM_VFXWIN; ++i)
            for (int j = 0; j < NUM_TOGGLE; ++j)
                mSharedData->toggles[i][j] = false;
        
        mSharedMem.setup(SHARED_DATA_KEY, sizeof(SharedData), true);
        bConnected = mSharedMem.connect();
        
        //----------
        // setup sound stream
        //----------
        soundStream.listDevices();
        soundStream.setDeviceID(SOUND_DEVICE_ID);
        mSmoothedVol = 0;
        mSmoothBalance = 0.93;
        mGain = 1.0;
        mBangTh = 0.6;
        mBang = false;
        soundStream.setup(this, 0, 2, 44100, WAVE_SIZE, 4);
        
        //----------
        // setup gui
        //----------
        gui1 = new ofxUISuperCanvas("PARAMETERS");
        gui1->addFPSSlider("FPS");
        gui1->addSlider("LEVEL(SMOOTHED)", 0, 1, &mSmoothedVol);
        gui1->addSlider("BANG_THRESHOLD", 0, 1, &mBangTh);
        gui1->addButton("BANG", &mBang);
        gui1->addSlider("SMOOTH_BALANCE", 0, 1, &mSmoothBalance);
        gui1->addSlider("GAIN", 0, 50, &mGain);
        gui1->addWaveform("CH.01(L)", mWaveL, WAVE_SIZE, -1, 1);
        gui1->addWaveform("CH.02(R)", mWaveR, WAVE_SIZE, -1, 1);
        gui1->addLabel("TOGGLES");
        gui1->addToggleMatrix("TOGGLE_MATRIX", NUM_VFXWIN, NUM_TOGGLE);
        gui1->autoSizeToFitWidgets();
        gui1->loadSettings("gui1.xml");
        
        ofAddListener(gui1->newGUIEvent, this, &ofApp::guiEvent);
    }
    
    void update()
    {
        mSmoothedVol = ofClamp(mSmoothedVol, 0, 1);
        
        // if not connected, try reconnect every 5 seconds or so
        if (!bConnected)
        {
            if (ofGetFrameNum() % 300 == 0) bConnected = mSharedMem.connect();
        }
        else {
            //----------
            // update data
            //----------
            mSharedData->level = mSmoothedVol;
            memcpy(mSharedData->wave, mWaveL, sizeof(mWaveL));
            mSharedData->mode = 0;

            if (mSmoothedVol > mBangTh)
            {
                if (!mBang)
                {
                    sendBang();
                    mBang = true;
                }
            }
            else mBang = false;
            mSharedMem.setData(mSharedData);
        }
    }
    
    void draw()
    {
        ofBackground(80);
        
    }
    
    void sendBang()
    {
        mSharedData->bang_switch != mSharedData->bang_switch;
    }
    
    void exit()
    {
        mSharedMem.close();
        gui1->saveSettings("gui1.xml");
        delete mSharedData;
        delete gui1;
    }
    
    void guiEvent(ofxUIEventArgs &e)
    {
        string name = e.widget->getName();
        int kind = e.widget->getKind();
        if (kind == 2)
        {
            ofxUIToggle * w = (ofxUIToggle *) e.widget;
            int vid = ofToInt(name.substr(name.size() - 2, 1));
            int tgl = ofToInt(name.substr(name.size() - 4, 1));
            mSharedData->toggles[vid][tgl] = w->getValue();
        }
    }
    
    void audioIn(float * input, int bufferSize, int nChannels)
    {
        float curVol = 0.0;
        int numCounted = 0;
        for (int i = 0; i < bufferSize; i++)
        {
            mWaveL[i]	= input[i*2]    * 0.5 * mGain;
            mWaveR[i]	= input[i*2+1]  * 0.5 * mGain;
            curVol += mWaveL[i] * mWaveL[i];
            curVol += mWaveR[i] * mWaveR[i];
            numCounted += 2;
        }
        
        curVol /= (float)numCounted;
        curVol = sqrt( curVol );
        
        mSmoothedVol *= mSmoothBalance;
        mSmoothedVol += (1.0 - mSmoothBalance) * curVol;
    }
};



//========================================================================
int main( )
{
	ofSetupOpenGL(1024,768,OF_WINDOW);
	ofRunApp(new ofApp());
}
