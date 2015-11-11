//
// Created by Patataa on 11/11/2015.
//

#ifndef SPAGHETTI_OBJECT_H
#define SPAGHETTI_OBJECT_H

#include <Type_Declarator.h>
#include <Ray.h>

class Object{
    glm::ivec2 m_position;
    glm::vec2 m_direction;

public:


    bool operator==(const glm::vec2 & position) {
        return glm::ivec2(position) == m_position;
    }

    Object(const  glm::ivec2 & position, const glm::vec2 & direction) :
            m_position(position),
            m_direction(direction)
    {
    }

    virtual ~Object(){};

    virtual bool interact(
            const Ray & ray,
            glm::vec2 & nextPosition,
            glm::ivec2 & nextTilePosition,
            glm::vec2 & nextDirection
    ) const = 0 ;

    const   glm::ivec2 &getPosition()const {
        return m_position;
    }

    const glm::vec2 &getDirection()const {
        return m_direction;
    }

    void setPosition(const Coord_type & position) {
        m_position = position;
    }

    void setDirection(const glm::ivec2 & direction) {
        m_direction = direction;
    }

};

#endif //SPAGHETTI_OBJECT_H
