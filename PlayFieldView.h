#ifndef __PLAY_FIELD_VIEW_H__
#define __PLAY_FIELD_VIEW_H__

#include "cocos2d.h"
#include "models/PlayFieldModel.h"
#include "views/CardView.h"
#include <functional>
#include <vector>

class PlayFieldView : public cocos2d::Layer
{
public:
    static PlayFieldView* create(const PlayFieldModel* playFieldModel);
    virtual bool init(const PlayFieldModel* playFieldModel);

    // 设置卡牌点击回调
    void setPlayFieldCardClickCallback(const std::function<void(int)>& callback);
    // 根据ID获取卡牌视图
    CardView* getCardViewById(int cardId) const;
    // 移除指定ID的卡牌视图
    void removeCardViewById(int cardId);
    // 【修改】默认参数移至头文件声明
    void moveCardToPosition(int cardId, const cocos2d::Vec2& targetPos, float duration = 0.3f);
    // 为单个 CardModel 创建并添加 CardView（Undo 恢复时使用）
    void addCardViewForModel(CardModel* card);

protected:
    PlayFieldView();
    virtual ~PlayFieldView();

    // 初始化背景
    void initBackground();
    // 创建所有卡牌视图
    void createCardViews();

private:
    // 【注】非静态成员仅在类内部声明，无需在类外定义
    const PlayFieldModel* _playFieldModel; // 主牌区数据模型（只读）
    std::vector<CardView*> _cardViews;     // 卡牌视图列表
    std::function<void(int)> _cardClickCallback; // 卡牌点击回调
};

#endif // __PLAY_FIELD_VIEW_H__