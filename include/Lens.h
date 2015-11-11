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

    glm::vec2 interact(const Ray & ray) const override {



        return;
    }

};

#endif //SPAGHETTI_LENS_H
