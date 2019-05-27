#pragma once
#include "Entity.h"
#include "map.h"
class Bullet :public Entity//класс пули
{
	float targetX, targetY;//отвечают за направление движения пули к цели
	float vx, vy, distance;
public:
	Bullet(sf::Image& , sf::String , Level& , float, float , int , int, float, float );
	void update(float);
};
