#pragma once
#include "Entity.h"
#include "map.h"
class MovingPlatform : public Entity {//����� ���������� ���������
public:
	MovingPlatform(sf::Image& , sf::String , Level& , float, float, int , int );
	void update(float);//������� ���������� ���������.
};

