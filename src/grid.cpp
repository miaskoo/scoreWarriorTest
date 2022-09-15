#include "grid.h"
#include "placeForChip.h"
#include "chip.h"
#include "functionHelper.h"

grid* grid::create(size_t width, size_t heigth) {
    auto object = new (std::nothrow) grid();
    if (object && object->init(width, heigth))
    {
        object->autorelease();
        return object;
    }
    CC_SAFE_DELETE(object);
    return nullptr;
}

void grid::setSizeElements(cocos2d::Size size) {
    sizeElement = size;
    setContentSize({gridWidth * sizeElement.width, gridHeight * sizeElement.height});
}

bool grid::init(size_t width, size_t height) {
    gridWidth = width;
    gridHeight = height;
    return true;
}

void grid::updateGrid() {
    float startW = 0.f;
    float startH = 0.f;
    auto pivot = getAnchorPoint();
    if (elementsNotTransformClass) {
        auto pos = getPosition();
        auto realPos = getRealPos();
        startW -= pos.x + realPos.x + sizeElement.width * pivot.x;
        startH -= pos.y + realPos.y + sizeElement.height * pivot.y;
    }
    else {
        startW = (gridWidth - 1) * sizeElement.width * pivot.x;
        startH = (gridHeight - 1) * sizeElement.height * pivot.y;
    }
    for (int w = 0; w < gridWidth; w++) {
        for (int h = 0; h < gridHeight; h++) {
            auto element = getChildByName(uintToStringId(w, h));
            if (!element || element->getNumberOfRunningActions() > 0U) {
                continue;
            }
            element->setContentSize(sizeElement);
            auto pos = cocos2d::Vec2(-startW + w * sizeElement.width, -startH + h * sizeElement.height);
            element->setPosition(pos);
        }
    }
}

const cocos2d::Size& grid::getSizeElement() const {
    return sizeElement;
}

void grid::addChild(cocos2d::Node* node) {
    scoreWarrior::Sprite::addChild(node);
    if (!inited) {
        inited = true;
        elementsNotTransformClass = !dynamic_cast<tranformClassInterface*>(node);
    }
}

void grid::updateRealTransform(const cocos2d::Mat4 &transform) {
    scoreWarrior::Sprite::updateRealTransform(transform);
    updateGrid();
}
