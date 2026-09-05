

#include "normalmap.hpp"
#include "../logger.hpp"
#include <string>
using std::string;

OKengine::normal_map::normal_map(string path): map(path){
	OKengine::logger::GetInstance().log("[normal_map::normal_map] loading normal map from path: " + path, debug_level::DEBUG);
}

int OKengine::normal_map::getID(){
	OKengine::logger::GetInstance().log("[normal_map::getID] returning " + std::to_string(map.getID()), debug_level::DEBUG);
	return map.getID();
}

void OKengine::normal_map::render(){
	OKengine::logger::GetInstance().log("[normal_map::render] begin, binding normal map", debug_level::DEBUG);
	map.bind();
}
