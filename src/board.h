#pragma once
#include "transformClass.h"
#include "enums.h"

class chip;
class placeForChip;
class grid;

class board : public scoreWarrior::Node  {
public:
    static board* create(size_t width, size_t heigth, size_t countColor);
    
    bool isHaveAvailableMoves();
    size_t getScore();
    
    cocos2d::CCCallFunc* createBotFunc();
    
    bool isDirty();
    void resetBoard();
protected:
    board() = default;
    ~board();
    
    bool init(size_t width, size_t height, size_t countColor);
    
    void initGrids();
    void createChipsArray();
    void destroyChipsArray();
    void initColorChips();
    void initColorForChip(chip* obj);
    void initChips();
    void initDataForChip(chip* obj);
    
    void markDirty();
    void unDirty();
    
    void swapChip(placeForChip* fromElement, placeForChip* toElement, moveDirectionChip dir);
    
    bool addChipsForMatching(chip* obj);
    void addChipsForMatchingFromAutomatch();
    void matchingSaveChips();
    
    std::vector<chip*> getMatchingChip(chip* obj);
    
    chip* getChipWithDir(chip* obj, moveDirectionChip dir);
    placeForChip* getGridElementWithDir(chip* obj, moveDirectionChip dir);
    placeForChip* getGridElementFromChip(chip* obj);
    placeForChip* getGridElementFromId(size_t idX, size_t idY);
    
    void clickCallbackForChips(chip* from, moveDirectionChip dir);
    
    cocos2d::Sequence* createActionSequenceForSwapChip(moveDirectionChip dir, std::function<void()> callback);
    cocos2d::Sequence* createActionSequenceForSwapAndReturnChip(moveDirectionChip dir, std::function<void()> callback);
    cocos2d::MoveBy* createActionMove(moveDirectionChip dir);
    
    void updateBoard();
    bool applyGravityForChip(chip* obj);
    bool elementIsNotHaveChip(size_t w, size_t h);
    void spawnFreeChip(size_t w, size_t h);
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
    
    moveDirectionChip getOppositeDirection(moveDirectionChip dir);
    
    std::pair<chip*, moveDirectionChip> getAvailableMove();
private:
    grid* gridPlaces = nullptr;
    grid* gridChips = nullptr;
    
    size_t gridWidth = 0U;
    size_t gridHeight = 0U;
    placeForChip*** places;
    
    const size_t minColor = 2U;
    const size_t maxColor = static_cast<size_t>(colorChip::SIZE);
    const size_t needCountChipsForMatch = 2U;
    size_t countColorBoard = minColor;
    bool dirty = false;
    std::set<chip*> chipsForMatching;
    std::list<chip*> freeChips;
    const moveDirectionChip dirGravity = moveDirectionChip::BOT;
    
    const float timeMoveAction = 0.1f;
    
    size_t score = 0U;
};
