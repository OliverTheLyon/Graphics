
#include "scene.hpp"
#include "light.hpp"
#include "mesh.hpp"
using OKengine::mesh;
#include "logger.hpp"

#include <memory>
using std::shared_ptr;
#include <vector>
using std::vector;
#include <algorithm>

scene::scene(): drawnObjects(), lightSources(){}

scene::scene(vector<shared_ptr<mesh>> meshSet): drawnObjects(meshSet), lightSources(){}

scene::scene(vector<shared_ptr<mesh>> meshSet, vector<shared_ptr<light>> lightSet): drawnObjects(meshSet), lightSources(lightSet){}


void scene::renderScene(){
		OKengine::logger::GetInstance().log("[scene::draw] begin", debug_level::DEBUG);
		for(int i = 0; i < lightSources.size(); i += 1){
			lightSources[i]->prepareShadows();
			for(int j = 0; j < drawnObjects.size(); j += 1){
				drawnObjects[i]->draw();
			}
		}

		for(int i = 0; i < drawnObjects.size(); i += 1){
			drawnObjects[i]->draw();
		}
}


void scene::addMesh(mesh m){
		OKengine::logger::GetInstance().log("[scene::addMesh] begin", debug_level::DEBUG);
		drawnObjects.push_back(std::make_shared<OKengine::mesh>(std::move(m)));

}

void scene::addLight(light l){}


void scene::removeMesh(mesh m){
		OKengine::logger::GetInstance().log("[scene::removeMesh] begin", debug_level::DEBUG);
		auto pos = std::find_if(drawnObjects.begin(), drawnObjects.end(), [&](auto& ptr){return *ptr == m;});
		if(drawnObjects.end() == pos){
			OKengine::logger::GetInstance().log("[scene::removeMesh] OKengine::mesh not found", debug_level::WARN);
			return;
		}
		drawnObjects.erase(pos);
}

void scene::removeLight(light l){}

