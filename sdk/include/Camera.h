#pragma once

#include "public/dataType.h"

#include "math/Vector.h"

#include "math/Matrix.h"


using namespace ActiveEngine;
// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const aeFLOAT YAW = -90.0f;
const aeFLOAT PITCH = 0.0f;
const aeFLOAT SPEED = 3.0f;
const aeFLOAT SENSITIVTY = 0.25f;
const aeFLOAT ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// Camera Attributes
	aeVec3f Position;
	aeVec3f Front;
	aeVec3f Up;
	aeVec3f Right;
	aeVec3f WorldUp;
	// Eular Angles
	aeFLOAT Yaw;
	aeFLOAT Pitch;
	// Camera options
	aeFLOAT MovementSpeed;
	aeFLOAT MouseSensitivity;
	aeFLOAT Zoom;

	// Constructor with vectors
	Camera(aeVec3f position = aeVec3f({ 0.0f, 0.0f, 0.0f }), aeVec3f up = aeVec3f({ 0.0f, 1.0f, 0.0f }),
		aeFLOAT yaw = YAW,
		aeFLOAT pitch = PITCH) : Front(aeVec3f({ 0.0f, 0.0f, -1.0f })), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}
	// Constructor with scalar values
	Camera(aeFLOAT posX, aeFLOAT posY, aeFLOAT posZ, aeFLOAT upX, aeFLOAT upY, aeFLOAT upZ, aeFLOAT yaw, aeFLOAT pitch) : Front(aeVec3f({ 0.0f, 0.0f, -1.0f })), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		this->Position = aeVec3f({ posX, posY, posZ });
		this->WorldUp =aeVec3f({upX, upY, upZ});
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	aeMat4f GetViewMatrix()
	{
		aeMat4f camViewMat;
		camViewMat.LookAt(this->Position, this->Position + this->Front, this->Up);
		return camViewMat;
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, aeFLOAT deltaTime)
	{
		aeFLOAT velocity = this->MovementSpeed * deltaTime;
		if (direction == FORWARD)
			this->Position += this->Front * velocity;
		if (direction == BACKWARD)
			this->Position -= this->Front * velocity;
		if (direction == LEFT)
			this->Position -= this->Right * velocity;
		if (direction == RIGHT)
			this->Position += this->Right * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(aeFLOAT xoffset, aeFLOAT yoffset, aeBOOL constrainPitch = true)
	{
		xoffset *= this->MouseSensitivity;
		yoffset *= this->MouseSensitivity;

		this->Yaw += xoffset;
		this->Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (this->Pitch > 89.0f)
				this->Pitch = 89.0f;
			if (this->Pitch < -89.0f)
				this->Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		this->updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(aeFLOAT yoffset)
	{
		if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
			this->Zoom -= yoffset;
		if (this->Zoom <= 1.0f)
			this->Zoom = 1.0f;
		if (this->Zoom >= 45.0f)
			this->Zoom = 45.0f;
	}

private:
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		//glm::vec3 front;
		//front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		//front.y = sin(glm::radians(this->Pitch));
		//front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		//this->Front = glm::normalize(front);
		//// Also re-calculate the Right and Up vector
		//this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		//this->Up = glm::normalize(glm::cross(this->Right, this->Front));
	}
};