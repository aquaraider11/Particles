//
// Created by aqua on 10.2.2019.
//

#ifndef SFML_PARTICLESYSTEM_CUSTOMSHAPE_H
#define SFML_PARTICLESYSTEM_CUSTOMSHAPE_H

#endif //SFML_PARTICLESYSTEM_CUSTOMSHAPE_H

#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Shape.hpp>

namespace ln {
    class customShape : public sf::Shape {
    public:
        explicit customShape(float radius, std::size_t pointCount = 30);

        void setRadius(float radius);

        void updateShape();

        float getRadius() const;

        void setPointCount(std::size_t count);

        virtual std::size_t getPointCount() const;

        virtual sf::Vector2f getPoint(std::size_t index) const;

    private:
        float m_radius;
        std::size_t m_pointCount;

    };

}