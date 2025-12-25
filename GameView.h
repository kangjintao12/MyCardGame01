#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include "cocos2d.h"
#include "PlayFieldView.h"
#include "StackView.h"
#include "models/GameModel.h"

/**
 * @brief 游戏总视图：整合主牌区和堆牌区视图，管理游戏整体UI
 */
class GameView : public cocos2d::Layer
{
public:
    GameView();
    ~GameView();

    static GameView* create(const GameModel* gameModel);

    // 初始化
    bool init(const GameModel* gameModel);

    // 获取主牌区视图
    PlayFieldView* getPlayFieldView() const { return _playFieldView; }

    // 获取堆牌区视图
    StackView* getStackView() const { return _stackView; }

private:
    const GameModel* _gameModel;          // 游戏模型（只读）
    PlayFieldView* _playFieldView;        // 主牌区视图
    StackView* _stackView;                // 堆牌区视图

    // 初始化视图布局
    void initViewLayout();
};

#endif // __GAME_VIEW_H__