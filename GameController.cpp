#include "GameController.h"
// 补充包含：LevelConfigLoader所在的头文件（对应之前的LevelConfig.h）
#include "configs/LevelConfig.h"
// 补充包含：GameModelFromLevelGenerator对应的头文件（需根据实际文件名调整）
#include "services/GameModelFromLevelGenerator.h"


GameController::GameController()
    : _gameModel(nullptr)
    , _gameView(nullptr)
    , _playFieldController(new PlayFieldController())
    , _stackController(new StackController())
    , _undoManager(new UndoManager())
{
}

GameController::~GameController()
{
        // release cocos2d::Ref owned objects
        CC_SAFE_RELEASE(_gameModel);
        CC_SAFE_RELEASE(_gameView);
    delete _playFieldController;
    _playFieldController = nullptr;
    delete _stackController;
    _stackController = nullptr;
    delete _undoManager;
    _undoManager = nullptr;
}

bool GameController::startGame(int levelId, cocos2d::Node* parentNode)
{
    if (!parentNode)
    {
        return false;
    }

    // 1. 加载关卡配置
    LevelConfig levelConfig = LevelConfigLoader::loadLevelConfig(levelId);

    // 2. 生成游戏模型
    _gameModel = GameModelFromLevelGenerator::generateGameModel(levelConfig, levelId);
    if (!_gameModel)
    {
        return false;
    }
    // 保持对 GameModel 的引用，保证其在游戏运行期间有效
    CC_SAFE_RETAIN(_gameModel);

    // 3. 创建游戏视图
    _gameView = GameView::create(_gameModel);
    if (!_gameView)
    {
        return false;
    }
    parentNode->addChild(_gameView);

    // 4. 初始化子控制器
    if (!initSubControllers())
    {
        return false;
    }

    return true;
}

bool GameController::initSubControllers()
{
    if (!_gameModel || !_gameView)
    {
        return false;
    }

    // 初始化回退管理器
    _undoManager->init(_gameModel, _gameView);

    // 初始化主牌区控制器
    PlayFieldModel* playFieldModel = _gameModel->getPlayFieldModel();
    PlayFieldView* playFieldView = _gameView->getPlayFieldView();
    StackModel* stackModel = _gameModel->getStackModel();
    if (playFieldModel && playFieldView && stackModel)
    {
        _playFieldController->init(playFieldModel, playFieldView, stackModel, _undoManager);
        // 设置堆牌区视图引用
        _playFieldController->setStackView(_gameView->getStackView());
    }
    else
    {
        return false;
    }

    // 初始化堆牌区控制器
    StackView* stackView = _gameView->getStackView();
    if (stackModel && stackView)
    {
        _stackController->init(stackModel, stackView, _undoManager);
    }
    else
    {
        return false;
    }

    return true;
}