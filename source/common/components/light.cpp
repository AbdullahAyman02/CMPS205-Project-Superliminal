#include "light.hpp"
#include "deserialize-utils.hpp"

namespace our {
    // Receives the mesh & material from the AssetLoader by the names given in the json object
    void LightComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        
        color = data.value("color", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        type = data.value("type", 0);
        inner_angle = data.value("inner_angle", 0.0f);
        outer_angle = data.value("outer_angle", 0.0f);
    }
}