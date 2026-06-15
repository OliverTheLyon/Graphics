/**
 * @file entity.cpp
 * @brief Implementation of OKengine::entity.
 *
 * Defines the entity constructors and the transform/mesh accessors declared in
 * entity.hpp.
 *
 * Part of OKengine.
 * @author Oliver Lyon
 * @date 2026-06-13
 */

#include "entity.hpp"

#include <utility>

namespace OKengine {

    // Member-initializer order must match declaration order: entity_mesh, transform_.
    // The transform defaults to position 0, identity rotation, unit scale.
    entity::entity() = default;

    entity::entity(glm::vec3 position)
        : transform_(position) {}

    entity::entity(std::shared_ptr<mesh> new_mesh)
        : entity_mesh(std::move(new_mesh)) {}

    entity::entity(glm::vec3 position, std::shared_ptr<mesh> new_mesh)
        : entity_mesh(std::move(new_mesh)), transform_(position) {}

    void entity::set_curr_position(glm::vec3 position) {
        transform_.set_position(position);
    }

    void entity::set_mesh(std::shared_ptr<mesh> new_mesh) {
        entity_mesh = std::move(new_mesh);
    }

    glm::vec3 entity::get_curr_position() const {
        return transform_.position();
    }

    transform& entity::get_transform() {
        return transform_;
    }

    const transform& entity::get_transform() const {
        return transform_;
    }

    const glm::mat4& entity::get_model_matrix() const {
        return transform_.matrix();
    }

    std::weak_ptr<mesh> entity::get_mesh() const {
        //TODO CHECK MESH EXISTS!
        return entity_mesh;
    }
}