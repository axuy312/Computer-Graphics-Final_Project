#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include "model.h"
#include "parameter.h"

class Player {
 public:
  void move(bool up, bool down, bool left, bool right, float* jump_ctrl, std::vector<std::vector<float>>* heightMap, std::vector<std::vector<float>>* textureMap);
  int ismove = NOT_MOVE;
  glm::vec3 position = PLAYER_DEFAULT_POSITION;  // Playerªì©l¦ì¸m
  glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 inertia = glm::vec3(0.0f, 0.0f, 0.0f);
  std::vector<Object *> objects;
  glm::mat4 rotateMatrix = glm::rotate(glm::identity<glm::mat4>(), glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
  glm::mat4 translateMatrix = glm::identity<glm::mat4>();
  glm::mat4 rightRotateMatrix = glm::rotate(glm::identity<glm::mat4>(), glm::radians(-PLAYER_ROTATE_SPEED), glm::vec3(0.0, 1.0, 0.0));
  glm::mat4 leftRotateMatrix = glm::rotate(glm::identity<glm::mat4>(), glm::radians(PLAYER_ROTATE_SPEED), glm::vec3(0.0, 1.0, 0.0));
  GLuint textures;
};