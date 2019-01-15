//
// Created by spex on 13.1.2019.
//

#ifndef SFML_PARTICLESYSTEM_IDISPLAYABLE_H
#define SFML_PARTICLESYSTEM_IDISPLAYABLE_H

#include <string>

class IDisplayable {
public:
    virtual void Update() {}
    virtual std::string GetText() { return "DISPLAY ERROR"; }
};

#endif //SFML_PARTICLESYSTEM_IDISPLAYABLE_H
