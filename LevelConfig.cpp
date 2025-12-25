#include "LevelConfig.h"

// LevelConfig构造函数：初始化第一关数据
LevelConfig::LevelConfig(int levelId)
{
    if (levelId == 1)
    {
        // 主牌区6张卡牌（对应JSON的Playfield）
        CardConfig card1;
        card1.cardFace = 12; // CFT_KING
        card1.cardSuit = 0;  // CST_CLUBS
        card1.position = cocos2d::Vec2(250, 1400);
        playfieldCards.push_back(card1);

        CardConfig card2;
        card2.cardFace = 2; // CFT_THREE
        card2.cardSuit = 0; // CST_CLUBS
        card2.position = cocos2d::Vec2(300, 1200);
        playfieldCards.push_back(card2);

        CardConfig card3;
        card3.cardFace = 2; // CFT_THREE
        card3.cardSuit = 1; // CST_DIAMONDS
        card3.position = cocos2d::Vec2(350, 1000);
        playfieldCards.push_back(card3);

        CardConfig card4;
        card4.cardFace = 2; // CFT_THREE
        card4.cardSuit = 0; // CST_CLUBS
        card4.position = cocos2d::Vec2(850, 1400);
        playfieldCards.push_back(card4);

        CardConfig card5;
        card5.cardFace = 2; // CFT_THREE
        card5.cardSuit = 0; // CST_CLUBS
        card5.position = cocos2d::Vec2(800, 1200);
        playfieldCards.push_back(card5);

        CardConfig card6;
        card6.cardFace = 1; // CFT_TWO
        card6.cardSuit = 3; // CST_SPADES
        card6.position = cocos2d::Vec2(750, 1000);
        playfieldCards.push_back(card6);

        // 堆牌区3张卡牌（对应JSON的Stack）
        CardConfig stackCard1;
        stackCard1.cardFace = 2; // CFT_THREE
        stackCard1.cardSuit = 0; // CST_CLUBS
        stackCard1.position = cocos2d::Vec2(150, 200); // 备用牌1（调整位置，避免重合）
        stackCards.push_back(stackCard1);

        CardConfig stackCard2;
        stackCard2.cardFace = 0; // CFT_ACE
        stackCard2.cardSuit = 2; // CST_HEARTS
        stackCard2.position = cocos2d::Vec2(200, 200); // 备用牌2（不完全重合）
        stackCards.push_back(stackCard2);

        CardConfig stackCard3;
        stackCard3.cardFace = 3; // CFT_FOUR（梅花4）
        stackCard3.cardSuit = 0; // CST_CLUBS
        stackCard3.position = cocos2d::Vec2(550, 200); // 底牌（与备用牌分隔）
        stackCards.push_back(stackCard3);
    }
}

// 加载关卡配置
LevelConfig LevelConfigLoader::loadLevelConfig(int levelId)
{
    return LevelConfig(levelId);
}