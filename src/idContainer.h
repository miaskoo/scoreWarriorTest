#pragma once

class idContainer {
public:
    void setId(unsigned int aXId, unsigned int aYId);
    int getXId();
    int getYId();
protected:
    idContainer() = default;
    ~idContainer() = default;
private:
    unsigned int id[2] = {0U, 0U};
};
