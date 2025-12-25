#ifndef __UNDO_MODEL_H__
#define __UNDO_MODEL_H__

#include "cocos2d.h"
#include "CardModel.h"
#include <vector>

// 回退操作类型
enum UndoOpType
{
    UOT_BACKUP_TO_BOTTOM, // 备用牌替换底牌
    UOT_PLAYFIELD_TO_BOTTOM // 主牌区卡牌替换底牌
};

// 回退记录结构体
struct UndoRecord
{
    UndoOpType opType;          // 操作类型
    CardModel* targetCard;      // 目标卡牌（被移动的卡牌）
    CardModel* prevBottomCard;  // 操作前的底牌
    cocos2d::Vec2 originalPos;  // 目标卡牌原位置

    UndoRecord() : opType((UndoOpType)-1), targetCard(nullptr), prevBottomCard(nullptr), originalPos(0, 0) {}
};

/**
 * @brief 回退模型：存储所有可回退的操作记录
 */
class UndoModel : public cocos2d::Ref
{
public:
    UndoModel();
    ~UndoModel();

    static UndoModel* create();

    // 添加回退记录
    void addUndoRecord(const UndoRecord& record);
    // 获取最后一条回退记录
    UndoRecord getLastUndoRecord();
    // 移除最后一条回退记录
    void removeLastUndoRecord();
    // 判断是否有可回退的记录
    bool hasUndoRecord() const { return !_undoRecords.empty(); }
    // 清空所有回退记录
    void clearUndoRecords() { _undoRecords.clear(); }

private:
    std::vector<UndoRecord> _undoRecords; // 回退记录列表
};

#endif // __UNDO_MODEL_H__