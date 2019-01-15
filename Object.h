#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>

class Object {
public:
    float					x;
    float					y;
    float                   size;

    Object( float x, float y, float size );

    //void					Draw( sf::RenderTarget &canvas );

private:
    sf::CircleShape		shape;
};

#endif