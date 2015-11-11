//
// Created by Patataa on 11/11/2015.
//

#ifndef SPAGHETTI_MIRROR_H
#define SPAGHETTI_MIRROR_H

#include "Object.h"
#include <glm/vec2.hpp>
#include <glm/detail/func_geometric.hpp>

class Mirror: public Object {
    const float radius = 0.3f;
public:
    template<typename ... Args>
    Mirror(Args &&... params) :
            Object(std::forward<Args>(params)...) {
    }

    virtual ~Mirror(){}

    virtual bool interact(
            const Ray & ray,
            glm::vec2 & nextPosition,
            glm::ivec2 & nextTilePosition,
            glm::vec2 & nextDirection
    ) const override {
        bool flag = false;

        glm::vec2 interPosition = (nextPosition - ray.m_position) / 2.0f;
        if (glm::length(interPosition - glm::vec2(ray.m_tilePosition) - glm::vec2(0.5f)) < radius) {
            nextPosition =  glm::vec2(ray.m_tilePosition) + glm::vec2(0.5f, 0.5f);
            nextTilePosition = ray.m_tilePosition;
            nextDirection *= -1;
            flag = true;
        }
        return flag;
    }

};


#endif //SPAGHETTI_MIRROR_H
