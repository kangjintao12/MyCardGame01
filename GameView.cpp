#include "GameView.h"

GameView::GameView()
    : _gameModel(nullptr)
    , _playFieldView(nullptr)
    , _stackView(nullptr)
{
}

GameView::~GameView()
{
    _playFieldView = nullptr;
    _stackView = nullptr;
}

GameView* GameView::create(const GameModel* gameModel)
{
    GameView* view = new GameView();
    if (view && view->init(gameModel))
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool GameView::init(const GameModel* gameModel)
{
    if (!Layer::init())
    {
        return false;
    }

    _gameModel = gameModel;
    if (!_gameModel)
    {
        return false;
    }

    // 初始化视图布局
    initViewLayout();

    return true;
}

void GameView::initViewLayout()
{
    // 创建主牌区视图
    _playFieldView = PlayFieldView::create(_gameModel->getPlayFieldModel());
    if (_playFieldView)
    {
        this->addChild(_playFieldView);
    }

    // 创建堆牌区视图
    _stackView = StackView::create(_gameModel->getStackModel());
    if (_stackView)
    {
        this->addChild(_stackView);
    }
}