//
// Created by Patataa on 11/11/2015.
//

#ifndef SPAGHETTI_LENS_H
#define SPAGHETTI_LENS_H

#include <Object.h>

class Lens : public Object {


public:
    template <typename ... Args>
       Lens(Args &&... params):
               Object(std::forward<Args>(params)...) {
    }

    glm::vec2 interact(
            Ray & ray,

            const glm::vec2 & oldDirection,
            const Coord_type & oldPosition,
            const Coord_type & newPosition,

            const Tile_Type & oldTile,
            const Tile_Type & newTile,

            float vlim,
            float hlim
    ) const override {

        glm::vec2 direction(oldDirection);
        if (newTile != oldTile) {
            (hlim < vlim ? direction.x : direction.y) *= -1;

            ray.push_back(newPosition);


        }


        return direction;

    };


};

#endif //SPAGHETTI_LENS_H
