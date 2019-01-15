//
// Created by spex on 13.1.2019.
//

#include <SFML/System/Clock.hpp>
#include "FPS.h"

void FPS::Update() {
    // calculate frames drawn
    MeasuredFrames += 1;

    // wait for measure interval
    if (Clock.getElapsedTime().asSeconds() < MeasureTime)
        return;

    // calculate average fps
    MeasuredTime = Clock.restart().asSeconds();
    Result = (unsigned int)(MeasuredFrames / MeasuredTime);
    MeasuredFrames = 0;
}

std::string FPS::GetText() {
    return "FPS: " + std::to_string(Result);
}
