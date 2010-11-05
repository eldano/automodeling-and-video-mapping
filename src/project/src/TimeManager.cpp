#include "TimeManager.h"

#include "ofAppRunner.h"

static double totalAnimTime;
static double deltaStartTime;       //Tiempo entre que comenz� la aplicaci�n y se hizo init a TimeManager

static map<float, pair<IEventListener*, TimedEventArg*> >::iterator it;

static float nextTimeEvent;

map<float, pair<IEventListener*, TimedEventArg*> > TimeManager::events;

TimeManager::TimeManager()
{
    //ctor
}

TimeManager::~TimeManager()
{
    //dtor
}

void TimeManager::Init() {
    totalAnimTime = 0;
    it = events.begin();
    nextTimeEvent = (*it).first;
    deltaStartTime = ofGetElapsedTimef();
}


void TimeManager::AddTimedEvent(float time, IEventListener *evtLstnr, string opName, string param) {
    TimedEventArg *timedEvtArg = new TimedEventArg();
    timedEvtArg->opName = opName;
    timedEvtArg->param = param;

    events.insert(make_pair(time, make_pair(evtLstnr, timedEvtArg)));
}

void TimeManager::Update() {
    totalAnimTime = ofGetElapsedTimef() - deltaStartTime;

    if(totalAnimTime > nextTimeEvent && events.size() > 0) {
        map<float, pair<IEventListener*, TimedEventArg*> >::iterator it2 = it;
        ofLog(OF_LOG_VERBOSE, "%f :: llamando a func...", totalAnimTime);
        TimedEventArg *timedEvtArg = (*it).second.second;
        timedEvtArg->_timestamp = totalAnimTime;
        (*it).second.first->event(timedEvtArg);
        ++it;
        events.erase(it2);
        nextTimeEvent = (*it).first;
    }
}