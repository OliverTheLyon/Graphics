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

#include <utility>

namespace OKengine {

        // The base entity constructor handles curr_pos and the mesh; movable_entity
        // only initializes its own members. Init-list order matches declaration order.
        movable_entity::movable_entity()
                : entity(DEFAULT_LOCATION),
                  last_pos_(DEFAULT_LOCATION),
                  acceleration_(DEFAULT_ACCELERATION),
                  mass_(DEFAULT_MASS) {}

        movable_entity::movable_entity(glm::vec3 position)
                : entity(position),
                  last_pos_(position),
                  acceleration_(DEFAULT_ACCELERATION),
                  mass_(DEFAULT_MASS) {}

        movable_entity::movable_entity(glm::vec3 position, std::shared_ptr<mesh> new_mesh)
                : entity(position, std::move(new_mesh)),
                  last_pos_(position),
                  acceleration_(DEFAULT_ACCELERATION),
                  mass_(DEFAULT_MASS) {}

        movable_entity::movable_entity(glm::vec3 curr_position, glm::vec3 last_position, std::shared_ptr<mesh> new_mesh)
                : entity(curr_position, std::move(new_mesh)),
                  last_pos_(last_position),
                  acceleration_(DEFAULT_ACCELERATION),
                  mass_(DEFAULT_MASS) {}

        movable_entity::movable_entity(std::shared_ptr<mesh> new_mesh)
                : entity(DEFAULT_LOCATION, std::move(new_mesh)),
                  last_pos_(DEFAULT_LOCATION),
                  acceleration_(DEFAULT_ACCELERATION),
                  mass_(DEFAULT_MASS) {}

        void movable_entity::set_last_position(glm::vec3 position) {
                //TODO SET Invalid Location, Overflow check, Speed Trap
                last_pos_ = position;
        }

        void movable_entity::set_position(glm::vec3 position) {
                //TODO CHECK VALID LOCATION
                set_curr_position(position);
                set_last_position(position);
        }

        void movable_entity::set_acceleration(glm::vec3 new_acceleration) {
                //TODO CHECK Overflow check, Speed Trap
                acceleration_ = new_acceleration;
        }

        void movable_entity::set_mass(float new_mass) {
                //TODO Invalid Input (negative floats, 0 and inf)
                mass_ = new_mass;
        }

        glm::vec3 movable_entity::get_last_position() const {
                return last_pos_;
        }

        glm::vec3 movable_entity::get_acceleration() const {
                return acceleration_;
        }

        float movable_entity::get_mass() const {
                return mass_;
        }

        void movable_entity::add_force(const glm::vec3 &force) {
                //TODO Determine how much force should be applied (multiplicative adjustment)
                acceleration_ += (FORCE_REDUCTION *  force);
        }

}
