#include "Camera.h"
#include <iostream>
#include "glm/ext.hpp"
#include <glm/gtx/string_cast.hpp>

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float angular_velocity = 100.f;
	float velocity = MovementSpeed * deltaTime;
	if (direction == Camera_Movement::FORWARD)
		Position += Forward * velocity;
	if (direction == Camera_Movement::BACKWARD)
		Position -= Forward * velocity;
	if (direction == Camera_Movement::LEFT)
		Position += Right * velocity;
	if (direction == Camera_Movement::RIGHT)
		Position -= Right * velocity;
	if (direction == Camera_Movement::UP)
		Position -= Up * velocity;
	if (direction == Camera_Movement::DOWN)
		Position += Up * velocity;
	if (direction == Camera_Movement::ROLL_LEFT)
		OffsetOrientation(0.0f, 0.0f, angular_velocity*deltaTime);
	if (direction == Camera_Movement::ROLL_RIGHT)
		OffsetOrientation(0.0f, 0.0f, -angular_velocity*deltaTime);
}

void Camera::ProcessMouseScroll(float yoffset)
{
	FOV -= (float)yoffset;
	if (FOV < 1.0f)
		FOV = 1.0f;
	if (FOV > 110.0f)
		FOV = 110.0f; 
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	OffsetOrientation(xoffset, yoffset, 0);
}

void Camera::SetViewport(int width, int height) {
	window_width = width;
	window_height = height;
	aspect = float(width) / float(height);
}


void Camera::Update() {
	projection = glm::perspective(glm::radians(FOV), aspect, near_clip, far_clip);
	glm::mat4 rotation = GetOrientation();
	Right = glm::vec3(rotation[0][0], rotation[1][0], rotation[2][0]);
	Up = glm::vec3(rotation[0][1], rotation[1][1], rotation[2][1]);
	Forward = glm::vec3(rotation[0][2], rotation[1][2], rotation[2][2]);

	glm::mat4 translation = glm::translate(glm::mat4(1.0f), Position);
	view = rotation * translation;
}

void Camera::GetMatricies(glm::mat4 &P, glm::mat4 &V) {
	P = projection;
	V = view;
}

void Camera::OffsetOrientation(float yaw, float pitch, float roll)
{
	glm::quat rotationChange  = glm::angleAxis(glm::radians(-pitch), WorldRight);
	rotationChange *= glm::angleAxis(glm::radians(-yaw), WorldUp);
	rotationChange *= glm::angleAxis(glm::radians(-roll), WorldForward);
	currentRotation = glm::normalize(rotationChange * currentRotation);
}

glm::mat4 Camera::GetOrientation()
{
    return glm::mat4_cast(currentRotation);
}

