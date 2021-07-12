#include "Camera.h"

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Forward, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float angular_velocity = 100.f;
	float velocity = MovementSpeed * deltaTime;
	if (direction == Camera_Movement::FORWARD)
		Position += Forward * velocity;
	if (direction == Camera_Movement::BACKWARD)
		Position -= Forward * velocity;
	if (direction == Camera_Movement::RIGHT)
		Position += Right * velocity;
	if (direction == Camera_Movement::LEFT)
		Position -= Right * velocity;
	if (direction == Camera_Movement::UP)
		Position += Up * velocity;
	if (direction == Camera_Movement::DOWN)
		Position -= Up * velocity;
	//if (direction == Camera_Movement::ROLL_LEFT)
	//	OffsetOrientation(0.0f, 0.0f, angular_velocity*deltaTime);
	//if (direction == Camera_Movement::ROLL_RIGHT)
	//	OffsetOrientation(0.0f, 0.0f, -angular_velocity*deltaTime);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw   += xoffset;
	Pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;

	// update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
	FOV -= (float)yoffset;
	if (FOV < 1.0f)
		FOV = 1.0f;
	if (FOV > 110.0f)
		FOV = 110.0f; 
}

void Camera::updateCameraVectors()
{
	// calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Forward = glm::normalize(front);
	// also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Forward, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up    = glm::normalize(glm::cross(Right, Forward));
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
	Yaw -= yaw;
	Pitch -= pitch;
	Roll -= roll;
}

glm::mat4 Camera::GetOrientation()
{
	glm::quat q = glm::angleAxis(glm::radians(Pitch), glm::vec3(1,0,0));

	q= q * glm::angleAxis(glm::radians(Yaw), glm::vec3(0,1,0));
	glm::vec3 tmp_forward = q * WorldForward;
	q = q* glm::angleAxis(glm::radians(Roll), glm::normalize(tmp_forward));

	Right = WorldRight * q;
	Up =  WorldUp * q;
	Forward = -WorldForward * q;

    return glm::mat4_cast(q);
}

