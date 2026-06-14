
#pragma once

#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <memory>

// mesh is only used through smart pointers here, so a forward declaration is
// enough -- no need to pull in mesh.hpp (and all of GLEW/GLFW) via this header.
class mesh;

namespace OKengine {
    class entity {

        private:
        std::shared_ptr<mesh> entity_mesh; //MESH
        glm::vec3 curr_pos;                //current centroid position

        public:

        static constexpr glm::vec3 DEFAULT_LOCATION = glm::vec3(0.0f);

        /**
         * constructor, initializes all parameters except mesh
         */
        entity();

        /**
         * constructor, initializes all parameters except mesh
         * @param position the entities location
         */
        entity(glm::vec3 position);

        /**
         * constructor, initializes all parameters
         * @param new_mesh the entities mesh
         */
        entity(std::shared_ptr<mesh> new_mesh);

        /**
         * constructor, initializes all parameters
         * @param position the entities location
         * @param new_mesh the entities mesh
         */
        entity(glm::vec3 position, std::shared_ptr<mesh> new_mesh);

        /**
         * placeholder for if entities have any memory ownership
         */
        ~entity(){};

        /**
         * Description: Used to assign the current location of the entity directly
         * Throws: Invalid Location
         * @param position (x,y,z) float vector for the location of the centroid
         */
        void set_curr_position(glm::vec3 position);

        /**
         * Description: Used to assign the mesh of the entity directly
         * Throws: Invalid Mesh
         * @param new_mesh new mesh pointer to replace the old one
         */
        void set_mesh(std::shared_ptr<mesh> new_mesh);

        /**
         * Description: Gets current (x,y,z) position of the entity.
         * @return pointer to (x,y,z) the current position of the entity.
         */
        glm::vec3* get_curr_position();

        /**
         * Description: Gets weak point to the mesh of the entity.
         * TODO: THROW EXCEPTION when no mesh in entity
         * @return weak copy pointer to the mesh inside the entity
         */
        std::weak_ptr<mesh> get_mesh();
    };
}
