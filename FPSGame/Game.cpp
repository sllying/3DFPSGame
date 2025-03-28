#include "Game.h"
#include <iostream>
#include <conio.h>
#include <thread>
#include <algorithm>
#include <io.h>
#include <fcntl.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef Enemies
//怪物数量
#define Enemiescount 5
#endif // !Enemies

#include <windows.h>

//包含头文件和预处理指令
//#include "M_Pi": 包含一个名为"M_Pi"的头文件。
//#ifndef和#endif: 用于条件编译，防止头文件被重复包含。
//#include <windows.h>: 包含Windows API的头文件，用于在Windows系统上进行编程。
//析构函数和构造函数
//~Game() : 游戏类的析构函数，用于释放游戏对象占用的资源。
//Game(int width, int height) : 游戏类的构造函数，用于初始化游戏对象，设置游戏窗口的宽度和高度。
//游戏物品相关函数
//spawnItems() : 生成游戏中的物品。
//updateItems() : 更新物品的状态，如位置、动画等。
//checkItemCollision() : 检查玩家与物品之间的碰撞。
//applyItemEffect(ItemType type) : 应用物品对玩家的效果，如增加生命值、武器等。
//drawItems() : 绘制游戏中的物品。
//drawItemPattern(const char* pattern, int size, int columnX, WORD color, float distance) : 绘制物品的图案，根据给定的模式、大小、列位置、颜色和距离进行绘制。
//游戏消息和界面相关函数
//showMessageBox(const std::wstring& title, const std::wstring& message) : 显示一个消息框，带有标题和消息内容。
//handleGameOver() : 处理游戏结束的逻辑，如显示游戏结束界面、记录分数等。
//showGameOverScreen() : 显示游戏结束界面。
//游戏控制台和地图相关函数
//initializeConsole() : 初始化游戏的控制台，可能用于调试或显示游戏信息。
//generateMap() : 生成游戏地图，包括墙壁、路径等元素。
//敌人相关函数
//spawnEnemies(int count) : 生成指定数量的敌人。
//checkCollision(float x, float y) : 检查在指定坐标处是否发生碰撞，可能用于检测敌人与玩家或墙壁之间的碰撞。
//子弹相关函数
//updateBullets(float deltaTime) : 更新子弹的位置和状态，根据时间间隔进行计算。
//checkBulletCollisions() : 检查子弹与敌人或其他物体之间的碰撞。
//射线投射和十字准线相关函数
//castRays() : 投射射线，可能用于实现光线追踪或视野效果。
//drawCrosshair() : 绘制十字准线，通常用于第一人称射击游戏中。
//updateCrosshair(float deltaTime) : 更新十字准线的位置或状态。
//敌人渲染相关函数
//drawEnemy(float distance, float angle, int screenX) : 绘制敌人，根据距离、角度和屏幕X坐标进行绘制。
//renderEnemies() : 渲染所有敌人，调用drawEnemy函数。
//updateEnemyRendering(Enemy& enemy) : 更新单个敌人的渲染状态。
//墙壁绘制相关函数
//drawWall(int x, float perpWallDist, float wallHeight) : 绘制墙壁，根据X坐标、垂直墙壁距离和墙壁高度进行绘制。
//getWallChar(float brightness) : 获取墙壁的字符表示，可能根据亮度进行选择。
//getWallColor(float brightness) : 获取墙壁的颜色，根据亮度进行调整。
//天空和地板相关函数
//getSkyChar(float brightness) : 获取天空的字符表示。
//getSkyColor(float brightness) : 获取天空的颜色。
//getFloorChar(float brightness) : 获取地板的字符表示。
//getFloorColor(float brightness) : 获取地板的颜色。
//武器和HUD相关函数
//drawWeapon() : 绘制武器，可能显示在玩家视角的底部或角落。
//updateWeapon(float deltaTime) : 更新武器的状态，如动画、换弹等。
//fireWeapon() : 处理武器的射击逻辑。
//drawHUD() : 绘制游戏的HUD（Heads - Up Display），显示生命值、弹药等信息。
//drawMinimap() : 绘制小地图，帮助玩家了解周围环境和敌人位置。
//游戏帧渲染和更新函数
//renderFrame() : 渲染游戏的一帧，包括绘制所有元素和更新显示。
//handleInput() : 处理玩家的输入，如键盘和鼠标事件。
//updateGame(float deltaTime) : 更新游戏的状态，包括玩家移动、敌人AI等，根据时间间隔进行计算。
//resetGame() : 重置游戏状态，可能用于重新开始游戏。
//run() : 运行游戏的主循环，调用其他函数来持续更新和渲染游戏。
//以上解释基于常见的游戏开发模式和函数命名惯例，具体的实现细节可能因项目而异。


void Game::cleanup() {
    try {
        // 清理屏幕缓冲区
        if (screenBuffer) {
            delete[] screenBuffer;
            screenBuffer = nullptr;
        }

        // 清理其他资源
        zBuffer.clear();
        renderQueue.clear();
        enemies.clear();

        // 重置控制台
        if (hConsole != INVALID_HANDLE_VALUE && hConsole != nullptr) {
            SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
            hConsole = nullptr;
        }
    }
    catch (...) {
        // 确保即使清理过程出错也不会抛出异常
    }

    isCleaningUp = false;
}


Game::Game(int width, int height)
    : screenWidth(width)
    , screenHeight(height)
    , playerX(2.0f)
    , playerY(2.0f)
    , playerAngle(3 * M_PI / 7)
    , playerFOV(3.14159f / 3.0f)
    , moveSpeed(5.0f)
    , rotateSpeed(3.0f)
    , isRunning(true)
    , score(0)
    , crosshairSpread(0.0f)  // 初始化准星扩散值
    , playerHealth(DEFAULT_PLAYER_HEALTH)
    , maxPlayerHealth(DEFAULT_PLAYER_HEALTH)
    , speedBoostTimer(0.0f)
    , originalMoveSpeed(5.0f)  // 存储原始移动速度
    , isUnderAttack(false)
    , attackCooldown(0.0f)
{

    try {
        // 确保宽度是偶数
        screenWidth = (screenWidth + 1) & ~1;

        // 获取标准输出句柄
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hConsole == INVALID_HANDLE_VALUE) {
            throw std::runtime_error("Failed to get console handle");
        }

        // 设置控制台模式
        DWORD consoleMode;
        if (!GetConsoleMode(hConsole, &consoleMode)) {
            throw std::runtime_error("Failed to get console mode");
        }
        consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if (!SetConsoleMode(hConsole, consoleMode)) {
            throw std::runtime_error("Failed to set console mode");
        }

        SMALL_RECT windowRect2 = { 0, 0, static_cast<SHORT>(screenWidth - 1), static_cast<SHORT>(screenHeight - 1) };
        COORD bufferSize = { static_cast<SHORT>(screenWidth), static_cast<SHORT>(screenHeight) };

        // 分配屏幕缓冲区
        screenBuffer = new CHAR_INFO[screenWidth * screenHeight];
        if (!screenBuffer) {
            throw std::runtime_error("Failed to allocate screen buffer");
        }

        // 设置控制台窗口和缓冲区大小
        SMALL_RECT windowRect = { 0, 0, static_cast<SHORT>(screenWidth - 1), static_cast<SHORT>(screenHeight - 1) };
        //COORD bufferSize = { static_cast<SHORT>(screenWidth), static_cast<SHORT>(screenHeight) };

        if (!SetConsoleWindowInfo(hConsole, TRUE, &windowRect)) {
            throw std::runtime_error("Failed to set console window info");
        }

        if (!SetConsoleScreenBufferSize(hConsole, bufferSize)) {
            throw std::runtime_error("Failed to set console buffer size");
        }
        // 初始化准星位置
        crosshairX = screenWidth / 2;
        crosshairY = screenHeight / 2;

        // 初始化其他资源
        zBuffer.resize(screenWidth);
        renderQueue.clear();

        // 初始化其他游戏组件
        generateMap();
        spawnEnemies(Enemiescount);
        spawnItems();
    }
    catch (const std::exception& e) {
        cleanup();
        throw;
    }
}

void Game::spawnItems() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(1, mapWidth - 2);
    std::uniform_int_distribution<> disY(1, mapHeight - 2);
    std::uniform_int_distribution<> disType(0, 2);

    // 生成一些随机道具
    for (int i = 0; i < 5; i++) {
        float x, y;
        bool validPosition = false;

        while (!validPosition) {
            x = disX(gen) + 0.5f;
            y = disY(gen) + 0.5f;

            if (!checkCollision(x, y)) {
                validPosition = true;
            }
        }

        ItemType type = static_cast<ItemType>(disType(gen));
        items.emplace_back(x, y, type);
    }
}

void Game::updateItems() {
    // 更新速度提升计时器
    if (speedBoostTimer > 0) {
        speedBoostTimer -= 0.016f;  // 假设固定时间步长
        if (speedBoostTimer <= 0) {
            moveSpeed = originalMoveSpeed;  // 恢复原始速度
        }
    }
}

void Game::checkItemCollision() {
    const float PICKUP_RADIUS = 0.5f;

    for (auto& item : items) {
        if (!item.active) continue;

        float dx = playerX - item.x;
        float dy = playerY - item.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist < PICKUP_RADIUS) {
            applyItemEffect(item.type);
            item.active = false;
        }
    }
}

void Game::applyItemEffect(ItemType type) {
    switch (type) {
    case ItemType::HEALTH_PACK:
        playerHealth = min(playerHealth + HEALTH_PACK_AMOUNT, maxPlayerHealth);
        break;

    case ItemType::AMMO_PACK:
        weapon.ammo += 30;  // 补充一个弹夹的弹药
        break;

    case ItemType::SPEED_BOOST:
        if (speedBoostTimer <= 0) {  // 如果没有处于加速状态
            originalMoveSpeed = moveSpeed;
            moveSpeed *= SPEED_BOOST_MULTIPLIER;
        }
        speedBoostTimer = SPEED_BOOST_DURATION;
        break;
    }
}

void Game::drawItems() {
    constexpr float VERY_CLOSE_DISTANCE = 2.0f;   // 非常近
    constexpr float CLOSE_DISTANCE = 4.0f;        // 近距离
    constexpr float MEDIUM_DISTANCE = 6.0f;       // 中等距离
    constexpr float MAX_DISTANCE = 7.0f;          // 最远可见距离

    for (const auto& item : items) {
        if (!item.active) continue;

        float dx = item.x - playerX;
        float dy = item.y - playerY;
        float distance = std::sqrt(dx * dx + dy * dy);

        // 超出最大距离不显示
        if (distance > MAX_DISTANCE) continue;

        float angle = std::atan2(dy, dx);
        while (angle - playerAngle > M_PI) angle -= 2 * M_PI;
        while (angle - playerAngle < -M_PI) angle += 2 * M_PI;

        float angleDiff = angle - playerAngle;
        if (std::abs(angleDiff) < playerFOV / 2.0f) {
            float screenX = (0.5f + angleDiff / playerFOV) * screenWidth;

            if (screenX >= 0 && screenX < screenWidth) {
                int columnX = static_cast<int>(screenX);

                if (distance < zBuffer[columnX]) {
                    renderQueue.emplace_back(distance, RenderLayer::ITEM,
                        [this, &item, distance, columnX]() {
                            // 根据距离选择不同的显示模式
                            const char* pattern;
                            WORD color;

                            // 获取基础颜色
                            switch (item.type) {
                            case ItemType::HEALTH_PACK:
                                color = FOREGROUND_RED | FOREGROUND_INTENSITY;
                                break;
                            case ItemType::AMMO_PACK:
                                color = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
                                break;
                            case ItemType::SPEED_BOOST:
                                color = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
                                break;
                            default:
                                return;
                            }

                            // 根据距离选择显示模式
                            if (distance <= VERY_CLOSE_DISTANCE) {
                                // 非常近 - 5x5 详细图案
                                switch (item.type) {
                                case ItemType::HEALTH_PACK:
                                    pattern =
                                        "[+-+]"
                                        "|HHP|"
                                        "+-=-+"
                                        "|HHP|"
                                        "[+-+]";
                                    break;
                                case ItemType::AMMO_PACK:
                                    pattern =
                                        "[===]"
                                        "|AMP|"
                                        "|-*-|"
                                        "|AMP|"
                                        "[===]";
                                    break;
                                case ItemType::SPEED_BOOST:
                                    pattern =
                                        "[>^<]"
                                        "|SPD|"
                                        "=>*<="
                                        "|SPD|"
                                        "[>v<]";
                                    break;
                                default:
                                    return;
                                }
                                drawItemPattern(pattern, 5, columnX, color, distance);
                            }
                            else if (distance <= CLOSE_DISTANCE) {
                                // 近距离 - 3x3 简化图案
                                switch (item.type) {
                                case ItemType::HEALTH_PACK:
                                    pattern =
                                        "+-+"
                                        "|H|"
                                        "+-+";
                                    break;
                                case ItemType::AMMO_PACK:
                                    pattern =
                                        "[=]"
                                        "AMO"
                                        "[=]";
                                    break;
                                case ItemType::SPEED_BOOST:
                                    pattern =
                                        ">^<"
                                        "SPD"
                                        ">v<";
                                    break;
                                default:
                                    return;
                                }
                                drawItemPattern(pattern, 3, columnX, color, distance);
                            }
                            else if (distance <= MEDIUM_DISTANCE) {
                                // 中等距离 - 简单符号
                                switch (item.type) {
                                case ItemType::HEALTH_PACK:
                                    pattern = "[+]";
                                    break;
                                case ItemType::AMMO_PACK:
                                    pattern = "[A]";
                                    break;
                                case ItemType::SPEED_BOOST:
                                    pattern = "[S]";
                                    break;
                                default:
                                    return;
                                }
                                drawItemPattern(pattern, 1, columnX, color, distance);
                            }
                            else {
                                // 远距离 - 单个点
                                pattern = "*";
                                drawItemPattern(pattern, 1, columnX, color & ~FOREGROUND_INTENSITY, distance);
                            }
                        });
                }
            }
        }
    }
}

// 添加辅助函数来绘制道具图案
void Game::drawItemPattern(const char* pattern, int size, int columnX, WORD color, float distance) {
    int centerY = screenHeight / 2;
    float scale = 1.0f / distance;
    //int itemSize = static_cast<int>(screenHeight * scale * 0.2f);
    //itemSize = max(size, itemSize);
    int itemSize = size;

    int startY = centerY - itemSize / 2;
    int startX = columnX - itemSize / 2;

    //int startY = centerY - size / 2;
    //int startX = columnX - size / 2;

    // 添加轻微浮动效果
    float floatOffset = sin(GetTickCount64() * 0.002f) * 2.0f;
    startY += static_cast<int>(floatOffset);

    // 绘制图案
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int screenY = startY + y * (itemSize / size);
            int screenX = startX + x * (itemSize / size);

            if (screenX >= 0 && screenX < screenWidth &&
                screenY >= 0 && screenY < screenHeight) {
                int index = screenY * screenWidth + screenX;
                char c = pattern[y * size + x];
                if (c != ' ') {  // 不绘制空格
                    screenBuffer[index].Char.AsciiChar = c;
                    screenBuffer[index].Attributes = color;
                }
            }
        }
    }

    // 添加简单阴影
    int shadowY = startY + itemSize;
    if (shadowY < screenHeight) {
        for (int x = 0; x < itemSize; x++) {
            int screenX = startX + x;
            if (screenX >= 0 && screenX < screenWidth) {
                int index = shadowY * screenWidth + screenX;
                screenBuffer[index].Char.AsciiChar = '-';
                screenBuffer[index].Attributes = FOREGROUND_INTENSITY;
            }
        }
    }
}

Game::~Game() {
    //delete[] screenBuffer;
    //cleanup();
}

void Game::showMessageBox(const std::wstring& title, const std::wstring& message) {
    MessageBoxW(nullptr, message.c_str(), title.c_str(), MB_OK | MB_ICONINFORMATION);
}

void Game::handleGameOver() {
    try
    {
        if (isGameOver) return;  // 防止重复处理

        isGameOver = true;
        showGameOverScreen();

        // 显示游戏结束弹窗
        showMessageBox(L"游戏结束", L"最终得分: " + std::to_wstring(score));


        // 等待一段时间或等待按键
        Sleep(2000);  // 等待2秒

        // 安全清理
        //cleanup();
    }
    catch (const std::exception&)
    {

    }

}

void Game::showGameOverScreen() {
    try
    {
        if (!screenBuffer) return;
        SMALL_RECT writeArea = { 0, 0, static_cast<SHORT>(screenWidth - 1), static_cast<SHORT>(screenHeight - 1) };
        COORD bufferSize = { static_cast<SHORT>(screenWidth), static_cast<SHORT>(screenHeight) };
        COORD bufferCoord = { 0, 0 };


        // 清空屏幕
        for (int i = 0; i < screenWidth * screenHeight; i++) {
            screenBuffer[i].Char.UnicodeChar = L' ';
            screenBuffer[i].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        }

        // 准备游戏结束消息
        std::wstring message = isVictory ? L"胜利！" : L"游戏结束";
        std::wstring subMessage = L"按任意键退出...";

        // 计算消息位置
        int messageX = (screenWidth - message.length()) / 2;
        int messageY = screenHeight / 2;

        // 渲染主消息
        for (size_t i = 0; i < message.length(); i++) {
            int index = messageY * screenWidth + messageX + i;
            if (index >= 0 && index < screenWidth * screenHeight) {
                screenBuffer[index].Char.UnicodeChar = message[i];
                screenBuffer[index].Attributes =
                    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            }
        }

        // 渲染子消息
        int subMessageX = (screenWidth - subMessage.length()) / 2;
        int subMessageY = messageY + 2;

        for (size_t i = 0; i < subMessage.length(); i++) {
            int index = subMessageY * screenWidth + subMessageX + i;
            if (index >= 0 && index < screenWidth * screenHeight) {
                screenBuffer[index].Char.UnicodeChar = subMessage[i];
                screenBuffer[index].Attributes =
                    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            }
        }

        // 更新屏幕
        SMALL_RECT writeArea2 = { 0, 0,
            static_cast<SHORT>(screenWidth - 1),
            static_cast<SHORT>(screenHeight - 1) };
        COORD bufferSize0 = { static_cast<SHORT>(screenWidth),
            static_cast<SHORT>(screenHeight) };
        //COORD bufferCoord = { 0, 0 };

        WriteConsoleOutput(hConsole, screenBuffer, bufferSize,
            bufferCoord, &writeArea);

    }
    catch (const std::exception&)
    {

    }
}


void Game::initializeConsole() {
    try
    {
        CONSOLE_CURSOR_INFO cursorInfo;

        // 获取控制台句柄
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);

        // 设置控制台窗口大小
        SMALL_RECT windowSize2 = { 0, 0, static_cast<SHORT>(screenWidth - 1), static_cast<SHORT>(screenHeight - 1) };
        SetConsoleWindowInfo(hConsole, TRUE, &windowSize2);

        // 设置控制台缓冲区大小
        COORD bufferSize3 = { static_cast<SHORT>(screenWidth), static_cast<SHORT>(screenHeight) };
        SetConsoleScreenBufferSize(hConsole, bufferSize3);

        // 隐藏光标
        //CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hConsole, &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(hConsole, &cursorInfo);

        // 设置输入模式
        SetConsoleMode(hConsoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

    }
    catch (const std::exception&)
    {

    }
}

void Game::generateMap() {
    try
    {
        mapWidth = 20;  // 地图宽度
        mapHeight = 20; // 地图高度

        // 初始化空地图
        map.resize(mapHeight);
        for (int y = 0; y < mapHeight; y++) {
            map[y].resize(mapWidth, ' ');
        }

        // 生成外墙
        for (int x = 0; x < mapWidth; x++) {
            map[0][x] = '#';            // 上边界
            map[mapHeight - 1][x] = '#';  // 下边界
        }
        for (int y = 0; y < mapHeight; y++) {
            map[y][0] = '#';           // 左边界
            map[y][mapWidth - 1] = '#';  // 右边界
        }

        // 生成中间的墙，留一个通道
        int centerX = mapWidth / 2;    // 中间位置
        int doorSize = 3;              // 通道大小
        int doorStart = mapHeight / 2 - doorSize / 2;  // 通道起始位置

        for (int y = 1; y < mapHeight - 1; y++) {
            // 如果不在通道范围内，放置墙
            if (y < doorStart || y > doorStart + doorSize) {
                map[y][centerX] = '#';
            }
        }




        // 确保出生点附近没有墙
        int spawnAreaSize = 3;
        for (int y = 1; y <= spawnAreaSize; y++) {
            for (int x = 1; x <= spawnAreaSize; x++) {
                map[y][x] = ' ';  // 清除左上角的墙，作为出生点区域
            }
        }


        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> disX(1, mapWidth - 2);
        std::uniform_int_distribution<> disY(1, mapHeight - 2);

        int numWalls = 10; // 生成墙的数量
        for (int i = 0; i < numWalls; i++) {
            int x = disX(gen);
            int y = disY(gen);
            if (map[y][x] == ' ') {
                map[y][x] = '#';
            }
        }
    }
    catch (const std::exception&)
    {

    }
}

void Game::spawnEnemies(int count) {
    try
    {
            std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(1, mapWidth - 2);
    std::uniform_int_distribution<> disY(1, mapHeight - 2);

    const float MIN_DISTANCE_FROM_PLAYER = 5.0f; // 与玩家的最小距离

    for (int i = 0; i < count; i++) {
        float x, y;
        bool validPosition = false;

        // 尝试找到有效的生成位置
        int attempts = 0;
        const int MAX_ATTEMPTS = 100;

        while (!validPosition && attempts < MAX_ATTEMPTS) {
            x = disX(gen) + 0.5f;
            y = disY(gen) + 0.5f;

            // 检查是否在墙上
            if (map[static_cast<int>(y)][static_cast<int>(x)] == '#') {
                attempts++;
                continue;
            }

            // 检查与玩家的距离
            float distToPlayer = std::sqrt(
                std::pow(x - playerX, 2) +
                std::pow(y - playerY, 2)
            );

            if (distToPlayer >= MIN_DISTANCE_FROM_PLAYER) {
                validPosition = true;
            }

            attempts++;
        }

        if (validPosition) {
            enemies.emplace_back(x, y);
        }
    }
    }
    catch (const std::exception&)
    {

    }

}

bool Game::checkCollision(float x, float y) {
    int mapX = static_cast<int>(x);
    int mapY = static_cast<int>(y);

    return map[mapY][mapX] == '#';
}


void Game::updateBullets(float deltaTime) {
    for (auto& bullet : bullets) {
        if (!bullet.active)
            continue;

        float moveX = cosf(bullet.angle) * 10.0f * deltaTime;
        float moveY = sinf(bullet.angle) * 10.0f * deltaTime;

        bullet.x += moveX;
        bullet.y += moveY;

        // 检查是否击中墙壁
        if (map[static_cast<int>(bullet.y)][static_cast<int>(bullet.x)] == '#') {
            bullet.active = false;
        }
    }

    // 移除无效的子弹
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return !b.active; }),
        bullets.end());
}

void Game::checkBulletCollisions() {
    for (auto& bullet : bullets) {
        if (!bullet.active)
            continue;

        for (auto& enemy : enemies) {
            if (!enemy.isAlive)
                continue;

            float dx = bullet.x - enemy.x;
            float dy = bullet.y - enemy.y;
            float dist = sqrtf(dx * dx + dy * dy);

            if (dist < 0.5f) { // 击中敌人
                enemy.isAlive = false;
                bullet.active = false;
                score += 100;
            }
        }
    }
}

void Game::castRays() {
    // 初始化/重置深度缓冲区
    zBuffer.resize(screenWidth, MAX_RENDER_DISTANCE);
    renderQueue.clear();

    for (int x = 0; x < screenWidth; x++) {
        // 计算射线角度
        float rayAngle = (playerAngle - playerFOV / 2.0f) +
            ((float)x / (float)screenWidth) * playerFOV;

        float sinRay = sin(rayAngle);
        float cosRay = cos(rayAngle);

        // DDA算法变量
        float rayDirX = cosRay;
        float rayDirY = sinRay;

        int mapX = static_cast<int>(playerX);
        int mapY = static_cast<int>(playerY);

        float deltaDistX = std::abs(1.0f / rayDirX);
        float deltaDistY = std::abs(1.0f / rayDirY);

        float sideDistX;
        float sideDistY;

        int stepX;
        int stepY;

        // 计算初始步长和方向
        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (playerX - mapX) * deltaDistX;
        }
        else {
            stepX = 1;
            sideDistX = (mapX + 1.0f - playerX) * deltaDistX;
        }

        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (playerY - mapY) * deltaDistY;
        }
        else {
            stepY = 1;
            sideDistY = (mapY + 1.0f - playerY) * deltaDistY;
        }

        // DDA算法
        bool hit = false;
        bool side = false; // 是否击中垂直面
        float perpWallDist = 0.0f; // 初始化距离变量

        while (!hit) {
            // 移动到下一个地图方格
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = false;
                perpWallDist = (mapX - playerX + (1 - stepX) / 2) / rayDirX;
            }
            else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = true;
                perpWallDist = (mapY - playerY + (1 - stepY) / 2) / rayDirY;
            }

            // 检查是否超出地图范围
            if (mapX < 0 || mapX >= mapWidth || mapY < 0 || mapY >= mapHeight) {
                perpWallDist = MAX_RENDER_DISTANCE;
                break;
            }

            // 检查是否击中墙
            if (map[mapY][mapX] == '#') {
                hit = true;
            }

            // 检查是否超出最大渲染距离
            if (perpWallDist > MAX_RENDER_DISTANCE) {
                perpWallDist = MAX_RENDER_DISTANCE;
                break;
            }
        }

        // 存储这一列的深度信息
        zBuffer[x] = perpWallDist;

        // 计算墙的高度
        int lineHeight;
        if (perpWallDist > 0) {
            lineHeight = static_cast<int>(screenHeight / perpWallDist);
        }
        else {
            lineHeight = screenHeight;
            perpWallDist = 0.1f; // 防止除以零
        }

        // 将墙的渲染添加到渲染队列
        renderQueue.emplace_back(perpWallDist, RenderLayer::WALL,
            [this, x, perpWallDist, lineHeight, side]() {
                drawWall(x, perpWallDist, lineHeight);
            });
    }
}


void Game::drawCrosshair() {
    // 计算准星在屏幕上的位置
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    // 确保位置有效
    if (centerX < 0 || centerX >= screenWidth ||
        centerY < 0 || centerY >= screenHeight) {
        return;
    }

    // 计算缓冲区索引
    size_t index = centerY * screenWidth + centerX;

    // 检查索引是否有效
    if (index >= screenWidth * screenHeight) {
        return;
    }

    try {
        // 绘制简单的十字准星
        // 中心点
        screenBuffer[index].Char.UnicodeChar = CROSSHAIR_CHAR;
        screenBuffer[index].Attributes = CROSSHAIR_COLOR;

        // 上
        if (centerY > 0) {
            screenBuffer[(centerY - 1) * screenWidth + centerX].Char.UnicodeChar = L'│';
            screenBuffer[(centerY - 1) * screenWidth + centerX].Attributes = CROSSHAIR_COLOR;
        }

        // 下
        if (centerY < screenHeight - 1) {
            screenBuffer[(centerY + 1) * screenWidth + centerX].Char.UnicodeChar = L'│';
            screenBuffer[(centerY + 1) * screenWidth + centerX].Attributes = CROSSHAIR_COLOR;
        }

        // 左
        if (centerX > 0) {
            screenBuffer[centerY * screenWidth + centerX - 1].Char.UnicodeChar = L'─';
            screenBuffer[centerY * screenWidth + centerX - 1].Attributes = CROSSHAIR_COLOR;
        }

        // 右
        if (centerX < screenWidth - 1) {
            screenBuffer[centerY * screenWidth + centerX + 1].Char.UnicodeChar = L'─';
            screenBuffer[centerY * screenWidth + centerX + 1].Attributes = CROSSHAIR_COLOR;
        }
    }
    catch (const std::exception& e) {
        // 错误处理
        // 可以添加日志记录
    }
}

void Game::updateCrosshair(float deltaTime) {
    try
    {
        // 根据玩家状态更新准星扩散
        bool isMoving = false;
        if (GetAsyncKeyState('W') & 0x8000 ||
            GetAsyncKeyState('S') & 0x8000 ||
            GetAsyncKeyState('A') & 0x8000 ||
            GetAsyncKeyState('D') & 0x8000) {
            isMoving = true;
        }

        // 更新准星扩散
        if (isMoving) {
            crosshairSpread = min(crosshairSpread + 5.0f * deltaTime, 10.0f);
        }
        else {
            crosshairSpread = max(0.0f, crosshairSpread - 3.0f * deltaTime);
        }

    }
    catch (const std::exception&)
    {

    }
}

void Game::drawEnemy(float distance, float angle, int screenX) {
    try {
        // 计算敌人在屏幕上的大小
        float enemyHeight = (screenHeight / distance) * ENEMY_SIZE;

        // 计算敌人在屏幕上的位置
        int drawStart = -enemyHeight / 2 + screenHeight / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = enemyHeight / 2 + screenHeight / 2;
        if (drawEnd >= screenHeight) drawEnd = screenHeight - 1;

        // 计算敌人的宽度，确保最小宽度
        int enemyWidth = static_cast<int>(enemyHeight * 0.6f); // 保持合适的宽高比
        //enemyWidth = max(enemyWidth, 5); // 确保最小宽度为5
        int startX = screenX - enemyWidth / 2;
        int endX = screenX + enemyWidth / 2;

        // 确保在屏幕范围内
        startX = max(0, startX);
        endX = min(screenWidth - 1, endX);

        // 根据距离计算亮度
        float brightness = 1.0f - min(distance / MAX_RENDER_DISTANCE, 0.9f);

        // 使用不同的字符表示敌人的不同部位
        const wchar_t* enemyChars[] = {
            L"╭───╮", // 头部
            L"│ @ │", // 眼睛
            L"│███│", // 身体
            L"╰───╯"  // 腿部
        };

        // 计算每个部位的位置
        float partHeight = enemyHeight / 4; // 4个部位

        for (int x = startX; x <= endX; x++) {
            if (x < 0 || x >= screenWidth) continue;

            // 只有当敌人在当前列的深度小于已存储的深度时才渲染
            if (distance >= zBuffer[x]) continue;

            for (int y = drawStart; y <= drawEnd; y++) {
                if (y < 0 || y >= screenHeight) continue;

                // 计算当前像素位于哪个部位
                int partIndex = static_cast<int>((y - drawStart) / partHeight);
                partIndex = min(3, max(0, partIndex));

                // 计算在当前部位中的相对位置
                if (enemyWidth == 0) {
                    enemyWidth = 1;
                }
                int relativeX = (x - startX) * 5 / enemyWidth; // 调整相对位置计算
                if (relativeX >= 0 && relativeX < 5) { // 每个部位5个字符宽
                    wchar_t ch = enemyChars[partIndex][relativeX];

                    // 设置颜色属性
                    WORD attributes = FOREGROUND_RED;  // 基础红色

                    // 根据距离和部位调整颜色
                    if (brightness > 0.7f) {
                        attributes |= FOREGROUND_INTENSITY;  // 近处时更亮
                    }

                    // 特殊部位的颜色处理
                    if (partIndex == 1 && ch == L'@') {  // 眼睛
                        attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;  // 明亮的红色
                    }

                    // 渲染像素
                    int index = y * screenWidth + x;
                    if (index >= 0 && index < screenWidth * screenHeight) {
                        screenBuffer[index].Char.UnicodeChar = ch;
                        screenBuffer[index].Attributes = attributes;
                    }
                }
            }
        }

    }
    catch (const std::exception&) {
        // 错误处理
    }
}
void Game::renderEnemies() {
    constexpr float VERY_CLOSE_DISTANCE = 1.0f;  // 定义近身距离阈值

    for (const auto& enemy : enemies) {
        if (!enemy.isAlive) continue;

        // 计算敌人相对于玩家的方向和距离
        float dx = enemy.x - playerX;
        float dy = enemy.y - playerY;
        float distance = std::sqrt(dx * dx + dy * dy);

        // 计算敌人的角度
        float angle = std::atan2(dy, dx);

        // 调整角度范围到 [-π, π]
        while (angle - playerAngle > M_PI) angle -= 2 * M_PI;
        while (angle - playerAngle < -M_PI) angle += 2 * M_PI;

        // 近身检测和处理
        if (distance <= VERY_CLOSE_DISTANCE) {
            float angleDiff = std::abs(angle - playerAngle);
            if (angleDiff > M_PI / 2) {
                // 敌人在玩家后方，强制显示在视野内
                float offset = (angle > playerAngle) ? 0.1f : -0.1f;
                angle = playerAngle + offset;
                distance = max(distance, 0.5f); // 确保最小显示距离
            }
        }

        // 检查敌人是否在视野内（近身时始终为true）
        float angleDiff = angle - playerAngle;
        if (distance <= VERY_CLOSE_DISTANCE || std::abs(angleDiff) < playerFOV / 2.0f) {
            // 计算敌人在屏幕上的X坐标
            float screenX = (0.5f + angleDiff / playerFOV) * screenWidth;

            // 处理近身情况下的屏幕位置
            if (distance <= VERY_CLOSE_DISTANCE) {
                // 确保敌人显示在屏幕中央附近
                screenX = max(screenWidth * 0.3f, min(screenX, screenWidth * 0.7f));
            }

            // 检查是否在屏幕范围内
            if (screenX >= 0 && screenX < screenWidth) {
                int columnX = static_cast<int>(screenX);

                // 近身时总是显示（忽略深度缓冲）
                if (distance <= VERY_CLOSE_DISTANCE || distance < zBuffer[columnX]) {
                    // 将敌人的渲染添加到渲染队列
                    renderQueue.emplace_back(distance, RenderLayer::ENEMY,
                        [this, distance, angle, columnX]() {
                            drawEnemy(distance, angle, columnX);
                        });
                }
            }
        }
    }
}
void Game::updateEnemyRendering(Enemy& enemy) {
    constexpr float VERY_CLOSE_DISTANCE = 1.0f;  // 定义近身距离阈值

    // 计算敌人相对于玩家的位置
    float dx = enemy.x - playerX;
    float dy = enemy.y - playerY;
    float distance = std::sqrt(dx * dx + dy * dy);
    float angle = std::atan2(dy, dx);

    // 调整角度到 [-π, π] 范围
    while (angle - playerAngle > M_PI) angle -= 2 * M_PI;
    while (angle - playerAngle < -M_PI) angle += 2 * M_PI;

    // 如果敌人近身，强制显示在玩家视野内
    if (distance <= VERY_CLOSE_DISTANCE) {
        // 计算敌人是否在玩家后方
        float angleDiff = std::abs(angle - playerAngle);
        if (angleDiff > M_PI / 2) {  // 敌人在玩家后方
            // 强制将敌人显示在玩家正前方，稍微偏离中心
            float offset = (angle > playerAngle) ? 0.1f : -0.1f;
            angle = playerAngle + offset;

            // 确保最小显示距离
            distance = max(distance, 0.5f);
        }
    }

    float screenX = (0.5f + (angle - playerAngle) / playerFOV) * screenWidth;

    if (screenX >= 0 && screenX < screenWidth) {
        int columnX = static_cast<int>(screenX);

        // 只在当前深度小于已存储深度时渲染
        if (distance < zBuffer[columnX]) {
            // 将敌人的渲染添加到渲染队列
            renderQueue.emplace_back(distance, RenderLayer::ENEMY,
                [this, distance, angle, columnX]() {
                    drawEnemy(distance, angle, columnX);
                });
        }
    }
}


void Game::drawWall(int x, float perpWallDist, float wallHeight) {
    try
    {
        int drawStart = -wallHeight / 2 + screenHeight / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = wallHeight / 2 + screenHeight / 2;
        if (drawEnd >= screenHeight) drawEnd = screenHeight - 1;

        // 计算墙壁的亮度和纹理
        float brightness = 1.0f - min(perpWallDist / 8.0f, 0.9f);
        char wallChar = getWallChar(brightness);
        WORD wallColor = getWallColor(brightness);

        // 绘制天空渐变
        for (int y = 0; y < drawStart; y++) {
            float skyBrightness = 1.0f - (float)y / drawStart;
            screenBuffer[y * screenWidth + x].Char.AsciiChar = getSkyChar(skyBrightness);
            screenBuffer[y * screenWidth + x].Attributes = getSkyColor(skyBrightness);
        }

        // 绘制墙壁渐变
        for (int y = drawStart; y < drawEnd; y++) {
            float wallGradient = (float)(y - drawStart) / (drawEnd - drawStart);
            screenBuffer[y * screenWidth + x].Char.AsciiChar = wallChar;
            screenBuffer[y * screenWidth + x].Attributes = wallColor;
        }

        // 绘制地板渐变
        for (int y = drawEnd; y < screenHeight; y++) {
            float floorBrightness = 1.0f - (float)(y - drawEnd) / (screenHeight - drawEnd);
            screenBuffer[y * screenWidth + x].Char.AsciiChar = getFloorChar(floorBrightness);
            screenBuffer[y * screenWidth + x].Attributes = getFloorColor(floorBrightness);
        }

    }
    catch (const std::exception&)
    {

    }


}

// 获取墙壁字符
char Game::getWallChar(float brightness) {
    // 使用不同的ASCII字符创造深度感
    static const char* wallChars = " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
    int index = (int)(brightness * (strlen(wallChars) - 1));
    return wallChars[index];
}

// 获取墙壁颜色
WORD Game::getWallColor(float brightness) {
    WORD baseColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    if (brightness > 0.8f) {
        return baseColor | FOREGROUND_INTENSITY;
    }
    else if (brightness > 0.5f) {
        return baseColor;
    }
    else if (brightness > 0.3f) {
        return baseColor & ~FOREGROUND_INTENSITY;
    }
    else {
        return FOREGROUND_INTENSITY;
    }
}

// 获取天空字符
char Game::getSkyChar(float brightness) {
    static const char* skyChars = " .`'";
    int index = (int)(brightness * (strlen(skyChars) - 1));
    return skyChars[index];
}

// 获取天空颜色
WORD Game::getSkyColor(float brightness) {
    if (brightness > 0.7f) {
        return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    }
    else if (brightness > 0.4f) {
        return FOREGROUND_BLUE;
    }
    else {
        return FOREGROUND_BLUE & ~FOREGROUND_INTENSITY;
    }
}

// 获取地板字符
char Game::getFloorChar(float brightness) {
    static const char* floorChars = " ._-=";
    int index = (int)(brightness * (strlen(floorChars) - 1));
    return floorChars[index];
}

// 获取地板颜色
WORD Game::getFloorColor(float brightness) {
    WORD baseColor = FOREGROUND_GREEN;
    if (brightness > 0.7f) {
        return baseColor | FOREGROUND_INTENSITY;
    }
    else if (brightness > 0.4f) {
        return baseColor;
    }
    else {
        return baseColor & ~FOREGROUND_INTENSITY;
    }
}

void Game::drawWeapon() {
    // 定义枪支的 ASCII 艺术图案
    const wchar_t* weaponIdle[] = {  // 静止状态的枪
        L"   ┌────┐   ",
        L"   │====│   ",
        L"┌──┘    └──┐",
        L"└──┐ AK  ┌─┘",
        L"   └────┘   "
    };

    const wchar_t* weaponFiring[] = {  // 开火状态的枪
        L"   **********",
        L"   │====│ ",
        L"┌──┘    └──┐",
        L"└──┐ AK  ┌─┘",
        L"   └────┘   "
    };

    // 计算武器在屏幕上的位置
    int weaponWidth = 13;  // 武器图案的宽度
    int weaponHeight = 5;  // 武器图案的高度
    int startX = screenWidth - weaponWidth - 2;  // 右边留出2个字符的边距
    int startY = screenHeight - weaponHeight - 1; // 下边留出1个字符的边距

    // 应用后坐力偏移
    startY += static_cast<int>(weapon.recoilOffset);

    // 选择要显示的图案（开火或静止）
    const wchar_t** currentFrame = weapon.isFiring ? weaponFiring : weaponIdle;

    // 渲染武器
    for (int y = 0; y < weaponHeight; y++) {
        for (int x = 0; x < weaponWidth; x++) {
            int screenX = startX + x;
            int screenY = startY + y;

            // 检查是否在屏幕范围内
            if (screenX >= 0 && screenX < screenWidth &&
                screenY >= 0 && screenY < screenHeight) {

                int index = screenY * screenWidth + screenX;
                if (index >= 0 && index < screenWidth * screenHeight) {
                    // 设置字符和颜色
                    screenBuffer[index].Char.UnicodeChar = currentFrame[y][x];

                    // 为开火动画设置特殊颜色
                    if (weapon.isFiring && currentFrame[y][x] == L'*') {
                        // 火焰效果使用明亮的黄色
                        screenBuffer[index].Attributes =
                            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                    }
                    else {
                        // 武器主体使用灰色
                        screenBuffer[index].Attributes =
                            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
                    }
                }
            }
        }
    }

    // 渲染弹药信息
    wchar_t ammoText[32];
    swprintf_s(ammoText, L"弹药: %d", weapon.ammo);
    int ammoX = screenWidth - 10;
    int ammoY = screenHeight - 1;

    for (int i = 0; ammoText[i] != L'\0'; i++) {
        int index = ammoY * screenWidth + (ammoX + i);
        if (index >= 0 && index < screenWidth * screenHeight) {
            screenBuffer[index].Char.UnicodeChar = ammoText[i];
            screenBuffer[index].Attributes =
                FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        }
    }
}
void Game::updateWeapon(float deltaTime) {
    // 更新开火动画
    if (weapon.isFiring) {
        weapon.fireAnimTime += deltaTime;
        if (weapon.fireAnimTime >= FIRE_ANIM_DURATION) {
            weapon.isFiring = false;
            weapon.fireAnimTime = 0.0f;
        }
    }

    // 更新后坐力
    if (weapon.recoilOffset > 0) {
        weapon.recoilOffset = max(0.0f,
            weapon.recoilOffset - RECOIL_RECOVERY * deltaTime);
    }
}
void Game::fireWeapon() {
    float currentTime = static_cast<float>(GetTickCount64()) / 1000.0f;

    // 检查射击间隔和弹药
    if (currentTime - weapon.lastFireTime >= FIRE_RATE && weapon.ammo > 0) {
        weapon.isFiring = true;
        weapon.fireAnimTime = 0.0f;
        weapon.recoilOffset += 1.0f;
        weapon.recoilOffset = min(weapon.recoilOffset, MAX_RECOIL);
        weapon.lastFireTime = currentTime;
        weapon.ammo--;

        // 播放射击音效（如果有的话）
        // PlaySound(...);
    }
}
void Game::drawHUD() {
    // 绘制分数

    
    std::wstring scoreText = L"分 数 : " + std::to_wstring(score) + L" 关 卡 ：" + std::to_wstring(TEMP2);
    for (size_t i = 0; i < scoreText.length(); i++) {
        screenBuffer[i].Char.UnicodeChar = scoreText[i];
        screenBuffer[i].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }



    // 显示剩余敌人数量
    int aliveEnemies = std::count_if(enemies.begin(), enemies.end(),
        [](const Enemy& e) { return e.isAlive; });
    std::wstring enemyText = L"剩 余 敌 人 : " + std::to_wstring(aliveEnemies);
    for (size_t i = 0; i < enemyText.length(); i++) {
        screenBuffer[screenWidth + i].Char.UnicodeChar = enemyText[i];
        screenBuffer[screenWidth + i].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
    }

    // 显示血量
    std::wstring healthText = L"血 量 : " + std::to_wstring(static_cast<int>(playerHealth));
    for (size_t i = 0; i < healthText.length(); i++) {
        screenBuffer[2 * screenWidth + i].Char.UnicodeChar = healthText[i];
        screenBuffer[2 * screenWidth + i].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
    }
    


    if (isUnderAttack) {
        std::wstring attackMessage = L"正 在 受 到 攻 击 !";
        for (size_t i = 0; i < attackMessage.length(); i++) {
            screenBuffer[3 * screenWidth + i].Char.UnicodeChar = attackMessage[i];
            screenBuffer[3 * screenWidth + i].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
        }
        TEMP++;
        if (TEMP > 100)
        {
            isUnderAttack = false;
			TEMP = 0;
        }
    }
    else {
		std::wstring attackMessage = L"保 持 谨 慎 !";
		for (size_t i = 0; i < attackMessage.length(); i++) {
			screenBuffer[3 * screenWidth + i].Char.UnicodeChar = attackMessage[i];
			screenBuffer[3 * screenWidth + i].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
		}
    }

}
void Game::drawMinimap() {

    // 计算小地图的缩放比例
    float scaleX = static_cast<float>(MINIMAP_WIDTH) / mapWidth;
    float scaleY = static_cast<float>(MINIMAP_HEIGHT) / mapHeight;

    // 绘制地图内容
    for (int y = 0; y < MINIMAP_HEIGHT; y++) {
        for (int x = 0; x < MINIMAP_WIDTH; x++) {
            // 计算对应的实际地图坐标
            int mapX = static_cast<int>(x / scaleX);
            int mapY = static_cast<int>(y / scaleY);

            // 确保坐标在地图范围内
            mapX = min(mapX, mapWidth - 1);
            mapY = min(mapY, mapHeight - 1);

            // 计算缓冲区位置
            int screenIndex = (y + MINIMAP_Y) * screenWidth + (x + MINIMAP_X);

            // 检查缓冲区边界
            if (screenIndex >= 0 && screenIndex < screenWidth * screenHeight) {
                // 绘制地图元素
                if (map[mapY][mapX] == '#') {
                    // 墙 - 使用"墙"字
                    screenBuffer[screenIndex].Char.UnicodeChar = WALL_CHAR;
                    screenBuffer[screenIndex].Attributes =
                        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
                }
                else {
                    // 空地 - 使用"地"字
                    screenBuffer[screenIndex].Char.UnicodeChar = SPACE_CHAR;
                    screenBuffer[screenIndex].Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                }
            }
        }
    }

    // 绘制玩家位置
    int playerScreenX = static_cast<int>(playerX * scaleX) + MINIMAP_X;
    int playerScreenY = static_cast<int>(playerY * scaleY) + MINIMAP_Y;

    if (playerScreenX >= MINIMAP_X &&
        playerScreenX < MINIMAP_X + MINIMAP_WIDTH &&
        playerScreenY >= MINIMAP_Y &&
        playerScreenY < MINIMAP_Y + MINIMAP_HEIGHT) {

        int playerIndex = playerScreenY * screenWidth + playerScreenX;
        if (playerIndex >= 0 && playerIndex < screenWidth * screenHeight) {
            // 玩家 - 使用"我"字
            screenBuffer[playerIndex].Char.UnicodeChar = PLAYER_CHAR;
            screenBuffer[playerIndex].Attributes =
                FOREGROUND_RED | FOREGROUND_INTENSITY;
        }
    }

    // 绘制敌人位置
    for (const auto& enemy : enemies) {
        if (!enemy.isAlive) continue;

        int enemyScreenX = static_cast<int>(enemy.x * scaleX) + MINIMAP_X;
        int enemyScreenY = static_cast<int>(enemy.y * scaleY) + MINIMAP_Y;

        if (enemyScreenX >= MINIMAP_X &&
            enemyScreenX < MINIMAP_X + MINIMAP_WIDTH &&
            enemyScreenY >= MINIMAP_Y &&
            enemyScreenY < MINIMAP_Y + MINIMAP_HEIGHT) {

            int enemyIndex = enemyScreenY * screenWidth + enemyScreenX;
            if (enemyIndex >= 0 && enemyIndex < screenWidth * screenHeight) {
                // 敌人 - 使用"敌"字
                screenBuffer[enemyIndex].Char.UnicodeChar = ENEMY_CHAR;
                screenBuffer[enemyIndex].Attributes =
                    FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            }
        }
    }

    // 绘制小地图边框
    // 绘制四个角
    // 左上角
    screenBuffer[(MINIMAP_Y - 1) * screenWidth + (MINIMAP_X - 1)].Char.UnicodeChar = BORDER_CHAR_TL;
    // 右上角
    screenBuffer[(MINIMAP_Y - 1) * screenWidth + (MINIMAP_X + MINIMAP_WIDTH)].Char.UnicodeChar = BORDER_CHAR_TR;
    // 左下角
    screenBuffer[(MINIMAP_Y + MINIMAP_HEIGHT) * screenWidth + (MINIMAP_X - 1)].Char.UnicodeChar = BORDER_CHAR_BL;
    // 右下角
    screenBuffer[(MINIMAP_Y + MINIMAP_HEIGHT) * screenWidth + (MINIMAP_X + MINIMAP_WIDTH)].Char.UnicodeChar = BORDER_CHAR_BR;

    // 绘制水平边框
    for (int x = MINIMAP_X; x < MINIMAP_X + MINIMAP_WIDTH; x++) {
        // 上边框
        screenBuffer[(MINIMAP_Y - 1) * screenWidth + x].Char.UnicodeChar = BORDER_CHAR_H;
        // 下边框
        screenBuffer[(MINIMAP_Y + MINIMAP_HEIGHT) * screenWidth + x].Char.UnicodeChar = BORDER_CHAR_H;
    }

    // 绘制垂直边框
    for (int y = MINIMAP_Y; y < MINIMAP_Y + MINIMAP_HEIGHT; y++) {
        // 左边框
        screenBuffer[y * screenWidth + (MINIMAP_X - 1)].Char.UnicodeChar = BORDER_CHAR_V;
        // 右边框
        screenBuffer[y * screenWidth + (MINIMAP_X + MINIMAP_WIDTH)].Char.UnicodeChar = BORDER_CHAR_V;
    }

    // 为所有边框设置颜色
    for (int y = MINIMAP_Y - 1; y <= MINIMAP_Y + MINIMAP_HEIGHT; y++) {
        for (int x = MINIMAP_X - 1; x <= MINIMAP_X + MINIMAP_WIDTH; x++) {
            int index = y * screenWidth + x;
            if (index >= 0 && index < screenWidth * screenHeight) {
                wchar_t ch = screenBuffer[index].Char.UnicodeChar;
                if (ch == BORDER_CHAR_V || ch == BORDER_CHAR_H ||
                    ch == BORDER_CHAR_TL || ch == BORDER_CHAR_TR ||
                    ch == BORDER_CHAR_BL || ch == BORDER_CHAR_BR) {
                    screenBuffer[index].Attributes = FOREGROUND_INTENSITY;
                }
            }
        }
    }

    for (const auto& item : items) {
        if (!item.active) continue;

        // 计算道具在小地图上的位置
        float scaleX = static_cast<float>(MINIMAP_WIDTH) / mapWidth;
        float scaleY = static_cast<float>(MINIMAP_HEIGHT) / mapHeight;

        int itemScreenX = static_cast<int>(item.x * scaleX) + MINIMAP_X;
        int itemScreenY = static_cast<int>(item.y * scaleY) + MINIMAP_Y;

        if (itemScreenX >= MINIMAP_X &&
            itemScreenX < MINIMAP_X + MINIMAP_WIDTH &&
            itemScreenY >= MINIMAP_Y &&
            itemScreenY < MINIMAP_Y + MINIMAP_HEIGHT) {

            int index = itemScreenY * screenWidth + itemScreenX;
            if (index >= 0 && index < screenWidth * screenHeight) {
                // 根据道具类型设置不同的颜色
                WORD itemColor;
                switch (item.type) {
                case ItemType::HEALTH_PACK:
                    itemColor = FOREGROUND_RED | FOREGROUND_INTENSITY;
                    break;
                case ItemType::AMMO_PACK:
                    itemColor = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                    break;
                case ItemType::SPEED_BOOST:
                    itemColor = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
                    break;
                }

                screenBuffer[index].Char.UnicodeChar = ITEM_MAP_CHAR;
                screenBuffer[index].Attributes = itemColor;
            }
        }
    }
}


void Game::renderFrame() {
    if (!screenBuffer) return;

    // 清空屏幕缓冲区
    for (int i = 0; i < screenWidth * screenHeight; i++) {
        screenBuffer[i].Char.UnicodeChar = L' ';
        screenBuffer[i].Attributes = 0;
    }

    // 投射射线并收集渲染对象
    castRays();

    // 处理敌人渲染
    renderEnemies();


    drawItems();
    // 添加武器渲染
    renderQueue.emplace_back(0.0f, RenderLayer::WEAPON,
        [this]() { drawWeapon(); });



    // 对渲染队列进行排序
    std::sort(renderQueue.begin(), renderQueue.end());

    // 执行渲染
    for (const auto& renderObj : renderQueue) {
        renderObj.renderFunc();
    }

    // 渲染小地图和HUD
    drawMinimap();
    drawHUD();

    // 4. 最后渲染准星（确保在最上层）
    drawCrosshair();


    // 将缓冲区写入控制台
    SMALL_RECT writeArea = { 0, 0,
        static_cast<SHORT>(screenWidth - 1),
        static_cast<SHORT>(screenHeight - 1) };
    COORD bufferSize = { static_cast<SHORT>(screenWidth),
        static_cast<SHORT>(screenHeight) };
    COORD bufferCoord = { 0, 0 };

    WriteConsoleOutput(hConsole, screenBuffer, bufferSize,
        bufferCoord, &writeArea);
}
void Game::handleInput() {
    // 处理键盘输入
    float moveX = 0, moveY = 0;

    if (GetAsyncKeyState('W') & 0x8000) {
        moveX += cosf(playerAngle) * moveSpeed;
        moveY += sinf(playerAngle) * moveSpeed;
    }
    if (GetAsyncKeyState('S') & 0x8000) {
        moveX -= cosf(playerAngle) * moveSpeed;
        moveY -= sinf(playerAngle) * moveSpeed;
    }
    if (GetAsyncKeyState('A') & 0x8000) {
        moveX += sinf(playerAngle) * moveSpeed;
        moveY -= cosf(playerAngle) * moveSpeed;
    }
    if (GetAsyncKeyState('D') & 0x8000) {
        moveX -= sinf(playerAngle) * moveSpeed;
        moveY += cosf(playerAngle) * moveSpeed;
    }

    // 旋转
    if (GetAsyncKeyState(VK_LEFT) & 0x8000)
        playerAngle -= rotateSpeed * 0.016f;
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
        playerAngle += rotateSpeed * 0.016f;

    // 射击
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        bullets.emplace_back(playerX, playerY, playerAngle);
        crosshairSpread = min(crosshairSpread + SHOOT_SPREAD, MAX_SPREAD);
    }

    // 射击控制（使用空格键）
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        fireWeapon();
    }

    // 换弹（使用R键）
    if (GetAsyncKeyState('R') & 0x8000 && weapon.ammo < 30) {
        weapon.ammo = 30;  // 重新装填到满弹夹
    }

    // 退出游戏
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        isRunning = false;

    // 碰撞检测
    float newX = playerX + moveX * 0.016f;
    float newY = playerY + moveY * 0.016f;

    if (!checkCollision(newX, playerY))
        playerX = newX;
    if (!checkCollision(playerX, newY))
        playerY = newY;
}
void Game::updateGame(float deltaTime) {
    // 更新子弹位置


   // 更新武器状态
    updateWeapon(deltaTime);

    updateBullets(deltaTime);

    // 检查子弹碰撞
    checkBulletCollisions();

    updateCrosshair(deltaTime);
    updateItems();
    checkItemCollision();

    if (attackCooldown > 0) {
        attackCooldown -= deltaTime;
    }

    // 如果玩家生命值为0，游戏结束
    if (playerHealth <= 0) {
        isRunning = false;
        isGameOver = true;
    }

    // 更新敌人行为（简单的AI）
    for (auto& enemy : enemies) {
        if (!enemy.isAlive)
            continue;

        // 简单的追踪玩家
        float dx = playerX - enemy.x;
        float dy = playerY - enemy.y;
        float dist = sqrtf(dx * dx + dy * dy);

        if (dist < 0.5f) { // 敌人碰到玩家
            if (attackCooldown <= 0) {
                isUnderAttack = true;
                playerHealth -= 10.0f;  // 每次碰撞减少10点生命值
                attackCooldown = 1.0f;  // 设置攻击冷却时间为1秒
            }

            if (playerHealth <= 0) {
                isRunning = false;
                isGameOver = true;
            }
        }

        if (dist < 5.0f) { // 在一定范围内追踪玩家
            float moveX = (dx / dist) * moveSpeed * 0.5f * deltaTime;
            float moveY = (dy / dist) * moveSpeed * 0.5f * deltaTime;

            float newX = enemy.x + moveX;
            float newY = enemy.y + moveY;

            if (!checkCollision(newX, enemy.y))
                enemy.x = newX;
            if (!checkCollision(enemy.x, newY))
                enemy.y = newY;
        }
    }
}


void Game::resetGame() {
    // 重置玩家位置和角度
    playerX = 2.0f;
    playerY = 2.0f;
    playerAngle = 3 * M_PI / 8; // 初始角度，指向右下角

    // 重置分数
    score = 0;

	// 重置玩家生命值
	playerHealth = 100.0f;

    // 清空子弹和敌人
    bullets.clear();
    enemies.clear();

    // 重新生成地图和敌人
    generateMap();
    spawnEnemies(Enemiescount);

    // 重置其他状态
    isRunning = true;
    isGameOver = false;
    crosshairSpread = 0.0f;
}

void Game::run() {
    try
    {
        auto lastTime = std::chrono::high_resolution_clock::now();

        while (isRunning) {
            // 计算帧时间
            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
            lastTime = currentTime;

            // 游戏循环
            handleInput();
            updateGame(deltaTime);
            renderFrame();

            // 检查游戏结束条件
            if (std::none_of(enemies.begin(), enemies.end(),
                [](const Enemy& e) { return e.isAlive; })) {
                // 所有敌人都被消灭，游戏胜利
                isRunning = false;
            }

            // 帧率限制
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
        }
    }
    catch (const std::exception&)
    {


    }

    // 游戏结束显示
    system("cls");
    std::cout << "\n\n\n\n\n\n\n\n\n";

    if (std::none_of(enemies.begin(), enemies.end(),
        [](const Enemy& e) { return e.isAlive; })) {
        // 所有敌人都被消灭，游戏胜利
        std::cout << "                                             ";

        std::cout << "游 戏 胜 利!\n\n";
		TEMP2++;
    }
    else {
        std::cout << "                                             ";

        std::cout << "游 戏 失 败!\n\n";

    }
    std::cout << "                                             ";

    std::cout << "最终得分: " << score << "\n";
    std::cout << "                                             ";

    std::cout << "...\n";
    //castRays();
    std::cout << "                                             ";

    std::cout << "按ESC退出...\n";
    std::cout << "                                             ";

    std::cout << "按空格重新开始...\n";

    while (true) {
        // 显示提示信息
        Sleep(50);
        // 检测按键
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            // 按下ESC键，退出循环
            break;
        }
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            // 按下空格键，重新开始
            std::cout << "重新开始...\n";
			resetGame();
			run();
        }

        // 添加一个小的延迟，避免占用过多的CPU资源
        Sleep(50);
    }
    //_getch();
    //
}