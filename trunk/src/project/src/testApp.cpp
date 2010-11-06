#include "testApp.h"
#include "AnimationController.h"
#include "Animation.h"
#include "LinearAnimation.h"
#include "Background.h"
#include "TimeManager.h"
#include "DrawEventArg.h"
#include "TimedEventArg.h"
#include "ofxXmlSettings.h"
#include <map>

ofxXmlSettings quadsXML;
ofxXmlSettings showConfig;

map<string, Quad2D*> quads;
map<string, Quad2D*>::iterator quadsIt;

map<string, Material*> materials;

string selectedQuadKey;
int selectedIdx = -1;
int selectedVtx = 0;
float xoffset = 5;
float yoffset = 5;

map<string, int> OscPorts;

map<string, string > MidiPorts;

map<string, Node > Network;


//--------------------------------------------------------------

testApp::testApp(string nodeName) : ofBaseApp() {
    this->nodeName = nodeName;
}

void testApp::setup(){

    setupLogging();

    //set background to black
	ofBackground(0, 0, 0);

    //for smooth animation, set vertical sync if we can
	ofSetVerticalSync(true);
	// also, frame rate:
	ofSetFrameRate(60);

    //load showconfig from xml
	loadShow();

    #ifdef CONSOLE
        ofSetWindowTitle("Console APP");
    #else
        ofSetWindowTitle("Client APP");
    #endif

    #ifndef CONSOLE
        //this->oscManager = new OscManager(OscPorts, false); //set as receiver
        this->oscManager = new OscManager(nodeName, Network, OscPorts, false); //set as sender
        this->oscManager->addListener(this, "/all");
        //this->synchManager->addListener(&animController, "/anim");

        this->midiManager = new MidiManager(); //midi  receiver
        this->midiManager->addListener(this, "");
    #else
        this->oscManager = new OscManager(nodeName, Network, OscPorts); //set as sender
    #endif

    TextureManager::LoadVideoTexture("cartoon", "cartoon.mov");
    TextureManager::LoadVideoTexture("fingers", "fingers.mov");
    TextureManager::LoadVideoTexture("cartoons", "cartoons.mov");

    /*quads.insert(pair<string, Quad2D*>("quad1", new Quad2D("quad1", 100,100, 250,80, 270,260, 80,250))) ;
    quads["quad1"]->setEnabled(true);

    quads.insert(pair<string, Quad2D*>("quad2", new Quad2D("quad2", 300,100, 450,80, 470,260, 280,250))) ;
    quads["quad2"]->setEnabled(true);*/

    /*LinearAnimation *anim5 = new LinearAnimation(materials["mat1"], AMBIENT_R, 2, 1.0);
    LinearAnimation *anim6 = new LinearAnimation(materials["mat1"], AMBIENT_R, 2, 0.0);

    AnimationLoop *loop1 = new AnimationLoop("loop1");
    loop1->AddAnimation(anim5);
    loop1->AddAnimation(anim6);

    animController.AddLoop("loop1", loop1);*/

    TimeManager::AddTimedEvent(0.5f, this, "SetVideo", "quad1", "cartoon");
    TimeManager::AddTimedEvent(0.5f, this, "SetVideo", "quad2", "fingers");

    TimeManager::AddTimedEvent(3.0f, this, "PlayVideo", "cartoon");
    TimeManager::AddTimedEvent(3.6f, this, "StopVideo", "cartoon");
    TimeManager::AddTimedEvent(5.0f, this, "PlayVideo", "fingers");

    TimeManager::AddTimedEvent(6.3f, this, "SetVideo", "quad2", "cartoons");
    TimeManager::AddTimedEvent(6.3f, this, "PlayVideo", "cartoons");

    TimeManager::Init();

    setupConsole();

}

//--------------------------------------------------------------
void testApp::update(){
    AnimationController::Update();

    #ifndef CONSOLE
        oscManager->checkForMessages();
    #endif

    TextureManager::Update();

    TimeManager::Update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetupScreen();

    for(quadsIt = quads.begin(); quadsIt != quads.end(); ++quadsIt) {
        (*quadsIt).second->draw();
    }

    console->render();

}

//--------------------------------------------------------------

void cycleQuadSelection(bool fwd) {
    if(quads.size() > 0) {
        int oldSelectedIdx = selectedIdx;
        if(fwd) {
            ++selectedIdx;
        }
        else {
            --selectedIdx;
        }
        selectedIdx %= quads.size();

        quadsIt = quads.begin();
        std::advance(quadsIt, oldSelectedIdx);
        (*quadsIt).second->setSelected(false);
        quadsIt = quads.begin();
        std::advance(quadsIt, selectedIdx);
        selectedQuadKey = (*quadsIt).first;
        (*quadsIt).second->setSelected(true);
    }
}

void testApp::keyPressed  (int key){

#ifdef CONSOLE

    if(key == 't') {
        TimeManager::Init();
    }

    if(key == 'g') {
        AnimationController::PlayLoop("loop1");
    }
    if(key == 'k') {
        loadQuads();
    }
    if(key == '1') {
        cycleQuadSelection(true);
    }
    if(key == '2') {
        cycleQuadSelection(false);
    }
    if(key == '3') {
        ++selectedVtx;
        selectedVtx %= 4;
    }
    if(key == '4') {
        ++selectedVtx;
        selectedVtx %= 4;
    }

    if(key == 'a') {
        if(!selectedQuadKey.empty()) {
            float x, y;
            quads[selectedQuadKey]->getPoint(selectedVtx, x, y);
            x -= xoffset;
            setPoint(selectedIdx, selectedVtx, x, y);
        }
    }
    if(key == 'd') {
        if(!selectedQuadKey.empty()) {
            float x, y;
            quads[selectedQuadKey]->getPoint(selectedVtx, x, y);
            x += xoffset;
            setPoint(selectedIdx, selectedVtx, x, y);
        }
    }
    if(key == 's') {
        if(!selectedQuadKey.empty()) {
            float x, y;
            quads[selectedQuadKey]->getPoint(selectedVtx, x, y);
            y += yoffset;
            setPoint(selectedIdx, selectedVtx, x, y);
        }
    }
    if(key == 'w') {
        if(!selectedQuadKey.empty()) {
            float x, y;
            quads[selectedQuadKey]->getPoint(selectedVtx, x, y);
            y -= yoffset;
            setPoint(selectedIdx, selectedVtx, x, y);
        }
    }
    /*if(key == ' ') {
        selectedIdx = addQuad(selectedIdx);
    }*/
    if(key == '\b') {
        if(quads.size() > 0 && selectedIdx >= 0) {
            quadsIt = quads.begin();
            std::advance(quadsIt, selectedIdx);
            (*quadsIt).second->setSelected(false);
            quads.erase(quadsIt);
            selectedQuadKey = "";
        }
    }
    if(key == 'l') {
        saveQuads();
        saveShow();
    }

    #endif
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    #ifdef CONSOLE
    if(selectedIdx >= 0 && selectedVtx >= 0)
    {
        //ofLog(OF_LOG_VERBOSE, "mouseDragged: ===== selectedIds=%d, selectedVtx=%d", selectedIdx,selectedVtx);
        setPoint(selectedIdx, selectedVtx, x, y);
    }
    #endif
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    #ifdef CONSOLE

    if(!selectedQuadKey.empty()) {
        selectedVtx = quads[selectedQuadKey]->getControlPointAt(x,y);
    }

    #endif
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

void testApp::quit(const std::vector<std::string> & args){
	std::exit(0);
}

void testApp::event(EventArg *e) {

    if (e->source.compare("TimedEventArg") == 0) {
        TimedEventArg *evtArg = (TimedEventArg*) e;
        if(evtArg->opName.compare("PlayVideo") == 0) {
            //El parametro indica que video iniciar.
            TextureManager::PlayVideo(evtArg->param1);
        }
        else if(evtArg->opName.compare("StopVideo") == 0) {
            //El parametro indica que video detener.
            TextureManager::StopVideo(evtArg->param1);
        }
        else if(evtArg->opName.compare("SetVideo") == 0) {
            //Param1 indica el quad al que asignar el video.
            //Param2 indica el id del video a asignar.
            quads[evtArg->param1]->getMaterial()->SetTextureParams(evtArg->param2, videoTexture, 0);
        }
    }
    else {
        DrawEventArg *evtArg = (DrawEventArg*) e;
        if (evtArg->_evtType == 0) { //setpoint
            setPoint(evtArg->_shapeId, evtArg->_vertexId, evtArg->_x, evtArg->_y, false);
        }
        else if (evtArg->_evtType == 1) { //draw new quad
            selectedIdx = addQuad(/*evtArg->_shapeId, */evtArg->source.c_str(), false);
        }
    }
}

void testApp::setupLogging() {
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofLog(OF_LOG_VERBOSE, "Configuring Logs");
    ofLog(OF_LOG_VERBOSE, "Initiating with nodeName="+this->nodeName);
}

void testApp::setupConsole()
{
    console = new ofxConsole();
    console->setDimentions(ofGetWidth(), ofGetHeight()/15);
	console->print("Mapping app started...");
	console->addFunction("quit", this, &testApp::quit);
	console->addFunction("addQuad", this, &testApp::addQuad);
}

void testApp::setPoint(int selectedIdx, int selectedVtx, int x, int y, bool sendEvent) {
    quadsIt = quads.begin();
    std::advance(quadsIt, selectedIdx);
    (*quadsIt).second->setPoint(selectedVtx, x, y);

    if (sendEvent) {
        ofxOscMessage oscMessage;
        oscMessage.addIntArg(selectedIdx);
        oscMessage.addIntArg(selectedVtx);
        oscMessage.addIntArg(x);
        oscMessage.addIntArg(y);
        this->oscManager->SendMessage(oscMessage, SETPOINT);
    }
}

void testApp::addQuad(const std::vector<std::string> & args)
{
	if(args.size()<2){
		console->print("Wrong number of arguments for < "+args[0]+" >!", CTEXT_ERROR);
		ofLog(OF_LOG_ERROR, "Wrong number of arguments for < "+args[0]+" >!");
		return;
	}
	string label = args[1];
    addQuad(label.c_str());
}

int testApp::addQuad(const char* label, bool sendEvent) {
    ofLog(OF_LOG_NOTICE, "Adding QUAD [%s, %d]", label, sendEvent);

    if (label == NULL) {
        char buffer [50];
        sprintf (buffer, "quad%d", quads.size());
        label = buffer;
    }

    Quad2D *q = new Quad2D(label);
    q->setSelected(true);
    q->setEnabled(true);
    quads.insert(pair<string, Quad2D*>(label, q));

    if (sendEvent) {
        ofxOscMessage oscMessage;
        oscMessage.addStringArg(label);
        this->oscManager->SendMessage(oscMessage, ADDQUAD);
    }

    return quads.size() - 1;
}

void testApp::loadQuads() {
    ofLog(OF_LOG_NOTICE, "loading project...");

    quadsXML.loadFile("./quads.xml");

    quadsXML.pushTag("quads");

    int numQuadItems = quadsXML.getNumTags("quadItem");

    for(int i = 0; i < numQuadItems; i++)
    {
        selectedIdx = addQuad(/*selectedIdx*/);

        quadsXML.pushTag("quadItem", i);

            double x0 = quadsXML.getAttribute("vx0", "x", 0.0, 0);
            double y0 = quadsXML.getAttribute("vx0", "y", 0.0, 0);

            double x1 = quadsXML.getAttribute("vx1", "x", 50.0, 0);
            double y1 = quadsXML.getAttribute("vx1", "y", 0.0, 0);

            double x2 = quadsXML.getAttribute("vx2", "x", 50.0, 0);
            double y2 = quadsXML.getAttribute("vx2", "y", 50.0, 0);

            double x3 = quadsXML.getAttribute("vx3", "x", 0.0, 0);
            double y3 = quadsXML.getAttribute("vx3", "y", 50.0, 0);

            setPoint(selectedIdx, 0, x0, y0);
            setPoint(selectedIdx, 1, x1, y1);
            setPoint(selectedIdx, 2, x2, y2);
            setPoint(selectedIdx, 3, x3, y3);

        quadsXML.popTag();
    }

    quadsXML.popTag();

    ofLog(OF_LOG_NOTICE, "project loaded. loaded quads: %i", numQuadItems);
}

void testApp::saveQuads() {

    ofLog(OF_LOG_NOTICE, "saving project...");
    quadsXML.clear();
    int quadsTagKey = quadsXML.addTag("quads");
    quadsXML.pushTag("quads", quadsTagKey);

    for(quadsIt = quads.begin(); quadsIt != quads.end(); ++quadsIt)
    {
        int quadItemTagKey = quadsXML.addTag("quadItem");
        quadsXML.pushTag("quadItem", quadItemTagKey);

        float x0, y0, x1, y1, x2, y2, x3, y3;
        (*quadsIt).second->getPoint(0, x0, y0);
        (*quadsIt).second->getPoint(1, x1, y1);
        (*quadsIt).second->getPoint(2, x2, y2);
        (*quadsIt).second->getPoint(3, x3, y3);

        int quadVx0Key = quadsXML.addTag("vx0");
        quadsXML.addAttribute("vx0", "x", x0, quadVx0Key);
        quadsXML.addAttribute("vx0", "y", y0, quadVx0Key);

        int quadVx1Key = quadsXML.addTag("vx1");
        quadsXML.addAttribute("vx1", "x", x1, quadVx1Key);
        quadsXML.addAttribute("vx1", "y", y1, quadVx1Key);

        int quadVx2Key = quadsXML.addTag("vx2");
        quadsXML.addAttribute("vx2", "x", x2, quadVx2Key);
        quadsXML.addAttribute("vx2", "y", y2, quadVx2Key);

        int quadVx3Key = quadsXML.addTag("vx3");
        quadsXML.addAttribute("vx3", "x", x3, quadVx3Key);
        quadsXML.addAttribute("vx3", "y", y3, quadVx3Key);

        quadsXML.popTag();
    }
    quadsXML.popTag();
    quadsXML.saveFile("./quads.xml");

    ofLog(OF_LOG_NOTICE, "Project saved.");

}


void testApp::loadShow() {

    ofLog(OF_LOG_NOTICE, "loading show config...");
    showConfig.loadFile("./SHOWCONF.xml");
    showConfig.pushTag("Show");

    string showName = showConfig.getAttribute("Data", "name", "", 0);
    //std::cout << " showName: "<< showName;

    showConfig.pushTag("Ports");
    int numItems = showConfig.getNumTags("OscPorts");
    showConfig.pushTag("OscPorts");

    for(int i = 0; i < numItems; i++)
    {
        string name = showConfig.getAttribute("Port", "name", "", i);
        string port = showConfig.getAttribute("Port", "port", "", i);
        //std::cout << " Port OscPorts Data: "<<  port << endl;

        OscPorts.insert(pair<string, int>(name, atoi(port.c_str())));
    }
    showConfig.popTag();

    numItems = showConfig.getNumTags("MidiPorts");
    showConfig.pushTag("MidiPorts");

    for(int i = 0; i < numItems; i++)
    {
        string name = showConfig.getAttribute("Port", "name", "", i);
        string port = showConfig.getAttribute("Port", "port", "", i);
        //std::cout << " Port MidiPorts Data: "<<  port;

        MidiPorts.insert(pair<string, string>(name, port));
    }
    showConfig.popTag();

    showConfig.popTag();

    showConfig.pushTag("Network");
    numItems = showConfig.getNumTags("Node");

    for(int i = 0; i < numItems; i++)
    {
        string name = showConfig.getAttribute("Node", "name", "", i);
        string address = showConfig.getAttribute("Node", "address", "", i);
        string port = showConfig.getAttribute("Node", "port", "", i);
        //std::cout << " Network Data: "<< name + " " + address + " "+ port<< endl;

        Node NodeData;
        string newStr (address);
        NodeData.address = newStr;//address;
        NodeData.port = atoi(port.c_str());
        Network.insert(pair<string, Node>(name, NodeData));

    }
    showConfig.popTag();

    showConfig.pushTag("Textures", 0);
    showConfig.pushTag("Images");
    numItems = showConfig.getNumTags("Image");

    for(int i = 0; i < numItems; i++)
    {
        string name = showConfig.getAttribute("Image", "name","", i);
        string path = showConfig.getAttribute("Image", "path","", i);
        //std::cout << " Images Data: "<< name + " " + path;

        TextureManager::LoadImageTexture(name, path);

    }
    showConfig.popTag();


    showConfig.pushTag("Videos");
    numItems = showConfig.getNumTags("Video");
    for(int i = 0; i < numItems; i++)
    {
        string name = showConfig.getAttribute("Video", "name","", i);
        string path = showConfig.getAttribute("Video", "path","", i);
        //std::cout << " Videos Data: "<< name + " " + path;

        TextureManager::LoadVideoTexture(name, path);
    }
    showConfig.popTag();

    showConfig.popTag();

    showConfig.pushTag("Shapes");
    showConfig.pushTag("Quads2D");
    numItems = showConfig.getNumTags("Quad2D");

    for(int i = 0; i < numItems; i++)
    {
        string id = showConfig.getAttribute("Quad2D", "id","", i);
        double x0 = showConfig.getAttribute("Quad2D", "x0", 0.0, i);
        double y0 = showConfig.getAttribute("Quad2D", "y0", 0.0, i);

        double x1 = showConfig.getAttribute("Quad2D", "x1", 50.0, i);
        double y1 = showConfig.getAttribute("Quad2D", "y1", 0.0, i);

        double x2 = showConfig.getAttribute("Quad2D", "x2", 50.0, i);
        double y2 = showConfig.getAttribute("Quad2D", "y2", 50.0, i);

        double x3 = showConfig.getAttribute("Quad2D", "x3", 50.0, i);
        double y3 = showConfig.getAttribute("Quad2D", "y3", 50.0, i);

        //std::cout << " Quad2D Data: "<< id<< endl;

        // add quad2
        int selIdx = addQuad(id.c_str(), false);
        setPoint(selIdx, 0, x0, y0, false);
        setPoint(selIdx, 1, x1, y1, false);
        setPoint(selIdx, 2, x2, y2, false);
        setPoint(selIdx, 3, x3, y3, false);

    }

    showConfig.popTag();

    showConfig.popTag();

    showConfig.pushTag("Events", 0);
    showConfig.pushTag("OscEvents");
    numItems = showConfig.getNumTags("Event");

    for(int i = 0; i < numItems; i++)
    {
        string name = showConfig.getAttribute("Event", "name","", i);
        string destination = showConfig.getAttribute("Event", "destination","", i);
        string delay = showConfig.getAttribute("Event", "delay","", i);
        //std::cout << " Event Data: "<< name + " " +destination+ " " +delay;

        //   add synchmanager event
    }

    showConfig.popTag();
    showConfig.popTag();

    ofLog(OF_LOG_NOTICE, "Show config loaded.");
}


void testApp::saveShow() {

    showConfig.loadFile("./SHOWCONF.xml");
    showConfig.pushTag("Show");
    showConfig.pushTag("Shapes");
    showConfig.pushTag("Quads2D");

    showConfig.clear();
    //showConfig.addTag("Quads2D");
    showConfig.pushTag("Quads2D");


        float x0;
        float y0;
        float x1;
        float y1;
        float x2;
        float y2;
        float x3;
        float y3;
        string id;
        map<string, Quad2D*>::iterator iter = quads.begin();
        while (iter != quads.end() )
        {
            int showTagKey = showConfig.addTag("Quad2D");
            id=iter->first;
            showConfig.addAttribute("Quad2D", "id", id, showTagKey);
            (iter->second)->getPoint(0,x0,y0);
            showConfig.addAttribute("Quad2D", "x0", x0, showTagKey);
            showConfig.addAttribute("Quad2D", "y0", y0, showTagKey);
            (iter->second)->getPoint(1,x1,y1);
            showConfig.addAttribute("Quad2D", "x1", x1, showTagKey);
            showConfig.addAttribute("Quad2D", "y1", y1, showTagKey);
            (iter->second)->getPoint(2,x2,y2);
            showConfig.addAttribute("Quad2D", "x2", x2, showTagKey);
            showConfig.addAttribute("Quad2D", "y2", y2, showTagKey);
            (iter->second)->getPoint(3,x3,y3);
            showConfig.addAttribute("Quad2D", "x3", x3, showTagKey);
            showConfig.addAttribute("Quad2D", "y3", y3, showTagKey);

            iter++;
            std::cout << " iter id quad2D: "<< id;

        }

    showConfig.popTag();
    showConfig.popTag();

    showConfig.saveFile("./SHOWCONF.xml");
    //ofLog(OF_LOG_NOTICE, "Show config saved.");
}
