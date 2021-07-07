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
const float YAW = 0.0f;
const float PITCH = 0.0f;
const float ROLL = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY =  0.1f;
const float FIELD_OF_VIEW = 70.0f;

class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front; // Local Z
    glm::vec3 Up; // Local Y
    glm::vec3 Right; // Local X
    glm::vec3 WorldUp = glm::vec3(0, 1, 0);
    glm::vec3 LookAt = glm::vec3(0, 0, 0);
    glm::quat orientation = glm::quat(0.0f, 0.0f, 0.0f, 1.0f); // XYZW
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
    Camera(
        int width,
        int height,
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW,
        float pitch = PITCH,
        float roll = ROLL
    ) : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        MovementSpeed(SPEED),
        MouseSensitivity(SENSITIVITY),
        FOV(FIELD_OF_VIEW),
        VP(glm::mat4(1.0f)),
        projection(glm::mat4(1.0f)),
        view(glm::mat4(1.0f))
    {
        window_width = width;
        window_height = height;
        aspect = float(width) / float(height);
        Position = position;
        FOV = FOV;
        Up = up;
        Right = glm::normalize(glm::cross(Front, Up));
        Yaw = yaw;
        Pitch = pitch;
        Roll = roll;
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset);


    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);
    void Reset();
    void SetViewport(int width, int height);
    void Update();

    void GetMatricies(glm::mat4& P, glm::mat4& V);

    void OffsetOrientation(float pitch, float yaw, float roll);
    glm::mat4 GetOrientation() const;
};