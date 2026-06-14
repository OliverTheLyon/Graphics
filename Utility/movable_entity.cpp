#include "movable_entity.hpp"

namespace OKengine {

        movable_entity::movable_entity() {

        }

        /**
         * constructor, initializes all parameters
         */
        movable_entity(std::shared_ptr<mesh> new_mesh);

        /**
         * placeholder for if entities have any memory ownership
         */
        ~movable_entity(){};

        /**
         * Description: This is a shorthand used to add forces to the object (collision)
         * Throws: invalid location, overflow catch
         * @param force (x,y,z) vector showing direction and intensity of force on entity
         */
        void add_force(const glm::vec3 &force);

        /**
         * Description: Used to assign the current location entity directly.
         * Throws: Invalid Location, Overflow check, Speed Trap
         * @param position (x,y,z) vector showing location for the centroid to be placed
         */
        void set_curr_position(glm::vec3 position);

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
        void set_mass(float mass);

        /**
         * Description: Sets the Mesh of the entity.
         * Throws: Invalid Input (check mesh is fully computable)
         * @param new_mesh shared pointer to mesh class
         */
        void set_mesh(std::shared_ptr<mesh> new_mesh);

        /**
         * Description: Gets current (x,y,z) position of the entity.
         * @return pointer to (x,y,z) the current position of the entity.
         */
        glm::vec3* get_curr_position();

        /**
         * Description: Gets last (x,y,z) position of the entity.
         * @return pointer to (x,y,z) last position of the entity.
         */
        glm::vec3* get_last_position();

        /**
         * Description: Gets (x,y,z) acceleration of the entity.
         * @return pointer to (x,y,z) acceleration of the entity.
         */
        glm::vec3* get_acceleration();

        /**
         * Description: Gets mass of the entity.
         * @return float mass
         */
        float      get_mass();

        /**
         * Description: Gets weak point to the mesh of the entity.
         * @return weak pointer to mesh of the entity.
         */
        std::weak_ptr<mesh> get_mesh();
}
