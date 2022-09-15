#pragma once

#include "transformClass.h"
#include "idContainer.h"

class chip;

class placeForChip : public scoreWarrior::Sprite, public idContainer {
public:
    static placeForChip* create();
    
    virtual bool init() override;
    
    void bindChip(chip* freeChip);
    void unbindChip();

    chip* getBindedChip();
 public:
    placeForChip() = default;
    ~placeForChip() = default;
    
    chip* bindedChip = nullptr;
};
