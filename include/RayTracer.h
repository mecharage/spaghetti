//
// Created by Patataa on 11/11/2015.
//

#ifndef SPAGHETTI_RAYTRACER_H
#define SPAGHETTI_RAYTRACER_H


#include "Ray.h"
#include "Map.h"

class RayTracer {

public:
    void trace( Ray & ray, const Map & map){
        //Todo: improve this step

        glm::ivec2 direction = ray.getInitialDirection();
        Coord_type position = ray[0];

        for(unsigned i = 0; i < 1000; ++i) {

        }
    }

};

#endif //SPAGHETTI_RAYTRACER_H
