#pragma once
#include "Entity.h"
#include "map.h"

//////////////////////////////////////////////////// À¿—— »√–Œ ¿////////////////////////
class Player :public Entity
{
	sf::View* view;
	float CurrentFrame = 0;
public:
	enum { left, right, up, down, jump, stay, right_Top } state;
	int playerScore;
	bool isShoot;

	Player(sf::Image& image, sf::String Name, Level& lev, float X, float Y, int W, int H,sf::View*);
	void setPlayerCoordinateForView(float x, float y, sf::View* view);
	void control(float time);
	void checkCollisionWithMap(float , float);
	void update(float time);
};

