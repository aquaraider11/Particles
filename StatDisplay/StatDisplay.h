//
// Created by spex on 13.1.2019.
//

#ifndef SFML_PARTICLESYSTEM_STATDISPLAY_H
#define SFML_PARTICLESYSTEM_STATDISPLAY_H


#include <vector>

#include "IDisplayable.h"

class StatDisplay {
private:
    std::vector<IDisplayable*> displays;

public:
    void AddDisplay(IDisplayable* display);
    std::string GetText();
};


#endif //SFML_PARTICLESYSTEM_STATDISPLAY_H
