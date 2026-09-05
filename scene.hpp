
#pragma once

#include <memory>
#include <vector>

#include "mesh.hpp"
#include "lighting/light.hpp"

class camera;

class scene{
	private:
		std::vector<std::shared_ptr<OKengine::mesh>> drawnObjects;
		std::vector<std::shared_ptr<light>> lightSources;
		std::shared_ptr<camera> cam_;

		float fov_ = 45.0f;
		float near_ = 0.1f;
		float far_ = 10.0f;

	public:
		scene();
		scene(std::vector<std::shared_ptr<OKengine::mesh>> meshSet);
		scene(std::vector<std::shared_ptr<OKengine::mesh>> meshSet, std::vector<std::shared_ptr<light>> lightSet);

		void renderScene(int width, int height);

		void addMesh(OKengine::mesh m);
		void addLight(light l);

		void removeMesh(OKengine::mesh m);
		void removeLight(light l);

		void setCamera(std::shared_ptr<camera> c);
};
