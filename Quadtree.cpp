#include "Quadtree.h"
#include "Particle.h"
#include <iostream>
#include <sstream>

using namespace std;

template <class T>
Quadtree<T>::Quadtree( float _x, float _y, float _width, float _height, int _level, int _maxLevel, int _maxObjects) :
        x		( _x ),
        y		( _y ),
        width	( _width ),
        height	( _height ),
        level	( _level ),
        maxLevel( _maxLevel ),
        maxObjects(_maxObjects)
{
    shape.setPosition( x, y );
    shape.setSize( sf::Vector2f( width, height ) );
    shape.setFillColor( sf::Color( 0, 0, 0, 0 ) );
    shape.setOutlineThickness( 1.0f );
    shape.setOutlineColor( sf::Color( 64, 128, 255 ) );

/*
    if ( level == _maxLevel ) {
        return;
    }

    NW = new Quadtree( x, y, width / 2.0f, height / 2.0f, level+1, _maxLevel, maxObjects);
    NE = new Quadtree( x + width / 2.0f, y, width / 2.0f, height / 2.0f, level+1, _maxLevel, maxObjects);
    SW = new Quadtree( x, y + height / 2.0f, width / 2.0f, height / 2.0f, level+1, _maxLevel, maxObjects);
    SE = new Quadtree( x + width / 2.0f, y + height / 2.0f, width / 2.0f, height / 2.0f, level+1, _maxLevel, maxObjects);
*/
}

template <class T>
bool Quadtree<T>::Subdivide(Quadtree<T> & target) {
    if (target.level >= maxLevel)
        return false;
    if (target.isSubdivided)
        return true;

    NW = new Quadtree( x, y, width / 2.0f, height / 2.0f, level+1, maxLevel, maxObjects);
    NE = new Quadtree( x + width / 2.0f, y, width / 2.0f, height / 2.0f, level+1, maxLevel, maxObjects);
    SW = new Quadtree( x, y + height / 2.0f, width / 2.0f, height / 2.0f, level+1, maxLevel, maxObjects);
    SE = new Quadtree( x + width / 2.0f, y + height / 2.0f, width / 2.0f, height / 2.0f, level+1, maxLevel, maxObjects);
    NW->parent = NE->parent = SE->parent = SW->parent = &target;
    target.isSubdivided = true;
    return true;
}

template <class T>
void Quadtree<T>::AddObject( T *object ) {
    T * Obj = object;

    if (!sf::FloatRect(this->x,this->y, this->width, this->height).contains(Obj->location()))
        return;

    if (this->objects.size() < this->maxObjects && !this->isSubdivided)
        this->objects.push_back(Obj);
    else if (this->Subdivide(*this))
    {
        for (const auto &i : objects)
        {
            this->NE->AddObject(i);
            this->NW->AddObject(i);
            this->SE->AddObject(i);
            this->SW->AddObject(i);
        }
        this->objects.clear();
        this->NE->AddObject(Obj);
        this->NW->AddObject(Obj);
        this->SE->AddObject(Obj);
        this->SW->AddObject(Obj);
    }
    else
        this->objects.push_back(object);

}

template <class T>
vector<T*> Quadtree<T>::GetObjectsAt( float _x, float _y ) {
    if ( !isSubdivided ) {
        return objects;
    }

    vector<T*> returnObjects, childReturnObjects;
    if ( !objects.empty() ) {
        returnObjects = objects;
    }
    if ( _x > x + width / 2.0f && _x < x + width ) {
        if ( _y > y + height / 2.0f && _y < y + height ) {
            childReturnObjects = SE->GetObjectsAt( _x, _y );
            returnObjects.insert( returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end() );
            return returnObjects;
        } else if ( _y > y && _y <= y + height / 2.0f ) {
            childReturnObjects = NE->GetObjectsAt( _x, _y );
            returnObjects.insert( returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end() );
            return returnObjects;
        }
    } else if ( _x > x && _x <= x + width / 2.0f ) {
        if ( _y > y + height / 2.0f && _y < y + height ) {
            childReturnObjects = SW->GetObjectsAt( _x, _y );
            returnObjects.insert( returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end() );
            return returnObjects;
        } else if ( _y > y && _y <= y + height / 2.0f ) {
            childReturnObjects = NW->GetObjectsAt( _x, _y );
            returnObjects.insert( returnObjects.end(), childReturnObjects.begin(), childReturnObjects.end() );
            return returnObjects;
        }
    }
    return returnObjects;
}

template <class T>
void Quadtree<T>::Clear() {

    //While we are not in the root tree
    while (this->level > 0)
    {
        //If the tree is not subdivided, clear it, and delete the tree
        if (!isSubdivided)
        {
            objects.clear();
            delete(this);
            return;
        } else // Else let's clear the subtrees first
        {
            NW->Clear();
            NE->Clear();
            SW->Clear();
            SE->Clear();
            this->isSubdivided = false;
        }//Check for funny edge case **DO NOT DELETE**
        if (!objects.empty())
        {
            objects.clear();
        }
    }
    //When we get to the root tree (this is probably where the clear function starts, because you give it the root tree)
    //If the tree is not subdivided, let's just clear the objects and return
    if (!isSubdivided)
    {
        objects.clear();
        return;
    } else //Else we'll clear the subtrees first, and get to use the fancy while loop above to DELETE TREES̃~!!!
    {
        NW->Clear();
        NE->Clear();
        SW->Clear();
        SE->Clear();
        this->isSubdivided = false;
    }//Let's check the funny edge case again for funzies.
    if (!objects.empty())
    {
        objects.clear();
    }


}



template <class T>
void Quadtree<T>::Draw( sf::RenderTarget &canvas ) {
    stringstream ss;
    ss << objects.size();
    string numObjectsStr = ss.str();
    canvas.draw( shape );
    if ( isSubdivided ) {
        NW->Draw( canvas );
        NE->Draw( canvas );
        SW->Draw( canvas );
        SE->Draw( canvas );
    }
}

template <class T>
bool Quadtree<T>::Contains( Quadtree *child, T *object ) {
    return	 !( object->x < child->x ||
                  object->y < child->y ||
                  object->x > child->x + child->width  ||
                  object->y > child->y + child->height ||
                  object->x + object->size_QT < child->x ||
                  object->y + object->size_QT < child->y ||
                  object->x + object->size_QT > child->x + child->width ||
                  object->y + object->size_QT > child->y + child->height );
}

template class Quadtree<Particle>;
