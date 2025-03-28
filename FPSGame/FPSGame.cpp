#include "Game.h"
#include <iostream>
#include <conio.h>
#include <io.h>
#include <fcntl.h>

//子弹显示
//地图添加玩家朝向
// 优化枪
//地图优化
//优化玩家血量
//结束窗口优化
//开始窗口优化

int main() {
    // 设置控制台标题
    SetConsoleTitleA("3D Shooter Game");
    SetConsoleOutputCP(GB2312_CHARSET);
    // 获取控制台句柄   
    //SetConsoleOutputCP(65001);

    // 显示游戏说明
    std::cout << "\n\n\n\n\n\n";
    std::cout << "                                             ";
    std::cout << "3D 射击游戏\n\n";
    std::cout << "                                             ";
    std::cout << "控制:\n";
    std::cout << "                                             ";
    std::cout << "W,A,S,D - 移动\n";
    std::cout << "                                             ";
    std::cout << "左/右箭头 - 旋转\n";
    std::cout << "                                             ";
    std::cout << "R - 换弹\n";
    std::cout << "                                             ";
    std::cout << "空格 - 射击\n";
    std::cout << "                                             ";
    std::cout << "ESC - 退出\n\n\n";
    // 在main函数中的游戏说明部分添加
    std::cout << "                                             ";
    std::cout << "道具说明:\n";
    std::cout << "                                             ";
    std::cout << "H - 生命值补充\n";
    std::cout << "                                             ";
    std::cout << "A - 弹药补充\n";
    std::cout << "                                             ";
    std::cout << "S - 临时提升速度\n\n\n";
    std::cout << "                                             ";
    std::cout << "按任意键开始...\n";
    _getch();


    try {
        // 创建游戏实例 (80x25是标准控制台大小)
        Game game(165, 50);
        game.run();
    }
    catch (const std::exception& e) {
        std::cout << "Error\n";
        std::cout << "窗口太小，推荐全屏游玩\n";
        std::cout << "                 Genshin\n";
        //std::cout << "" << e.what() << std::endl;
        return 1;
    }

    return 0;
}