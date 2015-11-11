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
	static void trace(Ray &ray, const Map &map) {
		//Todo: improve this step

		glm::vec2 direction(ray.getInitialDirection());
		Coord_type position = ray.getInitialPosition();
		glm::ivec2 tilePos(position);
		Tiles_Type<> col = map(tilePos);

		for (unsigned i = 0; i < 20; ++i) {
			Coord_type localPos = position - glm::vec2(tilePos);
			float const left = -localPos.x / direction.x;
			float const right = (1.0f - localPos.x) / direction.x;
			float const top = -localPos.y / direction.y;
			float const bottom = (1.0f - localPos.y) / direction.y;

			float const &hlim = direction.x > 0.0f ? right : left;
			float const &vlim = direction.y > 0.0f ? bottom : top;

			auto prevTile = tilePos;

			if (hlim < vlim) {
				// on sort du côté
				position += hlim * direction;
				tilePos.x += direction.x > 0.0f ? 1 : -1;
			} else {
				// on sort du haut/bas
				position += vlim * direction;
				tilePos.y += direction.y > 0.0f ? 1 : -1;
			}

			// position == nouvelle pos

			if (tilePos.x < 0 || tilePos.y < 0 || tilePos.x >= map.getWidth() || tilePos.y >= map.getHeight())
				break;

			Tiles_Type<> newTile = map(tilePos);
			if (newTile != col) {
				// tile solide
				(hlim < vlim ? direction.x : direction.y) *= -1;
				ray.push_back(position);
				tilePos = prevTile;
			}
		}
	}

};

#endif //SPAGHETTI_RAYTRACER_H
