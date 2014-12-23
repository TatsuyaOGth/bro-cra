#pragma once

#include "common.h"
#include "ofxState.h"

typedef struct {
	string name;
	float latitude;
	float longitude;
} City;

class Earth : public BaseState
{
    vector<City> cities;
    ofColor mCol;
    
public:
    Earth()
    {
        City newyork = { "hongo a", 40+47/60., -73 + 58/60. };
        City tokyo = { "hongo b", 35 + 40./60, 139 + 45/60. };
        City london = { "hongo c", 51 + 32/60., -5./60. };
        City shanghai = { "hongo d", 31 + 10/60., 121 + 28/60. };
        City buenosaires = { "hongo e", -34 + 35/60., -58 + 22/60. };
        City melbourne = { "hongo f" , -37 + 47/60., 144 + 58/60. };
        City detroit = { "hongo g", 42 + 19/60., -83 + 2 / 60. };
        
        cities.push_back( newyork );
        cities.push_back( tokyo );
        cities.push_back( london );
        cities.push_back( shanghai );
        cities.push_back( buenosaires );
        cities.push_back( melbourne );
        cities.push_back( detroit );
        cities.push_back( newyork );
        cities.push_back( tokyo );
        cities.push_back( london );
        cities.push_back( shanghai );
        cities.push_back( buenosaires );
        cities.push_back( melbourne );
        cities.push_back( detroit );
        
        mCol.set(0, 0, 0);
    }
    
    GET_NAME
    
    void bang()
    {
        for (vector<City>::iterator it = cities.begin(); it != cities.end(); it++) {
            it->latitude = ofRandom(-90, 90) + ofRandom(-90, 90) / 60;
            it->longitude = ofRandom(-90, 90) + ofRandom(-90, 90) / 60;
            it->name = ofToString(it->latitude) + " " + ofToString(it->longitude);
        }
        mCol.set(ofRandom(100), ofRandom(100), ofRandom(100));
    }
    
    void update()
    {
    }
    
    
    void draw()
    {
        ofSetSphereResolution(20);
        
        if (TOGGLE[T0]) {
            ofBackground(mCol);
        } else {
            ofBackground(0);
        }
        
        ofNoFill();
        
        
        ofPushMatrix();
        
        //translate so that the center of the screen is 0,0
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        ofSetColor(255, 255, 255, 20 + (LEVEL * 235));
        ofSetLineWidth(1);
        
        ofPushMatrix();
        
        ofRotate(ofGetFrameNum(), 0, 1, 0);
        float size = 300;
        if (TOGGLE[T1]) size = LEVEL * 300;
        ofDrawSphere(0, 0, 0, size);
        
        if (TOGGLE[T2]) {
            ofRotate(ofGetFrameNum(), 0, 2, 0);
            float size = 200;
            if (TOGGLE[T1]) size = LEVEL * 200;
            ofDrawSphere(0, 0, 0, size);
        }
        
        ofPopMatrix();
        
        
        ofSetColor(255);
        for(unsigned int i = 0; i < cities.size(); i++){
            
            ofQuaternion latRot, longRot, spinQuat;
            latRot.makeRotate(cities[i].latitude, 1, 0, 0);
            longRot.makeRotate(cities[i].longitude, 0, 1, 0);
            spinQuat.makeRotate(ofGetFrameNum(), 0, 1, 0);
            
            ofVec3f center = ofVec3f(0,0,300);
            ofVec3f worldPoint = latRot * longRot * spinQuat * center;
            
            ofLine(ofVec3f(0,0,0), worldPoint);
            
            ofDrawBitmapString(cities[i].name, worldPoint );
        }
        
        
        ofPopMatrix();
    }

};
