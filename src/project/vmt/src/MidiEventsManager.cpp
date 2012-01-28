#include "MidiEventsManager.h"
#include "ofUtils.h"

MidiEventsManager::MidiEventsManager()
{

}

MidiEventsManager::~MidiEventsManager()
{
    //dtor
}

void MidiEventsManager::addMidiEvent(ofxMidiEventArgs*	msg, string effectId) {
    events.insert(pair<ofxMidiEventArgs*,string>(msg,effectId));
}

bool MidiEventsManager::hasMidiEvent(ofxMidiEventArgs* msg) {
    return events.count(msg) > 0;
}

string MidiEventsManager::getEffectId(ofxMidiEventArgs* msg) {
    return events[msg];
}

SerializedNode* MidiEventsManager::Serialize() {
   SerializedNode *node = new SerializedNode("midievents");

    map<ofxMidiEventArgs*, string>::iterator eventsIt;
    for(eventsIt = events.begin(); eventsIt != events.end(); eventsIt++) {
        SerializedNode *eventNode = new SerializedNode("midievents");
        eventNode->addAttribute("midi", eventsIt->first);
        eventNode->addAttribute("effect", eventsIt->second);
        node->addChildNode(eventNode);
    }
    return node;
}

map<ofxMidiEventArgs*, string> MidiEventsManager::getMidiEvents(){
    return events;
}


