#include "player.h"
#include <cmath>

void Player::move(bool up, bool down, bool left, bool right, float* jump_ctrl, std::vector<std::vector<float>>* heightMap, std::vector<std::vector<float>>* textureMap) {
  glm::vec3 expected_position = position;
  int XOffset = (*heightMap)[0].size() / 2;
  int ZOffset = heightMap->size() / 2;
  int idxR, idxC;

  //鍵盤移動
  if (up) { //向前
    ismove = FORWARD;
    if (left && !right) { //左前
      rotateMatrix = leftRotateMatrix * rotateMatrix;
      direction =glm::normalize(glm::vec3(leftRotateMatrix * glm::vec4(direction, 1.0f)));
    } else if (right && !left) { //右前
      rotateMatrix = rightRotateMatrix * rotateMatrix;
      direction = glm::normalize(glm::vec3(rightRotateMatrix * glm::vec4(direction, 1.0f)));
    }
    expected_position += (direction * PLAYER_SPEED);
  } else if (down) { //向後
    ismove = BACKWARD;
    if (!left && right) {  //左後
      rotateMatrix = leftRotateMatrix * rotateMatrix;
      direction = glm::normalize(glm::vec3(leftRotateMatrix * glm::vec4(direction, 1.0f)));
    } else if (!right && left) {  //右後
      rotateMatrix = rightRotateMatrix * rotateMatrix;
      direction = glm::normalize(glm::vec3(rightRotateMatrix * glm::vec4(direction, 1.0f)));
    }
    expected_position -= (direction * PLAYER_SPEED);
  } else if (left && !right) {  //左轉
    ismove = TURN_LEFT;
    rotateMatrix = leftRotateMatrix * rotateMatrix;
    direction = glm::normalize(glm::vec3(leftRotateMatrix * glm::vec4(direction, 1.0f)));
  } else if (right && !left) {  //右轉
    ismove = TURN_RIGHT;
    rotateMatrix = rightRotateMatrix * rotateMatrix;
    direction = glm::normalize(glm::vec3(rightRotateMatrix * glm::vec4(direction, 1.0f)));
  } else {
    ismove = NOT_MOVE;
  }

  //慣性加速
  if ((ismove == NOT_MOVE) && (inertia != glm::vec3(0.0f, 0.0f, 0.0f))) ismove = SLIDE;
  expected_position += inertia;

  //前後碰撞偵測
  idxC = round((expected_position.x + XOffset));
  idxR = round(-expected_position.z + ZOffset);
  if (((*heightMap)[idxR][idxC] - expected_position.y) > 0 && ((*heightMap)[idxR][idxC] - expected_position.y) <= 2) { // 0 < 高度差 <= 2
    //遭遇阻擋
    idxC = round((position.x + XOffset));
    idxR = round(-position.z + ZOffset);
  } else {
    //移動成功
    position = expected_position;

    if ((*heightMap)[idxR][idxC] == position.y) { //如果站在地面上
      if ((*textureMap)[idxR][idxC] == ICE) { //冰面
        //移動產生慣性
        if (ismove == FORWARD) {
          inertia += (direction * INERTIA_ACC);
        } else if (ismove == BACKWARD) {
          inertia -= (direction * INERTIA_ACC);
        }
        //摩擦力減速
        if (glm::length(inertia) > 0.0f) {
          inertia -= (glm::normalize(inertia) * FRICTION);
        }
      } else {
        inertia = glm::vec3(0.0f, 0.0f, 0.0f);
      }
    }
  }

  if (jump_ctrl[VELOCITY_Y] != 0) ismove = JUMP;  //有Y軸初速

  if (position.y > (*heightMap)[idxR][idxC]) { //比地板高才會變地板
    jump_ctrl[FLOOR] = (*heightMap)[idxR][idxC];
  } else if (position.y < (*heightMap)[idxR][idxC]) { //比地板低落下
    jump_ctrl[FLOOR] = HELL;
  }

  position.y += ((jump_ctrl[VELOCITY_Y] * TIME_CUT) - 0.5 * G * TIME_CUT * TIME_CUT);
  jump_ctrl[VELOCITY_Y] = jump_ctrl[VELOCITY_Y] - G * TIME_CUT;

  if (jump_ctrl[FLOOR] == HELL) {
    //撞到上層地板
    if ((((*heightMap)[idxR][idxC] - MAP_THICKNESS) - (position.y + PLAYER_TALL)) > 0 &&
        (((*heightMap)[idxR][idxC] - MAP_THICKNESS) - (position.y + PLAYER_TALL)) < 0.05) {
      jump_ctrl[VELOCITY_Y] = V_REFLECT;
    }
  }

  if (position.y <= jump_ctrl[FLOOR]) { //落到地板
    jump_ctrl[CTRL] = DISABLE_JUMP;
    jump_ctrl[VELOCITY_Y] = 0;
    position.y = jump_ctrl[FLOOR];
  }

  if (position.y == HELL) { //墜入深淵
    position = PLAYER_DEFAULT_POSITION;  // Player初始位置
    inertia = glm::vec3(0.0f, 0.0f, 0.0f);
  }

  translateMatrix = glm::translate(glm::identity<glm::mat4>(), position);
}