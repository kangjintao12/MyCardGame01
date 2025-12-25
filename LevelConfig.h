#ifndef __LEVEL_CONFIG_H__
#define __LEVEL_CONFIG_H__

#include "cocos2d.h"
#include <vector>

// 卡牌配置结构体（对应JSON中的单张卡牌）
struct CardConfig
{
    int cardFace;          // 牌面类型（对应CardFaceType）
    int cardSuit;          // 花色类型（对应CardSuitType）
    cocos2d::Vec2 position; // 初始位置

    CardConfig() : cardFace(-1), cardSuit(-1), position(0, 0) {}
};

// 关卡配置类（存储单关的所有静态配置）
class LevelConfig
{
public:
    std::vector<CardConfig> playfieldCards; // 主牌区卡牌配置
    std::vector<CardConfig> stackCards;     // 堆牌区卡牌配置

    // 构造函数：初始化第一关配置（嵌入JSON内容）
    LevelConfig(int levelId);
};

// 关卡配置加载器（负责生成关卡配置）
class LevelConfigLoader
{
public:
    /**
     * @brief 加载指定关卡的配置
     * @param levelId 关卡ID
     * @return LevelConfig 关卡配置实例
     */
    static LevelConfig loadLevelConfig(int levelId);
};

#endif // __LEVEL_CONFIG_H__