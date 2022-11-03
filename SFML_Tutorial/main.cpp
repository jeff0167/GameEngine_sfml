#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <cmath>
#include <fstream> 
#include "Animation.h"
#include "Monobehaviour.h"
#include "GameObject.h"
#include <iterator>
#include <list>

using namespace sf;
using namespace std;

void Draw();
void KeyBoardInput();
void MouseInput();
void CollisionChecking();
void PlayerAnimState();
void Shoot();
CircleShape GetProjectile();

Vector2f normalize(const Vector2f& source)
{
	float length = sqrt((source.x * source.x) + (source.y * source.y));

	if (source.x != 0 && source.y != 0) {
		return Vector2f(source.x / length, source.y / length);
	}
	else
		return source;
}

int windowHeight = 1200;
int windowWidth = 1200;

RenderWindow window(VideoMode(windowWidth, windowHeight), "Pog champ", Style::Close | Style::Titlebar | Style::Resize);
CircleShape shape(100);
Vector2f windowPos = Vector2f(400, 400);

RectangleShape _player(Vector2f(100, 200));

float moveSpeed = 0.075f;
float velocityX = 0;
float velocityY = 0;

int frame = 0;
int timer = 0;

Texture hero;
Vector2u textureSize;

float deltaTime = 0;
Clock _clock;

Animation _playerIdle;

CircleShape origin(5, 10);

bool idle = true;

CircleShape projectiles[1];

list<CircleShape> proj;

int main() // cant inherit monobehaviour? well this is just a function, not a class
{
	// When used in declaration(string * ptr), it creates a pointer variable.
	// When not used in declaration, it act as a dereference operator.

	hero.loadFromFile("_sprites_heroes.png");

	textureSize = hero.getSize(); // 9 * 8
	textureSize.x /= 9;
	textureSize.y /= 8;

	_player.setPosition(60, 60);
	_player.setOrigin(textureSize.x * 3.5, hero.getSize().y / 2); //I do understand the origin, though for this instance I CHOOSE to set the origin to *3.5 as it gives the best result
	_player.setTexture(&hero);
	_player.setTextureRect(IntRect(textureSize.x * 1, textureSize.y * 7, textureSize.x, textureSize.y));

	_playerIdle = Animation(&hero, Vector2u(9, 8), 0.15f); // the whole tileset is now the same framerate :/

	proj = list<CircleShape>();

	for (size_t i = 0; i < 10; i++) // to get the size of an array you need the byte size and the byte type sizeof(myNumbers) / sizeof(int)   use foreach where possible
	{
		proj.push_front(CircleShape(10,10)); // use struct for simple data containers, NOT classes
	}

	for (size_t i = 0; i < 1; i++)
	{
		projectiles[i] = CircleShape(10, 10);
	}

	while (window.isOpen()) // checking window events
	{
		deltaTime = _clock.restart().asSeconds(); // this will get the time between frames

		sf::Event _event;
		while (window.pollEvent(_event))
		{
			switch (_event.type)
			{
			case Event::Closed:
				window.close();
				break;
			case Event::Resized:
				printf("New window width: %i New window heigth: %i \n", _event.size.width, _event.size.height);
			case Event::TextEntered:
				if (_event.text.unicode < 128) {
					printf("%c", _event.text.unicode);
				}
				break;
			}
		}

		KeyBoardInput();  // first check for input
		MouseInput();

		Monobehaviour::Instantiate(GameObject()); // yo what up! Cool

		PlayerAnimState(); // then animate based on input

		CollisionChecking(); // last check for collisions

		Draw();
	}
	return 0;
}

void PlayerAnimState() // would like to set the animation from the outside, this func will just call the animation
{
	if (velocityX < 0 && _player.getScale().x > 0) {
		_player.setScale(-1, _player.getScale().y);
	}
	else if (velocityX > 0 && _player.getScale().x < 0) {
		_player.setScale(1, _player.getScale().y);
	}

	if (velocityX != 0 || velocityY != 0) // we are moving
	{
		if (idle) {
			idle = false;
			_playerIdle.NextAnim();

		}
		_playerIdle.Update(7, 3, 6, deltaTime);
	}
	else // this way it still waits for the next frame before it will switch to the next animation, which makes it a little unresponsive
	{
		if (!idle)
		{
			idle = true;
			_playerIdle.NextAnim();
		}

		_playerIdle.Update(7, 0, 3, deltaTime); // we are standing still
	}
	_player.setTextureRect(_playerIdle.uvRect);
}

CircleShape GetProjectile()
{
	for (CircleShape var : proj)
	{
		if (var.getPosition() == _player.getPosition()) return var;
	}
}

void Shoot()
{
	projectiles[0].setPosition(_player.getPosition());
}

void MouseInput()
{
	if (Mouse::isButtonPressed(Mouse::Left))
	{
		Vector2i mousePos = Mouse::getPosition(window);
		_player.setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)); // can also be put outside the if statement for constant follow of mouse, love it
	}
}

void KeyBoardInput()
{
	if (Keyboard::isKeyPressed(Keyboard::Escape))
	{
		window.close();
	}

	if (Keyboard::isKeyPressed(Keyboard::Space))
	{
		Shoot();
	}

	if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
	{
		velocityX = -1;
	}
	if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
	{
		velocityX = 1;
	}
	if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) // this is wrong, if y goes down we should drop, like in a graph
	{
		velocityY = -1;
	}
	if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))
	{
		velocityY = 1;
	}

	_player.move(normalize(Vector2f(velocityX, velocityY)) * moveSpeed);
	//projectiles[0].setPosition(projectiles->getPosition().x + 0.1f, projectiles->getPosition().y);
}

void CollisionChecking() // now this is a hard part, here we are just doing it for one object
{
	velocityX = velocityY = 0;
	if (window.getSize().x < _player.getPosition().x + _player.getLocalBounds().width / 2) {
		_player.setPosition(window.getSize().x - _player.getLocalBounds().width / 2, _player.getPosition().y); // checking for window x
	}
	if (0 > _player.getPosition().x - _player.getLocalBounds().width / 2) {
		_player.setPosition(0 + _player.getLocalBounds().width / 2, _player.getPosition().y);
	}

	if (window.getSize().y < _player.getPosition().y + _player.getLocalBounds().height / 8) { // beware the 8
		_player.setPosition(_player.getPosition().x, window.getSize().y - _player.getLocalBounds().height / 8); // checking for window y
	}
	if (0 > _player.getPosition().y - _player.getLocalBounds().height / 2) {
		_player.setPosition(_player.getPosition().x, 0 + _player.getLocalBounds().height / 2);
	}
}

void Draw() // if you make an object, it has to be drawn, have to have a list of objects in the scene
{
	window.clear(Color(255, 204, 92)); // could be equal to camera solid backround
	window.draw(_player);
	window.draw(origin);

	window.draw(projectiles[0]);

	window.display();
}