#include "Camera.h"

glm::mat4 Camera::GetViewMatrix()
{
	if (!ArcRotation)
		return glm::lookAt(Position, Position + Front, Up);
	else
		return glm::lookAt(Position, Front, Up);
}


void Camera::ProcessKeyboard(Camera_Movement direction)
{
	if (direction == FORWARD)
		Position += Front * MovementSpeed;
	if (direction == BACKWARD)
		Position -= Front * MovementSpeed;
	if (direction == LEFT)
		Position -= Right * MovementSpeed;
	if (direction == RIGHT)
		Position += Right * MovementSpeed;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch -= yoffset;

		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;


	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

void Camera::UpdateRotation()
{
	updateCameraVectors();
}
void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	if (!ArcRotation) {
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	} else {
		Front = glm::vec3(0, 0, 0);
		glm::vec3 pos;
		pos.x = Distance * cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		pos.y = Distance * sin(glm::radians(Pitch));
		pos.z = Distance * sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Position = pos;
	}
}