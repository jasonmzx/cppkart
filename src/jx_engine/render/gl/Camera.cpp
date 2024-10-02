#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
    Camera::width = width;
    Camera::height = height;
    Position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, std::shared_ptr<Shader> &shader, const char *uniform)
{
    // Initializes matrices since otherwise they will be the null matrix
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // Makes camera look in the right direction from the right position
    view = glm::lookAt(Position, LookAt, Up);

    // Adds perspective to the scene
    projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

    viewProjection = projection * view;

    // Exports the camera matrix to the Vertex Shader
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, uniform), 1, GL_FALSE, glm::value_ptr(viewProjection));
}

void Camera::Inputs()
{
    // Handles key inputs
    const Uint8 *keyState = SDL_GetKeyboardState(NULL);

    if (keyState[SDL_SCANCODE_UP])
    {
        Position += speed * Orientation;
    }
    if (keyState[SDL_SCANCODE_LEFT])
    {
        Position += speed * -glm::normalize(glm::cross(Orientation, Up));
    }
    if (keyState[SDL_SCANCODE_DOWN])
    {
        Position += speed * -Orientation;
    }
    if (keyState[SDL_SCANCODE_RIGHT])
    {
        Position += speed * glm::normalize(glm::cross(Orientation, Up));
    }

    if (keyState[SDL_SCANCODE_SPACE])
    {
        Position += speed * Up;
    }
    if (keyState[SDL_SCANCODE_LSHIFT])
    {
        Position += speed * -Up;
    }
    if (keyState[SDL_SCANCODE_RCTRL])
    {
        speed = 5.2f;
    }
    else
    {
        speed = 0.5f;
    }

}

void Camera::VehicleFollowCamera(float pX, float pY, float pZ, float forwardX, float forwardY, float forwardZ, bool IsLookingFront) {

    glm::vec3 normalizedForwardVec = glm::vec3(forwardX, forwardY, forwardZ);

    glm::vec3 offset = normalizedForwardVec * 6.0f; // Scale down that vector's direction

    glm::vec3 heightOffset = glm::vec3(0.0f, 2.5f, 0.0f); // Add Height, Y up 

    if(IsLookingFront) {
        offset = -offset;
    }

    glm::vec3 targetVec = glm::vec3(pX, pY, pZ) - offset + heightOffset;

    float speedFactor = 0.05f; // As this value approaches 1, the clamp approaches the target, so less smooth
    Position = glm::mix(Position, targetVec, speedFactor); // LERP

    // Set the look-at position to the vehicle's position
    LookAt = glm::vec3(pX, pY, pZ);
}


void Camera::ProcessMouseLook(int mouseXRel, int mouseYRel)
{
    // Assuming firstClick has been handled outside this function

    // Convert the relative mouse movement into rotation angles
    float yaw = mouseXRel * sensitivity * 0.001f;   // Adjust sensitivity as needed
    float pitch = mouseYRel * sensitivity * 0.001f; // Adjust sensitivity as needed

    // Apply yaw - Rotate around the global up axis (Y-axis)
    glm::vec3 right = glm::normalize(glm::cross(Orientation, Up));
    Orientation = glm::rotate(Orientation, glm::radians(-yaw), Up);

    // Calculate pitch - Rotate around the camera's right axis
    glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-pitch), right);

    // Prevent flipping over at the zenith/nadir (top/bottom)
    if (glm::dot(newOrientation, Up) < 0.99f && glm::dot(newOrientation, Up) > -0.99f)
    {
        Orientation = newOrientation;
    }

    // Update LookAt based on the new orientation
    LookAt = Position + Orientation;
}

void Camera::GenerateRay(glm::vec3& rayStart, glm::vec3& rayEnd, float rayLength)
{
    rayStart = Position;
    glm::vec3 direction = glm::normalize(LookAt - Position);
    rayEnd = rayStart + direction * rayLength;
}


void Camera::setFrontLook(bool fl) {
    frontLook = fl;
}

void Camera::handleVehicleFollowEvent(const Event& event) {
    try {
        auto inputs = std::any_cast<std::tuple<float, float, float, float, float, float>>(event.data);
        float pX = std::get<0>(inputs);
        float pY = std::get<1>(inputs);
        float pZ = std::get<2>(inputs);

        // Back of the Vehicle, for camera offset (opposite of driving dir)

        float bX = std::get<3>(inputs);
        float bY = std::get<4>(inputs);
        float bZ = std::get<5>(inputs);


        if(!freeCamera) {
            VehicleFollowCamera(pX, pY, pZ, bX, bY, bZ, frontLook);
        }
        
    } catch (const std::bad_any_cast& e) {
        Logger::getInstance()->log(Logger::ERROR, "[Camera] Failed to cast event data: " + std::string(e.what()));
    }
}

void Camera::handleToggleFreeCamEvent(const Event& event) {
    try {
        auto inputs = std::any_cast<bool>(event.data);
        freeCamera = inputs;
        
    } catch (const std::bad_any_cast& e) {
        Logger::getInstance()->log(Logger::ERROR, "[Camera] Failed to cast event data: " + std::string(e.what()));
    }
}

void Camera::UpdateScreenSize(int w, int h)
{
    width = w;
    height = h;
}

glm::vec3 Camera::getCameraPosition() {
    return Position;
}

void Camera::setCameraPosition(glm::vec3 pos) {
    Position = pos;
}