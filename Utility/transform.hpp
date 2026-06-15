/**
 * @file transform.hpp
 * @brief 3D transform: position, rotation, and scale with a lazily cached model matrix.
 *
 * Defines OKengine::transform, the spatial state shared by anything that lives
 * in the game world. Stores translation/rotation/scale as the source of truth and
 * rebuilds the composed model matrix only when a component changes.
 *
 * Part of OKengine.
 * @author Oliver Lyon
 * @date 2026-06-14
 */

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace OKengine {

/**
 * @brief 3D transform (position, rotation, scale) with a lazily cached model matrix.
 *
 * Stores translation/rotation/scale as the source of truth
 * This is intended as header class to intuitively read and edit, and cleanly interpolatable.
 * We only rebuild the composed model matrix only when a component actually changes.
 */
class transform {
private:
    //~~~~~~~~~~~~~~~~VARIABLES~~~~~~~~~~~~~~~~

    glm::vec3 position_{0.0f};
    glm::quat rotation_{1.0f, 0.0f, 0.0f, 0.0f}; // identity (w, x, y, z)
    glm::vec3 scale_{1.0f};

    // Cache is recomputed on demand, so it stays mutable even on const access.
    mutable glm::mat4 cached_{1.0f};
    mutable bool dirty_{true};

public:
    //~~~~~~~~~~~~~~~~CONSTRUCTORS~~~~~~~~~~~~~~~~


    /** @brief Constructs an identity transform: position 0, identity rotation, unit scale. */
    transform() = default;

    /**
     * @brief Constructs a transform at the given position with identity rotation and unit scale.
     * @param position initial translation; rotation defaults to identity and
     *                 scale to 1.
     */
    explicit transform(glm::vec3 position) : position_(position) {}

    /**
     * @brief Constructs a transform from explicit position, rotation, and scale.
     * @param position initial translation
     * @param rotation initial orientation
     * @param scale    initial per-axis scale
     */
    transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
        : position_(position), rotation_(rotation), scale_(scale) {}

    //~~~~~~~~~~~~~~~~SETTERS~~~~~~~~~~~~~~~~

    /**
     * @brief Sets the absolute world position.
     * @param position new (x,y,z) translation
     */
    void set_position(glm::vec3 position)   { position_ = position; dirty_ = true; }
    /** @brief Sets the orientation. @param rotation new orientation quaternion */
    void set_rotation(glm::quat rotation)   { rotation_ = rotation; dirty_ = true; }
    /** @brief Sets the per-axis scale. @param scale new (x,y,z) scale factors */
    void set_scale(glm::vec3 scale)         { scale_ = scale;       dirty_ = true; }

    /** @brief Moves the transform by a delta instead of setting an absolute position. @param delta (x,y,z) offset to add */
    void translate(glm::vec3 delta)         { position_ += delta;   dirty_ = true; }

    //~~~~~~~~~~~~~~~~GETTERS~~~~~~~~~~~~~~~~

    /** @brief Gets the current position. @return current (x,y,z) world position. */
    glm::vec3 position() const { return position_; }
    /** @brief Gets the current orientation. @return current orientation as a quaternion. */
    glm::quat rotation() const { return rotation_; }
    /** @brief Gets the current scale. @return current (x,y,z) scale factors. */
    glm::vec3 scale()    const { return scale_; }

    /**
     * @brief Returns the composed model matrix (T * R * S).
     *
     * Rebuilt only when a component has changed since the last call, otherwise
     * the cached copy is returned.
     */
    const glm::mat4& matrix() const {
        if (dirty_) {
            cached_ = glm::translate(glm::mat4(1.0f), position_)
                    * glm::mat4_cast(rotation_)
                    * glm::scale(glm::mat4(1.0f), scale_);
            dirty_ = false;
        }
        return cached_;
    }
};

} // namespace OKengine
