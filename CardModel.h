#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include "cocos2d.h"
#include <string>

// 花色类型（复用Thrift定义）
enum CardSuitType
{
    CST_NONE = -1,
    CST_CLUBS,      // 梅花
    CST_DIAMONDS,   // 方块
    CST_HEARTS,     // 红桃
    CST_SPADES,     // 黑桃
    CST_NUM_CARD_SUIT_TYPES
};

// 牌面类型（复用Thrift定义）
enum CardFaceType
{
    CFT_NONE = -1,
    CFT_ACE,
    CFT_TWO,
    CFT_THREE,
    CFT_FOUR,
    CFT_FIVE,
    CFT_SIX,
    CFT_SEVEN,
    CFT_EIGHT,
    CFT_NINE,
    CFT_TEN,
    CFT_JACK,
    CFT_QUEEN,
    CFT_KING,
    CFT_NUM_CARD_FACE_TYPES
};

// 卡牌状态
enum CardState
{
    CS_COVERED,  // 覆盖状态
    CS_OPENED,   // 翻开状态
    CS_REMOVED   // 已移除状态
};

/**
 * @brief 卡牌模型：存储单张卡牌的核心数据
 */
class CardModel : public cocos2d::Ref
{
public:
    // 构造函数
    CardModel();
    ~CardModel();

    // 创建实例
    static CardModel* create(int cardId, CardSuitType suit, CardFaceType face, cocos2d::Vec2 position, CardState state = CS_OPENED);

    // 获取/设置卡牌ID
    int getCardId() const { return _cardId; }
    void setCardId(int cardId) { _cardId = cardId; }

    // 获取/设置花色
    CardSuitType getCardSuit() const { return _cardSuit; }
    void setCardSuit(CardSuitType suit) { _cardSuit = suit; }

    // 获取/设置牌面
    CardFaceType getCardFace() const { return _cardFace; }
    void setCardFace(CardFaceType face) { _cardFace = face; }

    // 获取/设置位置
    cocos2d::Vec2 getPosition() const { return _position; }
    void setPosition(cocos2d::Vec2 position) { _position = position; }

    // 获取/设置卡牌状态
    CardState getCardState() const { return _cardState; }
    void setCardState(CardState state) { _cardState = state; }

    // 序列化（存档用）
    std::string serialize() const;
    // 反序列化（读档用）
    void deserialize(const std::string& data);

private:
    int _cardId;                 // 唯一卡牌ID
    CardSuitType _cardSuit;      // 花色
    CardFaceType _cardFace;      // 牌面
    cocos2d::Vec2 _position;     // 位置
    CardState _cardState;        // 状态
};

#endif // __CARD_MODEL_H__