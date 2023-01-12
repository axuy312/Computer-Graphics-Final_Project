#include "camera.h"

Camera::Camera(glm::vec3 _position)
    : position(_position),
      up(0, 1, 0),
      front(0, 0, -1),
      right(1, 0, 0),
      rotation(glm::identity<glm::quat>()),
      projectionMatrix(1),
      viewMatrix(1) {}

void Camera::initialize(float aspectRatio) {
  updateProjectionMatrix(aspectRatio);
  updateViewMatrix(PLAYER_DEFAULT_POSITION, NOT_LOCK_VIEW);  // Playerªì©l¦ì¸m
}

void Camera::move(GLFWwindow* window, glm::vec3 player_position, int player_ismove, unsigned int eanbleLockView,
                  glm::vec3 player_direction) {
  bool ismoved = false;

  if (eanbleLockView) {
    // Keyboard part
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      if (distance > keyboardMoveSpeed) {
        distance -= keyboardMoveSpeed;
        ismoved = true;
      }
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      distance += keyboardMoveSpeed;
      ismoved = true;
    }
    // Camera fallow player
    position = player_position - (player_direction * distance) + glm::vec3(0.0f, distance, 0.0f);
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  } else {
    // Mouse part
    static double lastx = 0, lasty = 0;
    if (lastx == 0 && lasty == 0) {
      glfwGetCursorPos(window, &lastx, &lasty);
    } else {
      double xpos, ypos;
      glfwGetCursorPos(window, &xpos, &ypos);
      float dx = mouseMoveSpeed * static_cast<float>(xpos - lastx);
      float dy = mouseMoveSpeed * static_cast<float>(lasty - ypos);
      lastx = xpos;
      lasty = ypos;
      if (dx != 0 || dy != 0) {
        ismoved = true;
        glm::quat rx(glm::angleAxis(dx, glm::vec3(0, -1, 0)));
        glm::quat ry(glm::angleAxis(dy, glm::vec3(1, 0, 0)));
        rotation = rx * rotation * ry;
      }
    }
    // Keyboard part
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      position += front * keyboardMoveSpeed;
      ismoved = true;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      position -= front * keyboardMoveSpeed;
      ismoved = true;
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      position -= right * keyboardMoveSpeed;
      ismoved = true;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      position += right * keyboardMoveSpeed;
      ismoved = true;
    }
  }

  // Update view matrix if moved
  if (ismoved || player_ismove) {
    updateViewMatrix(player_position, eanbleLockView);
  }
}

void Camera::updateViewMatrix(glm::vec3 player_position, unsigned int eanbleLockView) {
  constexpr glm::vec3 original_front(0, 0, -1);
  constexpr glm::vec3 original_up(0, 1, 0);

  if (eanbleLockView) {
    front = glm::normalize(player_position - position);
    right = glm::cross(front, original_up);
    up = glm::cross(right, front);
    viewMatrix = glm::lookAt(position, player_position, up);
  } else {
    front = rotation * original_front;
    up = rotation * original_up;
    right = glm::cross(front, up);
    viewMatrix = glm::lookAt(position, position + front, up);
  }
}

void Camera::updateProjectionMatrix(float aspectRatio) {
  constexpr float FOV = glm::radians(45.0f);
  constexpr float zNear = 0.1f;
  constexpr float zFar = 100.0f;

  projectionMatrix = glm::perspective(FOV, aspectRatio, zNear, zFar);
}