#pragma once

#include "transformClass.h"
#include "enums.h"

class placeForChip;
class chip;

namespace cocos2d {
    class Node;
}

class grid : public scoreWarrior::Sprite {
public:
    static grid* create(size_t width, size_t heigth);
    
    void setSizeElements(cocos2d::Size size);
    void updateGrid();
    const cocos2d::Size& getSizeElement() const;
    
    virtual void addChild(cocos2d::Node* node) override;
    virtual void updateRealTransform(const cocos2d::Mat4 &transform) override;
protected:
    grid() = default;
    ~grid() = default;
    
    bool init(size_t width, size_t height);
private:
    size_t gridWidth = 0U;
    size_t gridHeight = 0U;
    bool inited = false;
    bool elementsNotTransformClass = false;
    
    cocos2d::Size sizeElement = cocos2d::Size::ZERO;
};
