	//
	//  Log.cpp
	//  Assignment 03
	//
	//

#include "Log.hpp"

Log::Log() {};

/*Log::Log(std::string eventName, char cd, double value, int weight){
	setEventName(eventName);
	setCD(cd);
	setValue(value);
	setWeight(weight);
}*/

Log::Log(std::string eventName, char cd, int weight){
	setEventName(eventName);
	setCD(cd);
	setWeight(weight);
}

Log::Log(std::string eventName, char cd, int weight, double value){
	setEventName(eventName);
	setCD(cd);
	setWeight(weight);
	setValue(value);
}

/*Log::Log(std::string eventName, char cd, double value, int weight){
	setEventName(eventName);
	setCD(cd);
	float v = value * 1.0;
	setValue(v);
	setWeight(weight);
}*/

Log::~Log() {}

void Log::setEventName(std::string eventName){
	this->eventName = eventName;
}

void Log::setValue(double value){
	this->values.push_back(value);
}

void Log::setWeight(int weight){
	this->weight = weight;
}

void Log::setCD(char CD){
	this->CD = CD;
}

std::string Log::getEventName(){
	return this->eventName;
}

double Log::getVal () {
	return this->val;
}

double Log::getValue(int index){
	return this->values[index];
}

int Log::getWeight(){
	return this->weight;
}

char Log::getCD(){
	return this->CD;
}

int Log::getDays(){
	return (int)this->values.size();
}

std::vector<double> Log::getVector(){
	return this->values;
}
