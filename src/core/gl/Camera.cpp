#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
    Camera::width = width;
    Camera::height = height;
    Position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, std::unique_ptr<Shader>& shader, const char *uniform)
{
    // Initializes matrices since otherwise they will be the null matrix
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // Makes camera look in the right direction from the right position
    
    if(!DEBUG) {
      view = glm::lookAt(Position, LookAt, Up);
    } else {
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

    if (keyState[SDL_SCANCODE_W])
    {
        Position += speed * Orientation;
    }
    if (keyState[SDL_SCANCODE_A])
    {
        Position += speed * -glm::normalize(glm::cross(Orientation, Up));
    }
    if (keyState[SDL_SCANCODE_S])
    {
        Position += speed * -Orientation;
    }
    if (keyState[SDL_SCANCODE_D])
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
    if (keyState[SDL_SCANCODE_LCTRL])
    {
        speed = 0.4f;
    }
    else
    {
        speed = 0.1f;
    }


    if(isMouse) {

    // Handles mouse inputs
    int mouseX, mouseY;
    Uint32 mouseButtons = SDL_GetMouseState(&mouseX, &mouseY);
    if (mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        if (firstClick) {
            // Center the mouse cursor
            SDL_WarpMouseInWindow(window, width / 2, height / 2);
            firstClick = false;
            SDL_ShowCursor(SDL_DISABLE); // Hide the mouse cursor
        } else {
            // SDL doesn't directly give us the previous position of the cursor,
            // so we compute the motion based on the current position and centering the cursor.
            int deltaX = mouseX - width / 2;
            int deltaY = mouseY - height / 2;

            // Process mouse look
            ProcessMouseLook(deltaX, deltaY, window);

            // Re-center the mouse cursor to avoid reaching the screen edge
            SDL_WarpMouseInWindow(window, width / 2, height / 2);
        }
    } else {
        if (!firstClick) {
            // Show the cursor when the left button is released and reset firstClick
            SDL_ShowCursor(SDL_ENABLE);
            firstClick = true;
        }
    }

    }   
}

void Camera::ProcessMouseLook(int mouseXRel, int mouseYRel, SDL_Window* window) {
    // Assuming firstClick has been handled outside this function

    // Convert the relative mouse movement into rotation angles
    float yaw = mouseXRel * sensitivity * 0.001f; // Adjust sensitivity as needed
    float pitch = mouseYRel * sensitivity * 0.001f; // Adjust sensitivity as needed

    // Apply yaw - Rotate around the global up axis (Y-axis)
    glm::vec3 right = glm::normalize(glm::cross(Orientation, Up));
    Orientation = glm::rotate(Orientation, glm::radians(-yaw), Up);

    // Calculate pitch - Rotate around the camera's right axis
    glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-pitch), right);

    // Prevent flipping over at the zenith/nadir (top/bottom)
    if (glm::dot(newOrientation, Up) < 0.99f && glm::dot(newOrientation, Up) > -0.99f) {
        Orientation = newOrientation;
    }

    // Update LookAt based on the new orientation
    LookAt = Position + Orientation;
}

