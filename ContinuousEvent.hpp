//
//  ContinuousEvent.hpp
//  A3 IDS
//
//

#ifndef ContinuousEvent_hpp
#define ContinuousEvent_hpp

#include <stdio.h>
#include "Event.hpp"

class ContinuousEvent: public Event {
private:
	float min, max;
	
public:
	ContinuousEvent();
	ContinuousEvent(std::string eventName, char CD, int weight, double min, double max);
	
	~ContinuousEvent();
	
	void setMin (double);
	void setMax (double);
	
	virtual double getMin();
	double getMax();
};

#endif /* ContinuousEvent_hpp */
