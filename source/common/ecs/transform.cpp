#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.inl>

namespace our {

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const {
        //TODO: (Req 3) Write this function
        // glm::mat4 scale_matrix = glm::mat4(1.0f), rotate_matrix = glm::mat4(1.0f), translate_matrix = glm::mat4(1.0f);
        glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), scale);
        float matrix[16];
        transform.getOpenGLMatrix(matrix);
        glm::mat4 transform_matrix = glm::make_mat4(matrix);

        // TRS = Scaling, Rotation then Translation
        return transform_matrix * scale_matrix; 
    }

     // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        // position = data.value("position", position);
        // rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale = data.value("scale", scale);

        glm::vec3 rotation(0, 0, 0);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));  // Read euler angles from app.jsonc, then convert them to radians, then to quaternions.
        glm::quat yawQuat = glm::angleAxis(rotation.y, glm::vec3(0, 1, 0));
        glm::quat pitchQuat = glm::angleAxis(rotation.x, glm::vec3(1, 0, 0));
        glm::quat rollQuat = glm::angleAxis(rotation.z, glm::vec3(0, 0, 1));
        glm::quat rotationQuat = yawQuat * pitchQuat * rollQuat;
        transform.setOrientation(r3d::Quaternion(rotationQuat.x, rotationQuat.y, rotationQuat.z, rotationQuat.w));

        glm::vec3 position = data.value("position", glm::vec3(0, 0, 0));
        transform.setPosition(r3d::Vector3(position.x, position.y, position.z));
    }

}