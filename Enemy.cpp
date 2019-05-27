#include "Enemy.h"



Enemy::Enemy(sf::Image& image, sf::String Name, Level& lvl, float X, float Y, int W, int H):Entity(image, Name, X, Y, W, H) 
{
	obj = lvl.GetObjects("solid");//инициализируем.получаем нужные объекты для взаимодействия врага с картой
	if (name == "EasyEnemy") {
		sprite.setTextureRect(sf::IntRect(0, 0, w, h));
		dx = 0.1;
	}
}

void Enemy::checkCollisionWithMap(float Dx, float Dy)
{
	for (int i = 0; i < obj.size(); i++)
		if (getRect().intersects(obj[i].rect))
		{
			if (obj[i].name == "solid")//если встретили препятствие
			{
				if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
				if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
				if (Dx > 0) { x = obj[i].rect.left - w;  dx = -0.1; sprite.scale(-1, 1); }
				if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0.1; sprite.scale(-1, 1); }
			}
		}
}

void Enemy::update(float time)
{
	static bool checker = 0;
	if (name == "EasyEnemy") {
		moveTimer += time;
		if (moveTimer > 3600) //меняет направление примерно каждые 3 сек
		{
			if (checker)
			{
				this->sprite.setTextureRect(sf::IntRect(0, 0, 150, 130));
				checker = 0;
			}
			else
			{
				this->sprite.setTextureRect(sf::IntRect(0, 130, 140, 130));
				checker = 1;
			}
			
			dx *= -1; 
			moveTimer = 0; 
		}
		checkCollisionWithMap(dx, 0);
		x += dx * time;
		sprite.setPosition(x + w / 2, y + h / 2);
		if (health <= 0) { life = false; }
	}
}
