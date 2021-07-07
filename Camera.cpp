#include "Camera.h"
#include <iostream>
#include "glm/ext.hpp"
#include <glm/gtx/string_cast.hpp>

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float angular_velocity = 100.f;
	float velocity = MovementSpeed * deltaTime;
	if (direction == Camera_Movement::FORWARD)
		Position += Front * velocity;
	if (direction == Camera_Movement::BACKWARD)
		Position -= Front * velocity;
	if (direction == Camera_Movement::LEFT)
		Position -= Right * velocity;
	if (direction == Camera_Movement::RIGHT)
		Position += Right * velocity;
	if (direction == Camera_Movement::UP)
		Position += Up * velocity;
	if (direction == Camera_Movement::DOWN)
		Position -= Up * velocity;
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
	if (FOV > 90.0f)
		FOV = 90.0f; 
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
	Front = glm::vec3(rotation[0][2], rotation[1][2], rotation[2][2]) * -1.f;
	std::cout << glm::to_string(Right) << " " << glm::to_string(Up) << " " << glm::to_string(Front) << std::endl;

	glm::mat4 translation = glm::translate(glm::mat4(1.0f), Position * -1.f);
	view = rotation * translation;
}

void Camera::GetMatricies(glm::mat4 &P, glm::mat4 &V) {
	P = projection;
	V = view;
}

void Camera::OffsetOrientation(float yaw, float pitch, float roll)
{
//	if (Pitch > 90 || Pitch < -90.f)
//		Yaw += yaw;
//	else
//		Yaw -= yaw;
//	if (Yaw > 360.f)
//		Yaw -= 360.0f;
//	else if (Yaw < 0.0f)
//		Yaw += 360.f;
	Yaw -= yaw;
    Pitch -= pitch;
    Roll -= roll;
}

glm::mat4 Camera::GetOrientation() const
{
    glm::quat q = glm::angleAxis(glm::radians(Pitch), glm::vec3(1,0,0));
	q *= glm::angleAxis(glm::radians(Yaw), glm::vec3(0,1,0));
	q *= glm::angleAxis(glm::radians(Roll), glm::normalize(Front));

    return glm::mat4_cast(q);
}

