#include "PlayFieldModel.h"

PlayFieldModel::PlayFieldModel()
{
}

PlayFieldModel::~PlayFieldModel()
{
    // 【新增】释放所有持有的CardModel，避免内存泄漏+野指针
    clearCards();
}

PlayFieldModel* PlayFieldModel::create()
{
    PlayFieldModel* model = new PlayFieldModel();
    if (model)
    {
        model->autorelease();
        return model;
    }
    CC_SAFE_DELETE(model);
    return nullptr;
}

void PlayFieldModel::addCard(CardModel* card)
{
    if (card && std::find(_cards.begin(), _cards.end(), card) == _cards.end())
    {
        CC_SAFE_RETAIN(card); // 【新增】持有卡牌，避免被自动释放池回收
        _cards.push_back(card);
    }
}

CardModel* PlayFieldModel::getCardById(int cardId) const
{
    for (auto card : _cards)
    {
        if (card && card->getCardId() == cardId)
        {
            return card;
        }
    }
    return nullptr;
}

void PlayFieldModel::removeCardById(int cardId)
{
    for (auto it = _cards.begin(); it != _cards.end(); ++it)
    {
        if ((*it)->getCardId() == cardId)
        {
            CC_SAFE_RELEASE(*it); // 【新增】释放要移除的卡牌
            _cards.erase(it);
            break;
        }
    }
}

// 【新增】清空所有卡牌并释放内存
void PlayFieldModel::clearCards()
{
    for (auto card : _cards)
    {
        CC_SAFE_RELEASE(card);
    }
    _cards.clear();
}

std::string PlayFieldModel::serialize() const
{
    std::stringstream ss;
    for (size_t i = 0; i < _cards.size(); ++i)
    {
        if (i > 0) ss << ";";
        ss << _cards[i]->serialize();
    }
    return ss.str();
}

void PlayFieldModel::deserialize(const std::string& data)
{
    clearCards(); // 【修改】先清空并释放原有卡牌，避免内存泄漏
    std::stringstream ss(data);
    std::string token;
    while (std::getline(ss, token, ';'))
    {
        CardModel* card = CardModel::create(-1, CST_NONE, CFT_NONE, cocos2d::Vec2(0, 0));
        card->deserialize(token);
        CC_SAFE_RETAIN(card); // 【新增】持有反序列化的卡牌
        _cards.push_back(card);
    }
}