all: IDS

IDS: IDS.o Event.o ContinuousEvent.o DiscreteEvent.o Stat.o Log.o
	g++ IDS.o Event.o ContinuousEvent.o DiscreteEvent.o Stat.o Log.o -o IDS
	@echo "Compilation successful.  Do \"make run\" to run the program."

IDS.o: IDS.cpp IDS.hpp Event.o ContinuousEvent.o DiscreteEvent.o Stat.o Log.o
	g++ -c IDS.cpp

Event.o: Event.cpp Event.hpp
	g++ -c Event.cpp

DiscreteEvent.o: DiscreteEvent.cpp DiscreteEvent.hpp
	g++ -c DiscreteEvent.cpp

ContinuousEvent.o: ContinuousEvent.cpp ContinuousEvent.hpp
	g++ -c ContinuousEvent.cpp

Stat.o: Stat.cpp Stat.hpp
	g++ -c Stat.cpp

Log.o: Log.cpp Log.hpp
	g++ -c Log.cpp

clean:
	@\rm *.o
	@\rm IDS
	@\rm *.log
	@echo "Directory cleaned."

run:
	./IDS Events.txt Stats.txt 1000 Isaac
