// 【修改】修正头文件路径（根据项目实际目录调整，若没有models目录则改为#include "UndoModel.h"）
#include "UndoModel.h"

UndoModel::UndoModel()
{
}

UndoModel::~UndoModel()
{
    // 释放回退记录中的所有CardModel
    for (auto& record : _undoRecords)
    {
        CC_SAFE_RELEASE(record.targetCard);
        CC_SAFE_RELEASE(record.prevBottomCard);
    }
    _undoRecords.clear();
}

UndoModel* UndoModel::create()
{
    UndoModel* model = new UndoModel();
    if (model)
    {
        model->autorelease();
        return model;
    }
    CC_SAFE_DELETE(model);
    return nullptr;
}

void UndoModel::addUndoRecord(const UndoRecord& record)
{
    // 持有记录中的卡牌，避免被提前释放
    UndoRecord newRecord = record;
    CC_SAFE_RETAIN(newRecord.targetCard);
    CC_SAFE_RETAIN(newRecord.prevBottomCard);
    _undoRecords.push_back(newRecord);
}

UndoRecord UndoModel::getLastUndoRecord()
{
    if (_undoRecords.empty())
    {
        return UndoRecord();
    }
    return _undoRecords.back();
}

void UndoModel::removeLastUndoRecord()
{
    if (!_undoRecords.empty())
    {
        // 释放要移除的记录中的卡牌
        auto& lastRecord = _undoRecords.back();
        CC_SAFE_RELEASE(lastRecord.targetCard);
        CC_SAFE_RELEASE(lastRecord.prevBottomCard);
        _undoRecords.pop_back();
    }
}