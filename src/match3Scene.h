#pragma once

#include "cocos2d.h"
#include "enums.h"
#include "transformClass.h"

class board;

class match3Scene : public cocos2d::Scene {
public:
    static match3Scene* create();
    virtual bool init() override;
    
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
    void createUiEndGame();
    void hideUiEndGame();
    void showUiEndGame();
    void startBot();
    void stopBot();
    
    void endGame();
    void newGame();
protected:
    match3Scene() = default;
    ~match3Scene() = default;
private:
    board* match3Board = nullptr;
    scoreWarrior::Label* labelScore = nullptr;
    scoreWarrior::Node* endGameUi = nullptr;
    cocos2d::ui::Button* endGameButton = nullptr;
    scoreWarrior::Label* endGameLabel = nullptr;
    
    stateGameScene state = stateGameScene::GAME;
    
    bool alredyCheckBoard = false;
    
    const size_t widthBoard = 4U;
    const size_t heightBoard = 5U;
    const size_t countColorBoard = 6U;
    const bool enableBot = false;
};
