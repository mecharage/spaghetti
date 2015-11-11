//
// Created by Patataa on 11/11/2015.
//

#ifndef SPAGHETTI_RAYTRACER_H
#define SPAGHETTI_RAYTRACER_H

#include "Type_Declarator.h"

#include "Ray.h"
#include "Map.h"

class RayTracer {
public:

	static Ray trace(
            const Map &map,
            const glm::vec2 &startingPosition,
            const glm::vec2 &startingDirection,
            unsigned iterMax = 20
    ) {
		//Todo: improve this step


        Ray ray(
                startingPosition,
                startingDirection,
                glm::ivec2(startingPosition)
        );


		Tile_Type currentTileColor = map(ray.m_tilePosition);
        Object * lastObject = nullptr;

		for (unsigned i = 0; i < iterMax; ++i) {

            glm::vec2 inTilePosition = ray.m_position - glm::vec2(ray.m_tilePosition);

			float const left = -inTilePosition.x / ray.m_direction.x;
			float const right = (1.0f - inTilePosition.x) / ray.m_direction.x;
			float const top = -inTilePosition.y / ray.m_direction.y;
			float const bottom = (1.0f - inTilePosition.y) / ray.m_direction.y;

			float const hlim = ray.m_direction.x > 0.0f ? right : left;
			float const vlim = ray.m_direction.y > 0.0f ? bottom : top;

            glm::vec2 nextPosition(ray.m_position);
            glm::vec2 nextDirection(ray.m_direction);
            glm::ivec2 nextTilePosition(ray.m_tilePosition);
			if (hlim < vlim) {
				// on sort du côté
                nextPosition += hlim * ray.m_direction;
				nextTilePosition.x += ray.m_direction.x > 0.0f ? 1 : -1;
			} else {
				// on sort du haut/bas
                nextPosition += vlim * ray.m_direction;
				nextTilePosition.y += ray.m_direction.y > 0.0f ? 1 : -1;
			}


            //Add Object Interaction
            for(const auto & object : map.getObjects()) {
                if(object.get() != lastObject && object->interact( ray, nextPosition, nextTilePosition, nextDirection))
                    lastObject = object.get();
            }

            // position == nouvelle pos
            //Detection du bord de la map
            if (nextTilePosition.x < 0 || nextTilePosition.y < 0 || nextTilePosition.x >= map.getWidth() || nextTilePosition.y >= map.getHeight())
                break;


            Tile_Type newTile = map(nextTilePosition);


            //Default Action
			if (newTile != map(ray.m_tilePosition)) {
				// tile solide
				(hlim < vlim ? ray.m_direction.x : ray.m_direction.y) *= -1;
				ray.push_back(nextPosition);
			}
            ray.m_direction = nextDirection;
            ray.m_position = nextPosition;
            ray.m_tilePosition = nextTilePosition;

		}
        return ray;
	}

};

#endif //SPAGHETTI_RAYTRACER_H
