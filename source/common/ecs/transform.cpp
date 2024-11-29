#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our {

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const {
        //TODO: (Req 3) Write this function
        glm::mat4 scale_matrix = glm::mat4(1.0f), rotate_matrix = glm::mat4(1.0f), translate_matrix = glm::mat4(1.0f);

        // Right multiplicative: glm::scale( X, vec3 ) = X * glm::scale( Identity, vec3 )
        scale_matrix = glm::scale(scale_matrix, scale);

        // Yaw (Y-axis rotation), Pitch (X-axis rotation), Roll (Z-axis rotation)
        rotate_matrix = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);

        // glm::translate( X, vec3 ) = X * glm::translate( Identity, vec3 )
        translate_matrix = glm::translate(translate_matrix, position);

        // TRS = Scaling, Rotation then Translation
        return translate_matrix * rotate_matrix * scale_matrix; 
    }

     // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale    = data.value("scale", scale);
    }

}