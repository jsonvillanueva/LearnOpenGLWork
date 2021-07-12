#pragma once
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>

enum class Camera_Movement {
    FORWARD=0,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    ROLL_LEFT,
    ROLL_RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float ROLL = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY =  0.1f;
const float FIELD_OF_VIEW = 90.0f;

class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Forward; // Local Z
    glm::vec3 Up; // Local Y
    glm::vec3 Right; // Local X
    glm::vec3 WorldRight = glm::vec3(1, 0, 0);
    glm::vec3 WorldUp = glm::vec3(0, 1, 0);
    glm::vec3 WorldForward = glm::vec3(0, 0, 1);
    glm::quat currentRotation = glm::quat(0.0f, 0.0f, 0.0f, 1.0f); // XYZW
    // euler Angles
    float Yaw;
    float Pitch;
    float Roll;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float FOV;


	int window_width;
	int window_height;

	float aspect;
	float near_clip = .1f;
	float far_clip = 1000.0f;

	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 VP;


    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Forward(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), FOV(FIELD_OF_VIEW)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Forward(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), FOV(FIELD_OF_VIEW)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }


    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset);


    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);
    void SetViewport(int width, int height);
    void updateCameraVectors();
    void Update();
    glm::mat4 GetViewMatrix();

    void GetMatricies(glm::mat4& P, glm::mat4& V);

    void OffsetOrientation(float pitch, float yaw, float roll);
    glm::mat4 GetOrientation();
};
