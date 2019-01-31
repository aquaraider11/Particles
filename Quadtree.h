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
    Quadtree( float x, float y, float width, float height, int level, int maxLevel, int maxObjects);

    void					AddObject( T *object );
    vector<T*>			    GetObjectsAt( float x, float y );
    void					Clear();

    void					Draw( sf::RenderTarget &canvas );

private:
    float					x;
    float					y;
    float					width;
    float					height;
    sf::FloatRect           FR;
    int						level;
    int						maxLevel;
    int                     maxObjects;
    vector<T*>			    objects;

    Quadtree *				parent;
    Quadtree *				NW = nullptr;
    Quadtree *				NE = nullptr;
    Quadtree *				SW = nullptr;
    Quadtree *				SE = nullptr;

    sf::RectangleShape		shape;

    bool					Contains( Quadtree *child, T *object );
    bool                    isSubdivided = false;
    bool                    Subdivide(Quadtree<T> & target);
};

#endif
