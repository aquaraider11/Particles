//
// Created by spex on 13.1.2019.
//

#include "StatDisplay.h"

void StatDisplay::AddDisplay(IDisplayable* display) {
    displays.push_back(display);
}

std::string StatDisplay::GetText() {
    std::string full;

    for (std::size_t i = 0; i < displays.size(); ++i) {
        displays[i]->Update();
        full += displays[i]->GetText() + "\n";
    }
    
    return full;
}
