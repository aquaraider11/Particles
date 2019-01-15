//
// Created by spex on 13.1.2019.
//

#include <cmath>

#include "Particle.h"

Particle::Particle(float x, float y, float size = 1)
{
    location_ = sf::Vector2f(x, y);
    velocity_ = sf::Vector2f(0, 0);
    this->size = size;

    obj_ = sf::CircleShape(this->size);
    obj_.setFillColor(sf::Color(255, 255, 255));
    obj_.setPosition(location_);
}

void Particle::update()
{
    // reduce velocity
    velocity_.x -= velocity_.x / (100 / (speed/0.5));
    velocity_.y -= velocity_.y / (100 / (speed/0.5));

    // update location
    location_ += velocity_;
    updateSize();
    obj_.setPosition(location_);
}

void Particle::move(const sf::Vector2f* vector, float multiplier)
{
    // apply vector to velocity
    velocity_.x += vector->x * speed * multiplier;
    velocity_.y += vector->y * speed * multiplier;
}

void Particle::updateSize()
{
    // calculate scale factor
    float scale = (float)(fabs(velocity_.x) + fabs(velocity_.y)) / 4;

    // apply upper and lower limit
    scale = std::fmin(SCALE_MAX, std::fmax(SCALE_MIN, scale));

    // set the scale
    obj_.setScale(scale, scale);
}

void Particle::setColor(uint8_t r, uint8_t g, uint8_t b)
{
    obj_.setFillColor(sf::Color(r, g, b));
}

void Particle::setSpeed(float speed)
{
    this->speed = speed;
}
