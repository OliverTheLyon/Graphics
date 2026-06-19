#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <mutex>

enum class debug_level { INFO=0, DEBUG=1,  WARN=2, ERROR=3};
namespace OKengine {
	class logger{

	private:
		std::ofstream outstream;
		debug_level detail;
		std::string logfile;
		std::mutex log_mutex_;
		
		/**
		 * @brief TODO
		 **/
		logger(std::string path="program.log", debug_level dl=debug_level::INFO);
	public:


		/**
		 * @brief TODO
		 **/
		~logger();
		static logger& GetInstance(){
			static logger instance;
			return instance;
		};

		/**
		 * @brief TODO
		 **/
		void SetLevel(debug_level lvl);
		/**
		 * @brief TODO
		 **/
		void log(const std::string & msg, debug_level level);
	};
}
