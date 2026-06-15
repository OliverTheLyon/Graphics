 /**
 * @file entity.hpp
 * @brief Base game entity: a positioned object in the world that may carry a mesh.
 *
 * Declares OKengine::entity, the root of the entity hierarchy. Owns a transform
 * (position/rotation/scale) and an optional shared mesh, and serves as the
 * polymorphic base for specialized entities such as movable_entity.
 *
 * Part of OKengine.
 * @author Oliver Lyon
 * @date 2026-06-13
 */

#pragma once

#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <memory>

#include "transform.hpp"

class mesh;

namespace OKengine {
    class entity {

        private:
        //~~~~~~~~~~~~~~~~VARIABLES~~~~~~~~~~~~~~~~

        std::shared_ptr<mesh> entity_mesh; //MESH
        transform transform_;              //position / rotation / scale (+ cached model matrix)

        public:

        /** @brief Default world-space location (the origin) used when none is supplied. */
        static constexpr glm::vec3 DEFAULT_LOCATION = glm::vec3(0.0f);

        //~~~~~~~~~~~~~~~~CONSTRUCTORS~~~~~~~~~~~~~~~~

        /**
         * @brief Constructs an entity at the default location (origin) with no
         * mesh attached. The transform starts at identity rotation and unit scale.
         */
        entity();

        /**
         * @brief Constructs an entity at the given position with no mesh attached.
         * @param position (x,y,z) world-space location of the entity's centroid
         */
        entity(glm::vec3 position);

        /**
         * @brief Constructs an entity at the default location (origin) that owns
         * a reference to the given mesh.
         * @param new_mesh shared mesh the entity will render with
         */
        entity(std::shared_ptr<mesh> new_mesh);

        /**
         * @brief Constructs an entity at the given position that owns a reference
         * to the given mesh.
         * @param position (x,y,z) world-space location of the entity's centroid
         * @param new_mesh shared mesh the entity will render with
         */
        entity(glm::vec3 position, std::shared_ptr<mesh> new_mesh);

        /**
         * @brief Virtual destructor so derived entities are destroyed correctly
         * through a base-class pointer. Defaulted so the shared mesh and transform
         * clean up after themselves.
         */
        virtual ~entity() = default;

        //~~~~~~~~~~~~~~~~SETTERS~~~~~~~~~~~~~~~~

        /**
         * @brief Used to assign the current location of the entity directly
         * Throws: Invalid Location
         * @param position (x,y,z) float vector for the location of the centroid
         */
        void set_curr_position(glm::vec3 position);

        /**
         * @brief Used to assign the mesh of the entity directly
         * Throws: Invalid Mesh
         * @param new_mesh new mesh pointer to replace the old one
         */
        void set_mesh(std::shared_ptr<mesh> new_mesh);

        //~~~~~~~~~~~~~~~~GETTERS~~~~~~~~~~~~~~~~

        /**
         * @brief Gets current (x,y,z) position of the entity.
         * @return copy of (x,y,z) the current position of the entity.
         */
        glm::vec3 get_curr_position() const;

        /**
         * @brief Gets the entity's transform (position / rotation / scale)
         * for direct manipulation of orientation and scale.
         * @return mutable reference to the entity's transform
         */
        transform& get_transform();

        /**
         * @brief Gets the entity's transform for read-only access.
         * @return const reference to the entity's transform
         */
        const transform& get_transform() const;

        /**
         * @brief Gets the composed model matrix for rendering. Cached by
         * the transform and only rebuilt when the transform changes.
         * @return const reference to the (T * R * S) model matrix
         */
        const glm::mat4& get_model_matrix() const;

        /**
         * @brief Gets weak point to the mesh of the entity.
         * @return weak copy pointer to the mesh inside the entity
         */
        std::weak_ptr<mesh> get_mesh() const;
    };
}
