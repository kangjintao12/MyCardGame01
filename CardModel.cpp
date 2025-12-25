#include "CardModel.h"
#include <sstream>

CardModel::CardModel()
    : _cardId(-1)
    , _cardSuit(CST_NONE)
    , _cardFace(CFT_NONE)
    , _position(0, 0)
    , _cardState(CS_COVERED)
{
}

CardModel::~CardModel()
{
}

CardModel* CardModel::create(int cardId, CardSuitType suit, CardFaceType face, cocos2d::Vec2 position, CardState state)
{
    CardModel* model = new CardModel();
    if (model)
    {
        model->_cardId = cardId;
        model->_cardSuit = suit;
        model->_cardFace = face;
        model->_position = position;
        model->_cardState = state;
        model->autorelease();
        return model;
    }
    CC_SAFE_DELETE(model);
    return nullptr;
}

std::string CardModel::serialize() const
{
    std::stringstream ss;
    ss << _cardId << ","
        << (int)_cardSuit << ","
        << (int)_cardFace << ","
        << _position.x << ","
        << _position.y << ","
        << (int)_cardState;
    return ss.str();
}

void CardModel::deserialize(const std::string& data)
{
    std::stringstream ss(data);
    std::string token;
    int index = 0;
    while (std::getline(ss, token, ','))
    {
        switch (index)
        {
        case 0: _cardId = std::stoi(token); break;
        case 1: _cardSuit = (CardSuitType)std::stoi(token); break;
        case 2: _cardFace = (CardFaceType)std::stoi(token); break;
        case 3: _position.x = std::stof(token); break;
        case 4: _position.y = std::stof(token); break;
        case 5: _cardState = (CardState)std::stoi(token); break;
        default: break;
        }
        index++;
    }
}