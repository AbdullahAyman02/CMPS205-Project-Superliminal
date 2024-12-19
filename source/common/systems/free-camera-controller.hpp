#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/rigid-body.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    // The free camera controller system is responsible for moving every entity which contains a FreeCameraControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic.
    // For more information, see "common/components/free-camera-controller.hpp"
    class FreeCameraControllerSystem
    {
        Application *app;          // The application in which the state runs
        bool mouse_locked = false; // Is the mouse locked

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app)
        {
            this->app = app;
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent
        void update(World *world, float deltaTime)
        {
            // First of all, we search for an entity containing both a CameraComponent and a FreeCameraControllerComponent
            // As soon as we find one, we break
            CameraComponent *camera = nullptr;
            FreeCameraControllerComponent *controller = nullptr;
            for (auto entity : world->getEntities())
            {
                camera = entity->getComponent<CameraComponent>();
                controller = entity->getComponent<FreeCameraControllerComponent>();
                if (camera && controller)
                    break;
            }
            // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            if (!(camera && controller))
                return;
            // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
            Entity *entity = camera->getOwner();

            // If the left mouse button is pressed, we lock and hide the mouse. This common in First Person Games.
            if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && !mouse_locked)
            {
                app->getMouse().lockMouse(app->getWindow());
                mouse_locked = true;
                // If the left mouse button is released, we unlock and unhide the mouse.
            }
            else if (!app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && mouse_locked)
            {
                app->getMouse().unlockMouse(app->getWindow());
                mouse_locked = false;
            }

            // We get a reference to the entity's position and rotation
            // glm::vec3& position = entity->localTransform.position;
            // glm::vec3& rotation = entity->localTransform.rotation;

            // Retrieve r3d position and rotation then convert them to glm types for calculation purposes.
            const r3d::Vector3 &pos = entity->localTransform.getPosition();
            const r3d::Quaternion orien = entity->localTransform.getOrientation();
            glm::vec3 position(pos.x, pos.y, pos.z);
            glm::quat rotation(orien.w, orien.x, orien.y, orien.z);

            // If the left mouse button is pressed, we get the change in the mouse location
            // and use it to update the camera rotation
            if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1))
            {
                glm::vec2 delta = app->getMouse().getMouseDelta();
                // rotation.x -= delta.y * controller->rotationSensitivity; // The y-axis controls the pitch
                // rotation.y -= delta.x * controller->rotationSensitivity; // The x-axis controls the yaw

                // TODO: needs revision (mainly regarding gimble lock i think)
                float pitch = -delta.y * controller->rotationSensitivity;
                float yaw = -delta.x * controller->rotationSensitivity;

                glm::quat yawQuat = glm::angleAxis(yaw, glm::vec3(0, 1, 0));

                // To prevent 360 degrees rotation (breaking the neck xD) we limit the pitch to 90 degrees
                // We also prevent the pitch from exceeding a certain angle from the XZ plane to prevent gimbal locks
                glm::mat4 M = camera->getOwner()->getLocalToWorldMatrix();
                // Compute true view matrix
                glm::vec3 eye = glm::vec3(M * glm::vec4(0, 0, 0, 1));
                glm::vec3 center = glm::vec3(M * glm::vec4(0, 0, -1, 1));
                glm::vec3 forward = glm::normalize(center - eye);

                // Check if the forward is above or below the vertical axis, if it is, we reset the pitch to 0 degrees
                if (glm::dot(forward, glm::vec3(0, 1, 0)) > 0.75f && pitch > 0)
                    pitch = 0.0f;
                else if (glm::dot(forward, glm::vec3(0, -1, 0)) > 0.75f && pitch < 0)
                    pitch = 0.0f;

                glm::quat pitchQuat = glm::angleAxis(pitch, glm::vec3(1, 0, 0));

                rotation = yawQuat * rotation * pitchQuat; // Not sure about this order
                rotation = glm::normalize(rotation);
            }

            // We prevent the pitch from exceeding a certain angle from the XZ plane to prevent gimbal locks
            // if (rotation.x < -glm::half_pi<float>() * 0.99f)
            //     rotation.x = -glm::half_pi<float>() * 0.99f;
            // if (rotation.x > glm::half_pi<float>() * 0.99f)
            //     rotation.x = glm::half_pi<float>() * 0.99f;
            // // This is not necessary, but whenever the rotation goes outside the 0 to 2*PI range, we wrap it back inside.
            // // This could prevent floating point error if the player rotates in single direction for an extremely long time.
            // rotation.y = glm::wrapAngle(rotation.y);

            // We update the camera fov based on the mouse wheel scrolling amount
            float fov = camera->fovY + app->getMouse().getScrollOffset().y * controller->fovSensitivity;
            fov = glm::clamp(fov, glm::pi<float>() * 0.01f, glm::pi<float>() * 0.99f); // We keep the fov in the range 0.01*PI to 0.99*PI
            camera->fovY = fov;

            // We get the camera model matrix (relative to its parent) to compute the front, up and right directions
            glm::mat4 matrix = entity->localTransform.toMat4();

            // TODO: needs revision
            glm::vec3 front = glm::normalize(glm::vec3(matrix * glm::vec4(0, 0, -1, 0)));
            front.y = 0; // We don't want the camera to move up when we move forward, so we project on the xz plane to prevent any slope differences.
            front = glm::normalize(front);
            glm::vec3 right = glm::normalize(glm::vec3(matrix * glm::vec4(1, 0, 0, 0)));
            right.y = 0; // We don't want the camera to move up when we move right, so we project on the xz plane to prevent any slope differences.
            right = glm::normalize(right);
            glm::vec3 up = glm::vec3(0, 1, 0); // The up vector is always (0, 1, 0) in world space

            glm::vec3 current_sensitivity = controller->positionSensitivity;
            // If the LEFT SHIFT key is pressed, we multiply the position sensitivity by the speed up factor
            if (app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT))
                current_sensitivity *= controller->speedupFactor;

            // We change the camera position based on the keys WASD/QE
            // S & W moves the player back and forth
            if (app->getKeyboard().isPressed(GLFW_KEY_W))
                position += front * (deltaTime * current_sensitivity.z);
            if (app->getKeyboard().isPressed(GLFW_KEY_S))
                position -= front * (deltaTime * current_sensitivity.z);
            // Q & E moves the player up and down
            if (app->getKeyboard().isPressed(GLFW_KEY_Q))
                position += up * (deltaTime * current_sensitivity.y);
            if (app->getKeyboard().isPressed(GLFW_KEY_E))
                position -= up * (deltaTime * current_sensitivity.y);
            // A & D moves the player left or right
            if (app->getKeyboard().isPressed(GLFW_KEY_D))
                position += right * (deltaTime * current_sensitivity.x);
            if (app->getKeyboard().isPressed(GLFW_KEY_A))
                position -= right * (deltaTime * current_sensitivity.x);

            // We set the entity's position and rotation to the new values
            entity->localTransform.setPosition(position);
            entity->localTransform.setOrientation(r3d::Quaternion(rotation.x, rotation.y, rotation.z, rotation.w));
            RigidBodyComponent *rigidBody = entity->getComponent<RigidBodyComponent>();
            if (rigidBody && rigidBody->getRigidBody())
            {

                glm::quat orientation = rotation;

                glm::vec3 forward = glm::rotate(orientation, glm::vec3(0, 0, -1));
                forward.y = 0;
                forward = glm::normalize(forward);

                float angle = glm::acos(glm::dot(forward, glm::vec3(0, 0, -1)));
                glm::vec3 cross = glm::cross(forward, glm::vec3(0, 0, -1));

                if(cross.y < 0)
                    angle = -angle;

                glm::quat rotationY = glm::angleAxis(angle, glm::vec3(0, 1, 0));
                r3d::Quaternion orientationQuat(rotationY.x, rotationY.y, rotationY.z, rotationY.w);

                r3d::Transform transform = rigidBody->getRigidBody()->getTransform();
                transform.setPosition(entity->localTransform.getPosition() + rigidBody->relativePosition);
                transform.setOrientation(orientationQuat);
                rigidBody->getRigidBody()->setTransform(transform);
            }
        }

        // When the state exits, it should call this function to ensure the mouse is unlocked
        void
        exit()
        {
            if (mouse_locked)
            {
                mouse_locked = false;
                app->getMouse().unlockMouse(app->getWindow());
            }
        }
    };
}
