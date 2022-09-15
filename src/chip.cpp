#include "chip.h"

chip* chip::create() {
    auto object = new (std::nothrow) chip();
    if (object && object->init())
    {
        object->autorelease();
        return object;
    }
    CC_SAFE_DELETE(object);
    return nullptr;
}

void chip::setColor(colorChip aColor) {
    color = aColor;
}

colorChip chip::getColorChip() const {
    return color;
}

void chip::updateTextureWithColor() {
    switch(color) {
        case colorChip::RED: {
            this->loadTextures("redChip.png", "redChipSelect.png");
            break;
        }
        case colorChip::BLUE: {
            this->loadTextures("blueChip.png", "blueChipSelect.png");
            break;
        }
        case colorChip::YELLOW: {
            this->loadTextures("yellowChip.png", "yellowChipSelect.png");
            break;
        }
        case colorChip::GREEN: {
            this->loadTextures("greenChip.png", "greenChipSelect.png");
            break;
        }
        case colorChip::PINK: {
            this->loadTextures("pinkChip.png", "pinkChipSelect.png");
            break;
        }
        case colorChip::NEGATIVE: {
            this->loadTextures("negativeChip.png", "negativeChipSelect.png");
            break;
        }
        case colorChip::EMPTY: {
            break;
        }
        default: {
            break;
        }
    }
}

bool chip::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event* /*unusedEvent*/) {
    if (!clickable) {
        return false;
    }
    if (cocos2d::ui::Button::onTouchBegan(touch, nullptr)) {
        startPos = touch->getLocation();
        startClick = true;
        return true;
    }
    return false;
}

void chip::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event* /*unusedEvent*/) {
    cocos2d::ui::Button::onTouchEnded(touch, nullptr);
    if (startClick) {
        moveDirectionChip direction = moveDirectionChip::NONE;
        auto requiredDistance = cocos2d::Size(50, 50);
        auto endPos = touch->getLocation();
        if (endPos.y > startPos.y + requiredDistance.height) {
            direction = moveDirectionChip::TOP;
        }
        else if (endPos.y < startPos.y - requiredDistance.height) {
            direction = moveDirectionChip::BOT;
        }
        if (endPos.x < startPos.x - requiredDistance.width) {
            if (direction != moveDirectionChip::NONE) {
                direction = moveDirectionChip::NONE;
            }
            else {
                direction = moveDirectionChip::LEFT;
            }
        }
        else if (endPos.x > startPos.x + requiredDistance.width) {
            if (direction != moveDirectionChip::NONE) {
                direction = moveDirectionChip::NONE;
            }
            else {
                direction = moveDirectionChip::RIGTH;
            }
        }
        if (touchEndCallback) {
            touchEndCallback(this, direction);
        }
        startClick = false;
    }
}

void chip::setTouchEndCallback(std::function<void(chip*, moveDirectionChip)> aTouchEndCallback) {
    touchEndCallback = aTouchEndCallback;
}

void chip::setClickable(bool value) {
    clickable = value;
}

bool chip::operator== (const chip& obj) {
    return color == obj.getColorChip();
}
