//
// Created by Patataa on 11/11/2015.
//

#ifndef SPAGHETTI_PRISM_H
#define SPAGHETTI_PRISM_H

#include "Object.h"

class Prism : public Object {

public:
    template<typename ... Args>
    Prism(Args &&... params) :
            Object(std::forward<Args>(params)...) {
    }

    virtual bool interact(
            const Ray & ray,
            glm::vec2 & nextPosition,
            glm::ivec2 & nextTilePosition,
            glm::vec2 & nextDirection
    ) const = 0 ;

};


#endif //SPAGHETTI_PRISM_H
