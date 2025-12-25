#include "GameModelFromLevelGenerator.h"

GameModel* GameModelFromLevelGenerator::generateGameModel(const LevelConfig& levelConfig, int levelId)
{
    GameModel* gameModel = GameModel::create();
    gameModel->setLevelId(levelId);

    // 初始化主牌区模型
    PlayFieldModel* playFieldModel = PlayFieldModel::create();
    int cardId = 0;
    // 添加主牌区卡牌
    for (const auto& cardConfig : levelConfig.playfieldCards)
    {
        CardModel* card = convertCardConfigToCardModel(cardConfig, cardId++);
        playFieldModel->addCard(card);
    }
    gameModel->setPlayFieldModel(playFieldModel);

    // 初始化堆牌区模型
    StackModel* stackModel = StackModel::create();
    // 添加备用牌（前2张）
    for (size_t i = 0; i < levelConfig.stackCards.size() - 1; ++i)
    {
        const auto& cardConfig = levelConfig.stackCards[i];
        CardModel* card = convertCardConfigToCardModel(cardConfig, cardId++);
        card->setCardState(CS_COVERED); // 备用牌默认覆盖
        stackModel->addBackupCard(card);
    }
    // 设置底牌（最后1张）
    const auto& bottomCardConfig = levelConfig.stackCards.back();
    CardModel* bottomCard = convertCardConfigToCardModel(bottomCardConfig, cardId++);
    stackModel->setBottomCard(bottomCard);
    stackModel->addBottomCardToHistory(bottomCard); // 初始底牌加入历史
    gameModel->setStackModel(stackModel);

    // 初始化回退模型
    UndoModel* undoModel = UndoModel::create();
    gameModel->setUndoModel(undoModel);

    return gameModel;
}

CardModel* GameModelFromLevelGenerator::convertCardConfigToCardModel(const CardConfig& config, int cardId)
{
    CardSuitType suit = (CardSuitType)config.cardSuit;
    CardFaceType face = (CardFaceType)config.cardFace;
    CardState state = (suit == CST_NONE || face == CFT_NONE) ? CS_COVERED : CS_OPENED;

    CardModel* card = CardModel::create(cardId, suit, face, config.position, state);
    return card;
}