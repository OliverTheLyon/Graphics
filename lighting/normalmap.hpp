
#include "../texture.hpp"
#include "../renderable.hpp"
#include <string>

namespace OKengine{
	class normal_map : public renderable{
		private:
			// pointer to the actual normal map
			texture map;
		public:
			/**
			 * @brief constructor
			 *
			 * @param path: a string outlining the path to the texture being used
			 * as a normal map
			 **/
			normal_map(std::string path);
		
			/**
			 * @brief the method which enables the graphics pipeline to make use
			 * of this map.
			 **/
			void render() override;

			/**
			 * @brief a method to get the id of the texture of this normalmap
			 **/
			int getID();
	};
}
