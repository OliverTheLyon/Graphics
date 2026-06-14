#include "logger.hpp"

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Utility/entity.hpp"

using std::string;

namespace OKengine {
	logger::logger(string path, debug_level dl): detail(dl), logfile(path){
		outstream = std::ofstream(logfile);
		log("[logger::logger] logger initialized, log file: " + path, debug_level::INFO);
	}


	void logger::log(std::string msg, debug_level level){
		if(!outstream.is_open()){
			throw std::runtime_error("[logger::log]: output file is not open");
		}
		auto now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(now);
		std::ostringstream oss;
		oss << std::put_time(std::localtime(&t), "%d/%m/%Y %H:%M:%S");
		string human_time = oss.str();
		string out;
		switch(level){
			case debug_level::ERROR:
				if(level < detail){
					return;
				}
				out = "[Error][" + human_time + "]: " + msg;
				break;
			case debug_level::WARN:
				if(level < detail){
					return;
				}
				out = "[Warning][" + human_time + "]: " + msg;
				break;
			case debug_level::INFO:
				if(level < detail){
					return;
				}
				out = "[Information][" + human_time + "]: " + msg;
				break;
			case debug_level::DEBUG:
				if(level < detail){
					return;
				}
				out = "[Debug][" + human_time + "]: " + msg;
				break;
			default:
				break;
		}
		out += "\n";
		if(outstream.is_open()){
			outstream.write(out.c_str(), out.size());
			outstream.flush();
		}
		else {
			std::cerr << out;
		}
	}


	void logger::SetLevel(debug_level lvl){
		log("[logger::SetLevel] changing log level", debug_level::INFO);
		detail = lvl;
	}


	logger::~logger(){
		if(outstream.is_open()){
			log("[logger::~logger] shutting down logger", debug_level::INFO);
			outstream.flush();
			outstream.close();
		}
	}
}
