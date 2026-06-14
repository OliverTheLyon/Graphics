#include "movable_entity.hpp"

namespace OKengine {

        movable_entity::movable_entity() {
                last_pos = OKengine::entity::DEFAULT_LOCATION;
                acceleration = DEFAULT_ACCELERATION;
                mass = DEFAULT_MASS;
                set_curr_position(glm::vec3(0.0f));
        }

        movable_entity::movable_entity(glm::vec3 position) {
                last_pos = position;
                acceleration = DEFAULT_ACCELERATION;
                mass = DEFAULT_MASS;
                set_curr_position(position);
        }

        movable_entity::movable_entity(glm::vec3 position, std::shared_ptr<mesh> new_mesh) {
                last_pos = position;
                acceleration = DEFAULT_ACCELERATION;
                mass = DEFAULT_MASS;
                set_curr_position(position);
                set_mesh(new_mesh);
        }

        movable_entity::movable_entity(glm::vec3 curr_position, glm::vec3 last_position, std::shared_ptr<mesh> new_mesh) {
                last_pos = last_position;
                acceleration = DEFAULT_ACCELERATION;
                mass = DEFAULT_MASS;
                set_curr_position(curr_position);
        }


        movable_entity::movable_entity(std::shared_ptr<mesh> new_mesh) {
                last_pos = OKengine::entity::DEFAULT_LOCATION;
                acceleration = DEFAULT_ACCELERATION;
                mass = DEFAULT_MASS;
                set_curr_position(OKengine::entity::DEFAULT_LOCATION);
        }

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

        void movable_entity::set_mesh(std::shared_ptr<mesh> new_mesh) {
                //TODO Invalid Input (check mesh is fully computable)
                mesh = new_mesh;
        }

        glm::vec3* movable_entity::get_last_position() {
                return(&last_pos);
        }

        glm::vec3* movable_entity::get_acceleration() {
              return(&acceleration);
        }

        float movable_entity::get_mass() {
                return(mass);
        }

}
