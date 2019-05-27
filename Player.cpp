#include "Player.h"



Player::Player(sf::Image& image, sf::String Name, Level& lev, float X, float Y, int W, int H,sf::View* _view) :Entity(image, Name, X, Y, W, H)
{
	view = _view;
	playerScore = isShoot = 0; state = stay; obj = lev.GetAllObjects();
	if (name == "Player1") {
		sprite.setTextureRect(sf::IntRect(0, 0, w, h));
	}
}

void Player::setPlayerCoordinateForView(float x, float y, sf::View* view)
{
	float tempX = x; float tempY = y;

	//if (x < 320) tempX = 320;
	//if (y < 240) tempY = 240;//верхнюю сторону
	if (y > 624) tempY = 624;//нижнюю стороню.для новой карты
	view->setCenter(tempX, tempY);
}

void Player::control(float time)
{
	if (sf::Keyboard::isKeyPressed) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			state = left; speed = 0.1;
			CurrentFrame += 0.005 * time;
			if (CurrentFrame > 3) CurrentFrame -= 3;

			this->sprite.setTextureRect(sf::IntRect(66 * int(CurrentFrame), 66, 58, 66));

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			state = right; speed = 0.1;
			CurrentFrame += 0.005 * time;
			if (CurrentFrame > 3) CurrentFrame -= 3;
			this->sprite.setTextureRect(sf::IntRect(66 * int(CurrentFrame), 130, 58, 66));

		}

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && (onGround)) {
			state = jump; dy = -0.6; onGround = false;

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			state = down;
		}

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) && (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))) {
			state = right_Top;
			CurrentFrame += 0.005 * time;
			if (CurrentFrame > 3) CurrentFrame -= 3;

		}

		/////выстрел
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			isShoot = true;
		}
	}
}

void Player::checkCollisionWithMap(float Dx, float Dy)
{
	for (int i = 0; i < obj.size(); i++)
		if (getRect().intersects(obj[i].rect))
		{
			if (obj[i].name == "solid")
			{
				if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
				if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
				if (Dx > 0) { x = obj[i].rect.left - w; }
				if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; }
			}
		}
}

void Player::update(float time)
{
	{
		control(time);
		switch (state)
		{
		case right:dx = speed; break;
		case left:dx = -speed; break;
		case up: break;
		case down: dx = 0; break;
		case stay: break;
		case right_Top: dx = speed; break;//состояние вправо вверх, просто продолжаем идти вправо
		}
		x += dx * time;
		checkCollisionWithMap(dx, 0);
		y += dy * time;
		checkCollisionWithMap(0, dy);
		sprite.setPosition(x + w / 2, y + h / 2);
		if (health <= 0) { life = false; }
		if (!isMove) { speed = 0; }
		if (life) { setPlayerCoordinateForView(x, y,this->view); }
		dy = dy + 0.0015 * time;
	}
}


