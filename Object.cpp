#include "Object.h"


Object::Object( float _x, float _y, float _size) :
        x		( _x ),
        y		( _y ),
        size	( _size )
{
    shape.setPosition( x, y );
    shape.setRadius( _size );
    shape.setFillColor( sf::Color( 32, 128, 255 ) );
}

/*void Object::Draw( sf::RenderTarget &canvas ) {
    canvas.draw( shape );
}*/