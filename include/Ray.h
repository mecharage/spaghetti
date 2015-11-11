//
// Created by Patataa on 11/11/2015.
//

#ifndef SPAGHETTI_RAY_H
#define SPAGHETTI_RAY_H

#include <vector>
#include <glm/vec2.hpp>
#include "Type_Declarator.h"


class Ray : public Ray_Type{

private:
    Coord_type m_initialPosition;
    glm::vec2 m_initialDirection;
public:
    template <typename ... Args>
    Ray(const glm::vec2 & initialPosition, const glm::ivec2 & initialDirection, Args && ... params):
            m_initialPosition(initialPosition),
            m_initialDirection(initialDirection),
            Ray_Type(std::forward<Args>(params)...){}

    const glm::vec2 &getInitialDirection() const {
        return m_initialDirection;
    }

    const Coord_type &getInitialPosition() const {
        return  m_initialPosition;
    }





};

#endif //SPAGHETTI_RAY_H
