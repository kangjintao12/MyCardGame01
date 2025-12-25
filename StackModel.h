// StackModel.h（仅补充注释，代码无修改）
#ifndef __STACK_MODEL_H__
#define __STACK_MODEL_H__

#include "cocos2d.h"
#include "CardModel.h"
#include <vector>
#include <algorithm> // 补充find所需头文件

/**
 * @brief 堆牌区模型：存储备用牌堆和底牌数据
 */
class StackModel : public cocos2d::Ref
{
public:
    StackModel();
    ~StackModel();

    static StackModel* create();

    // 添加备用牌（补充retain CardModel）
    void addBackupCard(CardModel* card);
    // 设置底牌（补充retain/release CardModel）
    void setBottomCard(CardModel* card);
    // 获取备用牌列表（修改为const引用返回，避免复制）
    const std::vector<CardModel*>& getBackupCards() const { return _backupCards; }
    // 获取当前底牌
    CardModel* getBottomCard() const { return _bottomCard; }
    // 移除指定备用牌（补充release CardModel）
    void removeBackupCardById(int cardId);
    // 获取底牌历史（修改为const引用返回）
    const std::vector<CardModel*>& getBottomCardHistory() const { return _bottomCardHistory; }
    // 添加底牌到历史（补充retain CardModel）
    void addBottomCardToHistory(CardModel* card);
    // 从历史弹出最后一张底牌（补充release CardModel，外部调用需retain返回的card）
    CardModel* popBottomCardHistory();

    // 序列化
    std::string serialize() const;
    // 反序列化
    void deserialize(const std::string& data);

private:
    std::vector<CardModel*> _backupCards;      // 备用牌堆
    CardModel* _bottomCard;                    // 当前底牌
    std::vector<CardModel*> _bottomCardHistory;// 底牌历史（回退用）
};

#endif // __STACK_MODEL_H__