#pragma once

#include "common.h"
#include "ofxState.h"

class RotationSphere : public BaseState
{
    typedef struct {
        string name;
        float latitude;
        float longitude;
    } City;

    vector<City> cities;
    vector<shared_ptr<RotationArc> > mArcs;

    int mode;
    
    ofParameter<int> mResolution;
    ofParameter<ofColor> mColor;
    ofParameter<bool> bRandomCol;
    
    
    
public:
    RotationSphere()
    {
        base::mParamGroup.setName(getName());
        base::setParameter(mResolution.set("resolution", 10, 5, 20));
        base::setParameter(mColor.set("color", ofColor(255,255,255,255),ofColor(0,0,0,0),ofColor(255,255,255,255)));
        base::setParameter(bRandomCol.set("random_color", false));

        
        // create little objects for each city.
        // A Lat/Lon like this:
        // Lewiston, Idaho 	W 46 24' N 117 2'
        // This translate to angles and degrees (1 degree = 1/60. of an angle)
        // West and South are negative values
        
        // here is a list of big cities and their positions
        // http://www.infoplease.com/ipa/A0001796.html
        
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
    }
    
    void setup()
    {
        mode = 0;
        
        if (mArcs.size()) mArcs.clear();
        for (int i = 0; i < cities.size(); i++) {
            mArcs.push_back(shared_ptr<RotationArc>(new RotationArc(300, 600, ofRandom(30, 180), 0, ofRandom(5, 20))));
        }
    }
    
    void update()
    {
        for (vector<shared_ptr<RotationArc> >::iterator it = mArcs.begin(); it != mArcs.end(); it++) {
            (*it)->update();
        }
    }
    
    void draw()
    {
        
        
        ofPushMatrix();
        ofPushStyle();
        ofNoFill();
        ofSetSphereResolution(mResolution);
        ofSetRectMode(OF_RECTMODE_CENTER);
        
        //translate so that the center of the screen is 0,0
        ofTranslate(WIDTH * 0.5, HEIGHT * 0.5);
        ofSetColor(mColor, 20 + (LEVEL * 235));
        ofSetLineWidth(1);
        
        
        ofRotate(ofGetFrameNum(), 0, 1, 0);
        float size = HEIGHT;
        ofDrawSphere(0, 0, 0, LEVEL * size);
        
        ofRotate(ofGetFrameNum(), 0, 2, 0);
        size = HEIGHT * 0.5;
        ofDrawSphere(0, 0, 0, size);
        
        
        for(unsigned int i = 0; i < cities.size(); i++){
            ofSetColor(199);
            ofQuaternion latRot, longRot, spinQuat;
            latRot.makeRotate(cities[i].latitude, 1, 0, 0);
            longRot.makeRotate(cities[i].longitude, 0, 1, 0);
            spinQuat.makeRotate(ofGetFrameNum(), 0, 1, 0);
            
            ofVec3f center = ofVec3f(0,0,HEIGHT * 0.5);
            ofVec3f worldPoint = latRot * longRot * spinQuat * center;
            
            ofLine(ofVec3f(0,0,0), worldPoint);
            drawPoint(worldPoint.x, worldPoint.y, worldPoint.z, (abs(sin(share::elapsedTime))), i);
            
            ofSetColor(220, 200, 210);
            ofDrawBitmapString(cities[i].name, worldPoint );
        }
        
        ofPopStyle();
        ofPopMatrix();
    }
    
    void getBang()
    {
        for (vector<City>::iterator it = cities.begin(); it != cities.end(); it++) {
            it->latitude = ofRandom(-90, 90) + ofRandom(-90, 90) / 60;
            it->longitude = ofRandom(-90, 90) + ofRandom(-90, 90) / 60;
            it->name = ofToString(it->latitude) + " " + ofToString(it->longitude);
        }
        
        mResolution = ofRandom(5, 20);
        if (bRandomCol) {
            ofColor col;
            col.setHsb(ofRandom(255), 200, 255);
            mColor = col;
        }
        
        mode = (int)ofRandom(4);
    }
    
    void drawPoint(int x, int y, int z, float size, int i)
    {
        if (bRandomCol) {
            ofColor col;
            col.setHsb(ofRandom(255), 255, 255);
            ofSetColor(col);
        }
        switch (mode) {
            default:
            case 0:
                ofFill();
                ofCircle(x, y, z, size * 5);
                ofNoFill();
                ofCircle(x, y, z, size * 10);
                ofCircle(x, y, z, size * 15);
                break;
                
            case 1:
                ofFill();
                ofRect(x, y, z, size * 3, size * 3);
                ofNoFill();
                ofRect(x, y, z, size * 5, size * 5);
                ofRect(x, y, z, size * 10, size * 30);
                break;
                
            case 2:
                ofNoFill();
                ofSetColor(255, 0, 0);
                ofLine(x-10, y, z,  x+10, y, z);
                ofLine(x, y-10, z, x, y+10 ,z);
                break;
            
            case 3:
                ofNoFill();
                ofSetLineWidth(2);
                mArcs[i]->pos.set(x, y);
                mArcs[i]->size = size * 20;
                mArcs[i]->draw();
                break;
                
        }
    }

};