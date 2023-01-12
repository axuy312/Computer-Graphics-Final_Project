/*Player control*/
#define PLAYER_DEFAULT_POSITION glm::vec3(0.0f, 2.0f, 0.0f)
#define PLAYER_TALL (1.7f)

/*Player moving control*/
#define G (9.8) //重力加速度
#define TIME_CUT (0.01f) //跌落時間控制
#define PLAYER_SPEED (0.025f) //移動速度
#define INERTIA_ACC (PLAYER_SPEED * (0.01f)) //慣性加速度
#define FRICTION (0.0001f)  //摩擦力係數
#define PLAYER_ROTATE_SPEED (0.5f) //轉動速度

//speed
#define SLOW_SPEED 0  //慢速
#define MID_SPEED 1  //中等速度
#define FAST_SPEED 2 //快速

#define UP 0 //案鍵上
#define DOWN 1 //案鍵下
#define LEFT 2 //案鍵左
#define RIGHT 3 //案鍵右

//ismove
#define NOT_MOVE 0 //沒移動
#define FORWARD 1 //人物向前
#define BACKWARD 2 //人物向後
#define TURN_LEFT 3 //人物左轉
#define TURN_RIGHT 4  //人物左轉
#define JUMP 5 //人物跳
#define SLIDE 6 //滑行
#define LOCK 10 //鎖定視角

//jump_ctrl
#define CTRL 0  //控制是否是跌落狀態
#define DISABLE_JUMP 0
#define FIRST_JUMP 1
#define SECOND_JUMP 2
#define VELOCITY_Y 1 //人物Y軸移動速度
#define V0 6  //初速(用於跳躍高度控制)
#define V_REFLECT (-2) //狀頂反彈速度
#define FLOOR 2 //地板

#define HELL (-100.0f) //跌落高度

/*Camera control*/
#define NOT_LOCK_VIEW 0
#define LOCK_VIEW 1
#define DEFAULT_DISTANCE (7.5f)
#define CAMERA_DEFAULT_POSITION glm::vec3(0, 4, 8)

/*Map control*/
#define MAP_0 0 //第0張地圖
#define MAP_1 2
#define MAP_2 4
#define BMP_HIGH_UNIT 10 //每一高度色差
#define MAP_THICKNESS 1
#define BMP_TEX_UNIT 50 //不同材質色差
#define GRASS 0
#define ICE 1
#define MUD 2
#define NO_TEX 10
#define BMP_PROP_UNIT 50 //不同道具色差
#define NO_PROP 0
#define FLAG 1

/*Light control*/
#define LIGHT_SPEED (0.05f)
#define DAY_NIGHT_RATIO 2

/*Filter control*/
#define BLACK_SCENE_THRESHOLD (HELL + 90.0f)  //黑畫面高度
