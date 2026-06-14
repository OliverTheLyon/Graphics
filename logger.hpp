#pragma once
#include <fstream>
#include <string>
#include <iostream>

enum class debug_level { INFO=0, DEBUG=1,  WARN=2, ERROR=3};
namespace OKengine {
	class logger{

	private:
		std::string logfile;
		debug_level detail;
		std::ofstream outstream;

		
		
		logger(std::string path="program.log", debug_level dl=debug_level::INFO);
	public:


		~logger();
		static logger& GetInstance(){
			static logger instance;
			return instance;
		};

		void SetLevel(debug_level lvl);
		void log(std::string msg, debug_level level);
	};
}