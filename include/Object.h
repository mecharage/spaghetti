//
// Created by Patataa on 11/11/2015.
//

#ifndef SPAGHETTI_OBJECT_H
#define SPAGHETTI_OBJECT_H

#include <Type_Declarator.h>
#include <Ray.h>

class Object{
    Coord_type m_position;
    glm::vec2 m_direction;

public:
    Object(const Coord_type & position, const glm::vec2 & direction) :
            m_position(position),
            m_direction(direction)
    {
    }

    virtual ~Object()=0;

    virtual glm::vec2 interact(
            Ray & ray,

            const glm::vec2 & oldDirection,
            const Coord_type & oldPosition,
            const Coord_type & newPosition,

            const Tile_Type & oldTile,
            const Tile_Type & newTile,

            float vlim,
            float hlim
    ) const {

        glm::vec2 direction(oldDirection);
        if (newTile != oldTile) {
            (hlim < vlim ? direction.x : direction.y) *= -1;

            ray.push_back(newPosition);


        }


        return direction;

    };

    const  Coord_type &getPosition()const {
        return m_position;
    }

    const glm::vec2 &getDirection()const {
        return m_direction;
    }

    void setPosition(const Coord_type & position) {
        m_position = position;
    }

    void setDirection(const glm::vec2 & direction) {
        m_direction = direction;
    }

};

#endif //SPAGHETTI_OBJECT_H
