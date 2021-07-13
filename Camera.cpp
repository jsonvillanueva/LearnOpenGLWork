#include "Camera.h"

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Forward, Up);
}

void Camera::ChangeMode(Camera_Mode mode)
{
	if (Mode == mode)
		return;

	double siny_cosp;
	double cosy_cosp;
	double sinp;
	switch (mode) {
	case Camera_Mode::FPS:
		std::cout << "FPS mode enabled\n";
		glm::quat q = currentRotation;
		sinp = 2.0 * (q.w*q.x - q.z*q.y);
		if (std::abs(sinp) >= 1)
			Pitch = -glm::degrees(std::copysign(glm::pi<double>() / 2.0, sinp)); // use 90 degrees if out of range
		else
			Pitch = -glm::degrees(std::asin(sinp));

		siny_cosp = 2.0 * (q.w * q.y + q.z * q.x);
		cosy_cosp = q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z;
		Yaw = glm::degrees(std::atan2(siny_cosp, cosy_cosp));
		Yaw -= 90;
		Roll = 0;
		break;
	case Camera_Mode::DOF:
		std::cout << "DOF mode enabled\n";
		currentRotation = glm::normalize(glm::toQuat(glm::lookAt(Position, Position + Forward, Up)));
		break;
	}
	this->Mode = mode;

}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
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
    if (direction == Camera_Movement::ROLL_LEFT)
    	OffsetRotation(0.0f, 0.0f, RollSensitivity*deltaTime);
    if (direction == Camera_Movement::ROLL_RIGHT)
    	OffsetRotation(0.0f, 0.0f, -RollSensitivity*deltaTime);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, float deltaTime)
{
	switch (Mode) {
	case Camera_Mode::FPS:
		Yaw   += MouseSensitivity*xoffset;
		Pitch += MouseSensitivity*yoffset;
		break;
	case Camera_Mode::DOF:
    	OffsetRotation(MouseSensitivity*xoffset*deltaTime, MouseSensitivity*yoffset*deltaTime, 0.0f);
		break;
	}
}

void Camera::ProcessMouseScroll(float yoffset)
{
	FOV -= (float)yoffset;
	if (FOV < 1.0f)
		FOV = 1.0f;
	if (FOV > 110.0f)
		FOV = 110.0f; 
	projection = glm::perspective(glm::radians(FOV), aspect, near_clip, far_clip);
}

void Camera::Update()
{
	switch (Mode) {
	case Camera_Mode::FPS:
		updateCameraFPS();
		break;
	case Camera_Mode::DOF:
		updateCameraDOF();
		break;
	}
}

void Camera::updateCameraFPS()
{
	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;
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
	projection = glm::perspective(glm::radians(FOV), aspect, near_clip, far_clip);
}

void Camera::updateCameraDOF() {
	glm::mat4 rotation = GetRotation();
	Right = glm::vec3(rotation[0][0], rotation[1][0], rotation[2][0]);
	Up = glm::vec3(rotation[0][1], rotation[1][1], rotation[2][1]);
	Forward = -glm::vec3(rotation[0][2], rotation[1][2], rotation[2][2]);
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), Position);
	view = rotation * translation;
}


void Camera::GetMatricies(glm::mat4 &P, glm::mat4 &V) {
	P = projection;
	V = view;
}

void Camera::OffsetRotation(float yaw, float pitch, float roll)
{
	currentRotation = currentRotation * glm::angleAxis(yaw, glm::normalize(Up));
	currentRotation = currentRotation * glm::angleAxis(-pitch, glm::normalize(Right));
	currentRotation = currentRotation * glm::angleAxis(roll, glm::normalize(Forward));
}

glm::mat4 Camera::GetRotation()
{
    return glm::mat4_cast(currentRotation);
}

