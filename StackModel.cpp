// StackModel.cpp（仅补充注释，代码无修改）
#include "StackModel.h"
#include <sstream> // 补充序列化所需头文件

StackModel::StackModel()
    : _bottomCard(nullptr)
{
}

StackModel::~StackModel()
{
    // 释放备用牌堆中的所有CardModel
    for (auto card : _backupCards)
    {
        CC_SAFE_RELEASE(card);
    }
    _backupCards.clear();

    // 释放当前底牌
    CC_SAFE_RELEASE(_bottomCard);
    _bottomCard = nullptr;

    // 释放底牌历史中的所有CardModel
    for (auto card : _bottomCardHistory)
    {
        CC_SAFE_RELEASE(card);
    }
    _bottomCardHistory.clear();
}

StackModel* StackModel::create()
{
    StackModel* model = new StackModel();
    if (model)
    {
        model->autorelease();
        return model;
    }
    CC_SAFE_DELETE(model);
    return nullptr;
}

void StackModel::addBackupCard(CardModel* card)
{
    if (card && std::find(_backupCards.begin(), _backupCards.end(), card) == _backupCards.end())
    {
        CC_SAFE_RETAIN(card); // 持有卡牌，避免被提前释放
        _backupCards.push_back(card);
    }
}

void StackModel::setBottomCard(CardModel* card)
{
    if (_bottomCard != card)
    {
        CC_SAFE_RELEASE(_bottomCard); // 释放旧底牌
        _bottomCard = card;
        CC_SAFE_RETAIN(_bottomCard);  // 持有新底牌
    }
}

void StackModel::removeBackupCardById(int cardId)
{
    for (auto it = _backupCards.begin(); it != _backupCards.end(); ++it)
    {
        if ((*it)->getCardId() == cardId)
        {
            CC_SAFE_RELEASE(*it); // 释放要移除的卡牌
            _backupCards.erase(it);
            break;
        }
    }
}

void StackModel::addBottomCardToHistory(CardModel* card)
{
    if (card)
    {
        CC_SAFE_RETAIN(card); // 持有历史卡牌
        _bottomCardHistory.push_back(card);
    }
}

CardModel* StackModel::popBottomCardHistory()
{
    if (_bottomCardHistory.empty())
    {
        return nullptr;
    }
    CardModel* card = _bottomCardHistory.back();
    _bottomCardHistory.pop_back();
    // 返回弹出的卡牌并对其retain，调用者使用后需release()
    if (card) CC_SAFE_RETAIN(card);
    return card;
}

std::string StackModel::serialize() const
{
    std::stringstream ss;
    // 序列化备用牌
    for (size_t i = 0; i < _backupCards.size(); ++i)
    {
        if (i > 0) ss << ";";
        ss << _backupCards[i]->serialize();
    }
    ss << "|";
    // 序列化底牌
    if (_bottomCard)
    {
        ss << _bottomCard->serialize();
    }
    ss << "|";
    // 序列化底牌历史
    for (size_t i = 0; i < _bottomCardHistory.size(); ++i)
    {
        if (i > 0) ss << ";";
        ss << _bottomCardHistory[i]->serialize();
    }
    return ss.str();
}

void StackModel::deserialize(const std::string& data)
{
    // 先释放原有数据
    for (auto card : _backupCards) CC_SAFE_RELEASE(card);
    _backupCards.clear();
    CC_SAFE_RELEASE(_bottomCard);
    _bottomCard = nullptr;
    for (auto card : _bottomCardHistory) CC_SAFE_RELEASE(card);
    _bottomCardHistory.clear();

    std::stringstream ss(data);
    std::string part;
    int partIndex = 0;

    while (std::getline(ss, part, '|'))
    {
        if (partIndex == 0)
        {
            // 反序列化备用牌
            std::stringstream partSs(part);
            std::string token;
            while (std::getline(partSs, token, ';'))
            {
                CardModel* card = CardModel::create(-1, CST_NONE, CFT_NONE, cocos2d::Vec2(0, 0));
                card->deserialize(token);
                CC_SAFE_RETAIN(card); // 持有反序列化的卡牌
                _backupCards.push_back(card);
            }
        }
        else if (partIndex == 1 && !part.empty())
        {
            // 反序列化底牌
            _bottomCard = CardModel::create(-1, CST_NONE, CFT_NONE, cocos2d::Vec2(0, 0));
            _bottomCard->deserialize(part);
            CC_SAFE_RETAIN(_bottomCard); // 持有底牌
        }
        else if (partIndex == 2 && !part.empty())
        {
            // 反序列化底牌历史
            std::stringstream partSs(part);
            std::string token;
            while (std::getline(partSs, token, ';'))
            {
                CardModel* card = CardModel::create(-1, CST_NONE, CFT_NONE, cocos2d::Vec2(0, 0));
                card->deserialize(token);
                CC_SAFE_RETAIN(card); // 持有历史卡牌
                _bottomCardHistory.push_back(card);
            }
        }
        partIndex++;
    }
}