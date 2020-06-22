//
//  Event.hpp
//  A3 IDS
//
//
//

#ifndef Event_hpp
#define Event_hpp

#include <stdio.h>
#include <string>
#include <iostream>

class Event {
private:
	std::string eventName;
	char CD;
	int weight;
	
public:
	Event();
	Event(std::string eventName, char CD, int weight);
	~Event();
	
	virtual std::string getEventName();
	virtual int getWeight();
	virtual char getCD();
	
	
	virtual void setEventName(std::string);
	virtual void setWeight(int);
	virtual void setCD (char);
};

#endif /* Event_hpp */
