#include"Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform)
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
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(viewProjection));
}

void Camera::Inputs(SDL_Window* window)
{
    // Handles key inputs
    const Uint8* keyState = SDL_GetKeyboardState(NULL);

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

    // Handles mouse inputs
    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

	//Trackpad just doesn't work...

    if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        // Hides mouse cursor
        SDL_ShowCursor(SDL_DISABLE);

        // Prevents camera from jumping on the first click
        if (firstClick)
        {
            SDL_WarpMouseInWindow(window, width / 2, height / 2);
            firstClick = false;
        }

        // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
        // and then "transforms" them into degrees 
        float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
        float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

        // Calculates upcoming vertical change in the Orientation
        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

        // Decides whether or not the next vertical Orientation is legal or not
        if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
        {
            Orientation = newOrientation;
        }

        // Rotates the Orientation left and right
        Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

        // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
        SDL_WarpMouseInWindow(window, width / 2, height / 2);
    }
    else
    {
        // Unhides cursor since the camera is not looking around anymore
        SDL_ShowCursor(SDL_ENABLE);
        // Makes sure the next time the camera looks around it doesn't jump
        firstClick = true;
    }
}
