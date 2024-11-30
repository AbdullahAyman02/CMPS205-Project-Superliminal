#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our {

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const {
        //TODO: (Req 3) Write this function
        // glm::mat4 scale_matrix = glm::mat4(1.0f), rotate_matrix = glm::mat4(1.0f), translate_matrix = glm::mat4(1.0f);
        glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), scale);

        r3d::Quaternion orientation = transform.getOrientation();
        glm::mat4 rotate_matrix = glm::mat4_cast(glm::quat(orientation.w, orientation.x, orientation.y, orientation.z));    // This function constructs a 4x4 matrix from a quaternion.

        r3d::Vector3 position = transform.getPosition();
        glm::mat4 translate_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, position.z));

        // TRS = Scaling, Rotation then Translation
        return translate_matrix * rotate_matrix * scale_matrix; 
    }

     // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        // position = data.value("position", position);
        // rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale = data.value("scale", scale);

        glm::vec3 euler_rotation = data.value("rotation", glm::vec3(0, 0, 0));  // Read euler angles from app.jsonc, then convert them to radians, then to quaternions.
        transform.setOrientation(r3d::Quaternion::fromEulerAngles(glm::radians(euler_rotation.x), glm::radians(euler_rotation.y), glm::radians(euler_rotation.z)));

        glm::vec3 position = data.value("position", glm::vec3(0, 0, 0));
        transform.setPosition(r3d::Vector3(position.x, position.y, position.z));
    }

}