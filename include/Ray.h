//
// Created by Patataa on 11/11/2015.
//

#ifndef SPAGHETTI_RAY_H
#define SPAGHETTI_RAY_H

#include <vector>
#include <glm/vec2.hpp>

typedef glm::vec2 Coord_type;
typedef std::vector<Coord_type> Ray_Type;

class Ray : public Ray_Type{

private:
    glm::ivec2 m_initialDirection;
public:
    template <typename ... Args>
    Ray(glm::ivec2 initialDirection, Args && ... params):
            m_initialDirection(initialDirection),
            Ray_Type(std::forward<Args>(params)...){}

    const glm::ivec2 &getInitialDirection() const {
        return m_initialDirection;
    }





};

#endif //SPAGHETTI_RAY_H
