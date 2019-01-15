#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

using namespace std;

template <class T>
class Quadtree {
public:
    Quadtree( float x, float y, float width, float height, int level, int maxLevel );

    void					AddObject( T *object );
    vector<T*>			    GetObjectsAt( float x, float y );
    void					Clear();

    void					SetFont( const sf::Font &font );
    void					Draw( sf::RenderTarget &canvas );

private:
    float					x;
    float					y;
    float					width;
    float					height;
    int						level;
    int						maxLevel;
    vector<T*>			    objects;

    Quadtree *				parent;
    Quadtree *				NW;
    Quadtree *				NE;
    Quadtree *				SW;
    Quadtree *				SE;

    sf::RectangleShape		shape;
    sf::Text				text;

    bool					Contains( Quadtree *child, T *object );
};

#endif
