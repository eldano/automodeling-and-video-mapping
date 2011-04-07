#ifndef SCENE_H
#define SCENE_H

#include "ofxCamera.h"
#include "ofxLight.h"

#include <string>
#include <map>
using namespace std;

class Scene
{
    public:
        Scene();
        virtual ~Scene();

        void draw();
        void setBackground(int r, int g, int b);
        ofxCamera* addCamera(string id);
        ofxCamera* getCamera(string id);
        ofxCamera* activateCamera(string id);
        ofxCamera* getActiveCamera();

        ofxLight* addLight(string id);
        ofxLight* getLight(string id);

    private:
        map<string, ofxCamera*> cameras;
        ofxCamera* activeCamera;
        map<string, ofxLight*> lights;
};

#endif // SCENE_H
