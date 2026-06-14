#include "entity.hpp"

#include <utility>

namespace OKengine {

    // Member-initializer order must match declaration order: entity_mesh, curr_pos.
    entity::entity()
        : curr_pos(DEFAULT_LOCATION) {}

    entity::entity(glm::vec3 position)
        : curr_pos(position) {}

    entity::entity(std::shared_ptr<mesh> new_mesh)
        : entity_mesh(std::move(new_mesh)), curr_pos(DEFAULT_LOCATION) {}

    entity::entity(glm::vec3 position, std::shared_ptr<mesh> new_mesh)
        : entity_mesh(std::move(new_mesh)), curr_pos(position) {}

    void entity::set_curr_position(glm::vec3 position) {
        curr_pos = position;
    }

    void entity::set_mesh(std::shared_ptr<mesh> new_mesh) {
        entity_mesh = new_mesh;
    }

    glm::vec3 entity::get_curr_position() const {
        return curr_pos;
    }

    std::weak_ptr<mesh> entity::get_mesh() const {
        //TODO CHECK MESH EXISTS!
        return entity_mesh;
    }
}