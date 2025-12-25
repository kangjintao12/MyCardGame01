#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include "cocos2d.h"
#include "PlayFieldModel.h"
#include "StackModel.h"
#include "UndoModel.h"

/**
 * @brief 游戏总模型：存储游戏所有动态数据
 */
class GameModel : public cocos2d::Ref
{
public:
    GameModel();
    ~GameModel();

    static GameModel* create();

    // 获取/设置主牌区模型
    PlayFieldModel* getPlayFieldModel() const { return _playFieldModel; }
    void setPlayFieldModel(PlayFieldModel* model); // 仅保留声明（实现移到cpp）

    // 获取/设置堆牌区模型
    StackModel* getStackModel() const { return _stackModel; }
    void setStackModel(StackModel* model); // 仅保留声明

    // 获取/设置回退模型
    UndoModel* getUndoModel() const { return _undoModel; }
    void setUndoModel(UndoModel* model); // 仅保留声明

    // 获取/设置关卡ID
    int getLevelId() const { return _levelId; }
    void setLevelId(int levelId) { _levelId = levelId; }

    // 序列化（存档）
    std::string serialize() const;
    // 反序列化（读档）
    void deserialize(const std::string& data);

private:
    PlayFieldModel* _playFieldModel; // 主牌区模型
    StackModel* _stackModel;         // 堆牌区模型
    UndoModel* _undoModel;           // 回退模型
    int _levelId;                    // 关卡ID
};

#endif // __GAME_MODEL_H__