#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "models/GameModel.h"
#include "views/GameView.h"
#include "PlayFieldController.h"
#include "StackController.h"
#include "managers/UndoManager.h"
#include "configs/LevelConfig.h"
#include "services/GameModelFromLevelGenerator.h"

/**
 * @brief 游戏总控制器：整合所有子控制器，负责游戏全局流程管理
 */
class GameController
{
public:
    GameController();
    ~GameController();

    /**
     * @brief 启动指定关卡
     * @param levelId 关卡ID
     * @param parentNode 游戏视图的父节点
     * @return bool 启动成功返回true，否则返回false
     */
    bool startGame(int levelId, cocos2d::Node* parentNode);

    /**
     * @brief 获取游戏视图
     * @return GameView* 游戏视图实例
     */
    GameView* getGameView() const { return _gameView; }

private:
    GameModel* _gameModel;              // 游戏总模型
    GameView* _gameView;                // 游戏总视图
    PlayFieldController* _playFieldController; // 主牌区控制器
    StackController* _stackController;  // 堆牌区控制器
    UndoManager* _undoManager;          // 回退管理器

    /**
     * @brief 初始化所有子控制器
     * @return bool 初始化成功返回true，否则返回false
     */
    bool initSubControllers();
};

#endif // __GAME_CONTROLLER_H__