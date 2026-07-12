
#pragma once

#include <memory>
#include <vector>

#include "mesh.hpp"
#include "light.hpp"


class scene{
	private:
		std::vector<std::shared_ptr<OKengine::mesh>> drawnObjects;
		std::vector<std::shared_ptr<light>> lightSources;

	public:
		scene();
		scene(std::vector<std::shared_ptr<OKengine::mesh>> meshSet);
		scene(std::vector<std::shared_ptr<OKengine::mesh>> meshSet, std::vector<std::shared_ptr<light>> lightSet);

		void renderScene();

		void addMesh(OKengine::mesh m);
		void addLight(light l);

		void removeMesh(OKengine::mesh m);
		void removeLight(light l);
};
