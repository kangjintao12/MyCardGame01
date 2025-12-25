#ifndef GAME_MODEL_FROM_LEVEL_GENERATOR_H
#define GAME_MODEL_FROM_LEVEL_GENERATOR_H

#include "models/GameModel.h"
#include "models/PlayFieldModel.h"
#include "models/StackModel.h"
#include "models/UndoModel.h"
#include "models/CardModel.h"
#include "configs/LevelConfig.h"

/**
 * @brief 从关卡配置生成游戏模型的工具类
 */
class GameModelFromLevelGenerator
{
public:
    /**
     * @brief 根据关卡配置生成完整游戏模型
     * @param levelConfig 关卡配置（包含主牌区/堆牌区卡牌配置）
     * @param levelId 关卡ID
     * @return GameModel* 生成的游戏模型（Cocos2d-x Ref自动内存管理）
     */
    static GameModel* generateGameModel(const LevelConfig& levelConfig, int levelId);

private:
    /**
     * @brief 将卡牌配置转换为卡牌模型（修正参数类型：LevelConfig → CardConfig）
     * @param config 单张卡牌的配置信息
     * @param cardId 卡牌唯一ID
     * @return CardModel* 生成的卡牌模型
     */
    static CardModel* convertCardConfigToCardModel(const CardConfig& config, int cardId);
};

#endif // GAME_MODEL_FROM_LEVEL_GENERATOR_H