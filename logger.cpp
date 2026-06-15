#include "logger.hpp"

#include <array>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

using std::string;

namespace OKengine {

	namespace {
		// Human-readable tag for each debug_level, indexed by its enum value.
		std::string level_tag(debug_level level){
			static const std::array<std::string, 4> tags = {
				"Information", // INFO  = 0
				"Debug",       // DEBUG = 1
				"Warning",     // WARN  = 2
				"Error"        // ERROR = 3
			};
			return tags[static_cast<std::size_t>(level)];
		}
	}

	logger::logger(string path, debug_level dl): detail(dl), logfile(path){
		outstream = std::ofstream(logfile);
		log("[logger::logger] logger initialized, log file: " + path, debug_level::INFO);
	}


	void logger::log(const std::string &msg, debug_level level){
		//lock for multi threading
		std::lock_guard<std::mutex> lock(log_mutex_);

		//early stop no need to run more if what we try to log does not matter
		if (level < detail) return;

		if(!outstream.is_open()){
			throw std::runtime_error("[logger::log]: output file is not open");
		}

		auto now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(now);

		std::tm tm_buf;
		localtime_r(&t, &tm_buf); //thread safe version

		std::ostringstream oss;
		oss << std::put_time(&tm_buf, "%d/%m/%Y %H:%M:%S");

		string human_time = oss.str();
		string out = "[" + level_tag(level) + "][" + human_time + "]: " + msg + "\n";

		outstream.write(out.c_str(), out.size());
		outstream.flush();
	}


	void logger::SetLevel(debug_level lvl){
		log("[logger::SetLevel] changing log level", debug_level::INFO);
		std::lock_guard<std::mutex> lock(log_mutex_); //lock so we don't mutate
		detail = lvl;
	}


	logger::~logger(){
		log("[logger::~logger] shutting down logger", debug_level::INFO);
		std::lock_guard<std::mutex> lock(log_mutex_);
		if(outstream.is_open()){
			outstream.flush();
			outstream.close();
		}
	}
}
