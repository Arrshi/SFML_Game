#include "MovingPlatform.h"




MovingPlatform::MovingPlatform(sf::Image& image, sf::String Name, Level& lvl, float X, float Y, int W, int H):Entity(image, Name, X, Y, W, H)
{
		sprite.setTextureRect(sf::IntRect(0, 0, W, H));//������������� 
		dx = 0.08;//����������� ��������� �� �
}

void MovingPlatform::update(float time)
{
	x += dx * time;//���������� �������� �� �����������
	moveTimer += time;//���������� ������
	if (moveTimer > 2000) { dx *= -1; moveTimer = 0; }//���� ������ �������� 2 ���, �� �������� ����������� �������� ���������, � ������ ����������
	sprite.setPosition(x + w / 2, y + h / 2);//������ ������� �������
}
