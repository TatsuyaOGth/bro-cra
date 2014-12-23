#pragma once

#include "BaseContentsInterface.h"

class KzdPatternExample : public BaseContentsInterface
{
    vector<PrimitiveObject>prims;
    
    static const int objectNum = 100; //何個描画するか。
    
//    bool rotAxisMode;
//    bool bReverse;
    
    
    ofParameter<ofVec3f> pos;
    ofParameter<ofVec3f> size;
    ofParameter<ofVec3f> rotate;
    ofParameter<ofColor> color;
    ofParameter<bool> bGlobalRotate;
    ofParameter<bool> bFillTog;
    ofParameter<bool> bBackgroundChange;

    ofParameter<bool> bReverseTog;
    ofParameter<bool> isRand;
    
    slide globalRotate;
    
public:
    KzdPatternExample()
    {
        for(int i = 0; i < objectNum; i++){
            PrimitiveObject drawObject;
            drawObject.drawMode = i % 2;
            drawObject.rotAxisMode = 0;
            prims.push_back(drawObject);
        }
        
        base::mParamGroup.setName(base::getName());
        base::mParamGroup.add(pos.set("pos",ofVec3f(0,0,0),ofVec3f(0,0,0),ofVec3f(1200,1200,1200)));
        base::mParamGroup.add(size.set("scale",ofVec3f(50,60,80),ofVec3f(0,0,0),ofVec3f(1000,1000,1000)));
        base::mParamGroup.add(rotate.set("rotate",ofVec3f(0,0,100),ofVec3f(0,0,0),ofVec3f(360.0,360.0,360.0)));
        base::mParamGroup.add(bGlobalRotate.set("bGlobalRotate",false));
        base::mParamGroup.add(color.set("color",ofColor(255,255,255,255),ofColor(0,0,0,0),ofColor(255,255,255,255)));
        base::mParamGroup.add(bFillTog.set("bFIll",false));
        base::mParamGroup.add(bReverseTog.set("bReverseTog",false));
        base::mParamGroup.add(isRand.set("isRand",true));
        
    }
    
    void setup()
    {
    }
    
    void update()
    {
        
        if(bGlobalRotate == false){
            globalRotate.set(0,0,0);
        }
        
        rotate.set(ofVec3f(rotate->x,rotate->y,rotate->z));
        globalRotate.update();
        
        
        //ここからはランダマイズされた値をインスタンスに突っ込んだりするよ！
        int aVal = 255;
//        if(bFill.y == 1)aVal = 30; //ofFillかましてると明る過ぎるので適当にアルファ下げるよ
        if (bFillTog) aVal = 30;
        
        color.set(ofColor(color->r,color->g,color->b,(int)(LEVEL * aVal)));
        
        ofColor currentColor = color;
        if(bReverseTog)currentColor.set(0,0,0,(int)(LEVEL * aVal));
        
        float cVol = LEVEL * 0.06;
        
        for(int i = 0; i <prims.size(); i++){
            prims[i].audioVal = LEVEL;
            
            prims[i].pos.x = (i - prims.size() * 0.5) * cVol * pos->x + ofGetWidth() * 0.5;
            prims[i].pos.y = (i - prims.size() * 0.5) * cVol * pos->y + ofGetHeight() * 0.5;
            prims[i].pos.z = (i - prims.size() * 0.5) * cVol * pos->z;
            
            prims[i].rotate.x = i * cVol * rotate->x;
            prims[i].rotate.y = i * cVol * rotate->y;
            prims[i].rotate.z = i * cVol * rotate->z;
            
            prims[i].size.x = i * cVol * size->x + 10;
            prims[i].size.y = i * cVol * size->y + 10;
            prims[i].size.z = i * cVol * size->z + 10;
            
            prims[i].color = currentColor;
            
            prims[i].bFill = bFillTog;
//            prims[i].rotAxisMode = rotAxisMode;
            prims[i].update();
        }

    }
    
    void draw()
    {
        ofPushStyle();
        if(bReverseTog == false){
            ofEnableBlendMode(OF_BLENDMODE_ADD);
        }
        else {
            ofBackground(150,150,150);
            ofBackgroundGradient(ofFloatColor(1,1,1,1), ofFloatColor(0,0,0,0));
            ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        }
        
        ofPushMatrix();
        ofTranslate(WIDTH*0.5, HEIGHT*0.5);
        ofRotateX(globalRotate.x);
        ofRotateY(globalRotate.y);
        ofRotateZ(globalRotate.z);
        ofTranslate(-WIDTH*0.5, -HEIGHT*0.5);
        
        for(int i = 0; i <prims.size(); i++){
            prims[i].draw();
        }
        ofPopMatrix();
        ofPopStyle();
    }
    
    void getBang()
    {
        if(isRand) randomiseAll();
        if(isRand) bFillTog = !bFillTog;
        if(isRand) bReverseTog = !bReverseTog;
        if(bGlobalRotate){
            globalRotate.set(ofRandom(0,720),
                             ofRandom(0,720),
                             ofRandom(0,720));
            // rotate.set(ofVec3f(ofRandom(0,100),ofRandom(0,100),ofRandom(0,100)));
        }

    }
    
    void randomiseAll(){
        
        int rand = (int)ofRandom(0,5);
        if(rand == 0)pos.set(ofVec3f(1200,0,0));
        else if(rand == 1)pos.set(ofVec3f(0,1200,0));
        else if(rand == 2)pos.set(ofVec3f(0,0,1200));
        else if(rand == 3)pos.set(ofVec3f(ofRandom(50,1200),ofRandom(50,1200),ofRandom(50,1200)));
        else if(rand == 4)pos.set(ofVec3f(0,0,0));
        
        rand = (int)ofRandom(0,5);
        if(rand == 0)rotate.set(ofVec3f(100,0,0));
        else if(rand == 1)rotate.set(ofVec3f(0,360,0));
        else if(rand == 2)rotate.set(ofVec3f(0,0,360));
        else if(rand == 3)rotate.set(ofVec3f(ofRandom(0,360),ofRandom(0,360),ofRandom(0,360)));
        else if(rand == 4)rotate.set(ofVec3f(0,0,0));
        
        rand = (int)ofRandom(0,4);
        if(rand == 0)size.set(ofVec3f(ofRandom(100,300),0,0));
        else if(rand == 1)size.set(ofVec3f(0,ofRandom(100,300),0));
        else if(rand == 2)size.set(ofVec3f(0,0,ofRandom(100,300)));
        else if(rand == 3)size.set(ofVec3f(ofRandom(0,300),ofRandom(0,1000),ofRandom(0,1000)));
        
    }
    
};