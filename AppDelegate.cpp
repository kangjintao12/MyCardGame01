#include "AppDelegate.h"
#include "cocos2d.h"
#include "controllers/GameController.h"
// Test scenes
#include "tests/StackViewTests.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // 初始化导演
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
        // 设置窗口大小和设计分辨率（符合需求）
        glview = GLViewImpl::createWithRect("CardGame01", Rect(0, 0, 1080, 2080), 0.5);
        director->setOpenGLView(glview);
    }

    // 设置设计分辨率
    glview->setDesignResolutionSize(1080, 2080, ResolutionPolicy::FIXED_WIDTH);

    // 关闭帧率显示
    director->setDisplayStats(false);
    // 设置帧率
    director->setAnimationInterval(1.0 / 60);

    // 创建游戏场景
    auto scene = Scene::create();
    // 创建游戏控制器并启动第一关
    GameController* gameController = new GameController();
    gameController->startGame(1, scene);

    // 运行场景
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
}