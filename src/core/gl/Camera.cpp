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

    if (!DEBUG)
    {
        view = glm::lookAt(Position, LookAt, Up);
    }
    else
    {
        view = glm::lookAt(Position, Position + Orientation, Up);
    }
    //  Adds perspective to the scene
    projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

    viewProjection = projection * view;

    // Exports the camera matrix to the Vertex Shader
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, uniform), 1, GL_FALSE, glm::value_ptr(viewProjection));
}

void Camera::Inputs(SDL_Window *window, bool isMouse)
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
        speed = 1.2f;
    }
    else
    {
        speed = 0.4f;
    }

    if (isMouse)
    { // Let the Mouse handle the camera or not

        // Handles mouse inputs
        int mouseX, mouseY;
        Uint32 mouseButtons = SDL_GetMouseState(&mouseX, &mouseY);

        if (firstClick)
        {
            // Center the mouse cursor
            SDL_WarpMouseInWindow(window, width / 2, height / 2);
            firstClick = false;
            SDL_ShowCursor(SDL_DISABLE); // Hide the mouse cursor
        }
        else
        {
            // SDL doesn't directly give us the previous position of the cursor,
            // so we compute the motion based on the current position and centering the cursor.
            int deltaX = mouseX - width / 2;
            int deltaY = mouseY - height / 2;

            // Process mouse look
            ProcessMouseLook(deltaX, deltaY, window);

            // Re-center the mouse cursor to avoid reaching the screen edge
            SDL_WarpMouseInWindow(window, width / 2, height / 2);
        }
    }
}

void Camera::VehicleFollowCamera(float pX, float pY, float pZ)
{
    auto targetVec = glm::vec3(pX + 1.0f, pY + 3.0f, pZ - 5.0f); //* Camera Offset
    auto dirVec = targetVec - Position;
     
     if (glm::distance2(targetVec, Position) > 0.02f)
        Position += dirVec * 0.03f;

      glm::vec3 lookAtPosition = glm::vec3(pX, pY, pZ);
      
      LookAt = lookAtPosition;
}

void Camera::ProcessMouseLook(int mouseXRel, int mouseYRel, SDL_Window *window)
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
