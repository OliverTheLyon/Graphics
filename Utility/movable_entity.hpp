
#pragma once

#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>

#include "mesh.hpp"
#include "entity.hpp"


namespace OKengine {
    class movable_entity: public OKengine::entity {
    private:
        glm::vec3 last_pos;         //last centroid position
        glm::vec3 acceleration;     //current state of acceleration
        float mass;                 //used for force calculations

    public:
        static constexpr float DEFAULT_MASS = 1.0f;
        static constexpr glm::vec3 DEFAULT_ACCELERATION = glm::vec3(0.0f);
        static constexpr float FORCE_REDUCTION = 0.9f;

        /**
         * constructor, initializes all parameters except mesh
         */
        movable_entity();

        /**
         * constructor, initializes all parameters except mesh
         * @param position the entities location
         */
        movable_entity(glm::vec3 position);

        /**
         * constructor, initializes all parameters
         */
        movable_entity(glm::vec3 position, std::shared_ptr<mesh> new_mesh);

        /**
         * constructor, initializes all parameters
         */
        movable_entity(glm::vec3 curr_position, glm::vec3 last_position, std::shared_ptr<mesh> new_mesh);

        /**
         * constructor, initializes all parameters
         */
        movable_entity(std::shared_ptr<mesh> new_mesh);

        /**
         * placeholder for if entities have any memory ownership
         */
        virtual ~movable_entity() = default;

        /**
         * Description: This is a shorthand used to add forces to the object (collision)
         * Throws: invalid location, overflow catch
         * @param force (x,y,z) vector showing direction and intensity of force on entity
         */
        void add_force(const glm::vec3 &force);

        /**
         * Description: Used to assign the previous location of the entity directly. Used to force velocity
         * Throws: Invalid Location, Overflow check, Speed Trap
         * @param position (x,y,z) vector showing location for the centroid was last placed
         */
        void set_last_position(glm::vec3 position);

        /**
         * Description: Used to assign the current and previous location of the entity directly.
         * Throws: Invalid Location
         * @param position (x,y,z) vector showing location for the centroid will be placed
         */
        void set_position(glm::vec3 position);

        /**
         * Description: Sets the acceleration of the entity.
         * Throws: Overflow check, Speed Trap
         * @param new_acceleration
         */
        void set_acceleration(glm::vec3 new_acceleration);

        /**
         * Description: Sets the acceleration of the entity.
         * Throws: Invalid Input (negative floats, 0 and inf)
         * @param mass float, new mass of entity (probably do unsinged int and convert when needed)
         */
        void set_mass(float new_mass);

        /**
         * Description: Gets last (x,y,z) position of the entity.
         * @return copy of (x,y,z) last position of the entity.
         */
        glm::vec3 get_last_position() const;

        /**
         * Description: Gets (x,y,z) acceleration of the entity.
         * @return copy of (x,y,z) acceleration of the entity.
         */
        glm::vec3 get_acceleration() const;

        /**
         * Description: Gets mass of the entity.
         * @return float mass
         */
        float     get_mass() const;

    };
}
