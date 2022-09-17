#include "board.h"
#include "grid.h"
#include "placeForChip.h"
#include "chip.h"
#include "functionHelper.h"

board* board::create(size_t width, size_t heigth, size_t countColor) {
    auto object = new (std::nothrow) board();
    if (object && object->init(width, heigth, countColor))
    {
        object->autorelease();
        return object;
    }
    CC_SAFE_DELETE(object);
    return nullptr;
}

void board::initColorChips() {
    for (int w = 0; w < gridWidth; w++) {
        for (int h = 0; h < gridHeight; h++) {
            auto element = places[w][h];
            auto bindedChip = element->getBindedChip();
            initColorForChip(bindedChip);
        }
    }
}

void board::initColorForChip(chip* obj) {
    if (!obj) {
        return;
    }
    auto color = static_cast<colorChip>(rand() % countColorBoard);
    obj->setColor(color);
    if (getMatchingChip(obj).size() >= needCountChipsForMatch) {
        for (size_t n = 0U; n < countColorBoard; n++) {
            obj->setColor(static_cast<colorChip>(n));
            if (getMatchingChip(obj).size() < needCountChipsForMatch) {
                break;
            }
        }
    }
    obj->updateTextureWithColor();
    auto chipSize = obj->getContentSize();
    auto sizePlace = gridPlaces->getSizeElement();
    obj->setScale(sizePlace.width / chipSize.width, sizePlace.height / chipSize.height);
}

placeForChip* board::getGridElementWithDir(chip* obj, moveDirectionChip dir) {
    placeForChip* result = nullptr;
    auto idX = obj->getXId();
    auto idY = obj->getYId();
    switch(dir) {
        case moveDirectionChip::TOP: {
            result = getGridElementFromId(idX, idY + 1);
            break;
        }
        case moveDirectionChip::BOT: {
            result = getGridElementFromId(idX, idY - 1);
            break;
        }
        case moveDirectionChip::RIGTH: {
            result = getGridElementFromId(idX + 1, idY);
            break;
        }
        case moveDirectionChip::LEFT: {
            result = getGridElementFromId(idX - 1, idY);
            break;
        }
        default: {
            break;
        }
    }
    return result;
}

chip* board::getChipWithDir(chip* obj, moveDirectionChip dir) {
    if (auto element = getGridElementWithDir(obj, dir)) {
        return element->getBindedChip();
    }
    return nullptr;
}

placeForChip* board::getGridElementFromChip(chip* obj) {
    auto idX = obj->getXId();
    auto idY = obj->getYId();
    return getGridElementFromId(idX, idY);
}

moveDirectionChip board::getOppositeDirection(moveDirectionChip dir) {
    moveDirectionChip result;
    switch(dir) {
        case moveDirectionChip::TOP: {
            result = moveDirectionChip::BOT;
            break;
        }
        case moveDirectionChip::BOT: {
            result = moveDirectionChip::TOP;
            break;
        }
        case moveDirectionChip::RIGTH: {
            result = moveDirectionChip::LEFT;
            break;
        }
        case moveDirectionChip::LEFT: {
            result = moveDirectionChip::RIGTH;
            break;
        }
        default: {
            break;
        }
    }
    return result;
}

void board::clickCallbackForChips(chip* from, moveDirectionChip dir) {
    placeForChip* toElement = getGridElementWithDir(from, dir);
    if (!toElement) {
        return;
    }
    chip* to = toElement->getBindedChip();
    if (!to) {
        return;
    }
    auto fromElement = getGridElementFromId(from->getXId(), from->getYId());
    swapChip(fromElement, toElement, dir);
}

void board::swapChip(placeForChip* fromElement, placeForChip* toElement, moveDirectionChip dir) {
    if (!fromElement || !toElement) {
        return;
    }
    
    auto from = fromElement->getBindedChip();
    auto to = toElement->getBindedChip();
    
    toElement->unbindChip();
    fromElement->unbindChip();
    if (from) {
        toElement->bindChip(from);
        from->setClickable(false);
    }
    if (to) {
        fromElement->bindChip(to);
        to->setClickable(false);
    }
    
    auto isMatch3 = addChipsForMatching(to) || addChipsForMatching(from);
    if (isMatch3) {
        auto callback = CC_CALLBACK_0(board::markDirty, this);
        if (from) {
            from->runAction(createActionSequenceForSwapChip(dir, callback));
        }
        if (to) {
            to->runAction(createActionSequenceForSwapChip(getOppositeDirection(dir), callback));
        }
    }
    else {
        toElement->unbindChip();
        fromElement->unbindChip();
        toElement->bindChip(to);
        fromElement->bindChip(from);
        
        
        if (from) {
            auto callback = CC_CALLBACK_0(chip::setClickable, from, true);
            from->runAction(createActionSequenceForSwapAndReturnChip(dir, callback));
        }
        if (to) {
            auto callback = CC_CALLBACK_0(chip::setClickable, to, true);
            to->runAction(createActionSequenceForSwapAndReturnChip(getOppositeDirection(dir), callback));
        }
    }
}

bool board::addChipsForMatching(chip* obj) {
    bool result = false;
    auto chipsMatch = getMatchingChip(obj);
    if (chipsMatch.size() >= needCountChipsForMatch) {
        result = true;
        for (auto& obj : chipsMatch) {
            chipsForMatching.insert(obj);
        }
        chipsForMatching.insert(obj);
    }
    return result;
}

cocos2d::Sequence* board::createActionSequenceForSwapChip(moveDirectionChip dir, std::function<void()> callback) {
    auto actionMove = createActionMove(dir);
    auto actionCallback = cocos2d::CCCallFunc::create(callback);
    return cocos2d::Sequence::create(actionMove, actionCallback, nullptr);
}

cocos2d::Sequence* board::createActionSequenceForSwapAndReturnChip(moveDirectionChip dir, std::function<void()> callback) {
    auto actionMove = createActionMove(dir);
    auto actionMove2 = createActionMove(getOppositeDirection(dir));
    auto actionCallback = cocos2d::CCCallFunc::create(callback);
    return cocos2d::Sequence::create(actionMove, actionMove2, actionCallback, nullptr);
}

cocos2d::MoveBy* board::createActionMove(moveDirectionChip dir) {
    float x = 0;
    float y = 0;
    auto& sizePlace = gridPlaces->getSizeElement();
    
    switch(dir) {
        case moveDirectionChip::TOP: {
            y = sizePlace.height;
            break;
        }
        case moveDirectionChip::BOT: {
            y = -sizePlace.height;
            break;
        }
        case moveDirectionChip::RIGTH: {
            x = sizePlace.width;
            break;
        }
        case moveDirectionChip::LEFT: {
            x = -sizePlace.width;
            break;
        }
        default: {
            break;
        }
    }
    return cocos2d::MoveBy::create(timeMoveAction, {x, y});
}

void board::markDirty() {
    dirty = true;
}

void board::unDirty() {
    dirty = false;
}

bool board::isDirty() {
    return dirty;
}

void board::updateBoard() {
    if (!isDirty()) {
        return;
    }

    bool stable = true;
    bool needUpdateChipGreed = false;
    
    for (int w = 0; w < gridWidth; w++) {
        for (int h = 0; h < gridHeight; h++) {
            auto currentElement = getGridElementFromId(w, h);
            auto currentChip = currentElement->getBindedChip();
            if (!currentChip) {
                continue;
            }
            bool isFalling = false;
            isFalling = currentChip->getNumberOfRunningActions() > 0U;
            if (!isFalling) {
                isFalling = applyGravityForChip(currentChip);
            }
            if (isFalling) {
                stable = false;
            }
        }
    }
    
    if (stable) {
        stable = chipsForMatching.empty();
        score += chipsForMatching.size();
        matchingSaveChips();
    }
    
    const auto topChipId = gridHeight - 1;
    const auto preTopChipId = gridHeight - 2;
    for (int w = 0; w < gridWidth; w++) {
        bool firstElementIsEmpty = elementIsNotHaveChip(w, topChipId);
        if (!firstElementIsEmpty) {
            continue;
        }
        auto secondChip = getGridElementFromId(w, preTopChipId)->getBindedChip();
        bool secondChipIsFalling = false;
        if (secondChip) {
            secondChipIsFalling = secondChip->getNumberOfRunningActions() > 0U;
        }
        if (secondChipIsFalling) {
            continue;
        }
        spawnFreeChip(w, topChipId);
        needUpdateChipGreed = true;
    }
        
    
    if (stable) {
        addChipsForMatchingFromAutomatch();
        stable = chipsForMatching.empty();
    }
    if (stable) {
        for (int w = 0; w < gridWidth; w++) {
            for (int h = 0; h < gridHeight; h++) {
                auto currentElement = getGridElementFromId(w, h);
                auto currentChip = currentElement->getBindedChip();
                currentChip->setClickable(true);
            }
        }
        unDirty();
    }
    if (needUpdateChipGreed) {
        gridChips->updateGrid();
    }
}

void board::addChipsForMatchingFromAutomatch() {
    for (int w = 0; w < gridWidth; w++) {
        for (int h = 0; h < gridHeight; h++) {
            auto currentChip = getGridElementFromId(w, h)->getBindedChip();
            if (!currentChip || currentChip->isStable()) {
                continue;
            }
            addChipsForMatching(currentChip);
            currentChip->setStable(true);
        }
    }
}

void board::matchingSaveChips() {
    for (auto& curChip : chipsForMatching) {
        getGridElementFromChip(curChip)->unbindChip();
        curChip->setVisible(false);
        curChip->stopAllActions();
        freeChips.push_back(curChip);
    }
    chipsForMatching.clear();
}

bool board::elementIsNotHaveChip(size_t w, size_t h) {
    auto element = getGridElementFromId(w, h);
    return !element || !element->getBindedChip();
}

void board::spawnFreeChip(size_t w, size_t h) {
    auto element = getGridElementFromId(w, h);
    if (element && !element->getBindedChip()) {
        if (!freeChips.empty()) {
            auto freeChip = freeChips.front();
            freeChips.pop_front();
            initColorForChip(freeChip);
            element->bindChip(freeChip);
            
            freeChip->setVisible(true);
            freeChip->setClickable(true);
            freeChip->setStable(false);
        }
        else {
            assert(false && "no free chip");
        }
    }
}

bool board::applyGravityForChip(chip *obj) {
    bool result = false;
    auto gravityElement = getGridElementWithDir(obj, dirGravity);
    if (gravityElement && !gravityElement->getBindedChip()) {
        result = true;
        
        auto currentElement = getGridElementFromChip(obj);
        obj->setClickable(false);
        obj->setStable(false);
        currentElement->unbindChip();
        gravityElement->bindChip(obj);
        obj->runAction(createActionMove(dirGravity));
    }
    return result;
}

void board::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) {
    scoreWarrior::Node::draw(renderer, transform, flags);
    updateBoard();
}

std::vector<chip*> board::getMatchingChip(chip* obj) {
    std::vector<chip*> result;
    std::vector<chip*> chipsForMatch;
    auto hOrientations = {moveDirectionChip::RIGTH, moveDirectionChip::LEFT};
    auto vOrientations = {moveDirectionChip::TOP, moveDirectionChip::BOT};
    for (const auto& orient : hOrientations) {
        auto nextObj = getChipWithDir(obj, orient);
        while (nextObj && *nextObj == *obj) {
            chipsForMatch.push_back(nextObj);
            nextObj = getChipWithDir(nextObj, orient);
        }
    }
    if (chipsForMatch.size() >= needCountChipsForMatch) {
        result.insert(result.end(), chipsForMatch.begin(), chipsForMatch.end());
    }
    chipsForMatch.clear();
    
    for (const auto& orient : vOrientations) {
        auto nextObj = getChipWithDir(obj, orient);
        while (nextObj && *nextObj == *obj) {
            chipsForMatch.push_back(nextObj);
            nextObj = getChipWithDir(nextObj, orient);
        }
    }
    if (chipsForMatch.size() >=  needCountChipsForMatch) {
        result.insert(result.end(), chipsForMatch.begin(), chipsForMatch.end());
    }
    
    return result;
}

bool board::init(size_t width, size_t height, size_t countColor) {
    gridWidth = width;
    gridHeight = height;
    if (countColor > maxColor) {
        countColor = maxColor;
    }
    if (countColor < minColor) {
        countColor = minColor;
    }
    countColorBoard = countColor;
    initGrids();
    createChipsArray();
    initChips();
    do {
        initColorChips();
    }
    while (!isHaveAvailableMoves());
    return true;
}

void board::initGrids() {
    gridPlaces = grid::create(gridWidth, gridHeight);
    gridPlaces->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    gridPlaces->setSizeElements(cocos2d::Size(60, 60));
    addChild(gridPlaces);
    
    gridChips = grid::create(gridWidth, gridHeight);
    gridChips->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    gridChips->setSizeElements(cocos2d::Size(60, 60));
    addChild(gridChips);
}

void board::createChipsArray() {
    places = new placeForChip**[gridWidth];
    for (int w = 0; w < gridWidth; w++) {
        places[w] = new placeForChip*[gridHeight];
    }
}


void board::destroyChipsArray() {
    if (places) {
        for (int w = 0; w < gridWidth; w++) {
            delete[] places[w];
            places[w] = nullptr;
        }
        delete[] places;
        places = nullptr;
    }
}

void board::initChips() {
    for (int w = 0; w < gridWidth; w++) {
        for (int h = 0; h < gridHeight; h++) {
            auto element = placeForChip::create();
            element->setId(w, h);
            places[w][h] = element;
            element->setName(uintToStringId(w,h));
            gridPlaces->addChild(element);
            auto chipForBind = chip::create();
            initDataForChip(chipForBind);
            element->bindChip(chipForBind);
            gridChips->addChild(chipForBind);
        }
    }
}

void board::initDataForChip(chip* obj) {
    obj->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    obj->setTouchEndCallback(CC_CALLBACK_2(board::clickCallbackForChips, this));
}

placeForChip* board::getGridElementFromId(size_t idX, size_t idY) {
    if (idX < gridWidth && idY < gridHeight) {
        return places[idX][idY];
    }
    return nullptr;
}

board::~board() {
    destroyChipsArray();
}

std::pair<chip*, moveDirectionChip> board::getAvailableMove() {
    for (int w = 0; w < gridWidth; w++) {
        for (int h = 0; h < gridHeight; h++) {
            auto element = getGridElementFromId(w, h);
            auto obj = element->getBindedChip();
            if (!obj) {
                continue;
            }
            
            auto orientations = {moveDirectionChip::RIGTH, moveDirectionChip::LEFT, moveDirectionChip::TOP, moveDirectionChip::BOT};
            auto hOrientations = {moveDirectionChip::RIGTH, moveDirectionChip::LEFT};
            auto vOrientations = {moveDirectionChip::TOP, moveDirectionChip::BOT};
            
            for (auto& orientation : orientations) {
                std::vector<chip*> result;
                std::vector<chip*> chipsForMatch;
                auto orientationObj = getChipWithDir(obj, orientation);
                auto ignodeOrientation = getOppositeDirection(orientation);
                if (!orientationObj) {
                    continue;
                }
                
                for (const auto& orient : hOrientations) {
                    if (orient == ignodeOrientation) {
                        continue;
                    }
                    auto nextObj = getChipWithDir(orientationObj, orient);
                    while (nextObj && *nextObj == *obj) {
                        chipsForMatch.push_back(nextObj);
                        nextObj = getChipWithDir(nextObj, orient);
                    }
                }
                if (chipsForMatch.size() >= needCountChipsForMatch) {
                    result.insert(result.end(), chipsForMatch.begin(), chipsForMatch.end());
                }
                chipsForMatch.clear();
                
                for (const auto& orient : vOrientations) {
                    if (orient == ignodeOrientation) {
                        continue;
                    }
                    auto nextObj = getChipWithDir(orientationObj, orient);
                    while (nextObj && *nextObj == *obj) {
                        chipsForMatch.push_back(nextObj);
                        nextObj = getChipWithDir(nextObj, orient);
                    }
                }
                if (chipsForMatch.size() >=  needCountChipsForMatch) {
                    result.insert(result.end(), chipsForMatch.begin(), chipsForMatch.end());
                }
                
                if (result.size() > 0U) {
                    return {obj, orientation};
                }
            }
        }
    }
    return {nullptr, moveDirectionChip::NONE};
}

cocos2d::CCCallFunc* board::createBotFunc() {
    return cocos2d::CCCallFunc::create([this](){
        if (dirty) {
            return;
        }
        auto move = getAvailableMove();
        if (move.first) {
            clickCallbackForChips(move.first, move.second);
        }
    });
}

size_t board::getScore() {
    return score;
}

bool board::isHaveAvailableMoves() {
    return getAvailableMove().first;
}

void board::resetBoard() {
    while (!isHaveAvailableMoves()) {
        initColorChips();
    }
    score = 0;
}
