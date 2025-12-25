#include "PlayFieldView.h"
#include "CardView.h" // 引入CardView头文件

PlayFieldView::PlayFieldView()
    : _playFieldModel(nullptr)
{
}

PlayFieldView::~PlayFieldView()
{
    _cardViews.clear();
    _cardClickCallback = nullptr; // 清空回调，避免野指针
}

PlayFieldView* PlayFieldView::create(const PlayFieldModel* playFieldModel)
{
    PlayFieldView* view = new PlayFieldView();
    if (view && view->init(playFieldModel))
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool PlayFieldView::init(const PlayFieldModel* playFieldModel)
{
    if (!Layer::init())
    {
        return false;
    }

    _playFieldModel = playFieldModel;
    if (!_playFieldModel)
    {
        return false;
    }

    // 初始化背景
    initBackground();
    // 创建卡牌视图
    createCardViews();

    return true;
}

void PlayFieldView::initBackground()
{
    // 主牌区背景：淡红色，尺寸1080*1500
    auto bg = cocos2d::LayerColor::create(cocos2d::Color4B(255, 204, 204, 255), 1080, 1500);
    bg->setPosition(0, 580); // 堆牌区高度580，主牌区在上方
    this->addChild(bg);
}

void PlayFieldView::createCardViews()
{
    const auto& cards = _playFieldModel->getCards();
    for (const auto& card : cards)
    {
        CardView* cardView = CardView::create(card);
        if (cardView)
        {
            // 设置卡牌点击回调
            cardView->setCardClickCallback([this](int cardId) {
                if (_cardClickCallback)
                {
                    _cardClickCallback(cardId);
                }
                });
            _cardViews.push_back(cardView);
            this->addChild(cardView);
        }
    }
}

void PlayFieldView::addCardViewForModel(CardModel* card)
{
    if (!card) return;
    // 如果已经存在对应的 CardView 则不重复创建
    CardView* exists = getCardViewById(card->getCardId());
    if (exists) return;

    CardView* cardView = CardView::create(card);
    if (cardView)
    {
        cardView->setCardClickCallback([this](int cardId) {
            if (_cardClickCallback)
            {
                _cardClickCallback(cardId);
            }
            });
        _cardViews.push_back(cardView);
        this->addChild(cardView);
    }
}

CardView* PlayFieldView::getCardViewById(int cardId) const
{
    for (auto cardView : _cardViews)
    {
        if (cardView && cardView->getCardId() == cardId)
        {
            return cardView;
        }
    }
    return nullptr;
}

void PlayFieldView::removeCardViewById(int cardId)
{
    for (auto it = _cardViews.begin(); it != _cardViews.end(); ++it)
    {
        if ((*it)->getCardId() == cardId)
        {
            (*it)->removeFromParent();
            _cardViews.erase(it);
            break;
        }
    }
}

// 【修改】移除实现中的默认参数（默认参数应在头文件声明时指定）
void PlayFieldView::moveCardToPosition(int cardId, const cocos2d::Vec2& targetPos, float duration)
{
    CardView* cardView = getCardViewById(cardId);
    if (cardView)
    {
        cardView->moveToPosition(targetPos, duration);
    }
}

void PlayFieldView::setPlayFieldCardClickCallback(const std::function<void(int)>& callback)
{
    _cardClickCallback = callback;
}