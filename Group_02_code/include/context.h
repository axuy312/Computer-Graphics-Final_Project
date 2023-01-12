#pragma once

#include <vector>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#include "model.h"
#include "camera.h"
#include "program.h"
#include "player.h"

// Global varaibles share between main.cpp and shader programs
class Context {
 public:
  std::vector<Program *> programs;
  std::vector<Model *> models;
  std::vector<Model *> terrainModels;
  std::vector<Object *> objects;
  std::vector<Object *> terrainObjects; 
  std::vector<Model*> playerModels;
  std::vector<Player *> players;
  Object* skybox;

  GLuint map = 0;
  std::vector<std::vector<float>> heightMap;
  std::vector<std::vector<float>> textureMap;
  std::vector<std::vector<float>> propsMap;

  GLuint shadowMapTexture;
  GLuint enableShadow = 1;
  GLuint enableEdgeDetection = 0;
  GLuint eanbleGrayscale = 0;
  GLuint eanbleLockView = 1;
  GLuint eanbleSpeed = 1;
  float eanbleBlackscene = 0;
  float gamma = 1.0f;

 public:
  float lightDegree = 30.0f;
  glm::vec3 lightDirection = glm::vec3(-0.3f, -0.5f, -0.2f);
  glm::vec3 lightAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
  glm::vec3 lightDiffuse = glm::vec3(0.8f, 0.8f, 0.8f);
  glm::vec3 lightSpecular = glm::vec3(0.3f, 0.3f, 0.3f);

 public:
  Camera *camera = 0;
  GLFWwindow *window = 0;
};