/**
 * @file movable_entity.cpp
 * @brief Implementation of OKengine::movable_entity.
 *
 * Defines the movable_entity constructors and its physics accessors/mutators
 * (force application, position/acceleration_/mass_) declared in movable_entity.hpp.
 *
 * Part of OKengine.
 * @author Oliver Lyon
 * @date 2026-06-14
 */

#include "movable_entity.hpp"
#include "logger.hpp"

#include <cmath>
#include <utility>

namespace OKengine {

        // The base entity constructor handles curr_pos and the mesh; movable_entity
        // only initializes its own members. Init-list order matches declaration order.
        movable_entity::movable_entity()
                : entity(DEFAULT_LOCATION),
                  last_pos_(DEFAULT_LOCATION),
                  acceleration_(DEFAULT_ACCELERATION),
                  mass_(DEFAULT_MASS) {
                logger::GetInstance().log("[movable_entity::movable_entity] default constructor", debug_level::DEBUG);
        }

        movable_entity::movable_entity(glm::vec3 position)
                : entity(position),
                  last_pos_(position),
                  acceleration_(DEFAULT_ACCELERATION),
                  mass_(DEFAULT_MASS) {
                logger::GetInstance().log("[movable_entity::movable_entity] constructor with position " + vec3_to_string(position), debug_level::DEBUG);
        }

        movable_entity::movable_entity(glm::vec3 position, std::shared_ptr<mesh> new_mesh)
                : entity(position, std::move(new_mesh)),
                  last_pos_(position),
                  acceleration_(DEFAULT_ACCELERATION),
                  mass_(DEFAULT_MASS) {
                logger::GetInstance().log("[movable_entity::movable_entity] constructor with position " + vec3_to_string(position) + " and mesh", debug_level::DEBUG);
        }

        movable_entity::movable_entity(glm::vec3 curr_position, glm::vec3 last_position, std::shared_ptr<mesh> new_mesh)
                : entity(curr_position, std::move(new_mesh)),
                  last_pos_(last_position),
                  acceleration_(DEFAULT_ACCELERATION),
                  mass_(DEFAULT_MASS) {
                logger::GetInstance().log("[movable_entity::movable_entity] constructor with position " + vec3_to_string(curr_position) + ", last position " + vec3_to_string(last_position) + " and mesh", debug_level::DEBUG);
        }

        movable_entity::movable_entity(std::shared_ptr<mesh> new_mesh)
                : entity(DEFAULT_LOCATION, std::move(new_mesh)),
                  last_pos_(DEFAULT_LOCATION),
                  acceleration_(DEFAULT_ACCELERATION),
                  mass_(DEFAULT_MASS) {
                logger::GetInstance().log("[movable_entity::movable_entity] constructor with mesh", debug_level::DEBUG);
        }

        void movable_entity::set_last_position(glm::vec3 position) {
                //TODO SET Invalid Location, Overflow check, Speed Trap
                logger::GetInstance().log("[movable_entity::set_last_position] begin, new last position " + vec3_to_string(position), debug_level::DEBUG);
                last_pos_ = position;
        }

        void movable_entity::set_position(glm::vec3 position) {
                //TODO CHECK VALID LOCATION
                logger::GetInstance().log("[movable_entity::set_position] begin, new position " + vec3_to_string(position), debug_level::DEBUG);
                set_curr_position(position);
                set_last_position(position);
        }

        void movable_entity::set_acceleration(glm::vec3 new_acceleration) {
                //TODO CHECK Overflow check, Speed Trap
                logger::GetInstance().log("[movable_entity::set_acceleration] begin, new acceleration " + vec3_to_string(new_acceleration), debug_level::DEBUG);
                acceleration_ = new_acceleration;
        }

        void movable_entity::set_mass(float new_mass) {
                //TODO Invalid Input (negative floats, 0 and inf)
                if (!std::isfinite(new_mass) || new_mass <= 0.f) {
                        logger::GetInstance().log("[movable_entity::set_mass] attempt to set invalid mass: " + std::to_string(new_mass), debug_level::WARN);
                }
                logger::GetInstance().log("[movable_entity::set_mass] begin, new mass: " + std::to_string(new_mass), debug_level::DEBUG);
                mass_ = new_mass;
        }

        glm::vec3 movable_entity::get_last_position() const {
                logger::GetInstance().log("[movable_entity::get_last_position] returning " + vec3_to_string(last_pos_), debug_level::DEBUG);
                return last_pos_;
        }

        glm::vec3 movable_entity::get_acceleration() const {
                logger::GetInstance().log("[movable_entity::get_acceleration] returning " + vec3_to_string(acceleration_), debug_level::DEBUG);
                return acceleration_;
        }

        float movable_entity::get_mass() const {
                logger::GetInstance().log("[movable_entity::get_mass] returning " + std::to_string(mass_), debug_level::DEBUG);
                return mass_;
        }

        void movable_entity::add_force(const glm::vec3 &force) {
                //TODO Determine how much force should be applied (multiplicative adjustment)
                logger::GetInstance().log("[movable_entity::add_force] begin, force " + vec3_to_string(force), debug_level::DEBUG);
                acceleration_ += (FORCE_REDUCTION *  force);
                logger::GetInstance().log("[movable_entity::add_force] success, resulting acceleration " + vec3_to_string(acceleration_), debug_level::DEBUG);
        }

}
