#ifndef __PLAY_FIELD_CONTROLLER_H__
#define __PLAY_FIELD_CONTROLLER_H__

#include "cocos2d.h"
#include "models/PlayFieldModel.h"
#include "views/PlayFieldView.h"
#include "models/StackModel.h"
#include "views/StackView.h"
#include "managers/UndoManager.h"

/**
 * @brief 主牌区控制器
 * 职责：协调主牌区模型与视图，处理主牌区卡牌点击事件、匹配规则判断、
 * 卡牌替换底牌逻辑，并记录回退信息，遵循MVC架构设计
 * 使用场景：游戏运行时，作为GameController的子控制器，管理主牌区所有业务逻辑
 */
class PlayFieldController
{
public:
    PlayFieldController();
    ~PlayFieldController();

    /**
     * @brief 初始化主牌区控制器
     * @param playFieldModel 主牌区数据模型
     * @param playFieldView 主牌区视图
     * @param stackModel 堆牌区数据模型
     * @param undoManager 回退管理器
     */
    void init(PlayFieldModel* playFieldModel, PlayFieldView* playFieldView, StackModel* stackModel, UndoManager* undoManager);

    /**
     * @brief 设置堆牌区视图引用
     * @param stackView 堆牌区视图
     */
    void setStackView(StackView* stackView);

    /**
     * @brief 处理主牌区卡牌点击事件
     * @param cardId 被点击卡牌ID
     * @return bool 处理成功返回true
     */
    bool handleCardClick(int cardId);

    /**
     * @brief 判断卡牌是否与底牌匹配
     * @param card 主牌区卡牌
     * @param bottomCard 堆牌区底牌
     * @return bool 匹配返回true
     */
    bool isCardMatchBottom(CardModel* card, CardModel* bottomCard) const;

private:
    PlayFieldModel* _playFieldModel;    // 主牌区数据模型
    PlayFieldView* _playFieldView;      // 主牌区视图
    StackModel* _stackModel;            // 堆牌区数据模型
    StackView* _stackView;              // 堆牌区视图（用于更新底牌显示）
    UndoManager* _undoManager;          // 回退管理器（用于记录操作日志）

    /**
     * @brief 执行主牌区卡牌替换底牌的核心逻辑
     * @param cardId 卡牌ID
     * @return bool 执行成功返回true
     */
    bool replaceBottomWithPlayFieldCard(int cardId);
};

#endif // __PLAY_FIELD_CONTROLLER_H__