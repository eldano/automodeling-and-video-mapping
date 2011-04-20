#include "OscUtil.h"

OscUtil::OscUtil()
{
    //ctor
}

OscUtil::~OscUtil()
{
    //dtor
}

ofxOscMessage OscUtil::createSetBackgroundMsg(int r, int g, int b){
    ofxOscMessage oscMessage;
    oscMessage.setAddress("/scene/setbackground");
    oscMessage.addIntArg(r);
    oscMessage.addIntArg(g);
    oscMessage.addIntArg(b);
    return oscMessage;
}

void OscUtil::processSetBackgroundMsg(ofxOscMessage msg, ISceneHandler *sceneHandler){
    int r = msg.getArgAsInt32(0);
    int g = msg.getArgAsInt32(1);
    int b = msg.getArgAsInt32(2);
    sceneHandler->setBackground(r,g,b);
}

ofxOscMessage OscUtil::createAddCameraMsg(string id)
{
    ofxOscMessage oscMessage;
    oscMessage.setAddress("/camera/add");
    oscMessage.addStringArg(id);
    return oscMessage;
}

void OscUtil::processAddCameraMsg(ofxOscMessage msg, ISceneHandler *sceneHandler){
    string id = msg.getArgAsString(0);
    sceneHandler->addCamera(id);
}

ofxOscMessage OscUtil::createSetCameraPosMsg(string id, ofxVec3f position)
{
    ofxOscMessage oscMessage;
    oscMessage.setAddress("/camera/setpos");
    oscMessage.addStringArg(id);
    oscMessage.addFloatArg(position[0]);
    oscMessage.addFloatArg(position[1]);
    oscMessage.addFloatArg(position[2]);
    return oscMessage;
}

void OscUtil::processSetCameraPosMsg(ofxOscMessage msg, ISceneHandler *sceneHandler){
    string id = msg.getArgAsString(0);
    float x = msg.getArgAsFloat(1);
    float y = msg.getArgAsFloat(1);
    float z = msg.getArgAsFloat(1);
    sceneHandler->setCameraPos(id, x, y, z);
}

ofxOscMessage OscUtil::createSetCameraEyeMsg(string id, ofxVec3f eye)
{
    ofxOscMessage oscMessage;
    oscMessage.setAddress("/camera/seteye");
    oscMessage.addStringArg(id);
    oscMessage.addFloatArg(eye[0]);
    oscMessage.addFloatArg(eye[1]);
    oscMessage.addFloatArg(eye[2]);
    return oscMessage;
}

void OscUtil::processSetCameraEyeMsg(ofxOscMessage msg, ISceneHandler *sceneHandler){
    string id = msg.getArgAsString(0);
    float x = msg.getArgAsFloat(1);
    float y = msg.getArgAsFloat(1);
    float z = msg.getArgAsFloat(1);
    sceneHandler->setCameraEye(id, x, y, z);
}

ofxOscMessage OscUtil::createActivateCameraMsg(string id){
    ofxOscMessage oscMessage;
    oscMessage.setAddress("/camera/activate");
    oscMessage.addStringArg(id);
    return oscMessage;
}

void OscUtil::processActivateCameraMsg(ofxOscMessage msg, ISceneHandler *sceneHandler){
    string camId = msg.getArgAsString(0);
    sceneHandler->activateCamera(camId);
}

ofxOscMessage OscUtil::createAddQuadMsg(string camId, string layerId, string quadId) {
    ofxOscMessage oscMessage;
    oscMessage.setAddress("/quad/add");
    oscMessage.addStringArg(camId);
    oscMessage.addStringArg(layerId);
    oscMessage.addStringArg(quadId);
    return oscMessage;
}

void OscUtil::processAddQuadMsg(ofxOscMessage msg, ISceneHandler *sceneHandler) {
    string camId = msg.getArgAsString(0);
    string layerId = msg.getArgAsString(1);
    string quadId = msg.getArgAsString(2);
    sceneHandler->addQuad(camId, layerId, quadId);
}

ofxOscMessage OscUtil::createSetQuadPointMsg(string camId, string layerId, string quadId,
                                           int point, float coordX, float coordY){
    ofxOscMessage oscMessage;
    oscMessage.setAddress("/quad/setpoint");
    oscMessage.addStringArg(camId);
    oscMessage.addStringArg(layerId);
    oscMessage.addStringArg(quadId);
    oscMessage.addIntArg(point);
    oscMessage.addFloatArg(coordX);
    oscMessage.addFloatArg(coordY);
    return oscMessage;
}

void OscUtil::processSetQuadPointMsg(ofxOscMessage msg, ISceneHandler *sceneHandler){
    string camId = msg.getArgAsString(0);
    string layerId = msg.getArgAsString(1);
    string quadId = msg.getArgAsString(2);
    int point = msg.getArgAsInt32(3);
    float coordX = msg.getArgAsFloat(4);
    float coordY = msg.getArgAsFloat(5);
    sceneHandler->setQuadPoint(camId, layerId, quadId, point, coordX, coordY);
}

ofxOscMessage OscUtil::createEnableQuadMsg(string camId, string layerId, string quadId, bool enabled){
    ofxOscMessage oscMessage;
    oscMessage.setAddress("/quad/enable");
    oscMessage.addStringArg(camId);
    oscMessage.addStringArg(layerId);
    oscMessage.addStringArg(quadId);
    oscMessage.addIntArg(enabled);
    return oscMessage;
}

void OscUtil::processEnableQuadMsg(ofxOscMessage msg, ISceneHandler *sceneHandler){
    string camId = msg.getArgAsString(0);
    string layerId = msg.getArgAsString(1);
    string quadId = msg.getArgAsString(2);
    bool enabled = msg.getArgAsInt32(3);
    sceneHandler->enableQuad(camId, layerId, quadId, enabled);
}

ofxOscMessage OscUtil::createAddLayerMsg(string camId, string layerId) {
    ofxOscMessage oscMessage;
    oscMessage.setAddress("/layer/add");
    oscMessage.addStringArg(camId);
    oscMessage.addStringArg(layerId);
    return oscMessage;
}

void OscUtil::processAddLayerMsg(ofxOscMessage msg, ISceneHandler *sceneHandler) {
    string camId = msg.getArgAsString(0);
    string layerId = msg.getArgAsString(1);
    sceneHandler->addLayer(camId, layerId);
}

ofxOscMessage OscUtil::createAddGroupMsg(string groupId){
    ofxOscMessage oscMessage;
    oscMessage.setAddress("/group/add");
    oscMessage.addStringArg(groupId);
    return oscMessage;
}

void OscUtil::processAddGroupMsg(ofxOscMessage msg, ISceneHandler *sceneHandler){
    string groupId = msg.getArgAsString(0);
    sceneHandler->addGroup(groupId);
}

ofxOscMessage OscUtil::createAddQuadToGroupMsg(string groupId, string camId, string layerId, string quadId){
    ofxOscMessage oscMessage;
    oscMessage.setAddress("/group/addquad");
    oscMessage.addStringArg(groupId);
    oscMessage.addStringArg(camId);
    oscMessage.addStringArg(layerId);
    oscMessage.addStringArg(quadId);
    return oscMessage;
}

void OscUtil::processAddQuadToGroupMsg(ofxOscMessage msg, ISceneHandler *sceneHandler){
    string groupId = msg.getArgAsString(0);
    string camId = msg.getArgAsString(0);
    string layerId = msg.getArgAsString(0);
    string quadId = msg.getArgAsString(0);
    sceneHandler->addQuadToGroup(groupId, camId, layerId, quadId);
}

int OscUtil::processMessageAction(ofxOscMessage m, ISceneHandler *sceneHandler) {
    if ( m.getAddress() == "/camera/add" ) {
        OscUtil::processAddCameraMsg(m, sceneHandler);
    } else if ( m.getAddress() == "/camera/setpos" ) {
        OscUtil::processSetCameraPosMsg(m, sceneHandler);
    } else if ( m.getAddress() == "/camera/seteye" ) {
        OscUtil::processSetCameraEyeMsg(m, sceneHandler);
    } else if ( m.getAddress() == "/quad/add") {
        OscUtil::processAddQuadMsg(m, sceneHandler);
    } else if ( m.getAddress() == "/quad/setpoint" ) {
        OscUtil::processSetQuadPointMsg(m, sceneHandler);
    } else if ( m.getAddress() == "/layer/add" ) {
        OscUtil::processAddLayerMsg(m, sceneHandler);
    } else if ( m.getAddress() == "/camera/activate" ) {
        OscUtil::processActivateCameraMsg(m, sceneHandler);
    } else if ( m.getAddress() == "/quad/enable" ) {
        OscUtil::processEnableQuadMsg(m, sceneHandler);
    } else if ( m.getAddress() == "/scene/setbackground" ) {
        OscUtil::processSetBackgroundMsg(m, sceneHandler);
    } else if ( m.getAddress() == "/group/add" ) {
        OscUtil::processAddGroupMsg(m, sceneHandler);
    } else if ( m.getAddress() == "/group/addquad" ) {
        OscUtil::processAddQuadToGroupMsg(m, sceneHandler);
    } else {
        return -1;
    }
    return 1;
}