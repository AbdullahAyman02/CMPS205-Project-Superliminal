#pragma once

#include "../ecs/component.hpp"
#include "../mesh/mesh.hpp"
#include "../material/material.hpp"
#include "../asset-loader.hpp"

namespace our {

    // This component denotes that any renderer should draw the given mesh using the given material at the transformation of the owning entity.
    class LightComponent : public Component {
    public:
        glm::vec4 color;
        int type;
        float inner_angle, outer_angle;

        // The ID of this component type is "Mesh Renderer"
        static std::string getID() { return "Light"; }

        // Receives the mesh & material from the AssetLoader by the names given in the json object
        void deserialize(const nlohmann::json& data) override;
    };

}