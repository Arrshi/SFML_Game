#include "Bullet.h"

Bullet::Bullet(sf::Image& image, sf::String Name, Level& lvl, float X, float Y, int W, int H, float tX, float tY) :Entity(image, Name, X, Y, W, H) {//�� ��� ��, ������ ����� � ����� ��������� ������ (int dir)
	obj = lvl.GetObjects("solid");//�������������� .�������� ������ ������� ��� �������������� ���� � ������
	x = X;
	y = Y;
	targetX = tX;
	targetY = tY;
	speed = 0.2;
	w = h = 16;
	life = true;
	distance = sqrt((targetX - x) - (targetY - y));
	vx = (targetX - x) / 100; // 100 - ���������
	vy = (targetY - y) / 100;
	//���� ������������� � ������������
}

void Bullet::update(float time)
{

	//x += speed*time*(targetX - x) /20;//���� �������� ���� �� �
	//y += speed*time*(targetY - y) /20;//�� �

	x += vx * time * speed;
	y += vy * time * speed;

	if (x <= 0) x = 1;// �������� ���� � ����� �����, ����� ��� ���������� ������ ��� �������� �� �������� �� ������ ����� � �� ���� ������
	if (y <= 0) y = 1;

	for (int i = 0; i < obj.size(); i++) {//������ �� �������� solid
		if (getRect().intersects(obj[i].rect)) //���� ���� ������ ���������� � �����,
		{
			life = false;// �� ���� �������
		}
	}

	sprite.setPosition(x + w / 2, y + h / 2);//�������� ������� ����
}

