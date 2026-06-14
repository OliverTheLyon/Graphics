#include "movable_entity.hpp"

#include <utility>

namespace OKengine {

        // The base entity constructor handles curr_pos and the mesh; movable_entity
        // only initializes its own members. Init-list order matches declaration order.
        movable_entity::movable_entity()
                : entity(DEFAULT_LOCATION),
                  last_pos(DEFAULT_LOCATION),
                  acceleration(DEFAULT_ACCELERATION),
                  mass(DEFAULT_MASS) {}

        movable_entity::movable_entity(glm::vec3 position)
                : entity(position),
                  last_pos(position),
                  acceleration(DEFAULT_ACCELERATION),
                  mass(DEFAULT_MASS) {}

        movable_entity::movable_entity(glm::vec3 position, std::shared_ptr<mesh> new_mesh)
                : entity(position, std::move(new_mesh)),
                  last_pos(position),
                  acceleration(DEFAULT_ACCELERATION),
                  mass(DEFAULT_MASS) {}

        movable_entity::movable_entity(glm::vec3 curr_position, glm::vec3 last_position, std::shared_ptr<mesh> new_mesh)
                : entity(curr_position, std::move(new_mesh)),
                  last_pos(last_position),
                  acceleration(DEFAULT_ACCELERATION),
                  mass(DEFAULT_MASS) {}

        movable_entity::movable_entity(std::shared_ptr<mesh> new_mesh)
                : entity(DEFAULT_LOCATION, std::move(new_mesh)),
                  last_pos(DEFAULT_LOCATION),
                  acceleration(DEFAULT_ACCELERATION),
                  mass(DEFAULT_MASS) {}

        void movable_entity::add_force(const glm::vec3 &force) {
                //TODO Determine how much force should be applied (multiplicative adjustment)
                acceleration += (FORCE_REDUCTION *  force);
        }

        void movable_entity::set_last_position(glm::vec3 position) {
                //TODO SET Invalid Location, Overflow check, Speed Trap
                last_pos = position;
        }

        void movable_entity::set_position(glm::vec3 position) {
                //TODO CHECK VALID LOCATION
                set_curr_position(position);
                set_last_position(position);
        }

        void movable_entity::set_acceleration(glm::vec3 new_acceleration) {
                //TODO CHECK Overflow check, Speed Trap
                acceleration = new_acceleration;
        }

        void movable_entity::set_mass(float new_mass) {
                //TODO Invalid Input (negative floats, 0 and inf)
                mass = new_mass;
        }

        glm::vec3 movable_entity::get_last_position() const {
                return last_pos;
        }

        glm::vec3 movable_entity::get_acceleration() const {
                return acceleration;
        }

        float movable_entity::get_mass() const {
                return mass;
        }

}
