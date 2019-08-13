#ifndef LOG_H
#define LOG_H
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>  
#define LOG_ERROR 1
#define LOG_WARNING 2
#define LOG_INFO 4
#define LOG_VERBOSE 8
#define LOG_LOADING 16
#define LOG_FILE_ERROR 32
#define LOG_FILE_WARNING 64
#define LOG_FILE_INFO 128
#define LOG_FILE_VERBOSE 256
#define LOG_TIME 512
#define LOG_COMPTIME 1024
#define LOG_FILE_TIME 2048
#define LOG_FILE_COMPTIME 4096
#define LOG_BASIC 31
#define LOG_BASIC_ALL 1567
#define LOG_FILE 480
#define LOG_FILE_ALL 6624
#define LOG_BOTH 511
#define LOG_ALL 8191

#define LOG_ERROR_LABEL "[ERROR]    "
#define LOG_WARNING_LABEL "[WARNING]  "
#define LOG_INFO_LABEL "[INFO]     "
#define LOG_VERBOSE_LABEL "[VERBOSE]  "
#define LOG_LOADING_LABEL "[LOADING]  "
#ifdef _WIN32
	#define LOG_ERROR_FORMAT "\e[1m\e[31m"
	#define LOG_WARNING_FORMAT "\e[1m\e[33m"
	#define LOG_INFO_FORMAT "\e[1m\e[36m"
	#define LOG_VERBOSE_FORMAT "\e[1m\e[37m"
	#define LOG_LOADING_FORMAT "\e[1m\e[32m"
	#define LOG_CLEAR_FORMAT "\e[0m"
	#define LOG_DULL_FORMAT "\e[2m"
	#ifdef _WIN64
      //define something for Windows (64-bit only)
   #else
      //define something for Windows (32-bit only)
   #endif
#elif __APPLE__
	#define LOG_ERROR_FORMAT "\e[1m\e[31m"
	#define LOG_WARNING_FORMAT "\e[1m\e[33m"
	#define LOG_INFO_FORMAT "\e[1m\e[36m"
	#define LOG_VERBOSE_FORMAT "\e[1m\e[37m"
	#define LOG_LOADING_FORMAT "\e[1m\e[32m"
	#define LOG_CLEAR_FORMAT "\e[0m"
	#define LOG_DULL_FORMAT "\e[2m"
#elif __linux__
	#define LOG_ERROR_FORMAT "\e[1m\e[31m"
	#define LOG_WARNING_FORMAT "\e[1m\e[33m"
	#define LOG_INFO_FORMAT "\e[1m\e[36m"
	#define LOG_VERBOSE_FORMAT "\e[1m\e[37m"
	#define LOG_LOADING_FORMAT "\e[1m\e[32m"
	#define LOG_CLEAR_FORMAT "\e[0m"
	#define LOG_DULL_FORMAT "\e[2m"
#elif __unix__ // all unices not caught above
	#define LOG_ERROR_FORMAT "\e[1m\e[31m"
	#define LOG_WARNING_FORMAT "\e[1m\e[33m"
	#define LOG_INFO_FORMAT "\e[1m\e[36m"
	#define LOG_VERBOSE_FORMAT "\e[1m\e[37m"
	#define LOG_LOADING_FORMAT "\e[1m\e[32m"
	#define LOG_CLEAR_FORMAT "\e[0m"
	#define LOG_DULL_FORMAT "\e[2m"
#elif defined(_POSIX_VERSION)
	#define LOG_ERROR_FORMAT "\e[1m\e[31m"
	#define LOG_WARNING_FORMAT "\e[1m\e[33m"
	#define LOG_INFO_FORMAT "\e[1m\e[36m"
	#define LOG_VERBOSE_FORMAT "\e[1m\e[37m"
	#define LOG_LOADING_FORMAT "\e[1m\e[32m"
	#define LOG_CLEAR_FORMAT "\e[0m"
	#define LOG_DULL_FORMAT "\e[2m"
#else
	#error "Unknown compiler"
#endif 

using namespace std;
enum LOG_TYPE {
	ERROR = 0,
	WARN = 1,
	INFO = 2,
	VERBOSE = 3,
	LOADING = 4
};
class Log {
private:
	ofstream logFile;
	bool logErr = false;
	bool logWrn = false;
	bool logInf = false;
	bool logVrb = false;
	bool logLod = false;
	bool logTme = false;
	bool logCmp = false;
	bool logFErr = false;
	bool logFWrn = false;
	bool logFInf = false;
	bool logFVrb = false;
	bool logFTme = false;
	bool logFCmp = false;
	bool logToFile = false;
	int activeLoading = -1;
	char* loadMessage;
	char loadStates[4] = {'|', '/', '-', '\\'};
	chrono::system_clock::time_point lastLoadIteration;
	chrono::system_clock::time_point start;
	void init(int flags) {
		start = chrono::system_clock::now();
		if(flags & 0x1) {logErr = true;}
		if((flags >> 1) & 0x1) {logWrn = true;}
		if((flags >> 2) & 0x1) {logInf = true;}
		if((flags >> 3) & 0x1) {logVrb = true;}
		if((flags >> 4) & 0x1) {logLod = true;}
		if((flags >> 5) & 0x1) {logFErr = true;}
		if((flags >> 6) & 0x1) {logFWrn = true;}
		if((flags >> 7) & 0x1) {logFInf = true;}
		if((flags >> 8) & 0x1) {logFVrb = true;}
		if((flags >> 9) & 0x1) {logTme = true;}
		if((flags >> 10) & 0x1) {logCmp = true;}
		if((flags >> 11) & 0x1) {logFTme = true;}
		if((flags >> 12) & 0x1) {logFCmp = true;}
	}
public:
	char* logFilename;
	string timeFormat = "%x %X";
	float loadInterval = .2;
	Log(int flags) {init(flags);};
	Log(char* file, int flags) {
		init(flags);
		logFilename = file;
		logFile.open(logFilename, ios::app);
		if(!logFile.is_open()) {
			cout << LOG_ERROR_FORMAT << LOG_ERROR_LABEL << "Failed to open " << logFilename << endl;
		}
		logToFile = true;
	}
	Log(string file, int flags) {
		init(flags);
		logFilename = const_cast<char*>(file.c_str());
		logFile.open(logFilename, ios::app);
		if(!logFile.is_open()) {
			cout << LOG_ERROR_FORMAT << LOG_ERROR_LABEL << "Failed to open " << logFilename << endl;
		}
		logToFile = true;
	}
	~Log() {
		if(logFile.is_open()) {
			logFile.close();
		}
	}
	void log(LOG_TYPE t, string msg) {
		log(t,const_cast<char*>(msg.c_str()));
	}
	void log(LOG_TYPE t, const char* msg) {
		string format = "";
		string label = "";
		string cLabel = "";
		string fileLabel = "";
		switch(t) {
			case ERROR:
				format = LOG_ERROR_FORMAT;
				label = fileLabel = LOG_ERROR_LABEL;
				break;
			case WARN:
				format = LOG_WARNING_FORMAT;
				label = fileLabel = LOG_WARNING_LABEL;
				break;
			case INFO:
				format = LOG_INFO_FORMAT;
				label = fileLabel = LOG_INFO_LABEL;
				break;
			case VERBOSE:
				format = LOG_VERBOSE_FORMAT;
				label = fileLabel = LOG_VERBOSE_LABEL;
				break;
			case LOADING:
				format = LOG_LOADING_FORMAT;
				label = fileLabel = LOG_LOADING_LABEL;
		}
		auto current = chrono::system_clock::now();
		cLabel = fileLabel = label;
		if(logCmp | logFCmp) {
			chrono::duration<double> elapsed = current-start;
			if(logCmp) cLabel = string(LOG_DULL_FORMAT) + "[" + to_string(elapsed.count()) + "s] " + LOG_CLEAR_FORMAT + format + cLabel;
			if(logFCmp) fileLabel = "[" + to_string(elapsed.count()) + "s] " + fileLabel;
		}
		if(logTme | logFTme) {
			time_t time = chrono::system_clock::to_time_t(current);
			struct tm * timeinfo = localtime(&time);
			char date[50];
			strftime(date,50,timeFormat.c_str(),timeinfo);
			if(logTme) cLabel = string(LOG_DULL_FORMAT) + "[" + string(date) + "] " + LOG_CLEAR_FORMAT + format + cLabel;
			if(logFTme) fileLabel = "[" + string(date) + "] " + fileLabel;
		}
		if((t == ERROR && logErr) || (t == WARN && logWrn) || (t == INFO && logInf) || (t == VERBOSE && logVrb) || (t == LOADING && logLod)) {
			cout << format << cLabel << msg << LOG_CLEAR_FORMAT << endl;
		}
		if(logToFile) {
			if((t == ERROR && logFErr) || (t == WARN && logFWrn) || (t == INFO && logFInf) || (t == VERBOSE && logFVrb)) {
				if(logFile.is_open()) {
					logFile << fileLabel << msg << endl;
				}
			}
		}
	}

	void error(string msg) {
		error(const_cast<char*>(msg.c_str()));
	}
	void error(const char* msg) {
		log(ERROR,msg);
	}

	void warn(string msg) {
		warn(const_cast<char*>(msg.c_str()));
	}
	void warn(const char* msg) {
		log(WARN,msg);
	}

	void info(string msg) {
		info(const_cast<char*>(msg.c_str()));
	}
	void info(const char* msg) {
		log(INFO,msg);
	}

	void verbose(string msg) {
		verbose(const_cast<char*>(msg.c_str()));
	}
	void verbose(char* msg) {
		log(VERBOSE,msg);
	}

	void load(string msg) {
		load(const_cast<char*>(msg.c_str()));
	}
	void load(char* msg) {
		if(activeLoading > -1) {endLoad();}
		activeLoading = 0;
		loadMessage = msg;
		lastLoadIteration = chrono::system_clock::now();
		cout << LOG_LOADING_FORMAT << flush;
		// while(i > -1) {
		// 	auto current = chrono::system_clock::now();
		// 	chrono::duration<double> elapsed = current-last;
		// 	if(elapsed.count() > .2) {
		// 		char c = states[i%4];
		// 		cout << "\r" << LOG_LOADING_LABEL << msg << "  " << c << flush;
		// 		--i;
		// 		last = chrono::system_clock::now();
		// 	}
		// }
		loading();
	}
	void loading() {
		auto current = chrono::system_clock::now();
		chrono::duration<double> elapsed = current-lastLoadIteration;
		if(activeLoading > -1) {
			if(elapsed.count() >= loadInterval) {
				++activeLoading;
				cout << "\r" << LOG_LOADING_LABEL << loadMessage << "  " << loadStates[activeLoading%4] << flush;
				lastLoadIteration = chrono::system_clock::now();
			}
			loading();
		}
	}
	void endLoad() {
		activeLoading = -1;
		cout << "\r" << LOG_LOADING_LABEL << loadMessage << "    " << flush;
		cout << LOG_CLEAR_FORMAT << endl;
		log(INFO, "Done Loading");
	}
};

#endif