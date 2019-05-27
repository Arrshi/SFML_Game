#include "Entity.h"



Entity::Entity(sf::Image& image, sf::String Name, float X, float Y, int W, int H):x(X),y(Y),w(W),h(H),name(Name),moveTimer(0)
{
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
}

sf::FloatRect Entity::getRect()const
{
		return sf::FloatRect(x, y, w, h);
}

