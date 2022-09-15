#include "placeForChip.h"
#include "chip.h"
#include "functionHelper.h"

placeForChip* placeForChip::create() {
    auto object = new (std::nothrow) placeForChip();
    if (object && object->init())
    {
        object->autorelease();
        return object;
    }
    CC_SAFE_DELETE(object);
    return nullptr;
}

bool placeForChip::init() {
    auto result = scoreWarrior::Sprite::init();
    setTexture("gridElement.png");
    return result;
}

void placeForChip::bindChip(chip* freeChip) {
    bindedChip = freeChip;
    bindedChip->setId(getXId(), getYId());
    bindedChip->setName(uintToStringId(getXId(), getYId()));
}

void placeForChip::unbindChip() {
    if (bindedChip) {
        bindedChip->setName("nullid");
    }
    bindedChip = nullptr;
}

chip* placeForChip::getBindedChip() {
    return bindedChip;
}

