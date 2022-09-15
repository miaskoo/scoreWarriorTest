#include "match3Scene.h"
#include "board.h"
#include "functionHelper.h"
#include "desktopVideoPlayer.h"

match3Scene* match3Scene::create() {
    auto object = new (std::nothrow) match3Scene();
    if (object && object->init())
    {
        object->autorelease();
        return object;
    }
    CC_SAFE_DELETE(object);
    return nullptr;
}

bool match3Scene::init() {
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    
    setContentSize(visibleSize);

    auto spriteScene = scoreWarrior::Sprite::create();
    spriteScene->setTexture("spriteScene.png");
    spriteScene->setContentSize(visibleSize);
    if (spriteIsValid(spriteScene)) {
        addChild(spriteScene);
    }
    else {
        assert(false && "'spriteScene.png'");
    }

    
    match3Board = board::create(widthBoard, heightBoard, countColorBoard);
    spriteScene->addChild(match3Board);
    //startBot();
    
    labelScore = scoreWarrior::Label::create();
    cocos2d::TTFConfig ttfConfig("fonts/arial.ttf", 23, cocos2d::GlyphCollection::DYNAMIC);
    labelScore->setTTFConfig(ttfConfig);
    labelScore->setAnchor(cocos2d::Vec2::ANCHOR_TOP_LEFT);
    labelScore->setPivot(cocos2d::Vec2::ANCHOR_TOP_LEFT);
    spriteScene->addChild(labelScore);
    
    createUiEndGame();
    hideUiEndGame();
    return true;
}

void match3Scene::startBot() {
    auto actionWait = cocos2d::DelayTime::create(0.1f);
    auto actionSequence = cocos2d::Sequence::create(match3Board->createBotFunc(), actionWait, nullptr);
    auto actionRepeat = cocos2d::RepeatForever::create(actionSequence);
    match3Board->runAction(actionRepeat);
}

void match3Scene::stopBot() {
    match3Board->stopAllActions();
}

void match3Scene::createUiEndGame() {
    endGameUi = scoreWarrior::Node::create();
    
    auto blackScreen = scoreWarrior::Sprite::create();
    blackScreen->setTexture("blackPixel.png");
    blackScreen->setContentSize(getContentSize());
    endGameUi->addChild(blackScreen);

    endGameLabel = scoreWarrior::Label::create();
    cocos2d::TTFConfig ttfConfig("fonts/arial.ttf", 23, cocos2d::GlyphCollection::DYNAMIC);
    endGameLabel->setTTFConfig(ttfConfig);
    endGameLabel->setAnchor(cocos2d::Vec2::ANCHOR_MIDDLE);
    endGameLabel->setPivot(cocos2d::Vec2::ANCHOR_MIDDLE);
    endGameUi->addChild(endGameLabel);

    endGameButton = cocos2d::ui::Button::create();
    endGameButton->loadTextures("whitePixel.png", "whitePixel.png");
    auto& sceneSize = getContentSize();
    endGameButton->setScale(sceneSize.width, sceneSize.height);
    endGameButton->setOpacity(0);
    endGameButton->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    endGameButton->setPosition({sceneSize.width * 0.5f, sceneSize.height * 0.5f});
    endGameButton->addTouchEventListener([this](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        newGame();
    });
    endGameUi->addChild(endGameButton);
    
    addChild(endGameUi);
}

void match3Scene::hideUiEndGame() {
    endGameUi->setVisible(false);
    endGameButton->setVisible(false);
    endGameLabel->setVisible(false);
}

void match3Scene::showUiEndGame() {
    endGameUi->setVisible(true);
    auto player = DesktopVideoPlayer::create();
    player->setFileName("endgame.mp4");
    player->setContentSize(getContentSize());
    player->callback = [player, this](){
        player->removeFromParent();
        endGameButton->setVisible(true);
        endGameLabel->setVisible(true);
        endGameLabel->setString(labelScore->getString() + " Continue? Just tap.");
    };
    endGameUi->addChild(player);
    player->playVideo();
}

void match3Scene::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) {
    cocos2d::Scene::draw(renderer, transform, flags);
    if (state == stateGameScene::GAME) {
        labelScore->setString("YOUR SCORE: " + std::to_string(match3Board->getScore()));
        if (!match3Board->isDirty()) {
            if (!alredyCheckBoard) {
                if (!match3Board->isHaveAvailableMoves()) {
                    endGame();
                }
                alredyCheckBoard = true;
            }
        }
        else {
            alredyCheckBoard = false;
        }
    }
}

void match3Scene::endGame() {
    state = stateGameScene::WAIT;
    showUiEndGame();
    //stopBot();
}

void match3Scene::newGame() {
    hideUiEndGame();
    state = stateGameScene::GAME;
    alredyCheckBoard = false;
    match3Board->resetBoard();
    //startBot();
}
