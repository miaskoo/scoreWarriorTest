#include "idContainer.h"

void idContainer::setId(unsigned int aXId, unsigned int aYId) {
    id[0] = aXId;
    id[1] = aYId;
}

int idContainer::getXId() {
    return id[0];
}

int idContainer::getYId() {
    return id[1];
}
