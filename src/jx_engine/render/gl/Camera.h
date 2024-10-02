#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include <memory> // For std::unique_ptr

//For User Input on Windows
#include <SDL2/SDL.h>

#include "jx_engine/event/Event.hpp"
#include "jx_engine/logs/Logger.hpp"

#include"shaderClass.h"

class Camera
{
public:
	// Stores the main vectors of the camera
	bool DEBUG;
	
	bool freeCamera = false;

	glm::vec3 Position;
	glm::vec3 getCameraPosition();
	void setCameraPosition(glm::vec3 pos);

	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 LookAt;
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 viewProjection;

	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;

	// Stores the width and height of the window
	int width;
	int height;

	// Adjust the speed of the camera and it's sensitivity when looking around
	float speed = 1.0f;
	float sensitivity = 50.0f;

	// Camera constructor to set up initial values
	Camera(int width, int height, glm::vec3 position);

	// Updates and exports the camera matrix to the Vertex Shader
	void Matrix(float FOVdeg, float nearPlane, float farPlane, std::shared_ptr<Shader>& shader, const char* uniform);
	
	void Inputs();
	void ProcessMouseLook(int mouseXRel, int mouseYRel);
	void UpdateScreenSize(int w, int h);

	void VehicleFollowCamera(float pX, float pY, float pZ, float backX, float backY, float backZ, bool IsLookingFront);

	void handleVehicleFollowEvent(const Event& event);
	void handleToggleFreeCamEvent(const Event& event);

	void GenerateRay(glm::vec3& rayStart, glm::vec3& rayEnd, float rayLength);

	void setFrontLook(bool fl);
	bool frontLook = false;
};
#endif