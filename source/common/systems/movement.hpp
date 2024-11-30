#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"
#include "../components/rigid-body.hpp"
#include "../components/free-camera-controller.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <reactphysics3d/reactphysics3d.h>
namespace r3d = reactphysics3d;

namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/movement.hpp"
    class MovementSystem
    {
    public:
        // This should be called every frame to update all entities containing a MovementComponent.
        void update(World *world, float deltaTime)
        {

            // First, set the new transform matrices for all the entities of the physics world.
            r3d::PhysicsWorld *physicsWorld = world->getPhysicsWorld();
            if (!physicsWorld)
                return;

            // R3D Physics World has a constant frame rate of 60Hz
            const float timeStep = 1.0f / 60.0f;

            // Add the time difference in the accumulator
            float accumulator = deltaTime;

            // While there is enough accumulated time to take a step
            while (accumulator > 0)
            {
                // If the remaining time (accumulator) is less than the time step, we take a smaller time step
                float step = accumulator < timeStep ? accumulator : timeStep;

                // Update the Dynamics world with a constant time step
                physicsWorld->update(step);

                // Decrease the accumulated time
                accumulator -= step;
            }

            // For each entity in the world
            for (auto entity : world->getEntities())
            {
                // Get the movement component if it exists
                MovementComponent *movement = entity->getComponent<MovementComponent>();
                // If the movement component exists
                if (movement)
                {
                    // Change the position and rotation based on the linear & angular velocity and delta time.
                    // entity->localTransform.position += deltaTime * movement->linearVelocity;
                    // entity->localTransform.rotation += deltaTime * movement->angularVelocity;

                    // Retrieve current position and orientation
                    const r3d::Vector3 &position = entity->localTransform.getPosition();
                    const r3d::Quaternion &orientation = entity->localTransform.getOrientation();
                    glm::vec3 position_vec = glm::vec3(position.x, position.y, position.z);
                    glm::quat orientation_quat = glm::quat(orientation.w, orientation.x, orientation.y, orientation.z);

                    // Update position
                    position_vec += deltaTime * movement->linearVelocity;

                    // Convert angular velocity to quaternion derivative
                    glm::quat angular_velocity_quat = glm::quat(0.0f, movement->angularVelocity.x, movement->angularVelocity.y, movement->angularVelocity.z);
                    glm::quat orientation_derivative = 0.5f * angular_velocity_quat * orientation_quat;

                    // Update orientation
                    orientation_quat += deltaTime * orientation_derivative;

                    // Normalize the quaternion
                    orientation_quat = glm::normalize(orientation_quat);

                    // Update the entity's transform
                    entity->localTransform.setPosition(position_vec);
                    entity->localTransform.setOrientation(r3d::Quaternion(orientation_quat.x, orientation_quat.y, orientation_quat.z, orientation_quat.w));
                }

                // Update the rigid body (if it exists)
                RigidBodyComponent *rigidBody = entity->getComponent<RigidBodyComponent>();
                if (rigidBody)
                {
                    FreeCameraControllerComponent *controller = entity->getComponent<FreeCameraControllerComponent>();
                    if (controller)
                    {
                        r3d::Transform transform = rigidBody->getRigidBody()->getTransform();
                        r3d::Quaternion q = entity->localTransform.getOrientation();
                        // Update the rigid body's position only, keeping the orientation constant.
                        // I'm still not entirely sure why but it was mentioned in the docs somewhere...
                        transform.setOrientation(q);
                        entity->localTransform.setTransform(transform);
                    }
                    else
                    {
                        // Update the entity's rigid body
                        entity->localTransform.setTransform(rigidBody->getRigidBody()->getTransform());
                    }
                }
            }
        }
    };
}
