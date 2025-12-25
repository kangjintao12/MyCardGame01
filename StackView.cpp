#include "StackView.h"

StackView::StackView()
    : _stackModel(nullptr)
    , _bottomCardView(nullptr)
{
}

StackView::~StackView()
{
    _backupCardViews.clear();
    _bottomCardView = nullptr;
}

StackView* StackView::create(const StackModel* stackModel)
{
    StackView* view = new StackView();
    if (view && view->init(stackModel))
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool StackView::init(const StackModel* stackModel)
{
    if (!Layer::init())
    {
        return false;
    }

    _stackModel = stackModel;
    if (!_stackModel)
    {
        return false;
    }

    // 初始化背景
    initBackground();
    // 创建备用牌视图
    createBackupCardViews();
    // 创建底牌视图
    createBottomCardView();
    // 创建回退按钮
    createUndoButton();

    return true;
}

void StackView::initBackground()
{
    // 堆牌区背景：淡黄色，尺寸1080*580
    auto bg = cocos2d::LayerColor::create(cocos2d::Color4B(255, 255, 204, 255), 1080, 580);
    bg->setPosition(0, 0);
    this->addChild(bg);
}

void StackView::createBackupCardViews()
{
    const auto& backupCards = _stackModel->getBackupCards();
    for (const auto& card : backupCards)
    {
        CardView* cardView = CardView::create(card);
        if (cardView)
        {
            // 设置备用牌点击回调
            cardView->setCardClickCallback([this](int cardId) {
                if (_backupCardClickCallback)
                {
                    _backupCardClickCallback(cardId);
                }
                });
            _backupCardViews.push_back(cardView);
            this->addChild(cardView);
        }
    }
}

void StackView::createBottomCardView()
{
    CardModel* bottomCard = _stackModel->getBottomCard();
    if (bottomCard)
    {
        _bottomCardView = CardView::create(bottomCard);
        if (_bottomCardView)
        {
            this->addChild(_bottomCardView);
        }
    }
}

void StackView::createUndoButton()
{
    // 回退按钮：尺寸79*78，位于底牌右侧
    auto undoBtn = cocos2d::Sprite::create("fanhui.png");
    if (undoBtn)
    {
        undoBtn->setScale(158 / undoBtn->getContentSize().width, 156 / undoBtn->getContentSize().height);
        // 底牌位置：(400,200)，按钮位于右侧
        undoBtn->setPosition(800, 200);
        this->addChild(undoBtn);

        // 添加触摸事件
        auto listener = cocos2d::EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [undoBtn](cocos2d::Touch* touch, cocos2d::Event* event) {
            cocos2d::Vec2 touchPos = undoBtn->convertTouchToNodeSpace(touch);
            cocos2d::Rect rect = cocos2d::Rect(0, 0, undoBtn->getContentSize().width, undoBtn->getContentSize().height);
            return rect.containsPoint(touchPos);
            };
        listener->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
            if (_undoBtnClickCallback)
            {
                _undoBtnClickCallback();
            }
            };
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, undoBtn);
    }
}

CardView* StackView::getCardViewById(int cardId) const
{
    // 先查备用牌
    for (auto cardView : _backupCardViews)
    {
        if (cardView && cardView->getCardId() == cardId)
        {
            return cardView;
        }
    }
    // 再查底牌
    if (_bottomCardView && _bottomCardView->getCardId() == cardId)
    {
        return _bottomCardView;
    }
    return nullptr;
}

void StackView::moveCardToPosition(int cardId, const cocos2d::Vec2& targetPos, float duration)
{
    CardView* cardView = getCardViewById(cardId);
    if (cardView)
    {
        cardView->moveToPosition(targetPos, duration);
    }
}

void StackView::updateBottomCardView(CardModel* newBottomCard)
{
    if (_bottomCardView)
    {
        _bottomCardView->removeFromParent();
        _bottomCardView = nullptr;
    }

    if (newBottomCard)
    {
        _bottomCardView = CardView::create(newBottomCard);
        if (_bottomCardView)
        {
            this->addChild(_bottomCardView);
        }
    }
}