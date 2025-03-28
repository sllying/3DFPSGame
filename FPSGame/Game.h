#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <random>
#include <chrono>
#include <functional>

class Game {
private:
    bool isGameOver;
    bool isVictory;
    bool isCleaningUp;
    void handleGameOver();
    void showGameOverScreen();
    void showMessageBox(const std::wstring& title, const std::wstring& message);

private:
    bool isUnderAttack;      // 是否正在受到攻击
    float attackCooldown;    // 攻击冷却时间，避免连续攻击

private:
    // 小地图参数
    // 添加用于小地图的汉字常量
    static const wchar_t WALL_CHAR = L'#';      // 墙
    static const wchar_t SPACE_CHAR = L' ';     // 空地
    static const wchar_t PLAYER_CHAR = L'P';    // 玩家
    static const wchar_t ENEMY_CHAR = L'敌';     // 敌人
    static const wchar_t BORDER_CHAR_V = L'│';   // 竖边框
    static const wchar_t BORDER_CHAR_H = L'─';   // 横边框
    static const wchar_t BORDER_CHAR_TL = L'┌';  // 左上角
    static const wchar_t BORDER_CHAR_TR = L'┐';  // 右上角
    static const wchar_t BORDER_CHAR_BL = L'└';  // 左下角
    static const wchar_t BORDER_CHAR_BR = L'┘';  // 右下角
    static const int DEFAULT_SCREEN_WIDTH = 52;  // 必须是偶数
    static const int DEFAULT_SCREEN_HEIGHT = 34;

    // 小地图尺寸也要考虑中文字符宽度
    static const int MINIMAP_WIDTH = 32;  // 确保是偶数
    static const int MINIMAP_HEIGHT = 20;
    static const int MINIMAP_X = 131;      // 确保起始位置考虑到中文字符宽度
    static const int MINIMAP_Y = 0;

    // 添加敌人渲染相关的常量
    static constexpr float ENEMY_SIZE = 0.6f;        // 敌人大小
    static constexpr float ENEMY_HEIGHT = 0.6f;      // 敌人高度

    // 添加敌人渲染函数
    void renderEnemies();
    void drawEnemy(float distance, float angle, int screenX);

    void drawMinimap();  // 绘制小地图的函数

    char getWallChar(float brightness);
    WORD getWallColor(float brightness);
    char getSkyChar(float brightness);
    WORD getSkyColor(float brightness);
    char getFloorChar(float brightness);
    WORD getFloorColor(float brightness);

    // 新增渲染参数
    static constexpr float VIEW_DISTANCE = 16.0f;  // 视距
    static constexpr float SHADE_DISTANCE = 8.0f;  // 阴影距离

    HANDLE hConsole;
    HANDLE hConsoleIn;

    // 屏幕缓冲
    CHAR_INFO* screenBuffer;
    int screenWidth;
    int screenHeight;

    // 玩家数据
    float playerX;
    float playerY;
    float playerAngle;
    float playerFOV;
    float moveSpeed;
    float rotateSpeed;

    // 地图数据
    std::vector<std::string> map;
    int mapWidth;
    int mapHeight;

    // 游戏状态
    bool isRunning;
    int score;

    // 敌人数据
    struct Enemy {
        float x;
        float y;
        float angle;  // 敌人朝向
        bool isAlive;

        Enemy(float _x, float _y)
            : x(_x)
            , y(_y)
            , angle(0.0f)
            , isAlive(true) {
        }
    };
    std::vector<Enemy> enemies;
private:
    void updateEnemyRendering(Enemy& enemy);
    // 子弹数据
    struct Bullet {
        float x;
        float y;
        float angle;
        bool active;

        Bullet(float _x, float _y, float _angle)
            : x(_x), y(_y), angle(_angle), active(true) {
        }
    };
    std::vector<Bullet> bullets;

    // 私有方法
    void initializeConsole();
    void generateMap();
    void spawnEnemies(int count);
    void handleInput();
    void updateGame(float deltaTime);
    void renderFrame();
    void castRays();
    void drawWall(int x, float perpWallDist, float wallHeight);
    void updateBullets(float deltaTime);
    void checkBulletCollisions();
    void drawHUD();
    void resetGame();
public:
    void cleanup();

    // 在 Game 类的 private 部分添加道具相关定义
private:
    // 道具类型
    enum class ItemType {
        HEALTH_PACK,     // 生命值补充
        AMMO_PACK,       // 弹药补充
        SPEED_BOOST      // 速度提升
    };

    // 道具结构体
    struct Item {
        float x;
        float y;
        ItemType type;
        bool active;

        Item(float _x, float _y, ItemType _type)
            : x(_x), y(_y), type(_type), active(true) {
        }
    };
    std::vector<Item> items;  // 道具容器

    // 道具显示字符
    static const wchar_t HEALTH_PACK_CHAR = L'H';  // 生命值
    static const wchar_t AMMO_PACK_CHAR = L'A';    // 弹药
    static const wchar_t SPEED_BOOST_CHAR = L'S';  // 速度

    // 在 Game 类的 private 部分添加
private:
    // 道具渲染相关常量
    static constexpr float ITEM_MIN_SIZE = 5.0f;        // 最小渲染大小
    static constexpr float ITEM_BASE_SCALE = 0.2f;      // 基础缩放系数
    static constexpr float ITEM_FLOAT_SPEED = 0.003f;   // 浮动动画速度
    static constexpr float ITEM_FADE_DISTANCE = 5.0f;   // 开始褪色的距离
    private:
        void drawItemPattern(const char* pattern, int size, int columnX, WORD color, float distance);
    // 玩家属性
    float playerHealth;           // 玩家生命值
    float maxPlayerHealth;        // 最大生命值
    float speedBoostTimer;        // 速度提升持续时间
    float originalMoveSpeed;      // 原始移动速度

    // 道具相关方法
    void spawnItems();           // 生成道具
    void updateItems();          // 更新道具状态
    void drawItems();            // 渲染道具
    void checkItemCollision();   // 检查道具碰撞
    void applyItemEffect(ItemType type);  // 应用道具效果

public:
    // 在构造函数中初始化这些值
    static constexpr float DEFAULT_PLAYER_HEALTH = 100.0f;
    static constexpr float HEALTH_PACK_AMOUNT = 30.0f;
    static constexpr float SPEED_BOOST_DURATION = 5.0f;  // 5秒
    static constexpr float SPEED_BOOST_MULTIPLIER = 1.5f;


public:
    Game(int width, int height);
    ~Game();

    void run();
    bool checkCollision(float x, float y);

    // 准星相关参数
    enum class CrosshairStyle {
        CLASSIC,    // 经典十字
        DOT,        // 点
        CIRCLE,     // 圆形
        DYNAMIC     // 动态准星
    };

    CrosshairStyle crosshairStyle = CrosshairStyle::DYNAMIC;
    float crosshairSpread = 0.0f;        // 准星扩散程度
    const float MAX_SPREAD = 20.0f;      // 最大扩散
    const float SPREAD_RECOVERY = 5.0f;   // 每秒恢复速度
    const float MOVE_SPREAD = 10.0f;      // 移动时的扩散
    const float SHOOT_SPREAD = 15.0f;     // 射击时的扩散
    int TEMP = 0;
    int TEMP2 = 1;
    int crosshairX;
    int crosshairY;

    // 准星配置
    static const WORD CROSSHAIR_COLOR = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    static const wchar_t CROSSHAIR_CHAR = L'十';  // 使用中文字符"十"作为准星

    void drawCrosshair();
    void updateCrosshair(float deltaTime);

    // 枪支状态
    struct Weapon {
        bool isFiring;            // 是否正在开火
        float fireAnimTime;       // 开火动画计时器
        float recoilOffset;       // 后坐力偏移
        int ammo;                // 弹药数量
        float lastFireTime;       // 上次开火时间

        Weapon() : isFiring(false), fireAnimTime(0.0f),
            recoilOffset(0.0f), ammo(30), lastFireTime(0.0f) {
        }
    } weapon;

    // 武器常量
    static constexpr float FIRE_ANIM_DURATION = 0.1f;   // 开火动画持续时间
    static constexpr float FIRE_RATE = 0.15f;          // 射击间隔
    static constexpr float MAX_RECOIL = 2.0f;          // 最大后坐力
    static constexpr float RECOIL_RECOVERY = 4.0f;     // 后坐力恢复速度

    void drawWeapon();           // 渲染武器
    void updateWeapon(float deltaTime);  // 更新武器状态
    void fireWeapon();           // 开火函数

    // 添加深度缓冲相关变量
    std::vector<float> zBuffer;
    static constexpr float MAX_RENDER_DISTANCE = 20.0f;
    static const wchar_t ITEM_MAP_CHAR = L'M';

    // 添加渲染优先级控制
    enum class RenderLayer {
        WALL = 0,
        ITEM = 1,
        ENEMY = 2,
        WEAPON = 3
    };


    struct RenderObject {
        float depth;
        RenderLayer layer;
        std::function<void()> renderFunc;

        RenderObject(float d, RenderLayer l, std::function<void()> func)
            : depth(d), layer(l), renderFunc(func) {
        }

        // 排序用的比较运算符
        bool operator<(const RenderObject& other) const {
            if (layer != other.layer) {
                return static_cast<int>(layer) < static_cast<int>(other.layer);
            }
            return depth > other.depth;  // 远处的先渲染
        }
    };
    std::vector<RenderObject> renderQueue;

};

