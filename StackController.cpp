#include "StackController.h"
#include "models/UndoModel.h"

StackController::StackController()
    : _stackModel(nullptr)
    , _stackView(nullptr)
    , _undoManager(nullptr)
{
}

StackController::~StackController()
{
    _stackModel = nullptr;
    _stackView = nullptr;
    _undoManager = nullptr;
}

void StackController::init(StackModel* stackModel, StackView* stackView, UndoManager* undoManager)
{
    _stackModel = stackModel;
    _stackView = stackView;
    _undoManager = undoManager;

    // 注册备用牌点击回调
    if (_stackView)
    {
        _stackView->setBackupCardClickCallback([this](int cardId) {
            this->handleBackupCardClick(cardId);
            });
    }

    // 注册回退按钮点击回调
    if (_stackView)
    {
        _stackView->setUndoBtnClickCallback([this]() {
            if (_undoManager)
            {
                _undoManager->undo();
            }
            });
    }
}

bool StackController::handleBackupCardClick(int cardId)
{
    if (!_stackModel || !_stackView || !_undoManager)
    {
        return false;
    }

    CardModel* backupCard = nullptr;
    // 查找对应的备用牌
    for (auto card : _stackModel->getBackupCards())
    {
        if (card && card->getCardId() == cardId)
        {
            backupCard = card;
            break;
        }
    }

    if (!backupCard || backupCard->getCardState() != CS_COVERED)
    {
        return false;
    }

    // 执行备用牌替换底牌逻辑
    return replaceBottomWithBackupCard(cardId);
}

bool StackController::replaceBottomWithBackupCard(int cardId)
{
    CardModel* targetCard = nullptr;
    for (auto card : _stackModel->getBackupCards())
    {
        if (card && card->getCardId() == cardId)
        {
            targetCard = card;
            break;
        }
    }
    CardModel* prevBottomCard = _stackModel->getBottomCard();
    if (!targetCard || !prevBottomCard)
    {
        return false;
    }

    // 1. 记录回退信息
    UndoRecord undoRecord;
    undoRecord.opType = UOT_BACKUP_TO_BOTTOM;
    undoRecord.targetCard = targetCard;
    undoRecord.prevBottomCard = prevBottomCard;
    undoRecord.originalPos = targetCard->getPosition();
    _undoManager->addUndoRecord(undoRecord);

    // 2. 获取底牌位置（目标位置）
    cocos2d::Vec2 bottomPos = prevBottomCard->getPosition();

    // 3. 视图层：播放卡牌移动动画，同时翻开备用牌
    _stackView->moveCardToPosition(cardId, bottomPos);
    targetCard->setCardState(CS_OPENED);
    auto cardView = _stackView->getCardViewById(cardId);
    if (cardView)
    {
        cardView->updateCardState(CS_OPENED);
    }

    // 4. 模型层：更新数据
    // 移除备用牌堆中的目标卡牌
    _stackModel->removeBackupCardById(cardId);
    // 记录底牌历史
    _stackModel->addBottomCardToHistory(targetCard);
    // 设置新底牌
    _stackModel->setBottomCard(targetCard);
    // 更新卡牌位置
    targetCard->setPosition(bottomPos);

    // 5. 视图层：更新底牌视图
    _stackView->updateBottomCardView(targetCard);

    return true;
}