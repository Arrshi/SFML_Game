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
	changeLevel(lvl, numberLevel);//для загрузки карты для нужного уровня

	sf::SoundBuffer shootBuffer;//создаём буфер для звука
	shootBuffer.loadFromFile("shoot.ogg");//загружаем в него звук
	sf::Sound shoot(shootBuffer);//создаем звук и загружаем в него звук из буфера

	sf::Music music;//создаем объект музыки
	music.openFromFile("music.ogg");//загружаем файл
	music.play();//воспроизводим музыку
	music.setLoop(true);


	sf::Image heroImage;
	heroImage.loadFromFile("images/$YakuzaBoss.png");

	sf::Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/enemy_knight_2.png");
	easyEnemyImage.createMaskFromColor(sf::Color(255, 0, 0));

	sf::Image movePlatformImage;
	movePlatformImage.loadFromFile("images/MovingPlatform.png");

	sf::Image BulletImage;//изображение для пули
	BulletImage.loadFromFile("images/bullet.png");//загрузили картинку в объект изображения
	BulletImage.createMaskFromColor(sf::Color(0, 0, 0));//маска для пули по черному цвету

	std::list<Entity*>  entities;
	std::list<Entity*>::iterator it;
	std::list<Entity*>::iterator it2;//второй итератор.для взаимодействия между объектами списка


	std::vector<Object> e = lvl.GetObjects("EasyEnemy");

	for (int i = 0; i < e.size(); i++) entities.push_back(new Enemy(easyEnemyImage, "EasyEnemy", lvl, e[i].rect.left, e[i].rect.top, 150, 130));

	Object player = lvl.GetObject("player");

	Player p(heroImage, "Player1", lvl, player.rect.left, player.rect.top, 58, 66,view);

	e = lvl.GetObjects("MovingPlatform");//забираем все платформы в вектор 

	for (int i = 0; i < e.size(); i++) entities.push_back(new MovingPlatform(movePlatformImage, "MovingPlatform", lvl, e[i].rect.left, e[i].rect.top, 95, 22));//закидываем платформу в список.передаем изображение имя уровень координаты появления (взяли из tmx карты), а так же размеры
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
			if (event.type == sf::Event::MouseButtonPressed)//если нажата клавиша мыши
				if (event.key.code == sf::Mouse::Left) //а именно левая, то стреляем 
				{ //и тут же снимаем координаты мыши в этом месте
					sf::Vector2i pixelPos = sf::Mouse::getPosition(window);//забираем коорд курсора
					sf::Vector2f pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)
					entities.push_back(new Bullet(BulletImage, "Bullet", lvl, p.x, p.y, 16, 16, pos.x, pos.y)); //и передаем в конструктор пули. создается пуля
					shoot.play();//играем звук пули
				}
		}

		for (it = entities.begin(); it != entities.end();)//говорим что проходимся от начала до конца
		{
			Entity* b = *it;//для удобства, чтобы не писать (*it)->
			b->update(time);//вызываем ф-цию update для всех объектов (по сути для тех, кто жив)
			if (b->life == false) { it = entities.erase(it); delete b; }// если этот объект мертв, то удаляем его
			else it++;//и идем курсором (итератором) к след объекту. так делаем со всеми объектами списка
		}

		for (it = entities.begin(); it != entities.end(); it++)//проходимся по эл-там списка
		{
			if (((*it)->name == "MovingPlatform") && ((*it)->getRect().intersects(p.getRect())))//если игрок столкнулся с объектом списка и имя этого объекта movingplatform
			{
				Entity* movPlat = *it;
				if ((p.dy > 0) || (p.onGround == false))//при этом игрок находится в состоянии после прыжка, т.е падает вниз
					if (p.y + p.h < movPlat->y + movPlat->h)//если игрок находится выше платформы, т.е это его ноги минимум (тк мы уже проверяли что он столкнулся с платформой)
					{
						p.y = movPlat->y - p.h + 3; p.x += movPlat->dx * time; p.dy = 0; p.onGround = true; // то выталкиваем игрока так, чтобы он как бы стоял на платформе
					}
			}

			if (((*it)->name == "EasyEnemy") && ((*it)->getRect().intersects(p.getRect())))
			{
				////////выталкивание врага
				if ((*it)->dx > 0)//если враг идет вправо
				{
					std::cout << "(*it)->x" << (*it)->x << "\n";//коорд игрока
					std::cout << "p.x" << p.x << "\n\n";//коорд врага

					(*it)->x = p.x - (*it)->w; //отталкиваем его от игрока влево (впритык)
					(*it)->dx = 0;//останавливаем

					std::cout << "new (*it)->x" << (*it)->x << "\n";//новая коорд врага
					std::cout << "new p.x" << p.x << "\n\n";//новая коорд игрока (останется прежней)
				}
				if ((*it)->dx < 0)//если враг идет влево
				{
					(*it)->x = p.x + p.w; //аналогично - отталкиваем вправо
					(*it)->dx = 0;//останавливаем
				}
				///////выталкивание игрока
				if (p.dx < 0) { p.x = (*it)->x + (*it)->w; }//если столкнулись с врагом и игрок идет влево то выталкиваем игрока
				if (p.dx > 0) { p.x = (*it)->x - p.w; }//если столкнулись с врагом и игрок идет вправо то выталкиваем игрока		
			}

			for (it2 = entities.begin(); it2 != entities.end(); it2++)
			{
				if ((*it)->getRect() != (*it2)->getRect())//при этом это должны быть разные прямоугольники
					if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "EasyEnemy") && ((*it2)->name == "EasyEnemy"))//если столкнулись два объекта и они враги
					{
						(*it)->dx *= -1;//меняем направление движения врага
						(*it)->sprite.scale(-1, 1);//отражаем спрайт по горизонтали
					}
			}

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) { lvl.levelNumber++; return true; }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) { return true; }//если таб, то перезагружаем игру
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { return false; }//если эскейп, то выходим из игры



		p.update(time);//перенесли сюда update игрока
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

void gameRunning(sf::RenderWindow& window, int& numberLevel) {//ф-ция перезагружает игру , если это необходимо
	if (startGame(window, numberLevel)) {
		if (numberLevel<3)
		{
			numberLevel++;
		}
		gameRunning(window, numberLevel);
	}//принимает с какого уровня начать игру
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "Kyrsach Cvetkova");
	int numberLevel = 1;//сначала 1-ый уровень
	gameRunning(window, numberLevel);
	return 0;
}