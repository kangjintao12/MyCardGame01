#include "UndoManager.h"
// 【新增】补充必要头文件，确保能调用Ref的getReferenceCount()
#include "models/UndoModel.h"
#include "models/StackModel.h"
#include "models/PlayFieldModel.h"

UndoManager::UndoManager()
    : _gameModel(nullptr)
    , _gameView(nullptr)
{
}

UndoManager::~UndoManager()
{
    _gameModel = nullptr;
    _gameView = nullptr;
}

void UndoManager::init(GameModel* gameModel, GameView* gameView)
{
    _gameModel = gameModel;
    _gameView = gameView;
}

void UndoManager::addUndoRecord(const UndoRecord& record)
{
    // 【修改】三层防护：GameModel非空 + UndoModel非空 + UndoModel未释放（引用计数>0）
    if (!_gameModel) return;
    UndoModel* undoModel = _gameModel->getUndoModel();
    if (!undoModel || undoModel->getReferenceCount() == 0) return;

    undoModel->addUndoRecord(record);
}

bool UndoManager::undo()
{
    if (!_gameModel || !_gameView || !hasUndoRecord())
    {
        return false;
    }

    UndoModel* undoModel = _gameModel->getUndoModel();
    // 【新增】二次防护：确保UndoModel未被释放
    if (!undoModel || undoModel->getReferenceCount() == 0)
    {
        return false;
    }

    UndoRecord lastRecord = undoModel->getLastUndoRecord();

    // 根据操作类型处理回退
    bool ret = false;
    switch (lastRecord.opType)
    {
    case UOT_BACKUP_TO_BOTTOM:
        ret = handleBackupToBottomUndo(lastRecord);
        break;
    case UOT_PLAYFIELD_TO_BOTTOM:
        ret = handlePlayFieldToBottomUndo(lastRecord);
        break;
    default:
        ret = false;
        break;
    }

    if (ret)
    {
        undoModel->removeLastUndoRecord();
    }

    return ret;
}

bool UndoManager::hasUndoRecord() const
{
    // 【修改】三层防护：GameModel非空 + UndoModel非空 + UndoModel未释放
    if (!_gameModel) return false;

    UndoModel* undoModel = _gameModel->getUndoModel();
    if (!undoModel || undoModel->getReferenceCount() == 0)
    {
        return false;
    }

    return undoModel->hasUndoRecord();
}

bool UndoManager::handleBackupToBottomUndo(const UndoRecord& record)
{
    if (!record.targetCard || !record.prevBottomCard)
    {
        return false;
    }

    StackModel* stackModel = _gameModel->getStackModel();
    // 【新增】防护：确保StackModel有效（非空+未释放）
    if (!stackModel || stackModel->getReferenceCount() == 0)
    {
        return false;
    }

    PlayFieldView* playFieldView = _gameView->getPlayFieldView();
    StackView* stackView = _gameView->getStackView();

    // 1. 移动当前底牌（原备用牌）回到原位置
    stackView->moveCardToPosition(record.targetCard->getCardId(), record.originalPos);
    // 2. 恢复之前的底牌
    stackModel->setBottomCard(record.prevBottomCard);
    stackView->updateBottomCardView(record.prevBottomCard);
    // 3. 将当前底牌重新加入备用牌堆
    stackModel->addBackupCard(record.targetCard);
    // 4. 从底牌历史弹出最后一条
    stackModel->popBottomCardHistory();

    return true;
}

bool UndoManager::handlePlayFieldToBottomUndo(const UndoRecord& record)
{
    if (!record.targetCard || !record.prevBottomCard)
    {
        return false;
    }

    GameModel* gameModel = _gameModel;
    GameView* gameView = _gameView;
    PlayFieldModel* playFieldModel = gameModel->getPlayFieldModel();
    StackModel* stackModel = gameModel->getStackModel();

    // 【新增】防护：确保子模型有效（非空+未释放）
    if (!playFieldModel || playFieldModel->getReferenceCount() == 0 ||
        !stackModel || stackModel->getReferenceCount() == 0)
    {
        return false;
    }

    PlayFieldView* playFieldView = gameView->getPlayFieldView();
    StackView* stackView = gameView->getStackView();

    // 1. 移动当前底牌（原主牌区卡牌）回到原位置
    playFieldView->moveCardToPosition(record.targetCard->getCardId(), record.originalPos);
    // 2. 恢复之前的底牌
    stackModel->setBottomCard(record.prevBottomCard);
    stackView->updateBottomCardView(record.prevBottomCard);
    // 3. 将当前底牌重新加入主牌区
    playFieldModel->addCard(record.targetCard);
    // 4. 从底牌历史弹出最后一条
    stackModel->popBottomCardHistory();
    // 1. 先把目标卡牌重新加入模型与视图（因为原操作已从主牌区移除视图）
    playFieldModel->addCard(record.targetCard);
    playFieldView->addCardViewForModel(record.targetCard);

    // 2. 将卡牌从底牌位置移动回原来的主牌区位置
    playFieldView->moveCardToPosition(record.targetCard->getCardId(), record.originalPos);

    // 3. 恢复之前的底牌并更新底牌视图
    stackModel->setBottomCard(record.prevBottomCard);
    stackView->updateBottomCardView(record.prevBottomCard);

    // 4. 从底牌历史弹出最后一条（pop 返回已 retain 的指针，需释放）
    {
        CardModel* popped = stackModel->popBottomCardHistory();
        if (popped) CC_SAFE_RELEASE(popped);
    }

    return true;
}