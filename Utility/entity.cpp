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
#include "logger.hpp"

#include <utility>

namespace OKengine {

    // Member-initializer order must match declaration order: entity_mesh, transform_.
    // The transform defaults to position 0, identity rotation, unit scale.
    entity::entity() {
        logger::GetInstance().log("[entity::entity] default constructor", debug_level::DEBUG);
    }

    entity::entity(glm::vec3 position)
        : transform_(position) {
        logger::GetInstance().log("[entity::entity] constructor with position " + vec3_to_string(position), debug_level::DEBUG);
    }

    entity::entity(std::shared_ptr<mesh> new_mesh)
        : entity_mesh(std::move(new_mesh)) {
        logger::GetInstance().log("[entity::entity] constructor with mesh (" + std::string(entity_mesh ? "set" : "null") + ")", debug_level::DEBUG);
    }

    entity::entity(glm::vec3 position, std::shared_ptr<mesh> new_mesh)
        : entity_mesh(std::move(new_mesh)), transform_(position) {
        logger::GetInstance().log("[entity::entity] constructor with position " + vec3_to_string(position) + " and mesh (" + std::string(entity_mesh ? "set" : "null") + ")", debug_level::DEBUG);
    }

    void entity::set_curr_position(glm::vec3 position) {
        logger::GetInstance().log("[entity::set_curr_position] begin, new position " + vec3_to_string(position), debug_level::DEBUG);
        transform_.set_position(position);
    }

    void entity::set_mesh(std::shared_ptr<mesh> new_mesh) {
        if (new_mesh == nullptr) {
            logger::GetInstance().log("[entity::set_mesh] attempt to set null mesh", debug_level::WARN);
        }
        logger::GetInstance().log("[entity::set_mesh] begin, replacing mesh", debug_level::DEBUG);
        entity_mesh = std::move(new_mesh);
    }

    glm::vec3 entity::get_curr_position() const {
        glm::vec3 pos = transform_.position();
        logger::GetInstance().log("[entity::get_curr_position] returning " + vec3_to_string(pos), debug_level::DEBUG);
        return pos;
    }

    transform& entity::get_transform() {
        logger::GetInstance().log("[entity::get_transform] returning mutable transform", debug_level::DEBUG);
        return transform_;
    }

    const transform& entity::get_transform() const {
        logger::GetInstance().log("[entity::get_transform] returning const transform", debug_level::DEBUG);
        return transform_;
    }

    const glm::mat4& entity::get_model_matrix() const {
        bool was_dirty = transform_.matrix_is_dirty();
        const glm::mat4 & model = transform_.matrix();
        logger::GetInstance().log("[entity::get_model_matrix] returning model matrix" + std::string(was_dirty ? " (rebuilt)" : " (cached)"), debug_level::DEBUG);
        return model;
    }

    std::weak_ptr<mesh> entity::get_mesh() const {
        //TODO CHECK MESH EXISTS!
        if (entity_mesh == nullptr) {
            logger::GetInstance().log("[entity::get_mesh] mesh requested but not set", debug_level::WARN);
        }
        return entity_mesh;
    }
}
