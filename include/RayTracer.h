//
// Created by Patataa on 11/11/2015.
//

#ifndef SPAGHETTI_RAYTRACER_H
#define SPAGHETTI_RAYTRACER_H

#include "Type_Declarator.h"

#include "Ray.h"
#include "Map.h"

class RayTracer {

    static void traceStep(glm::vec2 &dir) {

    }

public:
    void trace( Ray & ray, const Map & map, unsigned maxIter = 5){
        //Todo: improve this step

        Coord_type nextPosition;
        glm::vec2 direction(ray.getInitialDirection());
        Coord_type position = ray.getInitialPosition()   ;
        glm::ivec2 tilePos(position);
        Tiles_Type & currentTiles = map(position);

        for(unsigned i = 0; i < maxIter; ++i) {

            //Bounce with map limit
//            if(nextPosition.x < 0 || nextPosition.x >= map.getWidth())
//                position.x *= -1;
//            if(nextPosition.y < 0 || nextPosition.y >= map.getHeight())
//                position.y *= -1;

            //Check if next Position is a negative tile
            //direction = normalize(direction);

            {
                Coord_type localPos = position - glm::vec2(tilePos);
                float const left = localPos.x / direction.x;
                float const right = (1.0f - localPos.x) / direction.x;
                float const top = (1.0f - localPos.y) / direction.y;
                float const bottom = localPos.y / direction.y;

                float const &hlim = direction.x > 0.0f ? right : left;
                float const &vlim = direction.y > 0.0f ? bottom : top;

                auto prevTile = tilePos;
                auto prevPosition = position;

                if (hlim < vlim) {
                    // on sort du côté
                    position.x = direction.x > 0.0f ? std::ceil(position.x) : std::floor(position.x);
                    position.y += hlim * direction.y;
                    tilePos.x += direction.x > 0.0f ? 1 : -1;
                } else {
                    // on sort du haut/bas
                    position.y = direction.y > 0.0f ? std::ceil(position.y) : std::floor(position.y);
                    position.x += vlim * direction.x;
                    tilePos.y += direction.y > 0.0f ? 1 : -1;
                }

                // position == nouvelle pos
                ray.push_back(position);

                //TODO: be aware of the old tile to bounce only when required
                //Todo: Implement t
                Tiles_Type newTile = map(tilePos);
                if(newTile & 1) {
                    // tile solide
                    (hlim < vlim ? direction.x : direction.y) *= -1;
                    tilePos = prevTile;
                }
            }
        }
    }

};

#endif //SPAGHETTI_RAYTRACER_H
