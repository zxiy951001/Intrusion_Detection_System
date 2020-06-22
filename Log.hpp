	//
	//  Log.hpp
	//  Assignment 03
	//
	//

#ifndef Log_hpp
#define Log_hpp

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

class Log {
private:
	std::string eventName;
	std::vector<double> values; //SIZE DEPENDS ON NO OF DAYS
	double val;
	char CD;
	int weight;
	
	
public:
	Log();
	Log(std::string eventName, char cd, int weight);
	Log(std::string eventName, char cd, int weight, double val);
	//Log(std::string eventName, char cd, double value, int weight);
	~Log();
	
	std::string getEventName();
	double getValue(int);
	int getWeight();
	char getCD();
	int getDays();
	double getVal();
	std::vector<double> getVector();
	
	void setEventName(std::string);
	void setValue(double);
	void setWeight(int);
	void setCD(char);
};

#endif /* Log_hpp */
