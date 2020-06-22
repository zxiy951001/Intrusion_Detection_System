//
//  ContinuousEvent.cpp
//  A3 IDS
//
//

#include "ContinuousEvent.hpp"

ContinuousEvent::ContinuousEvent() {}
ContinuousEvent::~ContinuousEvent() {}
ContinuousEvent::ContinuousEvent(std::string eventName, char CD, int weight, double min, double max): Event(eventName, CD, weight) {
	setMin(min);
	setMax(max);
}

void ContinuousEvent::setMin(double min) {
	this->min = min;
}

void ContinuousEvent::setMax(double max) {
	this->max = max;
}

double ContinuousEvent::getMin() {
	return this->min;
}

double ContinuousEvent::getMax() {
	return this->max;
}
