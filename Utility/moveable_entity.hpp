
#pragma once

#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>

#include "mesh.hpp"


class moveable_entity {
    private:
        std::shared_ptr<mesh> mesh; //MESH
        glm::vec3 curr_pos;         //current centroid position
        glm::vec3 last_pos;         //last centroid position
        glm::vec3 acceleration;     //current state of acceleration
        float mass;                 //used for force calculations

    public:

        moveable_entity();
        ~moveable_entity();

        void add_force(const glm::vec3 &force);

        void set_curr_position(glm::vec3 position);
        void set_last_position(glm::vec3 position);
        void set_acceleration();
        void set_mass(float mass);
        void set_mesh(std::shared_ptr<mesh> new_mesh);

        glm::vec3* get_curr_position();
        glm::vec3* get_last_position();
        glm::vec3* get_acceleration();
        float      get_mass();
        std::weak_ptr<mesh> get_mesh();
};

