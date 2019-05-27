#pragma once
#include "Entity.h"
#include "map.h"
class MovingPlatform : public Entity {//класс движущейся платформы
public:
	MovingPlatform(sf::Image& , sf::String , Level& , float, float, int , int );
	void update(float);//функция обновления платформы.
};

