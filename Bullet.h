#pragma once
#include "Entity.h"
#include "map.h"
class Bullet :public Entity//����� ����
{
	float targetX, targetY;//�������� �� ����������� �������� ���� � ����
	float vx, vy, distance;
public:
	Bullet(sf::Image& , sf::String , Level& , float, float , int , int, float, float );
	void update(float);
};
