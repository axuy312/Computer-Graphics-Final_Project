#include "player.h"
#include <cmath>

void Player::move(bool up, bool down, bool left, bool right, float* jump_ctrl, std::vector<std::vector<float>>* heightMap, std::vector<std::vector<float>>* textureMap) {
  glm::vec3 expected_position = position;
  int XOffset = (*heightMap)[0].size() / 2;
  int ZOffset = heightMap->size() / 2;
  int idxR, idxC;

  //��L����
  if (up) { //�V�e
    ismove = FORWARD;
    if (left && !right) { //���e
      rotateMatrix = leftRotateMatrix * rotateMatrix;
      direction =glm::normalize(glm::vec3(leftRotateMatrix * glm::vec4(direction, 1.0f)));
    } else if (right && !left) { //�k�e
      rotateMatrix = rightRotateMatrix * rotateMatrix;
      direction = glm::normalize(glm::vec3(rightRotateMatrix * glm::vec4(direction, 1.0f)));
    }
    expected_position += (direction * PLAYER_SPEED);
  } else if (down) { //�V��
    ismove = BACKWARD;
    if (!left && right) {  //����
      rotateMatrix = leftRotateMatrix * rotateMatrix;
      direction = glm::normalize(glm::vec3(leftRotateMatrix * glm::vec4(direction, 1.0f)));
    } else if (!right && left) {  //�k��
      rotateMatrix = rightRotateMatrix * rotateMatrix;
      direction = glm::normalize(glm::vec3(rightRotateMatrix * glm::vec4(direction, 1.0f)));
    }
    expected_position -= (direction * PLAYER_SPEED);
  } else if (left && !right) {  //����
    ismove = TURN_LEFT;
    rotateMatrix = leftRotateMatrix * rotateMatrix;
    direction = glm::normalize(glm::vec3(leftRotateMatrix * glm::vec4(direction, 1.0f)));
  } else if (right && !left) {  //�k��
    ismove = TURN_RIGHT;
    rotateMatrix = rightRotateMatrix * rotateMatrix;
    direction = glm::normalize(glm::vec3(rightRotateMatrix * glm::vec4(direction, 1.0f)));
  } else {
    ismove = NOT_MOVE;
  }

  //�D�ʥ[�t
  if ((ismove == NOT_MOVE) && (inertia != glm::vec3(0.0f, 0.0f, 0.0f))) ismove = SLIDE;
  expected_position += inertia;

  //�e��I������
  idxC = round((expected_position.x + XOffset));
  idxR = round(-expected_position.z + ZOffset);
  if (((*heightMap)[idxR][idxC] - expected_position.y) > 0 && ((*heightMap)[idxR][idxC] - expected_position.y) <= 2) { // 0 < ���׮t <= 2
    //�D�J����
    idxC = round((position.x + XOffset));
    idxR = round(-position.z + ZOffset);
  } else {
    //���ʦ��\
    position = expected_position;

    if ((*heightMap)[idxR][idxC] == position.y) { //�p�G���b�a���W
      if ((*textureMap)[idxR][idxC] == ICE) { //�B��
        //���ʲ��ͺD��
        if (ismove == FORWARD) {
          inertia += (direction * INERTIA_ACC);
        } else if (ismove == BACKWARD) {
          inertia -= (direction * INERTIA_ACC);
        }
        //�����O��t
        if (glm::length(inertia) > 0.0f) {
          inertia -= (glm::normalize(inertia) * FRICTION);
        }
      } else {
        inertia = glm::vec3(0.0f, 0.0f, 0.0f);
      }
    }
  }

  if (jump_ctrl[VELOCITY_Y] != 0) ismove = JUMP;  //��Y�b��t

  if (position.y > (*heightMap)[idxR][idxC]) { //��a�O���~�|�ܦa�O
    jump_ctrl[FLOOR] = (*heightMap)[idxR][idxC];
  } else if (position.y < (*heightMap)[idxR][idxC]) { //��a�O�C���U
    jump_ctrl[FLOOR] = HELL;
  }

  position.y += ((jump_ctrl[VELOCITY_Y] * TIME_CUT) - 0.5 * G * TIME_CUT * TIME_CUT);
  jump_ctrl[VELOCITY_Y] = jump_ctrl[VELOCITY_Y] - G * TIME_CUT;

  if (jump_ctrl[FLOOR] == HELL) {
    //����W�h�a�O
    if ((((*heightMap)[idxR][idxC] - MAP_THICKNESS) - (position.y + PLAYER_TALL)) > 0 &&
        (((*heightMap)[idxR][idxC] - MAP_THICKNESS) - (position.y + PLAYER_TALL)) < 0.05) {
      jump_ctrl[VELOCITY_Y] = V_REFLECT;
    }
  }

  if (position.y <= jump_ctrl[FLOOR]) { //����a�O
    jump_ctrl[CTRL] = DISABLE_JUMP;
    jump_ctrl[VELOCITY_Y] = 0;
    position.y = jump_ctrl[FLOOR];
  }

  if (position.y == HELL) { //�Y�J�`�W
    position = PLAYER_DEFAULT_POSITION;  // Player��l��m
    inertia = glm::vec3(0.0f, 0.0f, 0.0f);
  }

  translateMatrix = glm::translate(glm::identity<glm::mat4>(), position);
}