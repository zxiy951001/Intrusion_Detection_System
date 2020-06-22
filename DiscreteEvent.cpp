//
//  DiscreteEvent.cpp
//  A3 IDS
//
//

#include "DiscreteEvent.hpp"

DiscreteEvent::DiscreteEvent() {}
DiscreteEvent::~DiscreteEvent() {}
DiscreteEvent::DiscreteEvent(std::string eventName, char CD, int weight, int min, int max): Event(eventName, CD, weight) {
	setMin(min);
	setMax(max);
}

void DiscreteEvent::setMin(int min) {
	this->min = min;
}

void DiscreteEvent::setMax(int max) {
	this->max = max;
}

int DiscreteEvent::getMin() {
	return this->min;
}

int DiscreteEvent::getMax() {
	return this->max;
}
