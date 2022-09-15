#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace scoreWarrior {
    class tranformClassInterface {
    public:
        virtual const cocos2d::Vec2& getRealPos() const = 0;
    protected:
        tranformClassInterface() = default;
        virtual ~tranformClassInterface() = default;
    };


    template<typename T>
    class transformClass : public T, public tranformClassInterface {
    public:
        
        static transformClass<T>* create() {
            auto object = new (std::nothrow) transformClass<T>();
            if (object && object->init())
            {
                object->autorelease();
                return object;
            }
            CC_SAFE_DELETE(object);
            return nullptr;
        }
        
        void setAnchor(const cocos2d::Vec2& aAnchor) {
            anchor = aAnchor;
            dirtyAnchor = true;
        }
        
        void setPivot(const cocos2d::Vec2& aPivot) {
            T::setAnchorPoint(aPivot);
            dirtyTranform = true;
        }
        
        virtual void setPosition(const cocos2d::Vec2& aPos) override {
            T::setPosition(aPos);
            dirtyTranform = true;
        }

        virtual const cocos2d::Vec2& getRealPos() const override {
            return realPos;
        }
        
        cocos2d::Vec2 calculateRealPos() const {
            cocos2d::Size parentSize;
            cocos2d::Vec2 parentPos;
            if (auto parent = this->getParent()) {
                parentSize = parent->getContentSize();
                if (auto transformInterface = dynamic_cast<const tranformClassInterface*>(parent)) {
                    parentPos = transformInterface->getRealPos();
                }
            }
            cocos2d::Vec2 result;
            result.x = parentPos.x + parentSize.width * anchor.x;
            result.y = parentPos.y + parentSize.height * anchor.y;
            return result;
        }
        
        void updateRealPos() {
            if (dirtyAnchor) {
                realPos = calculateRealPos();
                dirtyTranform = true;
            }
            dirtyAnchor = false;
        }
        
        virtual void updateRealTransform(const cocos2d::Mat4 &transform) {
            if (dirtyTranform) {
                realTransform = transform;
                realTransform.translate(realPos.x, realPos.y, 0.f);
            }
            dirtyTranform = false;
        }
        
        virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override {
            updateRealPos();
            updateRealTransform(transform);
            T::draw(renderer, realTransform, flags);
        }
    protected:
        transformClass() = default;
        ~transformClass() = default;
        
    protected:
        bool dirtyAnchor = true;
        bool dirtyTranform = true;
        cocos2d::Vec2 anchor = cocos2d::Vec2::ANCHOR_MIDDLE;
        cocos2d::Vec2 realPos = cocos2d::Vec2::ZERO;
        cocos2d::Mat4 realTransform = cocos2d::Mat4::ZERO;
    };


    using Node = transformClass<cocos2d::Node>;
    using Sprite = transformClass<cocos2d::Sprite>;
    using Label = transformClass<cocos2d::Label>;
    using Button = transformClass<cocos2d::ui::Button>;
}
