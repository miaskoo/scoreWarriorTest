#include "functionHelper.h"
#include "cocos2d.h"

bool spriteIsValid(cocos2d::Node* sprite) {
   if (sprite == nullptr ||
       sprite->getContentSize().width <= 0 ||
       sprite->getContentSize().height <= 0) {
       return false;
   }
   return true;
}

std::string uintToStringId(unsigned int xId, unsigned int yId) {
    return "x:" + std::to_string(xId) + "y:" + std::to_string(yId);
}
