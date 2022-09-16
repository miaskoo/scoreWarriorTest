#pragma once

#include "ui/CocosGUI.h"
#include "idContainer.h"
#include "enums.h"

class chip : public cocos2d::ui::Button, public idContainer {
public:
    static chip* create();
    
    void setColor(colorChip aColor);
    colorChip getColorChip() const;
    
    void updateTextureWithColor();
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event* /*unusedEvent*/) override;
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event* /*unusedEvent*/) override;
    void setTouchEndCallback(std::function<void(chip*, moveDirectionChip)> aTouchEndCallback);
    void setClickable(bool value);
    bool isStable();
    void setStable(bool value);
    
    bool operator== (const chip& obj);
protected:
    chip() = default;
    ~chip() = default;
private:
    colorChip color = colorChip::EMPTY;
    cocos2d::Vec2 startPos = cocos2d::Vec2::ZERO;
    bool startClick = false;
    bool clickable = true;
    bool stable = true;
    
    std::function<void(chip*, moveDirectionChip)> touchEndCallback = nullptr;
};
