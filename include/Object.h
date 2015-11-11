//
// Created by Patataa on 11/11/2015.
//

#ifndef SPAGHETTI_OBJECT_H
#define SPAGHETTI_OBJECT_H

#include <Type_Declarator.h>

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

    virtual glm::vec2 interact(const Ray & ray, const Tiles_Types<> & oldTiles, const Tiles_Tiles<> & newTiles  )const {

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
