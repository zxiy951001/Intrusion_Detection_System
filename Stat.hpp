	//
	//  Stat.hpp
	//  Assignment 03
	//
	//

#ifndef Stat_hpp
#define Stat_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

class Stat {
private:
	std::string eventName;
	float mean;
	float standardDeviation;
	
public:
	Stat();
	Stat(std::string eventName, float mean, float standardDeviation);
	
	std::string getEventName();
	float getMean();
	float getStandardDeviation();
	
	void setEventName(std::string);
	void setMean(float);
	void setStandardDeviation(float);
};

#endif /* Stat_hpp */
