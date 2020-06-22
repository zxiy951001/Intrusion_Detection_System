//
//  DiscreteEvent.hpp
//  A3 IDS
//


#ifndef DiscreteEvent_hpp
#define DiscreteEvent_hpp

#include <stdio.h>
#include "Event.hpp"

class DiscreteEvent: public Event {
private:
	int min, max;
	
public:
	DiscreteEvent();
	DiscreteEvent(std::string eventName, char CD, int weight, int min, int max);
	
	~DiscreteEvent();
	
	void setMin (int);
	void setMax (int);
	
	virtual int getMin();
	int getMax();
};

#endif /* DiscreteEvent_hpp */
