#pragma once
#include "ofxState.h"

#define DEFAULT_WINDOW_W 320
#define DEFAULT_WINDOW_H 240

#define WIDTH   ofGetWidth()
#define HEIGHT  ofGetHeight()
#define WAVE    common::wave
#define LEVEL   common::level
#define TOGGLE  common::toggle
#define BANG_KEY 'b'

#define GET_NAME \
string getName() \
{\
const type_info& id = typeid(*this);\
int stat;\
char *name = abi::__cxa_demangle(id.name(), 0, 0, &stat);\
if (name != NULL && stat == 0) {\
string myName(name);\
return myName;\
}\
return "";\
}


//-----------------------------------------------------------------------------------------------
// Shared Data
//-----------------------------------------------------------------------------------------------

namespace common
{
    static float level;
    static vector<float> wave;
    static bitset<5> toggle;
}



class BaseState : public itg::ofxState<>
{
public:
    virtual void bang() = 0;
};


//-----------------------------------------------------------------------------------------------
// Object
//-----------------------------------------------------------------------------------------------

class PrimitiveObject
{
public:
    
    ofPoint pos;
    ofVec3f size;
    ofVec3f rotate;
    ofColor color;
    int drawMode;
    bool bFill;
    float audioVal;
    int rotAxisMode;
    
    PrimitiveObject():bFill(false),rotAxisMode(1){
        color.set(255,255,255,255);
        size.set(10,10);
        drawMode = 0;
        pos.set(WIDTH * 0.5 ,HEIGHT * 0.5);
    }
    void update(){
        float max = WIDTH * 0.2;
        size.set(audioVal * max, audioVal * max);
    }
    
    void draw(){
        ofPushMatrix();
        ofPushStyle();
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofSetSphereResolution(2);
        if(bFill)ofFill();
        else ofNoFill();
        
        ofSetColor(color.r,color.g,color.b,color.a);
        
        if(rotAxisMode == 1)ofTranslate(pos.x,pos.y,pos.z);
        else if(rotAxisMode == 0)ofTranslate(WIDTH * 0.5, HEIGHT * 0.5);
        
        ofRotateX(rotate.x);
        ofRotateY(rotate.y);
        ofRotateZ(rotate.z);
        
        if(rotAxisMode == 0){
            ofTranslate(-WIDTH * 0.5, -HEIGHT * 0.5);
            ofTranslate(pos.x,pos.y,pos.z);
        }
        if(drawMode == 0) ofDrawBox(0,0,0,size.x,size.y,size.z);
        else ofDrawSphere(0,0,0,size.x * 0.5);
        
        ofPopStyle();
        ofPopMatrix();
        
    }
};


class RotationArc
{
    int length;
    float deg;
    
public:
    ofVec2f pos;
    ofVec2f pts;
    float size;
    float rot;
    
    RotationArc(int x, int y, int _length, float _size, float rotation):
    pos(ofVec2f(x, y)),
    size(_size),
    rot(rotation)
    {
        setLength(_length);
        deg = 0;
    }
    
    void update()
    {
        deg += rot;
    }
    
    void draw()
    {
        ofPushMatrix();
        ofTranslate(pos);
        ofRotate(deg, 0, 0, 1);
        
        ofBeginShape();
        for (unsigned int i = 0; i < length; i++) {
            float c = ((float)i / 360) * TWO_PI;
            float x = sin(c) * size;
            float y = cos(c) * size;
            ofVertex(x, y);
        }
        ofEndShape(false);
        
        ofPopMatrix();
    }
    
    void setLength(int v)
    {
        length = ofClamp(v, 0, 360);
    }
};




//-----------------------------------------------------------------------------------------------
// Animation Unit
//-----------------------------------------------------------------------------------------------

class BaseAnimationUnit
{
protected:
    typedef BaseAnimationUnit base;
    bool bDied;
    
    double getLastFrameTime() { return ofGetLastFrameTime() * 1000; }
    
public:
    BaseAnimationUnit():bDied(false){}
    
    virtual void update() = 0;
    virtual void draw() = 0;
    
    bool isDied() { return bDied; }
};

typedef list<shared_ptr<BaseAnimationUnit> >::iterator anm_it;




class Perticle : public BaseAnimationUnit
{
    const ofVec2f mPos;
    const float mLife;
    float mVecX;
    float mVecY;
    float mX;
    float mY;
    const float mAccel;
    float mDecey;
    float mSize;
    
public:
    Perticle(float x, float y, float vecX, float vecY, float size, float accel, float life):
    mPos(ofVec2f(x, y)),
    mLife(life),
    mVecX(vecX),
    mVecY(vecY),
    mSize(size),
    mAccel(accel)
    {
        mX = mPos.x;
        mY = mPos.y;
        mDecey = mLife;
    }
    
    void update()
    {
        mX += mVecX;
        mY += mVecY;
        mVecX *= mAccel;
        mVecY *= mAccel;
        mDecey -= base::getLastFrameTime();
        if (mDecey < 0) bDied = true;
    }
    
    void draw()
    {
        if (mDecey > mLife * 0.3) {
            ofCircle(mX, mY, mSize);
        } else {
            // blink
            float rand = ofRandomuf();
            if (rand > 0.3) {
                ofCircle(mX, mY, mSize);
            }
        }
    }
};




class slide : public ofPoint{
private:
    
    ofPoint p;
    
    float speed;
    
public:
    
    slide():speed(0.1){
    }
    
    void set(float _px,float _py,float _pz){
        p.x = _px;
        p.y = _py;
        p.z = _pz;
    }
    
    void set(float _px,float _py){
        p.x = _px;
        p.y = _py;
    }
    
    void imSet(float _px,float _py,float _pz){
        p.x = _px;
        p.y = _py;
        p.z = _pz;
        
        x = _px;
        y = _py;
        z = _pz;
    }
    
    void imSet(float _px,float _py){
        p.x = _px;
        p.y = _py;
        
        x = _px;
        y = _py;
        
    }
    
    void update(){
        x += (p.x - x) * speed;
        y += (p.y - y) * speed;
        z += (p.z - z) * speed;
    }
};




