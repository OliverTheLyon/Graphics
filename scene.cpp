
#include "scene.hpp"
#include "lighting/light.hpp"
#include "mesh.hpp"
using OKengine::mesh;
#include "camera.hpp"
#include "logger.hpp"

#include <memory>
using std::shared_ptr;
#include <vector>
using std::vector;
#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>

scene::scene(): drawnObjects(), lightSources(){
	OKengine::logger::GetInstance().log("[scene::scene] default constructor", debug_level::DEBUG);
}

scene::scene(vector<shared_ptr<mesh>> meshSet): drawnObjects(meshSet), lightSources(){
	OKengine::logger::GetInstance().log("[scene::scene] constructor with " + std::to_string(meshSet.size()) + " meshes", debug_level::DEBUG);
}

scene::scene(vector<shared_ptr<mesh>> meshSet, vector<shared_ptr<light>> lightSet): drawnObjects(meshSet), lightSources(lightSet){
	OKengine::logger::GetInstance().log("[scene::scene] constructor with " + std::to_string(meshSet.size()) + " meshes and " + std::to_string(lightSet.size()) + " lights", debug_level::DEBUG);
}


void scene::renderScene(int width, int height){
		OKengine::logger::GetInstance().log("[scene::renderScene] begin, viewport " + std::to_string(width) + "x" + std::to_string(height)
			+ " (" + std::to_string(drawnObjects.size()) + " objects, " + std::to_string(lightSources.size()) + " lights)", debug_level::DEBUG);
		OKengine::logger::GetInstance().log("[scene::renderScene] shadow mapping", debug_level::DEBUG);
		glCullFace(GL_FRONT);
		for(int i = 0; i < lightSources.size(); i += 1){
			if(!lightSources[i]->hasShadowShader()){
				continue;
			}
			lightSources[i]->prepareShadows();
			for(int j = 0; j < drawnObjects.size(); j += 1){
				drawnObjects[j]->draw(lightSources[i]->shadowShader());
			}
			lightSources[i]->cleanupShadows();
		}
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);


		OKengine::logger::GetInstance().log("[scene::renderScene] regular rendering", debug_level::DEBUG);
		for(int i = 0; i < lightSources.size(); i += 1){
			lightSources[i]->bind();
		}

		if(cam_ != nullptr){
			glm::mat4 proj = glm::perspective(glm::radians(fov_), (float)width/(float)height, near_, far_);
			glm::mat4 vp = proj * cam_->matrix();
			glm::vec3 viewPos = cam_->getPosition();
			for(int i = 0; i < drawnObjects.size(); i += 1){
				drawnObjects[i]->draw(vp, viewPos);
			}
		} else {
			for(int i = 0; i < drawnObjects.size(); i += 1){
				drawnObjects[i]->draw();
			}
		}
		OKengine::logger::GetInstance().log("[scene::renderScene] success, returning", debug_level::DEBUG);
}


void scene::addMesh(mesh m){
		OKengine::logger::GetInstance().log("[scene::addMesh] begin", debug_level::DEBUG);
		drawnObjects.push_back(std::make_shared<OKengine::mesh>(std::move(m)));
		OKengine::logger::GetInstance().log("[scene::addMesh] success, scene now holds " + std::to_string(drawnObjects.size()) + " objects", debug_level::DEBUG);

}

void scene::addLight(light l){
	OKengine::logger::GetInstance().log("[scene::addLight] begin", debug_level::DEBUG);
	lightSources.push_back(std::make_shared<light>(l));
	OKengine::logger::GetInstance().log("[scene::addLight] success, scene now holds " + std::to_string(lightSources.size()) + " lights", debug_level::DEBUG);
}


void scene::removeMesh(mesh m){
		OKengine::logger::GetInstance().log("[scene::removeMesh] begin", debug_level::DEBUG);
		auto pos = std::find_if(drawnObjects.begin(), drawnObjects.end(), [&](auto& ptr){return *ptr == m;});
		if(drawnObjects.end() == pos){
			OKengine::logger::GetInstance().log("[scene::removeMesh] OKengine::mesh not found", debug_level::WARN);
			return;
		}
		drawnObjects.erase(pos);
		OKengine::logger::GetInstance().log("[scene::removeMesh] success, scene now holds " + std::to_string(drawnObjects.size()) + " objects", debug_level::DEBUG);
}

void scene::removeLight(light l){
		OKengine::logger::GetInstance().log("[scene::removeLight] begin", debug_level::DEBUG);
	auto pos = std::find_if(lightSources.begin(), lightSources.end(), [&](auto & ptr){return *ptr == l;});
	if(pos == lightSources.end()){
		OKengine::logger::GetInstance().log("[scene::removeLight] OKengine::light not found", debug_level::WARN);
		return;
	}
	lightSources.erase(pos);
	OKengine::logger::GetInstance().log("[scene::removeLight] success, scene now holds " + std::to_string(lightSources.size()) + " lights", debug_level::DEBUG);
}

void scene::setCamera(std::shared_ptr<camera> c){
	OKengine::logger::GetInstance().log("[scene::setCamera] setting camera", debug_level::DEBUG);
	cam_ = c;
}

