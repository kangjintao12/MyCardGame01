#ifndef __STACK_CONTROLLER_H__
#define __STACK_CONTROLLER_H__

#include "cocos2d.h"
#include "models/StackModel.h"
#include "views/StackView.h"
#include "managers/UndoManager.h"

/**
 * @brief 堆牌区控制器：处理堆牌区备用牌点击、底牌管理的业务逻辑
 */
class StackController
{
public:
    StackController();
    ~StackController();

    /**
     * @brief 初始化堆牌区控制器
     * @param stackModel 堆牌区模型
     * @param stackView 堆牌区视图
     * @param undoManager 回退管理器
     */
    void init(StackModel* stackModel, StackView* stackView, UndoManager* undoManager);

    /**
     * @brief 处理备用牌点击事件
     * @param cardId 被点击的备用牌ID
     * @return bool 处理成功返回true，否则返回false
     */
    bool handleBackupCardClick(int cardId);

private:
    StackModel* _stackModel;      // 堆牌区模型
    StackView* _stackView;        // 堆牌区视图
    UndoManager* _undoManager;    // 回退管理器

    /**
     * @brief 执行备用牌替换底牌的逻辑
     * @param cardId 备用牌ID
     * @return bool 执行成功返回true，否则返回false
     */
    bool replaceBottomWithBackupCard(int cardId);
};

#endif // __STACK_CONTROLLER_H__