//
//  IDS.cpp
//  A3 IDS
//
//

#include "IDS.hpp"

	//The command to run the program -> the last arg is the NAME of the employee you want to generate the Stat file for.
	//IDS Event.txt Stat.txt 100 Sionggo
int main (int argc, char** argv) {
	if (argc == 4 || argc == 5) {
		eventFile = argv[1];
		statsFile = argv[2];
		noOfDays = atoi(argv[3]);
		
		if (argc == 5) {
				//defaults to peter
			username = argv[4];
			std::stringstream ss("");
			
				//logfile naming format
			ss << username << "_" << noOfDays << "_Daily.log";
			logFile = ss.str();
		}
	} else if (argc != 1) {
		std::cout 	<< "Invalid number of arguments!\n";
		exit (-1);
	} else {
		std::cout 	<< "Using defaults.\n";
	}
	
	srand((int)time(NULL));
	
	// ~~~ LEARNING PHASE ~~~
	std::cout 	<< "\nStarting Learning Phase" << std::endl;
	std::cout 	<< std::setw(30) << std::setfill('#') << "#" << std::endl;
	if (init() == EXIT_FAILURE) {
		std::cout 	<< "Please make sure the files are present and consistent.\n\a";
		exit (-1);
	}
	
		//generate and write logs
	std::cout 	<< std::setw(20) << std::setfill('~') << "~" << std::endl;
	generateLogs(noOfDays);
	writeLogFile(noOfDays, logFile);
	
	// ~~~ ANALYSIS PHASE ~~~
		//start reading the logs
		//generate new stats file
	std::cout 	<< std::setw(20) << std::setfill('~') << "~" << std::endl;
	std::cout 	<< "\nStarting Analysis Phase\n";
	std::cout 	<< std::setw(30) << std::setfill('#') << "#" << std::endl;
	readLogs(logFile);

//	// ~~~  ALERT PHASE ~~~
// for the looping thing, enter -1 to exit
	std::cout 	<< std::setw(20) << std::setfill('~') << "~" << std::endl;
	std::cout 	<< "\nStarting Alert Phase\n";
	std::cout 	<< std::setw(30) << std::setfill('#') << "#" << std::endl;
	
	while (true) {
		anomalies = 0;
		int liveDataDays = generateLiveData();
		outfileToArray("live_data.log");
		readGeneratedStats(baselineStats);
		compareLogs(liveDataDays);
		std::cout 	<< std::setw(30) << std::setfill('=') << "=" << std::endl;
		std::cout 	<< "~~~~ ANOMALIES FOUND: " << anomalies << " ~~~~" << std::endl;
	}

	return EXIT_SUCCESS;
}

int init() {
	int success = EXIT_SUCCESS;

	//IF ANY OF THE CONDITIONS FAILED
	if (	readEvents(eventFile) == EXIT_FAILURE || readStats(statsFile) == EXIT_FAILURE) {
		success = EXIT_FAILURE;
	} else {
		std::cout 	<< "DONE!\n";
		std::cout 	<< std::setw(20) << std::setfill('~') << "~" << std::endl;
		std::cout 	<< "Total number of Events read \t\t -> " << events.size() << std::endl;
		std::cout 	<< "Number of events in statistical data \t -> " << stats.size() << std::endl;
		
		if (events.size() != stats.size()) {
			success = EXIT_FAILURE;
			std::cout 	<< "ERROR!  The number of entries in " << eventFile
						<< " and " << statsFile << " do not match!\n";
		}
		else {
			std::vector<bool> checks (events.size(), false);
			
				//supposed to check if all the elements in stats and events tally
				//by the end of this for loop, all the elements in checks should be true
			for (int event = 0, n = (int)events.size(); event < n; ++event) {
				for (int stat = 0, o = (int)stats.size(); stat < o; ++stat) {
					if (events[event]->getEventName() == stats[stat]->getEventName())
						checks[event] = true;
				}
			}
			
			int count = 0;
			for (std::vector<bool>::iterator it = checks.begin(); it != checks.end(); ++it) {
				if (!*it) {
					success = EXIT_FAILURE;
					std::cout 	<< "ERROR!  The event: \"" << events[count]->getEventName()
								<< "\" does not exist in " << statsFile << std::endl;
				}
				
				++count;
			}
		}
		
		std::cout 	<< "Data is "
		<< ((success == EXIT_SUCCESS ? "CONSISTENT" : "INCONSISTENT"))
		<< "!" << std::endl;
	}

	return success;
}

int readEvents(std::string filename) {
	int success = EXIT_SUCCESS;
	std::fstream infile (filename.c_str(), std::ios::in);

	if (!infile) {
		infile.close();
		std::cout 	<< "Failed to read " << filename << "! Exiting.." << std::endl;
		success = EXIT_FAILURE;
	} else {
		std::cout 	<< "Reading events from " << filename << "!\n";
		int count;
		infile >> count;
		infile.ignore(256, '\n');

		std::string line, delim = ":";
		while (std::getline(infile, line) ) {
			std::vector<std::string> tokens = tokenize(line, delim);
			std::string eventName = tokens[0];
			eventNames.push_back(eventName);

			if (tokens[1] == "D") {
				char CD = 'D';
				int min, max, weight;
				
				min = atoi(tokens[2].c_str());
				if(tokens[3] == ""){
					max = 9999;
				}
				else{
					max = atoi(tokens[3].c_str());
				}
				weight = atoi(tokens[4].c_str());
				events.push_back(new DiscreteEvent(eventName, CD, weight, min, max));
				
			} else if (tokens[1] == "C") {
				char CD = 'C';
				float min, max;
				int weight;
				
				min = atoi(tokens[2].c_str());
				if(tokens[3] == ""){
					max = 9999.99;
				}
				else{
					max = atof(tokens[3].c_str());
				}
				weight = atoi(tokens[4].c_str());
				events.push_back(new ContinuousEvent(eventName, CD, weight, min, max));
			}
		}
	}

	return success;
}

int readStats(std::string filename) {
	int success = EXIT_SUCCESS;
	std::fstream infile (filename.c_str(), std::ios::in);

	if (!infile) {
		infile.close();
		std::cout 	<< "Failed to read " << filename << "! Exiting.." << std::endl;
		success = EXIT_FAILURE;
	} else {
		std::cout 	<< "Reading stats from " << filename << "!\n";
		int count;
		infile >> count;
		infile.ignore(256, '\n');

		std::string line, delim = ":";
		while (std::getline(infile, line) ) {
			std::vector<std::string> tokens = tokenize(line, delim);
			std::string eventName = tokens[0];
			float mean, standardDeviation;
			mean = atof(tokens[1].c_str());
			standardDeviation = atof(tokens[2].c_str());

			//TEST
			stats.push_back(new Stat(eventName, mean, standardDeviation)); 
		}
	}

	return success;
}

int readGeneratedStats(std::string filename) {
	int success = EXIT_SUCCESS;
	std::fstream infile (filename.c_str(), std::ios::in);

	if (!infile) {
		infile.close();
		std::cout 	<< "Failed to read " << filename << "! Exiting.." << std::endl;
		success = EXIT_FAILURE;
	} else {
		int count;
		infile >> count;
		infile.ignore(256, '\n');

		std::string line, delim = ":";
		while (std::getline(infile, line) ) {
			std::vector<std::string> tokens = tokenize(line, delim);
			std::string eventName = tokens[0];
			float mean, standardDeviation;
			mean = atof(tokens[1].c_str());
			standardDeviation = atof(tokens[2].c_str());

			//TEST
			generatedStats.push_back(new Stat(eventName, mean, standardDeviation)); 

		}
	}

	return success;
}

double generateValue(double mean, double stdDev){

	bool hasSpare = false;
	double spare = 0.0;

	if(hasSpare)
	{
		hasSpare = false;
		return mean + stdDev * spare;
	}

	hasSpare = true;
	static double u, v, s;
	do
	{
		u = (rand() / ((double) RAND_MAX)) * 2.0 - 1.0;
		v = (rand() / ((double) RAND_MAX)) * 2.0 - 1.0;
		s = u * u + v * v;
	}
	while( (s >= 1.0) || (s == 0.0) );

	s = sqrt(-2.0 * log(s) / s);
	spare = v * s;

	return mean + stdDev * u * s;
}

double findMean(std::vector<double> numbers) {
	float total = 0;
	int count = 0;
	for (std::vector<double>::iterator it = numbers.begin(); it != numbers.end(); ++it) {
		total += *it;
		
		++count;
	}
	
	total = total / count;
	return total;
}

double findVariance (std::vector<double> numbers, double mean) {
	double variance = 0.0;
	int count = 0;
	
	for (std::vector<double>::iterator it = numbers.begin(); it != numbers.end(); ++it) {
		double temp = *it - mean;
		
		if (temp > 0) {
			temp = (temp * temp);
			variance += temp;
		}
		
		++count;
	}
	variance *= 2;
	variance = variance / count;
	
	return variance;
}

void writeLogFile(int noOfDays, std::string filename){
	std::cout << "Writing logs to " << filename << "..\n";
	std::fstream outFile(logFile.c_str(), std::ios::out);
	
	if (!outFile) {
		outFile.close();
		std::cout << "\n****" << filename << " failed to open for data writing****" << std::endl;
	} else {
		for(int k = 0; k < noOfDays; k++){
			std::stringstream data;
			outFile << "~~~ Day " << k + 1 << " ~~~" << std::endl;
			for (int i = 0; i < events.size(); ++i) {
				data.str("");
				data << logs[i]->getEventName() << ":";
				data << logs[i]->getCD() << ":";
				
				if ( logs[i]->getCD() == 'C' ) {
					data << std::setprecision(2) << std::fixed << logs[i]->getValue(k) << ":";
				} else {
					data << std::setprecision(0) << std::fixed << logs[i]->getValue(k) << ":";
				}

				data << logs[i]->getWeight() << ":" << std::endl;

				outFile << data.str().c_str();
			}
		}
		
		std::cout 	<< "DONE!\n";
		outFile.close();
	}
}

void generateLogs(int numberOfLogs) {
	std::cout << "Generating logs for " << numberOfLogs << " days..\n";
 	for(int i = 0; i < events.size(); i++){
		//PUSH EVENTS INTO VECTOR OF LOGS
		logs.push_back(new Log(events[i]->getEventName(), events[i]->getCD(), events[i]->getWeight()));

		//GENERATE VALUES FOR EVENTS ACCORDING TO NUMBER OF DAYS
		for(int k = 0; k < numberOfLogs; k++){

			if(events[i]->getCD() == 'C'){
				ContinuousEvent cEvent = *dynamic_cast<ContinuousEvent*>(events[i]);
				double value;
				do{
					value = generateValue(stats[i]->getMean(), stats[i]->getStandardDeviation());
				}while(withinStatisticC(value, cEvent.getMin(), cEvent.getMax()) == false);
				
				logs[i]->setValue(value);
			}

			if(events[i]->getCD() == 'D'){
				DiscreteEvent dEvent = *dynamic_cast<DiscreteEvent*>(events[i]);
				double value;
				do{
					value = generateValue(stats[i]->getMean(), stats[i]->getStandardDeviation());
				}while(withinStatisticC(value, dEvent.getMin(), dEvent.getMax()) == false);
				
				logs[i]->setValue(value);
			}
		}
	}
	
	std::cout 	<< "DONE!\n";
}

void writeStats (std::vector<double> mean, std::vector<double> stdDiv) {
	std::stringstream output, statFilename;
	statFilename << username << "_" << noOfDays << "_stats.txt";
	baselineStats = statFilename.str();
	std::fstream outfile (baselineStats.c_str(), std::ios::out);
	
	std::cout 	<< "Generating " << baselineStats << " (baseline statistics)..\n";
//	std::string filename = "newStats.txt";
//	std::fstream outfile(filename.c_str(), std::ios::out);

	if (!outfile) {
		std::cout 	<< "FAIL!\n";
		outfile.close();
	} else {
		outfile << events.size() << std::endl;

		for (int i = 0; i < events.size(); ++i) {
			output.str("");

			if ( logs[i]->getCD() == 'C' ) {
				output 	<< events[i]->getEventName() << ":" 
				<< std::setprecision(2) << std::fixed 
				<< mean[i] << ":" 
				<< std::setprecision(2) << std::fixed 
				<< stdDiv[i] << ":";
			} else {
				output 	<< events[i]->getEventName() << ":" 
				<< std::setprecision(0) << std::fixed 
				<< mean[i] << ":" 
				<< std::setprecision(0) << std::fixed 
				<< stdDiv[i] << ":";
			}
			
			outfile << output.str() << std::endl;
		}

		outfile.close();
	}
}

void readLogs(std::string filename) {
	
	std::cout << "Reading " << filename << std::endl;
	std::cout << "Events for each Log \t-> " << eventNames.size() << std::endl << std::endl;
	for(int i = 0; i < events.size(); i++){
		std::cout << "\t->" << eventNames[i] << std::endl;
	}

	std::cout << std::endl;

	std::fstream infile (filename.c_str(), std::ios::in);

	if (!infile) {
		std::cout 	<< "Could not open " << filename << "! Please make sure it exists!\n";
		infile.close();
	} else {
		std::string line, delim = ":";
		std::stringstream ss ("");
		int day = 1;

			//store the total login, eSent, eReceived, timeOnline;
		std::vector<double> means (events.size(), 0.0), stdDivs (events.size(), 0.0);

		ss << "~~~ Day " << day << " ~~~";
		while (std::getline(infile, line)) {
			if (line == ss.str()) {
					//showing ALL the information is a bit much
				if (day % 30 == 0) {
					std::cout 	<< "Processing information for day " << day << "!\n";
				}
				
				++day;
				ss.str("");

				ss << "~~~ Day " << day << " ~~~";
			} else {
				std::vector<std::string> tokens = tokenize(line, delim);
				std::string eventName = tokens[0];
				tokens.pop_back(); 	//POP BACK BECAUSE LAST ELEMENT IS BLANK

				for (int i = 0, n = (int)logs.size(); i < n; ++i) {
					if (logs[i]->getEventName() == eventName) {
						std::vector<double> totals = logs[i]->getVector();
						means[i] = findMean(totals);
						stdDivs[i] = sqrt(findVariance(totals, means[i]));
						break;
					}
				}
			}
		}

		std::cout 	<< "Finished reading " << day - 1 << " logs\n";
		writeStats(means, stdDivs);

		infile.close();
	}
}

void doAnalysis(std::string dailyFile, std::string statsFile) {
	std::string line, delim = ":";
	std::vector <std::vector <std::string> > input, stats;
	double totalWeight = 0;
	double daysWeight = 0;
	double threshold;

	//calculate totalWeight
	for (int i = 0; i < events.size(); ++i) {
		totalWeight += events[i]->getWeight();
	}

	//set threshold
	threshold = 2 * totalWeight;

	//read stats figures
	std::fstream sFile (statsFile.c_str(), std::ios::in);
	if (!sFile) {
		std::cout 	<< "COULD NOT FIND " << statsFile << "!" << std::endl;
		sFile.close();
	} else {
		std::string count;
		std::getline(sFile, count);
		while (std::getline(sFile, line)) {
			stats.push_back(tokenize(line, delim));
		}

		sFile.close();
	}
	
	// read daily figures
	std::fstream dFile (dailyFile.c_str(), std::ios::in);
	if (!dFile) {
		std::cout 	<< "COULD NOT FIND " << dailyFile << "!" << std::endl;
		dFile.close();
	} else {
		std::string count;
		std::getline(dFile, count);
		while (std::getline(dFile, line)) {
			input.push_back(tokenize(line, delim));
		}

		dFile.close();
	}

	std::cout 	<< input.size() << " ** " << stats.size() << std::endl;

	//(statsMean - dailyMean) / statsSD
	//files read, now COMPARE
	for (int i = 0, n = (int)input.size(); i < n; ++i) {
		for (int j = 0, o = (int)stats.size(); j < o; ++j) {
			//compare the name
			if (input[i][0] == stats[i][0]) {
				double statsMean, dailyMean, statsSD;

				statsMean = atof(stats[i][1].c_str());
				dailyMean = atof(input[i][1].c_str());
				statsSD = atof(stats[i][2].c_str());
				daysWeight += ((statsMean - dailyMean) / statsSD);
				break;
			}
		}
	}

	if (daysWeight > threshold) {
		std::cout 	<< "INTRUSION MOTHERFUCKER!\n";
	}

	std::cout 	<< "Threshold ->\t" 	<< threshold << std::endl;
	std::cout 	<< "CurrWeight ->\t" 	<< daysWeight << std::endl;
}

bool withinStatisticC(float value, double min, double max){
	//IF VALUE IS GREATER THAN MEAN-SD && VALUE IS SMALLER THAN MEAN+SD
	if(value >= min && value <= max){
		return true;
	}
	else
		return false;
}


bool withinStatisticD(int value, int min, int max){
	//IF VALUE IS GREATER THAN MEAN-SD && VALUE IS SMALLER THAN MEAN+SD
	if(value >= min && value <= max){
		return true;
	}
	else
		return false;
}

int generateLiveData(){
	std::string liveDataFile, noOfDays;
	int days = 0;
	
	std::cout << "\nPlease enter the live data stats file [-1 to quit]: ";
	std::getline(std::cin, liveDataFile);
		//std::cin.clear();
		//std::cin.ignore(256, '\n');

	if (liveDataFile == "-1") exit(0);
	
	std::fstream infile (liveDataFile.c_str(), std::ios::in);

	if (!infile.good()) {
		infile.close();
		std::cout 	<< "\nFailed to read " << liveDataFile << "! Exiting.." << std::endl;
	} else {
		std::cout 	<< "Number of days of data to be generated from " << liveDataFile << ": ";
			//std::cin 	>> days;
		
		std::getline(std::cin, noOfDays);
		days = atoi(noOfDays.c_str());
		std::cin.clear();
		
		while (!days) {
			std::cout 	<< "\nError!  Not a valid number." << std::endl;
			std::cin.clear();
			std::cin.ignore(256, '\n');
			std::cout 	<< "\nNumber of days of data to be generated from " << liveDataFile << ": ";
			std::getline(std::cin, noOfDays);
			days = atoi(noOfDays.c_str());
			std::cin.clear();
		}
		
		int count;
		infile >> count;
		infile.ignore(256, '\n');

		std::string line, delim = ":";
		while (std::getline(infile, line) ) {
			std::vector<std::string> tokens = tokenize(line, delim);
			std::string eventName = tokens[0];
			float mean, standardDeviation;
			mean = atof(tokens[1].c_str());
			standardDeviation = atof(tokens[2].c_str());

			//TEST
			liveData.push_back(new Stat(eventName, mean, standardDeviation)); 
		}
		
		infile.close();
		generateLiveLogs(days);
		writeLiveLogFile(days, "live_data.log");
	}
	
	return days;
}


void generateLiveLogs(int numberOfLogs) {
	//TEST MAKING LOGS
	//LOOP ACCORDING TO NUMBER OF EVENTS
	//std::cout << events.size() << std::endl;

	std::cout << "Generating " << numberOfLogs << " days of LIVE DATA..\n";
	for(int i = 0; i < events.size(); i++){
		//PUSH EVENTS INTO VECTOR OF LOGS
		liveLogs.push_back(new Log(events[i]->getEventName(),
								   events[i]->getCD(),
								   events[i]->getWeight()));

		//GENERATE VALUES FOR EVENTS ACCORDING TO NUMBER OF DAYS
		for(int k = 0; k < numberOfLogs; k++){

			if(events[i]->getCD() == 'C'){
				ContinuousEvent cEvent = *dynamic_cast<ContinuousEvent*>(events[i]);
				double value;
				do{
					value = generateValue(liveData[i]->getMean(), liveData[i]->getStandardDeviation());
				}while(withinStatisticC(value, cEvent.getMin(), cEvent.getMax()) == false);
				
				liveLogs[i]->setValue(value);
			}

			if(events[i]->getCD() == 'D'){
				DiscreteEvent dEvent = *dynamic_cast<DiscreteEvent*>(events[i]);
				double value;
				do{
					value = generateValue(liveData[i]->getMean(), liveData[i]->getStandardDeviation());
				}while(withinStatisticC(value, dEvent.getMin(), dEvent.getMax()) == false);
				
				liveLogs[i]->setValue(value);
			}
		}
	}
	std::cout 	<< "DONE!\n";
}

void writeLiveLogFile(int noOfDays, std::string filename){
	std::fstream outFile(filename.c_str(), std::ios::out);
	
	if (!outFile) {
		outFile.close();
		std::cout << "\n*****" << filename << " failed to open for data writing*****" << std::endl;
	} else {
		
		//FOR LOOP TO EXTRACT DAILY EVENTS FROM LOGSHEET
		for(int k = 1; k <= noOfDays; k++){
			std::stringstream data;
			outFile << "~~~ Day " << k << " ~~~" << std::endl;
			for (int i = 0; i < events.size(); ++i) {
				data.str("");
				data << liveLogs[i]->getEventName() << ":";
				data << liveLogs[i]->getCD() << ":";
				
				if ( logs[i]->getCD() == 'C' ) {
					data << std::setprecision(2) << std::fixed << liveLogs[i]->getValue(k) << ":";
				} else {
					data << std::setprecision(0) << std::fixed << liveLogs[i]->getValue(k) << ":";
				}

				data << liveLogs[i]->getWeight() << ":" << std::endl;

				outFile << data.str().c_str();
			}
		}
		
		outFile.close();
	}
}

void outfileToArray(std::string filename){
		//std::cout << "####################################################################" << std::endl;
	
	std::fstream infile (filename.c_str(), std::ios::in);
	std::string line, delim = ":";
	while(std::getline(infile, line)){
		for(int i = 0; i < eventNames.size(); i++){
			std::getline(infile,line);
			std::vector<std::string> tokens = tokenize(line, delim);
			char cd = 'k';
			if(tokens[1] == "C"){
				cd = 'C';
			}else if(tokens[1] == "D"){
				cd = 'D';
			}
			std::string eventName = tokens[0];
			double value = atof(tokens[2].c_str());
			int weight = atoi(tokens[3].c_str());
			liveLogs.push_back(new Log(eventName, cd, weight, value));
		}
	}
	infile.close();
}

void compareLogs(int noOfDays){
	int weight=0;
	for(int i = 0; i < events.size(); i++){
		weight += events[i]->getWeight();
	}
	
	weight = 2 * weight;
		//std::cout << "test " << weight << std::endl;
	double temp = 0;
	float value[noOfDays];
	for(int i = 0; i < noOfDays; i++){
			//header
		std::cout 	<< std::setw(30) << std::setfill('=') << "=" << std::endl;
		std::cout << "\nDay " << (i + 1) << std::endl;
		std::cout 	<< std::setw(5) << std::setfill('-') << "-" << std::endl;
		value[i] = 0.0;
		
			//body
		for(int j = 0; j < events.size(); j++){
			temp = value[i];
			value[i] += std::abs(generatedStats[j]->getMean() - liveLogs[j]->getValue(i+1));
			std::cout 	<< "Event : " << events[j]->getEventName() << std::endl;
			if(value[i] != 0){
				value[i] = value[i] / generatedStats[j]->getStandardDeviation();
			}
			value[i] = value[i] * liveLogs[j]->getWeight();
			value[i] += temp;
		}
		
			//summary
		std::cout 	<< std::setw(30) << std::setfill('~') << "~" << std::endl;
		std::cout 	<< "Threshold:\t" << weight
					<< std::setprecision(2) << std::fixed
					<< "\nCurrent:\t" << value[i] << std::endl;
		
		if(value[i] > weight){
			std::cout 	<< "@@@@@@Anomaly detected!@@@@@@@" << std::endl;
			++anomalies;
		} else {
			std::cout 	<< "No anomalies detected.\n";
		}
	}	

}

bool compareWithStats(std::vector<Log*> dailyLog, std::vector<Stat*> stats) {
	bool ans = false;
	double weight = 0;
	double temp = 0;
	
	for(int i = 0; i < events.size(); i++){
		weight += events[i]->getWeight();
	}

	weight = 2 * weight;
	std::cout << " stats size " << stats.size() << std::endl << " dailylog size " << dailyLog.size() << std::endl;
	for(int i = 0; i < stats.size(); i++){
		for(int k = 0; k < dailyLog.size(); k++){
			std::cout << "Day " << (k + 1);
//			std::cout 	<< stats[i]->getEventName() << std::endl;
			std::cout 	<< dailyLog[k]->getEventName() << " " << dailyLog[k]->getVal() << std::endl;
			
//			if (stats[i]->getEventName() == dailyLog[i]->getEventName()) {
//					//	temp = stats[i]->getMean() - dailyLog[i]->getValue(0);
//					//std::cout 	<< dailyLog[i]->getValue(0) << std::endl;
//			}
		}
	}
	
	return ans;
}







