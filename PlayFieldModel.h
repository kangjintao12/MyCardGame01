#ifndef __PLAY_FIELD_MODEL_H__
#define __PLAY_FIELD_MODEL_H__

#include "cocos2d.h"
#include "CardModel.h"
#include <vector>
#include <algorithm>  // 【新增】补充find所需头文件
#include <sstream>    // 【新增】补充序列化所需头文件

/**
 * @brief 主牌区模型：存储主牌区所有卡牌数据
 */
class PlayFieldModel : public cocos2d::Ref
{
public:
    PlayFieldModel();
    ~PlayFieldModel();

    static PlayFieldModel* create();

    // 添加卡牌（补充retain CardModel）
    void addCard(CardModel* card);
    // 获取卡牌列表（【修改】改为const引用返回，避免复制无效内存）
    const std::vector<CardModel*>& getCards() const { return _cards; }
    // 根据ID获取卡牌
    CardModel* getCardById(int cardId) const;
    // 移除卡牌（补充release CardModel）
    void removeCardById(int cardId);
    // 清空所有卡牌（补充释放逻辑）
    void clearCards();

    // 序列化
    std::string serialize() const;
    // 反序列化
    void deserialize(const std::string& data);

private:
    std::vector<CardModel*> _cards; // 主牌区卡牌列表
};

#endif // __PLAY_FIELD_MODEL_H__