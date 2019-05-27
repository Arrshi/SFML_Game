#include "Bullet.h"

Bullet::Bullet(sf::Image& image, sf::String Name, Level& lvl, float X, float Y, int W, int H, float tX, float tY) :Entity(image, Name, X, Y, W, H) {//всё так же, только взяли в конце состояние игрока (int dir)
	obj = lvl.GetObjects("solid");//инициализируем .получаем нужные объекты для взаимодействия пули с картой
	x = X;
	y = Y;
	targetX = tX;
	targetY = tY;
	speed = 0.2;
	w = h = 16;
	life = true;
	distance = sqrt((targetX - x) - (targetY - y));
	vx = (targetX - x) / 100; // 100 - дистанция
	vy = (targetY - y) / 100;
	//выше инициализация в конструкторе
}

void Bullet::update(float time)
{

	//x += speed*time*(targetX - x) /20;//само движение пули по х
	//y += speed*time*(targetY - y) /20;//по у

	x += vx * time * speed;
	y += vy * time * speed;

	if (x <= 0) x = 1;// задержка пули в левой стене, чтобы при проседании кадров она случайно не вылетела за предел карты и не было ошибки
	if (y <= 0) y = 1;

	for (int i = 0; i < obj.size(); i++) {//проход по объектам solid
		if (getRect().intersects(obj[i].rect)) //если этот объект столкнулся с пулей,
		{
			life = false;// то пуля умирает
		}
	}

	sprite.setPosition(x + w / 2, y + h / 2);//задается позицию пуле
}

