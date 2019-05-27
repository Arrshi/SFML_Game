#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "mission.h"
#include "iostream"
#include "map.h"
#include <vector>
#include <list>
#include "Entity.h"
#include"Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "MovingPlatform.h"



void changeLevel(Level& lvl, int& numberLevel) {
	if (numberLevel == 1) { lvl.LoadFromFile("map.tmx"); }
	if (numberLevel == 2) { lvl.LoadFromFile("map2.tmx"); }
	if (numberLevel == 3) { lvl.LoadFromFile("map3.tmx"); }
}

bool startGame(sf::RenderWindow& window, int& numberLevel) {
	sf::View* view = new sf::View;
	view->reset(sf::FloatRect(0, 0, 640, 480));

	Level lvl;
	changeLevel(lvl, numberLevel);//��� �������� ����� ��� ������� ������

	sf::SoundBuffer shootBuffer;//������ ����� ��� �����
	shootBuffer.loadFromFile("shoot.ogg");//��������� � ���� ����
	sf::Sound shoot(shootBuffer);//������� ���� � ��������� � ���� ���� �� ������

	sf::Music music;//������� ������ ������
	music.openFromFile("music.ogg");//��������� ����
	music.play();//������������� ������
	music.setLoop(true);


	sf::Image heroImage;
	heroImage.loadFromFile("images/$YakuzaBoss.png");

	sf::Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/enemy_knight_2.png");
	easyEnemyImage.createMaskFromColor(sf::Color(255, 0, 0));

	sf::Image movePlatformImage;
	movePlatformImage.loadFromFile("images/MovingPlatform.png");

	sf::Image BulletImage;//����������� ��� ����
	BulletImage.loadFromFile("images/bullet.png");//��������� �������� � ������ �����������
	BulletImage.createMaskFromColor(sf::Color(0, 0, 0));//����� ��� ���� �� ������� �����

	std::list<Entity*>  entities;
	std::list<Entity*>::iterator it;
	std::list<Entity*>::iterator it2;//������ ��������.��� �������������� ����� ��������� ������


	std::vector<Object> e = lvl.GetObjects("EasyEnemy");

	for (int i = 0; i < e.size(); i++) entities.push_back(new Enemy(easyEnemyImage, "EasyEnemy", lvl, e[i].rect.left, e[i].rect.top, 150, 130));

	Object player = lvl.GetObject("player");

	Player p(heroImage, "Player1", lvl, player.rect.left, player.rect.top, 58, 66,view);

	e = lvl.GetObjects("MovingPlatform");//�������� ��� ��������� � ������ 

	for (int i = 0; i < e.size(); i++) entities.push_back(new MovingPlatform(movePlatformImage, "MovingPlatform", lvl, e[i].rect.left, e[i].rect.top, 95, 22));//���������� ��������� � ������.�������� ����������� ��� ������� ���������� ��������� (����� �� tmx �����), � ��� �� �������
	sf::Clock clock;
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();

		clock.restart();
		time = time / 800;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed)//���� ������ ������� ����
				if (event.key.code == sf::Mouse::Left) //� ������ �����, �� �������� 
				{ //� ��� �� ������� ���������� ���� � ���� �����
					sf::Vector2i pixelPos = sf::Mouse::getPosition(window);//�������� ����� �������
					sf::Vector2f pos = window.mapPixelToCoords(pixelPos);//��������� �� � ������� (������ �� ����� ����)
					entities.push_back(new Bullet(BulletImage, "Bullet", lvl, p.x, p.y, 16, 16, pos.x, pos.y)); //� �������� � ����������� ����. ��������� ����
					shoot.play();//������ ���� ����
				}
		}

		for (it = entities.begin(); it != entities.end();)//������� ��� ���������� �� ������ �� �����
		{
			Entity* b = *it;//��� ��������, ����� �� ������ (*it)->
			b->update(time);//�������� �-��� update ��� ���� �������� (�� ���� ��� ���, ��� ���)
			if (b->life == false) { it = entities.erase(it); delete b; }// ���� ���� ������ �����, �� ������� ���
			else it++;//� ���� �������� (����������) � ���� �������. ��� ������ �� ����� ��������� ������
		}

		for (it = entities.begin(); it != entities.end(); it++)//���������� �� ��-��� ������
		{
			if (((*it)->name == "MovingPlatform") && ((*it)->getRect().intersects(p.getRect())))//���� ����� ���������� � �������� ������ � ��� ����� ������� movingplatform
			{
				Entity* movPlat = *it;
				if ((p.dy > 0) || (p.onGround == false))//��� ���� ����� ��������� � ��������� ����� ������, �.� ������ ����
					if (p.y + p.h < movPlat->y + movPlat->h)//���� ����� ��������� ���� ���������, �.� ��� ��� ���� ������� (�� �� ��� ��������� ��� �� ���������� � ����������)
					{
						p.y = movPlat->y - p.h + 3; p.x += movPlat->dx * time; p.dy = 0; p.onGround = true; // �� ����������� ������ ���, ����� �� ��� �� ����� �� ���������
					}
			}

			if (((*it)->name == "EasyEnemy") && ((*it)->getRect().intersects(p.getRect())))
			{
				////////������������ �����
				if ((*it)->dx > 0)//���� ���� ���� ������
				{
					std::cout << "(*it)->x" << (*it)->x << "\n";//����� ������
					std::cout << "p.x" << p.x << "\n\n";//����� �����

					(*it)->x = p.x - (*it)->w; //����������� ��� �� ������ ����� (�������)
					(*it)->dx = 0;//�������������

					std::cout << "new (*it)->x" << (*it)->x << "\n";//����� ����� �����
					std::cout << "new p.x" << p.x << "\n\n";//����� ����� ������ (��������� �������)
				}
				if ((*it)->dx < 0)//���� ���� ���� �����
				{
					(*it)->x = p.x + p.w; //���������� - ����������� ������
					(*it)->dx = 0;//�������������
				}
				///////������������ ������
				if (p.dx < 0) { p.x = (*it)->x + (*it)->w; }//���� ����������� � ������ � ����� ���� ����� �� ����������� ������
				if (p.dx > 0) { p.x = (*it)->x - p.w; }//���� ����������� � ������ � ����� ���� ������ �� ����������� ������		
			}

			for (it2 = entities.begin(); it2 != entities.end(); it2++)
			{
				if ((*it)->getRect() != (*it2)->getRect())//��� ���� ��� ������ ���� ������ ��������������
					if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "EasyEnemy") && ((*it2)->name == "EasyEnemy"))//���� ����������� ��� ������� � ��� �����
					{
						(*it)->dx *= -1;//������ ����������� �������� �����
						(*it)->sprite.scale(-1, 1);//�������� ������ �� �����������
					}
			}

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) { lvl.levelNumber++; return true; }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) { return true; }//���� ���, �� ������������� ����
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { return false; }//���� ������, �� ������� �� ����



		p.update(time);//��������� ���� update ������
		window.setView(*view);
		window.clear(sf::Color(77, 83, 140));
		lvl.Draw(window);


		for (it = entities.begin(); it != entities.end(); it++) {
			window.draw((*it)->sprite);
		}
		window.draw(p.sprite);
		window.display();
	}
}

void gameRunning(sf::RenderWindow& window, int& numberLevel) {//�-��� ������������� ���� , ���� ��� ����������
	if (startGame(window, numberLevel)) {
		if (numberLevel<3)
		{
			numberLevel++;
		}
		gameRunning(window, numberLevel);
	}//��������� � ������ ������ ������ ����
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "Kyrsach Cvetkova");
	int numberLevel = 1;//������� 1-�� �������
	gameRunning(window, numberLevel);
	return 0;
}