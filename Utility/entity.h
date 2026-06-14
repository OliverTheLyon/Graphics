
#pragma once

#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>


namespace OKengine {
    class entity {

        private:
        std::shared_ptr<mesh> mesh; //MESH
        glm::vec3 curr_pos;         //current centroid position

        public:

        static constexpr glm::vec3 DEFAULT_LOCATION = glm::vec3(0.0f);

        /**
         * generic constructor
         */
        entity();

        /**
         *
         * @param pos the entities mesh
         */
        entity(glm::vec3 position);

        /**
         *
         * @param new_mesh the entities mesh
         */
        entity(std::shared_ptr<mesh> new_mesh);

        /**
         *
         * @param new_mesh the entities mesh
         */
        entity(glm::vec3 position, std::shared_ptr<mesh> new_mesh);

        /**
         * placeholder for if entities have any memory ownership
         */
        ~entity(){};

        /**
         *
         * @param position (x,y,z) float vector for the location of the centroid
         */
        void set_curr_position(glm::vec3 position);

        /**
         *
         * @param new_mesh new mesh pointer to replace the old one
         */
        void set_mesh(std::shared_ptr<mesh> new_mesh);

        /**
         *
         * @return returns a pointer to the 3 float vector holding the current position
         */
        glm::vec3* get_curr_position();

        /**
         * TODO: THROW EXCEPTION when no mesh in entity
         * @return weak copy pointer to the mesh inside the entity
         */
        std::weak_ptr<mesh> get_mesh();
    };
}
