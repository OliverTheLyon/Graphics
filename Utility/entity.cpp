#include "entity.h"

namespace OKengine {

    entity::entity() {
        curr_pos = glm::vec3(0.0f); //defult location
    }

    entity::entity(glm::vec3 position) {
        curr_pos = position;
    }

    entity::entity(std::shared_ptr<mesh> new_mesh) {
        curr_pos = glm::vec3(0.0f); //defult location
        entity_mesh = new_mesh;
    }

    entity::entity(glm::vec3 position, std::shared_ptr<mesh> new_mesh) {
        curr_pos = position;
        entity_mesh = new_mesh;
    }

    void entity::set_curr_position(glm::vec3 position) {
        curr_pos = position;
    }

    void entity::set_mesh(std::shared_ptr<mesh> new_mesh) {
        entity_mesh = new_mesh;
    }

    glm::vec3* entity::get_curr_position() {
        return &curr_pos;
    }

    std::weak_ptr<mesh> entity::get_mesh() {
        //TODO CHECK MESH EXISTS!
        return entity_mesh;
    }
}