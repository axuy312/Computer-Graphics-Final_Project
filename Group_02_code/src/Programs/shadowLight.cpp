#include <iostream>
#include "context.h"
#include "program.h"

void ShadowLightProgram::doMainLoop() {
  glUseProgram(programId);

  /* TODO#2-3: Render scene with shadow mapping
   *           1. Copy from LightProgram
   *           2. Pass LightViewMatrix, fakeLightPos, shadowMap, enableShadow to shader program
   * Note:     LightViewMatrix and fakeLightPos are the same as what we used is ShadowProgram
   */ 
  // LightViewMatrix
  GLfloat near_plane = 10.0f, far_plane = 75.0f;
  glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
  glm::mat4 lightView = glm::lookAt(ctx->lightDirection * -100.0f, glm::vec3(0.0f), glm::vec3(1.0));
  glm::mat4 lightSpaceMatrix = lightProjection * lightView;
  const float* lvm = glm::value_ptr(lightSpaceMatrix);
  GLint lvmatLoc = glGetUniformLocation(programId, "LightViewMatrix");
  glUniformMatrix4fv(lvmatLoc, 1, GL_FALSE, lvm);
  // fakeLightPos
  const float* flp = glm::value_ptr(ctx->lightDirection * -10.0f);
  GLint flpLoc = glGetUniformLocation(programId, "fakeLightPos");
  glUniform3f(flpLoc, lvm[0], lvm[1], lvm[2]);
  // shadowMap
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, ctx->shadowMapTexture);
  glUniform1i(glGetUniformLocation(programId, "shadowMap"), 1);
  // enableShadow
  GLint eSmatLoc = glGetUniformLocation(programId, "enableShadow");
  glUniform1i(eSmatLoc, ctx->enableShadow);

  glUniform3fv(glGetUniformLocation(programId, "dl.direction"), 1, glm::value_ptr(ctx->lightDirection));
  glUniform3fv(glGetUniformLocation(programId, "dl.ambient"), 1, glm::value_ptr(ctx->lightAmbient));
  glUniform3fv(glGetUniformLocation(programId, "dl.diffuse"), 1, glm::value_ptr(ctx->lightDiffuse));
  glUniform3fv(glGetUniformLocation(programId, "dl.specular"), 1, glm::value_ptr(ctx->lightSpecular));

  GLint eSmatLoc0 = glGetUniformLocation(programId, "isCubeTexture");
  glUniform1i(eSmatLoc0, 0);
  eSmatLoc0 = glGetUniformLocation(programId, "isCube");
  glUniform1i(eSmatLoc0, 0);

  const float* p = ctx->camera->getProjectionMatrix();
  GLint pmatLoc = glGetUniformLocation(programId, "Projection");
  glUniformMatrix4fv(pmatLoc, 1, GL_FALSE, p);

  const float* v = ctx->camera->getViewMatrix();
  GLint vmatLoc = glGetUniformLocation(programId, "ViewMatrix");
  glUniformMatrix4fv(vmatLoc, 1, GL_FALSE, v);

  const float* vp = ctx->camera->getPosition();
  GLint mmatLoc = glGetUniformLocation(programId, "viewPos");
  glUniform3f(mmatLoc, vp[0], vp[1], vp[2]);

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
    glBindVertexArray(model->vao);

    glm::mat4 transformMatrix = ctx->players[0]->translateMatrix * ctx->players[0]->rotateMatrix;
    const float* m = glm::value_ptr(transformMatrix * model->modelMatrix);
    mmatLoc = glGetUniformLocation(programId, "ModelMatrix");
    glUniformMatrix4fv(mmatLoc, 1, GL_FALSE, m);

    glm::mat4 TIMatrix = glm::transpose(glm::inverse(model->modelMatrix));
    const float* ti = glm::value_ptr(TIMatrix);
    mmatLoc = glGetUniformLocation(programId, "TIModelMatrix");
    glUniformMatrix4fv(mmatLoc, 1, GL_FALSE, ti);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ctx->players[i]->textures);
    glUniform1i(glGetUniformLocation(programId, "ourTexture"), 0);
    glDrawArrays(model->drawMode, 0, model->numVertex);
  }

  int obj_num = (int)ctx->objects.size();
  for (int i = 0; i < obj_num; i++) {
    int modelIndex = ctx->objects[i]->modelIndex;
    Model* model = ctx->models[modelIndex];
    glBindVertexArray(model->vao);

    const float* m = glm::value_ptr(ctx->objects[i]->transformMatrix * model->modelMatrix);
    GLint mmatLoc = glGetUniformLocation(programId, "ModelMatrix");
    glUniformMatrix4fv(mmatLoc, 1, GL_FALSE, m);

    glm::mat4 TIMatrix = glm::transpose(glm::inverse(model->modelMatrix));
    const float* ti = glm::value_ptr(TIMatrix);
    mmatLoc = glGetUniformLocation(programId, "TIModelMatrix");
    glUniformMatrix4fv(mmatLoc, 1, GL_FALSE, ti);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, model->textures[ctx->objects[i]->textureIndex]);
    glUniform1i(glGetUniformLocation(programId, "ourTexture"), 0);
    glDrawArrays(model->drawMode, 0, model->numVertex);
  }

  for (int i = 0; i < ctx->terrainObjects.size(); i++) {
    GLint eSmatLoc = glGetUniformLocation(programId, "isCubeTexture");
    glUniform1i(eSmatLoc, 1);
    eSmatLoc = glGetUniformLocation(programId, "isCube");
    glUniform1i(eSmatLoc, 1);

    int modelIndex = ctx->terrainObjects[i]->modelIndex;
    Model* model = ctx->terrainModels[modelIndex];
    glBindVertexArray(model->vao);

    const float* m = glm::value_ptr(ctx->terrainObjects[i]->transformMatrix * model->modelMatrix);
    GLint mmatLoc = glGetUniformLocation(programId, "ModelMatrix");
    glUniformMatrix4fv(mmatLoc, 1, GL_FALSE, m);

    glm::mat4 TIMatrix = glm::transpose(glm::inverse(model->modelMatrix));
    const float* ti = glm::value_ptr(TIMatrix);
    mmatLoc = glGetUniformLocation(programId, "TIModelMatrix");
    glUniformMatrix4fv(mmatLoc, 1, GL_FALSE, ti);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, model->textures[ctx->terrainObjects[i]->textureIndex]);
    glUniform1i(glGetUniformLocation(programId, "blockTexture"), 2);
    glDrawArrays(model->drawMode, 0, model->numVertex);
  }

  glUseProgram(0);
}
