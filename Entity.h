#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "map.h"
////////////////////////////////////ќбщий класс-родитель//////////////////////////
class Entity {
public:
	std::vector<Object> obj;
	float dx, dy, x, y, speed, moveTimer;
	int w, h, health;
	bool life, isMove, onGround;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::String name;
	Entity(sf::Image& image, sf::String Name, float X, float Y, int W, int H) ;

	sf::FloatRect getRect()const; 

	virtual void update(float time) = 0;//все потомки переопредел€ют эту ф-цию

};

