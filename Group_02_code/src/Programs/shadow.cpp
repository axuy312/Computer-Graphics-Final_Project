#include <iostream>
#include "context.h"
#include "opengl_context.h"
#include "program.h"

GLfloat borderColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};

ShadowProgram::ShadowProgram(Context* ctx) : Program(ctx) {
  vertProgramFile = "../assets/shaders/shadow.vert";
  fragProgramFIle = "../assets/shaders/shadow.frag";

  // TODO#2-0: comment this line if your computer is poor
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &SHADOW_MAP_SIZE);
  std::cout << "Current depth map size is " << SHADOW_MAP_SIZE << std::endl;

  /* TODO#2-1 Generate frame buffer and depth map for shadow program
   *          1. Generate frame buffer and store to depthMapFBO
   *          2. Generate depthmap texture and store to  ctx->shadowMapTexture
   *             - texure size is SHADOW_MAP_SIZE * SHADOW_MAP_SIZE
   *          3. properly setup depthmap's texture paremters
   *             - Set texture wrap to "clamp to border" and use border color provider above
   *          4. bind texture to framebuffer's depth buffer and disable color buffer read and write
   * Hint:
   *          - glGenFramebuffers
   *          - glGenTextures
   *          - glBindTexture
   *          - glTexImage2D
   *          - glTexParameteri
   *          - GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T
   *          - glTexParameterfv
   *          - GL_TEXTURE_BORDER_COLOR
   *          - glBindFramebuffer
   *          - glFramebufferTexture2D
   *          - glDrawBuffer
   *          - glReadBuffer
   */
  //1. Generate frame buffer and store to depthMapFBO
  glGenFramebuffers(1, &depthMapFBO);

  //2. Generate depthmap texture and store to ctx->shadowMapTexture
  const GLuint SHADOW_WIDTH = SHADOW_MAP_SIZE, SHADOW_HEIGHT = SHADOW_MAP_SIZE;
  GLuint depthMap;
  glGenTextures(1, &depthMap);
  glBindTexture(GL_TEXTURE_2D, depthMap);
  ctx->shadowMapTexture = depthMap;
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

  //3. properly setup depthmap's texture paremters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  //4. bind texture to framebuffer's depth buffer and disable color buffer read and write
  GLfloat borderColor[] = {1.0, 1.0, 1.0, 1.0};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowProgram::doMainLoop() {
  glUseProgram(programId);
  /* TODO#2-2: Render depth map with shader
   *           1. Change viewport to depth map size
   *           2. Bind out framebuffer
   *           3. A directional light doesn't have a source position. 
   *              However, for shadown map we need to render the scene from a light's perspective 
   *              and thus render the scene from a position somewhere along the lines of the light direction.
   *              The position of the light need to be "ctx->lightDirection * -10.0f"
   *           4. Properly set LightViewMatrix (projection matrix * view matrux)
   *           5. Rander all scene models as usual
   *           6. restore viewport and framebuffer
                  (get screen size from OpenGLContext::getWidth, OpenGLContext::getHeight)
   * Note:     1. You can consider the light as a camera, so you can use glm::lookAt to generate correct lightView matrix
   *           2. For the direction light we need orthogonal projection rather than perspective projection
   *              (the near plane, far plane value is provided, the image size is [-10~10], [-10~10]
   */

  
    

    //1. Change viewport to depth map size
    const GLuint SHADOW_WIDTH = SHADOW_MAP_SIZE, SHADOW_HEIGHT = SHADOW_MAP_SIZE;
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

    //2. Bind out framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    //3. A directional light doesn't have a source position. 
    //However, for shadown map we need to render the scene from a light's perspective 
    //and thus render the scene from a position somewhere along the lines of the light direction.
    //The position of the light need to be "ctx->lightDirection * -10.0f"
    //4. Properly set LightViewMatrix(projection matrix * view matrux)
    GLfloat near_plane = 10.0f, far_plane = 75.0f;
    glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(ctx->lightDirection * -100.0f, glm::vec3(0.0f), glm::vec3(1.0));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
    const float* v = glm::value_ptr(lightSpaceMatrix);
    GLint vmatLoc = glGetUniformLocation(programId, "LightViewMatrix");
    glUniformMatrix4fv(vmatLoc, 1, GL_FALSE, v);
    glCullFace(GL_FRONT);
    //5. Render all scene models as usual
    clock_t ct = clock();
    int timeIdx;
    if ((ctx->players[0]->ismove == FORWARD) || (ctx->players[0]->ismove == TURN_LEFT) ||
        (ctx->players[0]->ismove == TURN_RIGHT)) {
      timeIdx = ct / ((clock_t)200) % 6;
    } else if (ctx->players[0]->ismove == BACKWARD) {
      timeIdx = ct / ((clock_t)200) % 6;
      timeIdx = 5 - timeIdx;
    } else if (ctx->players[0]->ismove == JUMP) {
      timeIdx = 3;
    } else {
      timeIdx = 0;
    }

    for (int i = 0; i < ctx->players.size(); i++) {
      int modelIndex = ctx->players[0]->objects[timeIdx]->modelIndex;
      Model* model = ctx->playerModels[modelIndex];

      glm::mat4 transformMatrix = ctx->players[0]->translateMatrix * ctx->players[0]->rotateMatrix;
      const float* m = glm::value_ptr(transformMatrix * model->modelMatrix);
      GLint mmatLoc = glGetUniformLocation(programId, "ModelMatrix");
      glUniformMatrix4fv(mmatLoc, 1, GL_FALSE, m);

      glBindVertexArray(model->vao);
      glDrawArrays(model->drawMode, 0, model->numVertex);
      glBindVertexArray(0);
    }

    int obj_num = (int)ctx->objects.size();
    for (int i = 0; i < obj_num; i++) {
      int modelIndex = ctx->objects[i]->modelIndex;
      Model* model = ctx->models[modelIndex];

      const float* m = glm::value_ptr(ctx->objects[i]->transformMatrix * model->modelMatrix);
      GLint mmatLoc = glGetUniformLocation(programId, "ModelMatrix");
      glUniformMatrix4fv(mmatLoc, 1, GL_FALSE, m);

      glBindVertexArray(model->vao);
      glDrawArrays(model->drawMode, 0, model->numVertex);
      glBindVertexArray(0);
    }

    for (int i = 0; i < ctx->terrainObjects.size(); i++) {
      int modelIndex = ctx->terrainObjects[i]->modelIndex;
      Model* model = ctx->terrainModels[modelIndex];

      const float* m = glm::value_ptr(ctx->terrainObjects[i]->transformMatrix * model->modelMatrix);
      GLint mmatLoc = glGetUniformLocation(programId, "ModelMatrix");
      glUniformMatrix4fv(mmatLoc, 1, GL_FALSE, m);

      glBindVertexArray(model->vao);
      glDrawArrays(model->drawMode, 0, model->numVertex);
      glBindVertexArray(0);
    }

    glCullFace(GL_BACK);
    //6. restore viewport and framebuffer(get screen size from OpenGLContext::getWidth, OpenGLContext::getHeight)
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, OpenGLContext::getWidth(), OpenGLContext::getHeight());

  glUseProgram(0);
}