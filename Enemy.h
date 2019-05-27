#pragma once
#include "Entity.h"
#include "map.h"
class Enemy :public Entity 
{
public:

	Enemy(sf::Image& , sf::String , Level& , float , float , int , int );
	void checkCollisionWithMap(float, float );
	void update(float);
};
