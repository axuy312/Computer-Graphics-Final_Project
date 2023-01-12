/*Player control*/
#define PLAYER_DEFAULT_POSITION glm::vec3(0.0f, 2.0f, 0.0f)
#define PLAYER_TALL (1.7f)

/*Player moving control*/
#define G (9.8) //���O�[�t��
#define TIME_CUT (0.01f) //�^���ɶ�����
#define PLAYER_SPEED (0.025f) //���ʳt��
#define INERTIA_ACC (PLAYER_SPEED * (0.01f)) //�D�ʥ[�t��
#define FRICTION (0.0001f)  //�����O�Y��
#define PLAYER_ROTATE_SPEED (0.5f) //��ʳt��

//speed
#define SLOW_SPEED 0  //�C�t
#define MID_SPEED 1  //�����t��
#define FAST_SPEED 2 //�ֳt

#define UP 0 //����W
#define DOWN 1 //����U
#define LEFT 2 //���䥪
#define RIGHT 3 //����k

//ismove
#define NOT_MOVE 0 //�S����
#define FORWARD 1 //�H���V�e
#define BACKWARD 2 //�H���V��
#define TURN_LEFT 3 //�H������
#define TURN_RIGHT 4  //�H������
#define JUMP 5 //�H����
#define SLIDE 6 //�Ʀ�
#define LOCK 10 //��w����

//jump_ctrl
#define CTRL 0  //����O�_�O�^�����A
#define DISABLE_JUMP 0
#define FIRST_JUMP 1
#define SECOND_JUMP 2
#define VELOCITY_Y 1 //�H��Y�b���ʳt��
#define V0 6  //��t(�Ω���D���ױ���)
#define V_REFLECT (-2) //�����ϼu�t��
#define FLOOR 2 //�a�O

#define HELL (-100.0f) //�^������

/*Camera control*/
#define NOT_LOCK_VIEW 0
#define LOCK_VIEW 1
#define DEFAULT_DISTANCE (7.5f)
#define CAMERA_DEFAULT_POSITION glm::vec3(0, 4, 8)

/*Map control*/
#define MAP_0 0 //��0�i�a��
#define MAP_1 2
#define MAP_2 4
#define BMP_HIGH_UNIT 10 //�C�@���צ�t
#define MAP_THICKNESS 1
#define BMP_TEX_UNIT 50 //���P�����t
#define GRASS 0
#define ICE 1
#define MUD 2
#define NO_TEX 10
#define BMP_PROP_UNIT 50 //���P�D���t
#define NO_PROP 0
#define FLAG 1

/*Light control*/
#define LIGHT_SPEED (0.05f)
#define DAY_NIGHT_RATIO 2

/*Filter control*/
#define BLACK_SCENE_THRESHOLD (HELL + 90.0f)  //�µe������
