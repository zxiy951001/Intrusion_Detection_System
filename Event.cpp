//
//  Event.cpp
//  A3 IDS
//
//

#include "Event.hpp"

Event::Event() {}
Event::~Event() {}
Event::Event(std::string eventName, char CD, int weight) {
	setEventName(eventName);
	setCD(CD);
	setWeight(weight);
}

void Event::setEventName(std::string eventName) {
	this->eventName = eventName;
}

void Event::setWeight(int weight) {
	this->weight = weight;
}

void Event::setCD(char cd) {
	this->CD = cd;
}

std::string Event::getEventName() {
	return this->eventName;
}

int Event::getWeight() {
	return this->weight;
}

char Event::getCD() {
	return this->CD;
}
