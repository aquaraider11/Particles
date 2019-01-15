//
// Created by spex on 13.1.2019.
//

#ifndef SFML_PARTICLESYSTEM_FPS_H
#define SFML_PARTICLESYSTEM_FPS_H


#include "IDisplayable.h"

class FPS : public IDisplayable {
private:
    sf::Clock Clock;
    float MeasureTime = 1;

    float MeasuredTime = 1;
    unsigned int MeasuredFrames = 0;
    unsigned int Result = 0;

public:
    void Update();
    std::string GetText();
};


#endif //SFML_PARTICLESYSTEM_FPS_H
