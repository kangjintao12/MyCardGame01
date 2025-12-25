#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include "cocos2d.h"
#include "models/UndoModel.h"
#include "models/GameModel.h"
#include "views/GameView.h"

/**
 * @brief 回退管理器：管理回退记录，实现游戏操作回滚
 */
class UndoManager
{
public:
    UndoManager();
    ~UndoManager();

    /**
     * @brief 初始化回退管理器
     * @param gameModel 游戏模型
     * @param gameView 游戏视图
     */
    void init(GameModel* gameModel, GameView* gameView);

    /**
     * @brief 添加回退记录
     * @param record 回退记录
     */
    void addUndoRecord(const UndoRecord& record);

    /**
     * @brief 执行一次回退操作
     * @return bool 是否回退成功
     */
    bool undo();

    /**
     * @brief 判断是否有可回退的记录
     * @return bool 有可回退记录返回true，否则返回false
     */
    bool hasUndoRecord() const;

private:
    GameModel* _gameModel;    // 游戏模型（可修改）
    GameView* _gameView;      // 游戏视图（可操作）

    // 处理备用牌替换底牌的回退
    bool handleBackupToBottomUndo(const UndoRecord& record);
    // 处理主牌区卡牌替换底牌的回退
    bool handlePlayFieldToBottomUndo(const UndoRecord& record);
};

#endif // __UNDO_MANAGER_H__