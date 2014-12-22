#include "ofMain.h"

#include "../../SharedData.h"
#include "ofxSharedMemory.h"
#include "ofxUI.h"


class ofApp : public ofBaseApp
{
    ofxSharedMemory<SharedDataStruct *> mSharedMem;
    bool bConnected;
    
    ofxUISuperCanvas * gui;
    
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
        SharedData.level = 0;
        for (int i = 0; i < WAVE_SIZE; ++i) SharedData.wave[i] = 0;
        SharedData.mode = 0;
        mSharedMem.setup(SHARED_DATA_KEY, sizeof(SharedData), true);
        bConnected = mSharedMem.connect();
        
        //----------
        // setup sound stream
        //----------
        soundStream.listDevices();
        mSmoothedVol = 0;
        mSmoothBalance = 0.93;
        mGain = 1.0;
        mBangTh = 0.6;
        soundStream.setup(this, 0, 2, 44100, WAVE_SIZE, 4);
        
        //----------
        // setup gui
        //----------
        gui = new ofxUISuperCanvas("PARAMETERS");
        gui->setColorBack(ofColor(0));
        gui->addFPSSlider("FPS");
        gui->addSlider("LEVEL(SMOOTHED)", 0, 1, &mSmoothedVol);
        gui->addSlider("BANG_THRESHOLD", 0, 1, &mBangTh);
        gui->addButton("BANG", &mBang);
        gui->addSlider("SMOOTH_BALANCE", 0, 1, &mSmoothBalance);
        gui->addSlider("GAIN", 0, 50, &mGain);
        gui->addWaveform("CH.01(L)", mWaveL, WAVE_SIZE, -1, 1);
        gui->addWaveform("CH.02(R)", mWaveR, WAVE_SIZE, -1, 1);
        gui->autoSizeToFitWidgets();
        gui->loadSettings("parameters.xml");
        
    }
    
    void update()
    {
        mSmoothedVol = ofClamp(mSmoothedVol, 0, 1);
        
        // if not connected, try reconnect every 5 seconds or so
        if (!bConnected)
        {
            if (ofGetFrameNum() % 300 == 0) bConnected = mSharedMem.connect();
        }
        
        // bang
        if (mSmoothedVol > mBangTh)
        {
            mBang = true;
        }
        
        // update data
        
    }
    
    void draw()
    {
        ofBackground(0, 0, 0);
        
    }
    
    void sendBang()
    {
        
    }
    
    void exit()
    {
        mSharedMem.close();
        gui->saveSettings("parameters.xml");
        delete gui;
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
