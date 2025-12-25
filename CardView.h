#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include "models/CardModel.h"
#include <functional>
#include <string>

class CardView : public cocos2d::Sprite
{
public:
    static CardView* create(const CardModel* cardModel);
    virtual bool init(const CardModel* cardModel);

    // 设置卡牌点击回调
    void setCardClickCallback(const std::function<void(int)>& callback);
    // 获取卡牌ID
    int getCardId() const;
    // 移动卡牌到目标位置
    void moveToPosition(const cocos2d::Vec2& targetPos, float duration);
    // 更新卡牌状态（覆盖/翻开）
    void updateCardState(CardState state);

protected:
    CardView();
    virtual ~CardView();

    // 绘制卡牌正面
    void drawCardFront();
    // 绘制卡牌背面
    void drawCardBack();
    // 获取数字图标路径
    std::string getNumberIconPath(bool isBig, bool isRed, CardFaceType face);
    // 获取花色图标路径
    std::string getSuitIconPath(CardSuitType suit);

    // 触摸事件回调
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

private:
    // 【注】非静态成员仅在类内部声明，无需在类外定义
    int _cardId;                     // 卡牌ID
    CardSuitType _cardSuit;          // 花色
    CardFaceType _cardFace;          // 牌面
    CardState _cardState;            // 卡牌状态
    std::function<void(int)> _cardClickCallback; // 卡牌点击回调
};

#endif // __CARD_VIEW_H__