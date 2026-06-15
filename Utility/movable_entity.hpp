/**
 * @file movable_entity.hpp
 * @brief Entity with physics state: velocity (via previous position), acceleration, and mass.
 *
 * Declares OKengine::movable_entity, an entity subclass for objects that move
 * and respond to forces. Adds the state needed for Verlet-style integration on
 * top of the base entity's transform.
 *
 * Part of OKengine.
 * @author Oliver Lyon
 * @date 2026-06-14
 */

#pragma once

#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>

#include "entity.hpp"   // base class; also forward-declares mesh, used here only via shared_ptr


namespace OKengine {
    class movable_entity: public OKengine::entity {
    private:
        //~~~~~~~~~~~~~~~~VARIABLES~~~~~~~~~~~~~~~~

        glm::vec3 last_pos_;         //last centroid position
        glm::vec3 acceleration_;     //current state of acceleration
        float mass_;                 //used for force calculations

    public:
        static constexpr float DEFAULT_MASS = 1.0f;
        static constexpr glm::vec3 DEFAULT_ACCELERATION = glm::vec3(0.0f);
        static constexpr float FORCE_REDUCTION = 0.9f;

        //~~~~~~~~~~~~~~~~CONSTRUCTORS~~~~~~~~~~~~~~~~

        /**
         * @brief Constructs a movable entity at the default location (origin)
         * with default mass, zero acceleration, and no mesh attached.
         */
        movable_entity();

        /**
         * @brief Constructs a movable entity at the given position with default
         * mass, zero acceleration, and no mesh. The previous position is seeded to the
         * same value, so the initial velocity is zero.
         * @param position (x,y,z) starting location of the entity's centroid
         */
        movable_entity(glm::vec3 position);

        /**
         * @brief Constructs a movable entity at the given position owning the
         * given mesh, with default mass and zero acceleration.
         * @param position (x,y,z) starting location of the entity's centroid
         * @param new_mesh shared mesh the entity will render with
         */
        movable_entity(glm::vec3 position, std::shared_ptr<mesh> new_mesh);

        /**
         * @brief Constructs a movable entity with distinct current and previous
         * positions. The gap between them seeds the initial velocity for Verlet
         * integration. Owns the given mesh, with default mass and zero acceleration.
         * @param curr_position (x,y,z) current location of the centroid
         * @param last_position (x,y,z) previous location; (current - last) is the implied velocity
         * @param new_mesh      shared mesh the entity will render with
         */
        movable_entity(glm::vec3 curr_position, glm::vec3 last_position, std::shared_ptr<mesh> new_mesh);

        /**
         * @brief Constructs a movable entity at the default location (origin)
         * owning the given mesh, with default mass and zero acceleration.
         * @param new_mesh shared mesh the entity will render with
         */
        movable_entity(std::shared_ptr<mesh> new_mesh);

        /**
         * @brief Virtual destructor so movable entities are destroyed correctly
         * through a base-class pointer. Defaulted -- members clean up after themselves.
         */
        ~movable_entity() override = default;

        //~~~~~~~~~~~~~~~~SETTERS~~~~~~~~~~~~~~~~

        /**
         * @brief Used to assign the previous location of the entity directly. Used to force velocity
         * Throws: Invalid Location, Overflow check, Speed Trap
         * @param position (x,y,z) vector showing location for the centroid was last placed
         */
        void set_last_position(glm::vec3 position);

        /**
         * @brief Used to assign the current and previous location of the entity directly.
         * Throws: Invalid Location
         * @param position (x,y,z) vector showing location for the centroid will be placed
         */
        void set_position(glm::vec3 position);

        /**
         * @brief Sets the entity's acceleration directly, replacing the current value.
         * Throws: Overflow check, Speed Trap
         * @param new_acceleration (x,y,z) acceleration vector to apply
         */
        void set_acceleration(glm::vec3 new_acceleration);

        /**
         * @brief Sets the entity's mass, used in force/acceleration calculations.
         * Throws: Invalid Input (negative floats, 0 and inf)
         * @param new_mass new mass of entity (probably do unsigned int and convert when needed)
         */
        void set_mass(float new_mass);

        //~~~~~~~~~~~~~~~~GETTERS~~~~~~~~~~~~~~~~

        /**
         * @brief Gets last (x,y,z) position of the entity.
         * @return copy of (x,y,z) last position of the entity.
         */
        glm::vec3 get_last_position() const;

        /**
         * @brief Gets (x,y,z) acceleration of the entity.
         * @return copy of (x,y,z) acceleration of the entity.
         */
        glm::vec3 get_acceleration() const;

        /**
         * @brief Gets mass of the entity.
         * @return float mass
         */
        float     get_mass() const;

        //~~~~~~~~~~~~~~~~UTILITY~~~~~~~~~~~~~~~~

        /**
         * @brief This is a shorthand used to add forces to the object (collision)
         * Throws: invalid location, overflow catch
         * @param force (x,y,z) vector showing direction and intensity of force on entity
         */
        void add_force(const glm::vec3 &force);

    };
}
