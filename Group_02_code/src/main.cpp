#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

#include <GLFW/glfw3.h>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#undef GLAD_GL_IMPLEMENTATION
#include <glm/glm.hpp>

#include <glm/ext/matrix_transform.hpp>

#include "camera.h"
#include "context.h"
#include "gl_helper.h"
#include "model.h"
#include "opengl_context.h"
#include "program.h"
#include "utils.h"
#include "constants.h"
#include "bitmap.h"
#include "parameter.h"


void initOpenGL();
void resizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int, int action, int);
bool pressed[4] = {false};
float jump_ctrl[3] = {0};  // enable, 初速, 地板高度

Context ctx;
FilterProgram* fp;

void loadPrograms() {
  /* TODO#1~3 uncoment lines to enable shader programs
   * Notes:
   *   SkyboxProgram for TODO#1
   *   ShadowProgram, ShadowLightProgram for TODO#2
   *     - also comment out default LightProgram
   *   FilterProgramBindFrameAdapter, FilterProgram for TODO#3
   *     - FilterProgramBindFrameAdapter is used to change render buffer for skybox and light program
   */
  fp = new FilterProgram(&ctx);
  ctx.programs.push_back(new ShadowProgram(&ctx));
  ctx.programs.push_back(new FilterProgramBindFrameAdapter(&ctx, fp));
  ctx.programs.push_back(new SkyboxProgram(&ctx));
  //ctx.programs.push_back(new LightProgram(&ctx));
  ctx.programs.push_back(new ShadowLightProgram(&ctx));
  ctx.programs.push_back(fp);

  // TODO#0: You can trace light program before doing hw to know how this template work and difference from hw2
  for (auto iter = ctx.programs.begin(); iter != ctx.programs.end(); iter++) {
    if (!(*iter)->load()) {
      std::cout << "Load program fail, force terminate" << std::endl;
      exit(1);
    }
  }
  glUseProgram(0);
}

void loadModels() {
  // TODO#0: You can trace light program before doing hw to know how this template work and difference from hw2
  Model* m;

  std::vector<std::string>player1ObjDirs(6);
  player1ObjDirs[0] = "model00";
  player1ObjDirs[1] = "model01";
  player1ObjDirs[2] = "model02";
  player1ObjDirs[3] = "model03";
  player1ObjDirs[4] = "model04";
  player1ObjDirs[5] = "model05";

  ctx.players.push_back(new Player());//Player 1
  ctx.players[0]->textures = createTexture("../assets/models/cube/Binary_0.png");
  for (int i = 0; i < 6; i++) {
    std::string path = "../assets/models/Rampaging T-Rex/" + player1ObjDirs[i] + "/model.obj";
    m = Model::fromObjectFile(path.c_str());
    m->modelMatrix = glm::scale(m->modelMatrix, glm::vec3(0.4f, 0.4f, 0.4f));
    attachGeneralObjectVAO(m);
    ctx.playerModels.push_back(m);
  }
  
  
  m = Model::fromObjectFile("../assets/flag/flag.obj");
  m->textures.push_back(createTexture("../assets/flag/flag.png"));
  m->modelMatrix = glm::scale(m->modelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
  attachGeneralObjectVAO(m);
  ctx.models.push_back(m);

  
  /* TODO#1-1: Add skybox mode
   *         1. Create a model and manually set box positions 
   *            (you can get positions data from variable skyboxVertices)
   * 
   *         2. gl_helper.cpp -> skybox.vert -> skybox.frag
   *         2. Add texture cubemap to model->textures with createCubemap(blueSkyboxfaces) 
   *            (you need to implement createCubemap)
   * 
   *         3. model.cpp
   *         3. implement attachSkyboxVAO to create VAO from skybox 
   *            (you can refer to attachGeneralObjectVAO above)
   * 
   *         4. Set m->numVertex
   *         5. put model to ctx.models
   * 
   */
  m = new Model();
  for (int i = 0; i < 108; i++) {
    m->positions.push_back(skyboxVertices[i]);
  }
  m->numVertex = 36;
  char faces[6][30] = {"../assets/redplanet/right.png",
                       "../assets/redplanet/left.png",
                       "../assets/redplanet/bot.png",
                       "../assets/redplanet/top.png",
                       "../assets/redplanet/front.png",
                       "../assets/redplanet/back.png"};
  m->textures.push_back(createCubemap(faces));
  attachSkyboxVAO(m);
  ctx.models.push_back(m);

  /// <summary>
  /// 方塊 Model
  /// </summary>
  m = new Model();
  for (int i = 0; i < 108; i++) {
    m->positions.push_back(blockVertices[i]);
    m->normals.push_back(blockNormals[i]);
  }
  m->numVertex = 36;
  
  char grassFaces[6][30] = {"../assets/minecraft/right.png", "../assets/minecraft/left.png",
                       "../assets/minecraft/bot.png",   "../assets/minecraft/top.png",
                       "../assets/minecraft/front.png", "../assets/minecraft/back.png"};
  m->textures.push_back(createCubemap(grassFaces));

  char iceFaces[6][30] = {"../assets/minecraft/ice.png", "../assets/minecraft/ice.png",
                              "../assets/minecraft/ice.png",   "../assets/minecraft/ice.png",
                              "../assets/minecraft/ice.png", "../assets/minecraft/ice.png"};
  m->textures.push_back(createCubemap(iceFaces));

  float scale = 0.5;
  m->modelMatrix =
      glm::translate(glm::scale(m->modelMatrix, glm::vec3(scale, scale, scale)), glm::vec3(0.0f, -1.0f, 0.0f));
  attachBlockVAO(m);
  ctx.terrainModels.push_back(m);
}

void setupObjects(const char* mf) {
  ctx.players[0]->objects.clear();
  ctx.players[0]->ismove = NOT_MOVE;
  ctx.players[0]->position = PLAYER_DEFAULT_POSITION;
  ctx.players[0]->inertia = glm::vec3(0.0f, 0.0f, 0.0f);
  for (int i = 0; i < ctx.playerModels.size(); i++) {
    ctx.players[0]->objects.push_back(new Object(i, glm::identity<glm::mat4>()));
  }

  ctx.heightMap.clear();
  ctx.textureMap.clear();
  ctx.propsMap.clear();
  ctx.terrainObjects.clear();
  ctx.objects.clear();

  //載入地形 & 道具
  std::string path = "../assets/map/" + std::string(mf);
  const char* f = path.c_str();
  struct BMPImage* mapImage = loadBMP(f);

  int XOffset = mapImage->pixelWidth / 2;
  int ZOffset = mapImage->pixelHeight / 2;
  for (int r = 0; r < mapImage->pixelHeight; r++) {
    std::vector<float> rowHeights;
    std::vector<float> texture_idx;
    std::vector<float> props;
    for (int c = 0; c < mapImage->pixelWidth; c++) {
      if (mapImage->pixels[r * mapImage->pixelWidth + c].r != 0) {
        rowHeights.push_back(mapImage->pixels[r * mapImage->pixelWidth + c].r / BMP_HIGH_UNIT);  //高度存在紅色
        texture_idx.push_back(mapImage->pixels[r * mapImage->pixelWidth + c].g / BMP_TEX_UNIT);  //材質存在綠色
        props.push_back(mapImage->pixels[r * mapImage->pixelWidth + c].b / BMP_PROP_UNIT);       //道具存在藍色
        float x = (float)c - XOffset;
        float z = -((float)r - ZOffset);
        float y = (float)(mapImage->pixels[r * mapImage->pixelWidth + c].r) / BMP_HIGH_UNIT;
        ctx.terrainObjects.push_back(new Object(0, glm::translate(glm::identity<glm::mat4>(), glm::vec3(x, y, z))));
        (*ctx.terrainObjects.rbegin())->textureIndex = texture_idx.back();
        if (props.back() == 1) {
          ctx.objects.push_back(new Object(0, glm::translate(glm::identity<glm::mat4>(), glm::vec3(x, y, z))));
        }
      } else {
        rowHeights.push_back(HELL);
        texture_idx.push_back(NO_TEX);
        props.push_back(NO_PROP);
      }
    }
    ctx.heightMap.push_back(rowHeights);
    ctx.textureMap.push_back(texture_idx);
    ctx.propsMap.push_back(props);
  }
  //========================================


  /* TODO#1-1: Uncomment to create skybox Object
   * Note:     Skybox object is put in Context::skybox rather than Context::objects 
   */
  ctx.skybox = new Object(1, glm::translate(glm::identity<glm::mat4>(), glm::vec3(0, 0, 0)));
}

int main() {
  initOpenGL();
  GLFWwindow* window = OpenGLContext::getWindow();
  /* TODO#0: Change window title to "HW3 - `your student id`"
   *         Ex. HW3 - 311550000
   */
  glfwSetWindowTitle(window, "Final Project");

  // Init Camera helper
  Camera camera(CAMERA_DEFAULT_POSITION);  //相機初始位置
  camera.initialize(OpenGLContext::getAspectRatio());
  // Store camera as glfw global variable for callbasks use
  glfwSetWindowUserPointer(window, &camera);
  ctx.camera = &camera;
  ctx.window = window;

  loadModels();
  loadPrograms();

  // Main rendering loop
  while (!glfwWindowShouldClose(window)) {
    // Loading map
    if (ctx.map == MAP_0) {
      setupObjects("map2.bmp");
      ctx.map++;
    } else if (ctx.map == MAP_1) {
      setupObjects("map1.bmp");
      ctx.map++;
    } else if (ctx.map == MAP_2) {
      setupObjects("map2.bmp");
      ctx.map++;
    }

    // Polling events.
    glfwPollEvents();
    // Update camera position and view
    camera.move(window, ctx.players[0]->position, ctx.players[0]->ismove, ctx.eanbleLockView, ctx.players[0]->direction);
    // Update player position
    ctx.players[0]->move(pressed[UP], pressed[DOWN], pressed[LEFT], pressed[RIGHT], jump_ctrl, &(ctx.heightMap), &(ctx.textureMap));
    // GL_XXX_BIT can simply "OR" together to use.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /// TO DO Enable DepthTest
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearDepth(1.0f);

    //濾鏡變換
    if (ctx.players[0]->position.y < BLACK_SCENE_THRESHOLD) {
      ctx.eanbleBlackscene = (ctx.players[0]->position.y - HELL) / (BLACK_SCENE_THRESHOLD - HELL);
    }
    if (ctx.players[0]->position == PLAYER_DEFAULT_POSITION) ctx.eanbleBlackscene = 0;

    if (glm::length(ctx.players[0]->inertia) < 0.035f) {
      ctx.eanbleSpeed = SLOW_SPEED;
    } else if (glm::length(ctx.players[0]->inertia) < 0.06f) {
      ctx.eanbleSpeed = MID_SPEED;
    } else {
      ctx.eanbleSpeed = FAST_SPEED;
    }

    //光源移動
    if (ctx.lightDegree < 180) {
      ctx.lightDegree += LIGHT_SPEED;
    } else {
      ctx.lightDegree += (LIGHT_SPEED * DAY_NIGHT_RATIO);
      if (ctx.lightDegree >= 360) ctx.lightDegree = 0;
    }
    ctx.lightDirection = glm::vec3(-0.3, -0.3 * sinf(glm::radians(ctx.lightDegree)), -0.3 * cosf(glm::radians(ctx.lightDegree)));

    //勝利條件
    int XOffset = ctx.heightMap[0].size() / 2;
    int ZOffset = ctx.heightMap.size() / 2;
    int idxC = round((ctx.players[0]->position.x + XOffset));
    int idxR = round(-ctx.players[0]->position.z + ZOffset);
    if ((ctx.heightMap[idxR][idxC] == ctx.players[0]->position.y) && (ctx.propsMap[idxR][idxC] == FLAG)) {
      ctx.map++;
    }

    // TODO#0: You can trace light program before doing hw to know how this template work and difference from hw2
    size_t sz = ctx.programs.size();
    for (size_t i = 0; i < sz; i++) {
      ctx.programs[i]->doMainLoop();
    }
    

#ifdef __APPLE__
    // Some platform need explicit glFlush
    glFlush();
#endif
    glfwSwapBuffers(window);
  }
  return 0;
}

void keyCallback(GLFWwindow* window, int key, int, int action, int) {
  // There are three actions: press, release, hold(repeat)
  if (action == GLFW_PRESS) {
    if (key == GLFW_KEY_UP) pressed[UP] = true;
    if (key == GLFW_KEY_DOWN) pressed[DOWN] = true;
    if (key == GLFW_KEY_LEFT) pressed[LEFT] = true;
    if (key == GLFW_KEY_RIGHT) pressed[RIGHT] = true;
    if (key == GLFW_KEY_SPACE) {
      if (jump_ctrl[CTRL] == DISABLE_JUMP) {
        jump_ctrl[CTRL] = FIRST_JUMP;
        jump_ctrl[VELOCITY_Y] = V0;
      } else if (jump_ctrl[CTRL] == FIRST_JUMP) {
        jump_ctrl[CTRL] = SECOND_JUMP;
        jump_ctrl[VELOCITY_Y] = V0;
      }
    }
  }
  if (action == GLFW_RELEASE) {
    if (key == GLFW_KEY_UP) pressed[UP] = false;
    if (key == GLFW_KEY_DOWN) pressed[DOWN] = false;
    if (key == GLFW_KEY_LEFT) pressed[LEFT] = false;
    if (key == GLFW_KEY_RIGHT) pressed[RIGHT] = false;
  }

  if (action == GLFW_REPEAT) {
    switch (key) {
      case GLFW_KEY_H:
        ctx.gamma += 0.05f;
        break;
      case GLFW_KEY_J:
        ctx.gamma -= 0.05f;
        if (ctx.gamma < 0.0f) {
          ctx.gamma = 0.0f;
        }
        break;
      case GLFW_KEY_K:
        ctx.lightDegree += 1.0f;
        break;
      case GLFW_KEY_L:
        ctx.lightDegree -= 1.0f;
        break;
      default:
        break;
    }
    return;
  }
  // Press ESC to close the window.
  if (key == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    return;
  }
  if (action == GLFW_PRESS) {
    switch (key) {
      case GLFW_KEY_F9: {
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
          // Show the mouse cursor
          glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
          // Hide the mouse cursor
          glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        break;
      }
      case GLFW_KEY_Y:
        ctx.enableShadow = !ctx.enableShadow;
        break;
      case GLFW_KEY_U:
        ctx.enableEdgeDetection = !ctx.enableEdgeDetection;
        break;
      case GLFW_KEY_I:
        ctx.eanbleGrayscale = !ctx.eanbleGrayscale;
        break;
      case GLFW_KEY_L: //鎖定視角
        ctx.eanbleLockView = !ctx.eanbleLockView;
        ctx.players[0]->ismove = LOCK;
        break;
      default:
        break;
    }
  }
}

void resizeCallback(GLFWwindow* window, int width, int height) {
  // TODO#3 uncomment this to update frame buffer size when window size chnage
  fp->updateFrameBuffer(width, height);
  OpenGLContext::framebufferResizeCallback(window, width, height);
  auto ptr = static_cast<Camera*>(glfwGetWindowUserPointer(window));
  if (ptr) {
    ptr->updateProjectionMatrix(OpenGLContext::getAspectRatio());
  }
}

void initOpenGL() {
  // Initialize OpenGL context, details are wrapped in class.
#ifdef __APPLE__
  // MacOS need explicit request legacy support
  OpenGLContext::createContext(21, GLFW_OPENGL_ANY_PROFILE);
#else
  OpenGLContext::createContext(21, GLFW_OPENGL_ANY_PROFILE);
//  OpenGLContext::createContext(43, GLFW_OPENGL_COMPAT_PROFILE);
#endif
  GLFWwindow* window = OpenGLContext::getWindow();
  glfwSetKeyCallback(window, keyCallback);
  glfwSetFramebufferSizeCallback(window, resizeCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#ifndef NDEBUG
  OpenGLContext::printSystemInfo();
  // This is useful if you want to debug your OpenGL API calls.
  OpenGLContext::enableDebugCallback();
#endif
}