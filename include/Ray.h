//
// Created by Patataa on 11/11/2015.
//

#ifndef SPAGHETTI_RAY_H
#define SPAGHETTI_RAY_H

#include <vector>
#include <glm/vec2.hpp>
#include "Type_Declarator.h"


class Ray : public Ray_Type {

public:

    glm::vec2 m_position;
    glm::vec2 m_direction;
    glm::ivec2 m_tilePosition;

	template<typename ... Args>
	Ray(
            const glm::vec2& position,
            const glm::vec2& direction,
            const glm::ivec2& tilePosition,
            Args &&... params)
		:
		Ray_Type(std::forward<Args>(params)...),
        m_position(position),
        m_direction(direction),
        m_tilePosition(tilePosition)
    {
		push_back(m_position);
	}
};

#endif //SPAGHETTI_RAY_H
