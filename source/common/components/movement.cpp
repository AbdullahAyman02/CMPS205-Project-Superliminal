#include "movement.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    // Reads linearVelocity & angularVelocity from the given json object
    void MovementComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        linearVelocity = data.value("linearVelocity", linearVelocity);
        // angularVelocity = glm::radians(data.value("angularVelocity", angularVelocity));

        // Convert the euler rotation to a quaternion (in radians)
        glm::vec3 euler_rotation = data.value("angularVelocity", glm::vec3(0, 0, 0));
        angularVelocity = glm::quat(0, glm::radians(euler_rotation.x), glm::radians(euler_rotation.y), glm::radians(euler_rotation.z));
    }
}