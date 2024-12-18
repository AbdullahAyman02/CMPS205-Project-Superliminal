#pragma once

// #include "../ecs/world.hpp"
#include "../components/rigid-body.hpp"

#include <reactphysics3d/reactphysics3d.h>
namespace r3d = reactphysics3d;

namespace our
{
    class ButtonContactListener : public r3d::EventListener
    {
    public:
        void onContact(const r3d::CollisionCallback::CallbackData &callbackData) override
        {
            for (uint32_t i = 0; i < callbackData.getNbContactPairs(); i++)
            {
                auto pair = callbackData.getContactPair(i);
                // Check if the pair involves the button entity
                if (isButton(static_cast<const r3d::RigidBody*>(pair.getBody1())))
                {
                    // Button is being stepped on
                    onButtonPressed(static_cast<Entity*>(pair.getBody1()->getUserData()));
                }
                else if (isButton(static_cast<const r3d::RigidBody*>(pair.getBody2())))
                {
                    // Button is being stepped on
                    onButtonPressed(static_cast<Entity*>(pair.getBody2()->getUserData()));
                }
            }
        }

        bool isButton(const r3d::RigidBody *body)
        {
            // Check if this is the button object
            Entity* entity = static_cast<Entity*>(body->getUserData());
            return entity->name == "Button";
        }

        void onButtonPressed(Entity* button)
        {
            // Logic for what happens when button is pressed
            // activateDoorOrCube();
            World* world = button->getWorld();
            for (auto entity : world->getEntities())
            {
                if (entity->name == "Cube")
                {
                    // Move the cube
                    r3d::RigidBody* cubeBody = entity->getComponent<RigidBodyComponent>()->getRigidBody();
                    r3d::Vector3 position = cubeBody->getTransform().getPosition();
                    position.y += 5.0f;
                    cubeBody->setTransform(r3d::Transform(position, cubeBody->getTransform().getOrientation()));

                    entity->localTransform.setPosition(glm::vec3(position.x, position.y, position.z));
                }
            }
        }
    };
}