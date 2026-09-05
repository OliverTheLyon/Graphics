#pragma once
#include <fstream>
#include <string>
#include <mutex>
#include <glm/ext/vector_float3.hpp>

enum class debug_level { INFO=0, DEBUG=1,  WARN=2, ERROR=3};
namespace OKengine {

	/**
	 * @brief Formats a glm::vec3 as "(x, y, z)" for embedding in log messages.
	 **/
	inline std::string vec3_to_string(const glm::vec3 & v){
		return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
	}

	class logger{

	private:
		std::ofstream outstream;
		debug_level detail;
		std::string logfile;
		std::mutex log_mutex_;
		
		/**
		 * @brief constructor for the logger singleton class 
		 *
		 * @param path (optional): the path to the output file, defaults to program.log in the current working directory.
		 * @param dl (optional): the debug_level to filter logging by, defaults to INFO (the lowest).
		 * note: higher debug levels will log messages from lower debug_levels but not from higher ones.
		 **/
		logger(std::string path="program.log", debug_level dl=debug_level::INFO);
	public:


		/**
		 * @brief A method to get a reference to the singleton
		 **/
		~logger();
		static logger& GetInstance(){
			static logger instance;
			return instance;
		};

		/**
		 * @brief A method to change the debug level of the logger.
		 *
		 * @param lvl: the debug_level desired.
		 **/
		void SetLevel(debug_level lvl);

		/**
		 * @brief A method to append to the log file.
		 *
		 * @param msg: the string to be appended to the log file.
		 * @param level: the debug_level of the message, will be prepended in 
		 * the log file.
		 *
		 * Note: messages parameters have been formatted as "[class::method] message" 
		 * thus far by convention.
		 **/
		void log(const std::string & msg, debug_level level);
	};
}
