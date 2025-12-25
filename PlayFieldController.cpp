#include "PlayFieldController.h"
#include "managers/UndoManager.h"
#include "models/UndoModel.h"
#include "views/StackView.h"

/**
 * @brief 构造函数：初始化成员变量为nullptr
 */
PlayFieldController::PlayFieldController()
    : _playFieldModel(nullptr)
    , _playFieldView(nullptr)
    , _stackModel(nullptr)
    , _stackView(nullptr)
    , _undoManager(nullptr)
{
}

/**
 * @brief 析构函数：清空指针引用，避免野指针
 */
PlayFieldController::~PlayFieldController()
{
    _playFieldModel = nullptr;
    _playFieldView = nullptr;
    _stackModel = nullptr;
    _stackView = nullptr;
    _undoManager = nullptr;
}

/**
 * @brief 初始化主牌区控制器
 * @param playFieldModel 主牌区数据模型
 * @param playFieldView 主牌区视图
 * @param stackModel 堆牌区数据模型（用于匹配判断）
 * @param undoManager 回退管理器（用于记录操作日志）
 */
void PlayFieldController::init(PlayFieldModel* playFieldModel, PlayFieldView* playFieldView, StackModel* stackModel, UndoManager* undoManager)
{
    _playFieldModel = playFieldModel;
    _playFieldView = playFieldView;
    _stackModel = stackModel;
    _undoManager = undoManager;

    // 注册主牌区卡牌点击回调，将视图事件转发到控制器业务逻辑
    if (_playFieldView)
    {
        _playFieldView->setPlayFieldCardClickCallback([this](int cardId) {
            this->handleCardClick(cardId);
            });
    }
}

/**
 * @brief 设置堆牌区视图引用（用于更新底牌显示）
 * @param stackView 堆牌区视图
 */
void PlayFieldController::setStackView(StackView* stackView)
{
    _stackView = stackView;
}

/**
 * @brief 处理主牌区卡牌点击事件
 * @param cardId 被点击的卡牌唯一ID
 * @return bool 处理成功返回true，失败返回false（如卡牌未翻开、不匹配等）
 */
bool PlayFieldController::handleCardClick(int cardId)
{
    // 前置校验：模型/视图/管理器不能为空
    if (!_playFieldModel || !_playFieldView || !_stackModel || !_undoManager)
    {
        return false;
    }

    // 获取点击的卡牌和当前底牌
    CardModel* targetCard = _playFieldModel->getCardById(cardId);
    CardModel* currentBottomCard = _stackModel->getBottomCard();

    // 校验卡牌状态和匹配条件
    if (!targetCard ||                          // 卡牌不存在
        targetCard->getCardState() != CS_OPENED || // 卡牌未翻开
        !currentBottomCard ||                    // 底牌不存在
        !isCardMatchBottom(targetCard, currentBottomCard)) // 不满足匹配规则
    {
        return false;
    }

    // 执行卡牌替换底牌的核心逻辑
    return replaceBottomWithPlayFieldCard(cardId);
}

/**
 * @brief 判断主牌区卡牌是否与底牌匹配
 * @param card 主牌区待判断卡牌
 * @param bottomCard 堆牌区当前底牌
 * @return bool 数字差1返回true（忽略花色），否则返回false
 */
bool PlayFieldController::isCardMatchBottom(CardModel* card, CardModel* bottomCard) const
{
    // 空指针校验
    if (!card || !bottomCard)
    {
        return false;
    }

    // 获取卡牌数字（枚举转整型），判断差值是否为1
    int cardFaceValue = static_cast<int>(card->getCardFace());
    int bottomFaceValue = static_cast<int>(bottomCard->getCardFace());
    return (abs(cardFaceValue - bottomFaceValue) == 1);
}

/**
 * @brief 执行主牌区卡牌替换底牌的逻辑（含数据更新、动画播放、回退记录）
 * @param cardId 主牌区卡牌唯一ID
 * @return bool 执行成功返回true，失败返回false
 */
bool PlayFieldController::replaceBottomWithPlayFieldCard(int cardId)
{
    // 获取目标卡牌和当前底牌
    CardModel* targetCard = _playFieldModel->getCardById(cardId);
    CardModel* prevBottomCard = _stackModel->getBottomCard();
    if (!targetCard || !prevBottomCard || !_stackView)
    {
        return false;
    }

    // 1. 记录回退信息（供后续撤销操作使用）
    UndoRecord undoRecord;
    undoRecord.opType = UOT_PLAYFIELD_TO_BOTTOM;       // 操作类型：主牌区卡牌替换底牌
    undoRecord.targetCard = targetCard;                // 被移动的卡牌
    undoRecord.prevBottomCard = prevBottomCard;        // 替换前的底牌
    undoRecord.originalPos = targetCard->getPosition(); // 卡牌原始位置
    _undoManager->addUndoRecord(undoRecord);

    // 2. 获取底牌位置（卡牌移动的目标位置）
    cocos2d::Vec2 bottomCardPos = prevBottomCard->getPosition();

    // 3. 视图层：播放卡牌平移动画（从原始位置移动到底牌位置）
    _playFieldView->moveCardToPosition(cardId, bottomCardPos);

    // 4. 模型层：更新游戏数据状态
    _playFieldModel->removeCardById(cardId);          // 从主牌区移除目标卡牌
    _stackModel->addBottomCardToHistory(targetCard);  // 将目标卡牌加入底牌历史
    _stackModel->setBottomCard(targetCard);           // 设置目标卡牌为新底牌
    targetCard->setPosition(bottomCardPos);           // 更新卡牌的模型位置

    // 5. 视图层：更新界面显示
    _playFieldView->removeCardViewById(cardId);        // 从主牌区视图移除卡牌
    _stackView->updateBottomCardView(targetCard);      // 更新堆牌区底牌视图

    return true;
}