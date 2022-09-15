#include "factoryScene.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include <assert.h>
#include <string>
#include "transformClass.h"
#include "functionHelper.h"
#include "match3Scene.h"

cocos2d::Scene* factoryScene::createMatch3Scene() {
    auto scene = match3Scene::create();

    auto size = scene->getContentSize();
    
    auto closeButton = cocos2d::ui::Button::create();
    closeButton->loadTextures("CloseNormal.png", "CloseSelected.png");
    auto sizeButton = closeButton->getContentSize();
    closeButton->setAnchorPoint(cocos2d::Vec2::ANCHOR_TOP_RIGHT);
    closeButton->setPosition(cocos2d::Vec2(size.width, size.height));
    closeButton->addTouchEventListener([](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        cocos2d::Director::getInstance()->end();
    });
    
    if (spriteIsValid(closeButton)) {
        scene->addChild(closeButton);
    }
    else {
        assert(false && "'CloseNormal.png' and 'CloseSelected.png'");
    }
    
    return scene;
}
