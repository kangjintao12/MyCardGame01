#ifndef __STACK_VIEW_H__
#define __STACK_VIEW_H__

#include "cocos2d.h"
#include "CardView.h"
#include "models/StackModel.h"
#include <vector>

// 备用牌点击回调
typedef std::function<void(int cardId)> BackupCardClickCallback;
// 回退按钮点击回调
typedef std::function<void()> UndoBtnClickCallback;

/**
 * @brief 堆牌区视图：负责堆牌区界面绘制、卡牌管理和回退按钮
 */
class StackView : public cocos2d::Layer
{
public:
    StackView();
    ~StackView();

    static StackView* create(const StackModel* stackModel);

    // 初始化
    bool init(const StackModel* stackModel);

    // 设置备用牌点击回调
    void setBackupCardClickCallback(const BackupCardClickCallback& callback) { _backupCardClickCallback = callback; }
    // 设置回退按钮点击回调
    void setUndoBtnClickCallback(const UndoBtnClickCallback& callback) { _undoBtnClickCallback = callback; }

#ifdef RUN_STACKVIEW_UNIT_TESTS
    // Helpers for unit tests (only enabled when RUN_STACKVIEW_UNIT_TESTS is defined)
    bool hasBackupCardClickCallback() const { return static_cast<bool>(_backupCardClickCallback); }
    bool hasUndoBtnClickCallback() const { return static_cast<bool>(_undoBtnClickCallback); }
    void triggerBackupCardClick(int cardId) { if (_backupCardClickCallback) _backupCardClickCallback(cardId); }
    void triggerUndoBtnClick() { if (_undoBtnClickCallback) _undoBtnClickCallback(); }
#endif

    // 根据卡牌ID获取卡牌视图
    CardView* getCardViewById(int cardId) const;

    // 移动卡牌到目标位置
    void moveCardToPosition(int cardId, const cocos2d::Vec2& targetPos, float duration = 0.3f);

    // 更新底牌视图
    void updateBottomCardView(CardModel* newBottomCard);

private:
    const StackModel* _stackModel;                          // 堆牌区模型（只读）
    std::vector<CardView*> _backupCardViews;                // 备用牌视图列表
    CardView* _bottomCardView;                              // 底牌视图
    BackupCardClickCallback _backupCardClickCallback;       // 备用牌点击回调
    UndoBtnClickCallback _undoBtnClickCallback;             // 回退按钮点击回调

    // 初始化堆牌区背景
    void initBackground();
    // 创建备用牌视图
    void createBackupCardViews();
    // 创建底牌视图
    void createBottomCardView();
    // 创建回退按钮
    void createUndoButton();
};

#endif // __STACK_VIEW_H__