#include "GameModel.h"
#include "cocos2d.h"

// 必须实现 GameModel::create() 静态函数
GameModel* GameModel::create()
{
    GameModel* model = new GameModel();
    if (model)
    {
        model->autorelease(); // Cocos Ref类必须调用autorelease
        return model;
    }
    CC_SAFE_DELETE(model);
    return nullptr;
}

GameModel::GameModel()
    : _playFieldModel(nullptr)
    , _stackModel(nullptr)
    , _undoModel(nullptr)
    , _levelId(-1)
{
}

GameModel::~GameModel()
{
    // 析构时释放子模型
    CC_SAFE_RELEASE(_playFieldModel);
    CC_SAFE_RELEASE(_stackModel);
    CC_SAFE_RELEASE(_undoModel);
}

// 修复：设置子模型时retain，避免被自动释放池回收
void GameModel::setPlayFieldModel(PlayFieldModel* model)
{
    if (_playFieldModel != model)
    {
        CC_SAFE_RELEASE(_playFieldModel); // 释放旧对象
        _playFieldModel = model;
        CC_SAFE_RETAIN(_playFieldModel); // 持有新对象
    }
}

void GameModel::setStackModel(StackModel* model)
{
    if (_stackModel != model)
    {
        CC_SAFE_RELEASE(_stackModel);
        _stackModel = model;
        CC_SAFE_RETAIN(_stackModel);
    }
}

void GameModel::setUndoModel(UndoModel* model)
{
    if (_undoModel != model)
    {
        CC_SAFE_RELEASE(_undoModel);
        _undoModel = model;
        CC_SAFE_RETAIN(_undoModel);
    }
}




std::string GameModel::serialize() const
{
    std::stringstream ss;
    ss << _levelId << "|";
    if (_playFieldModel)
    {
        ss << _playFieldModel->serialize();
    }
    ss << "|";
    if (_stackModel)
    {
        ss << _stackModel->serialize();
    }
    ss << "|";
    // UndoModel暂不序列化（回退记录仅当前会话有效）
    return ss.str();
}

void GameModel::deserialize(const std::string& data)
{
    std::stringstream ss(data);
    std::string part;
    int partIndex = 0;

    while (std::getline(ss, part, '|'))
    {
        if (partIndex == 0)
        {
            _levelId = std::stoi(part);
        }
        else if (partIndex == 1 && _playFieldModel)
        {
            _playFieldModel->deserialize(part);
        }
        else if (partIndex == 2 && _stackModel)
        {
            _stackModel->deserialize(part);
        }
        partIndex++;
    }
}