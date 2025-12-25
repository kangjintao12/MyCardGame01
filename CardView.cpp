#include "CardView.h"
#include <string>

CardView::CardView()
    : _cardId(-1)
    , _cardSuit(CST_NONE)
    , _cardFace(CFT_NONE)
    , _cardState(CS_COVERED)
{
}

CardView::~CardView()
{
    _cardClickCallback = nullptr; // 清空回调，避免野指针
}

CardView* CardView::create(const CardModel* cardModel)
{
    CardView* view = new CardView();
    if (view && view->init(cardModel))
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool CardView::init(const CardModel* cardModel)
{
    if (!cocos2d::Sprite::init())
    {
        return false;
    }

    if (!cardModel)
    {
        return false;
    }

    _cardId = cardModel->getCardId();
    _cardSuit = cardModel->getCardSuit();
    _cardFace = cardModel->getCardFace();
    _cardState = cardModel->getCardState();

    // 设置卡牌位置
    this->setPosition(cardModel->getPosition());

    // 绘制卡牌
    if (_cardState == CS_OPENED)
    {
        drawCardFront();
    }
    else
    {
        drawCardBack();
    }

    // 添加触摸事件
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(CardView::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(CardView::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void CardView::drawCardFront()
{
    // 卡牌正面背景
    this->setTexture("res/card_general.png");

    // 判断颜色（红桃/方块为红色，梅花/黑桃为黑色）
    bool isRed = (_cardSuit == CST_HEARTS || _cardSuit == CST_DIAMONDS);

    // 1. 左上方小数字
    std::string smallNumPath = getNumberIconPath(false, isRed, _cardFace);
    auto smallNumSprite = cocos2d::Sprite::create(smallNumPath);
    if (smallNumSprite)
    {
        smallNumSprite->setPosition(50, this->getContentSize().height - 50);
        smallNumSprite->setScale(0.8f);
        this->addChild(smallNumSprite);
    }

    // 2. 右上方花色
    std::string suitPath = getSuitIconPath(_cardSuit);
    auto suitSprite = cocos2d::Sprite::create(suitPath);
    if (suitSprite)
    {
        suitSprite->setPosition(this->getContentSize().width - 50, this->getContentSize().height - 50);
        suitSprite->setScale(0.8f);
        this->addChild(suitSprite);
    }

    // 3. 中间下方大数字
    std::string bigNumPath = getNumberIconPath(true, isRed, _cardFace);
    auto bigNumSprite = cocos2d::Sprite::create(bigNumPath);
    if (bigNumSprite)
    {
        bigNumSprite->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2 - 50);
        bigNumSprite->setScale(1.2f);
        this->addChild(bigNumSprite);
    }
}

void CardView::drawCardBack()
{
    // 卡牌背面背景
    this->setTexture("res/card_general_back.png");
    // 设置淡蓝色
    this->setColor(cocos2d::Color3B(173, 216, 230)); // 淡蓝色
    this->setOpacity(255);
}

std::string CardView::getNumberIconPath(bool isBig, bool isRed, CardFaceType face)
{
    std::string prefix = isBig ? "big_" : "small_";
    std::string color = isRed ? "red_" : "black_";
    std::string faceStr;

    switch (face)
    {
    case CFT_ACE: faceStr = "A"; break;
    case CFT_TWO: faceStr = "2"; break;
    case CFT_THREE: faceStr = "3"; break;
    case CFT_FOUR: faceStr = "4"; break;
    case CFT_FIVE: faceStr = "5"; break;
    case CFT_SIX: faceStr = "6"; break;
    case CFT_SEVEN: faceStr = "7"; break;
    case CFT_EIGHT: faceStr = "8"; break;
    case CFT_NINE: faceStr = "9"; break;
    case CFT_TEN: faceStr = "10"; break;
    case CFT_JACK: faceStr = "J"; break;
    case CFT_QUEEN: faceStr = "Q"; break;
    case CFT_KING: faceStr = "K"; break;
    default: faceStr = "A"; break;
    }

    return "res/number/" + prefix + color + faceStr + ".png";
}

std::string CardView::getSuitIconPath(CardSuitType suit)
{
    switch (suit)
    {
    case CST_CLUBS: return "res/suits/club.png";
    case CST_DIAMONDS: return "res/suits/diamond.png";
    case CST_HEARTS: return "res/suits/heart.png";
    case CST_SPADES: return "res/suits/spade.png";
    default: return "res/suits/club.png";
    }
}

bool CardView::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    // 判断触摸是否在卡牌范围内
    cocos2d::Vec2 touchPos = this->convertTouchToNodeSpace(touch);
    cocos2d::Rect rect = cocos2d::Rect(0, 0, this->getContentSize().width, this->getContentSize().height);
    return rect.containsPoint(touchPos);
}

void CardView::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    // 触发点击回调
    if (_cardClickCallback)
    {
        _cardClickCallback(_cardId);
    }
}

void CardView::moveToPosition(const cocos2d::Vec2& targetPos, float duration)
{
    // 平移动画
    auto moveAction = cocos2d::MoveTo::create(duration, targetPos);
    this->runAction(moveAction);
}

void CardView::updateCardState(CardState state)
{
    if (_cardState == state)
    {
        return;
    }

    _cardState = state;
    this->removeAllChildren(); // 清除原有内容

    if (state == CS_OPENED)
    {
        drawCardFront();
    }
    else
    {
        drawCardBack();
    }
}

void CardView::setCardClickCallback(const std::function<void(int)>& callback)
{
    _cardClickCallback = callback;
}

int CardView::getCardId() const
{
    return _cardId;
}