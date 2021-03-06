#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/* Possible Directions */
enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

/* Default Values */
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        = 45.0f;

class Camera {
public:
    /* Attributes */
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    /* Euler Angles */
    float Yaw;
    float Pitch;

    /* Options */
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    Camera(glm::vec3 position, glm::vec3 up = glm::vec3(0,1,0), float yaw = YAW, float pitch = PITCH);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    glm::mat4 GetViewMatrix();

    void ProcessKeyboard(CameraMovement direction, float deltaTime);
    void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch);
    void ProcessMouseScroll(float yOffset);

private:
    void UpdateCameraVectors();
};

#endif