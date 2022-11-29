#include <SFML/Graphics.hpp>
#include "Pch.h"
#include "Animation.h"
#include "Canvas.h"
#include "Physics.h"
#include "Component.h"
#include "Monobehaviour.h"
#include "ParticleSystem.h"
#include "CircleCollider.h"
#include "Input.h"
#include "Mathf.h"
#include "Debug.h"
#include "Rigidbody.h"

using namespace sf;
using namespace std;

void Draw();
void KeyBoardInput();
void MouseInput();
void CollisionChecking();
void PlayerAnimState();

int windowHeight = 1200;
int windowWidth = 1200;

RenderWindow window(VideoMode(windowWidth, windowHeight), "Dragon game", Style::Close | Style::Titlebar | Style::Resize);

RectangleShape _player(Vector2f(100, 200));

float moveSpeed = 5.0f;

Texture hero;

Time _time;
float deltaTime = 0;
Clock _clock;

Animation _playerAnim;

bool idle = true;

Canvas* myCanvas = Canvas::GetInstance();
Physics* myPhysics = Physics::GetInstance(); // in the future, physics simulation should happen on it's own thread
Debug& debug = *Debug::GetInstance();

GameObject go, zoro2, g;
Rigidbody rb, rb2;

int main() // When used in declaration(string * ptr), it creates a pointer variable, when not used in declaration, it act as a dereference operator.
{	
	myCanvas->AddWindow(window); // could you somehow send it on initialize!?
	hero.loadFromFile("_sprites_heroes.png"); // unsigned int means the int can only be positive 
	Vector2u textureSize = hero.getSize(); // 9 * 8
	textureSize.x /= 9;
	textureSize.y /= 8;

	_player.setPosition(600, 600);
	_player.setOrigin(textureSize.x * (float)3.5, hero.getSize().y / 2); // check what these values give,  the character in the image are not centered in their so called box, need a better image to do this with
	_player.setTexture(&hero);
	_player.setTextureRect(IntRect(textureSize.x * 1, textureSize.y * 7, textureSize.x, textureSize.y));

	_playerAnim = Animation(&hero, Vector2u(9, 8), 0.15f); // the whole tileset is now the same framerate :/

	ParticleSystem particles(10000, Color::Blue);
	ParticleSystem particlesPlayer(10000, Color::Black); // try not and go over 100.000 particles, preferably under 50k

	particlesPlayer.SetEmitterTransform(_player);

	go = GameObject(_player, rb);

	CircleCollider cool = CircleCollider();
	cool.size = 50;
	cool.offsetPos = Vector2f(0, 0);
	go.AddComponent(cool);

	RectangleShape Supp(Vector2f(1, 1));
	Supp.setSize(Vector2f(50, 50));
	Supp.setOrigin(25, 25);
	Supp.setPosition(Vector2f(500, 500));
	Supp.setFillColor(Color::Blue);

	BoxCollider box = BoxCollider();
	box.offsetPos = Vector2f(0, 0);
	box.size = 25;

	zoro2 = GameObject(Supp, box);
	Rigidbody rbd = Rigidbody();
	zoro2.AddComponent(rbd);

	RectangleShape Supp2(Vector2f(1, 1));
	Supp2.setSize(Vector2f(50, 50));
	Supp2.setOrigin(25, 25);
	Supp2.setPosition(Vector2f(300, 300));
	Supp2.setFillColor(Color::Blue);

	BoxCollider box2 = BoxCollider();
	box2.offsetPos = Vector2f(0, 0);
	box2.size = 25;

	g = GameObject(Supp2, box2);
	g.AddComponent(rb2);

	//float sizev = 1;
	//float sizev2 = 1;

	//Vector2f v = Vector2f(2.5, 1.5);
	//Vector2f v2 = Vector2f(3.25, 2.25);

	//Vector2f moveDir = Vector2f(0.4,0); // this we will normalize
	//moveDir = Mathf::Normalize(moveDir); // 1,0
	//debug.Log(moveDir);

	//Vector2f distanceDir = Mathf::DirectionVector(Vector2f(v2.x +(sizev / 2) + (sizev2 / 2), v2.y + (sizev / 2) + (sizev2 / 2)), Vector2f(v.x + (sizev / 2) + (sizev2 / 2), v.y + (sizev / 2) + (sizev2 / 2)));
	//debug.Log(distanceDir);

	//// find allowed distance then minus actual distance
	//Vector2f allowedDistance = Vector2f(sizev / 2 + sizev2 / 2, sizev / 2 + sizev2 / 2) - distanceDir;
	//debug.Log(allowedDistance);

	//Vector2f FinalDir = Vector2f(moveDir.x * allowedDistance.x, moveDir.y * allowedDistance.y);
	//debug.Log(FinalDir);

	//Vector2f finalDestination = Vector2f(v2.x + FinalDir.x, v2.y + FinalDir.y);
	//debug.Log(finalDestination);

	//debug.Log(g);

	//debug.Log("g has this many components " + to_string(g.GetComponents().size())); 2
	//debug.Log("g's collider has this many components " + to_string(box2.gameObject->GetComponents().size())); 0, should be the same    // we have a pointer to a pointer


	// Here we simply do bound/collision overlap testing

	//model.getGlobalBounds().intersects(rect.getGlobalBounds())
	//sf::Rect<T>
	//sf::Rect< T >::Intersects(const Rect< T > &rectangle   )

	//

	window.setFramerateLimit(120); // smooth constant fps, this should also be changeable
	while (window.isOpen()) // checking window events
	{
		_time = _clock.getElapsedTime();
		_clock.restart();
		deltaTime = _time.asSeconds(); // this will get the time between frames

		Event _event;
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

		MouseInput();
		KeyBoardInput();  // first check for input

		myPhysics->PhysicsUpdate();
		myPhysics->PhysicsCollisionUpdate();

		//Vector2i mouse = Mouse::getPosition(window);
		//particles.SetEmitterVector(Vector2f(static_cast<float>(mouse.x), static_cast<float>(mouse.y))); 
		//particles.Update(_time); 

		//particlesPlayer.Update(_time); // could we not send a ref for it to use, maybe like have a global time pr frame generation, so you just need to create the object and it updates itself

		PlayerAnimState(); // then animate based on input
		CollisionChecking(); // last check for collisions
		Draw();
	}
	return 0;
}

void PlayerAnimState()
{
	if (rb.velocity.x < 0 && _player.getScale().x > 0) { // what way are we facing?
		_player.setScale(-1, _player.getScale().y);
	}
	else if (rb.velocity.x > 0 && _player.getScale().x < 0) {
		_player.setScale(1, _player.getScale().y);
	}

	if (rb.velocity != Mathf::Zero()) // we are moving
	{
		if (idle) {
			idle = false;
			_playerAnim.NextAnim();
		}
		_playerAnim.Update(7, 3, 6, deltaTime); // there was a comment about it being a bit unresponsive but I don't currently see the problem
	}
	else
	{
		if (!idle)
		{
			idle = true;
			_playerAnim.NextAnim();
		}

		_playerAnim.Update(7, 0, 3, deltaTime);
	}
	_player.setTextureRect(_playerAnim.uvRect);
}

void Shoot() // set it so you can destroy after an interval, basicly an invoke from unity or a coroutine 
{
	CircleShape* s = new CircleShape(10, 50); // dude the player origin is messed up, it like wont update with the player moving around
	s->setPosition(_player.getPosition() + Vector2f(0,100) + Vector2f(+40, 0) + (Vector2f(50, 0) * _player.getScale().x));
	s->setOrigin(_player.getOrigin());          
	s->setFillColor(Color::Blue); // + is down, lol, dude it confuses me every time, have you guys not had math?

	Rigidbody* r = new Rigidbody();
	GameObject g = GameObject(*s, *r);
	r->velocity = Vector2f(_player.getScale().x * 20, 0);
}

void MouseInput() // gameObject should have a destroy method you can call
{
	if (Mouse::isButtonPressed(Mouse::Left))
	{
		Vector2i mousePos = Mouse::getPosition(window);
		//zoro2.transform->setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
		_player.setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)); // can also be put outside the if statement for constant follow of mouse, love it
	}
}

void KeyBoardInput()
{
	rb.velocity = Mathf::Zero();
	if (Input::GetKey(Keyboard::Escape, Input::KeyDown)) window.close();

	if (Input::GetKey(Keyboard::Space, Input::KeyDown)) Shoot();

	if (Input::GetKey(Keyboard::Left, Input::KeyHeld) || Input::GetKey(Keyboard::A, Input::KeyHeld))
	{
		rb.velocity.x = -1;
	}
	if (Input::GetKey(Keyboard::Right, Input::KeyHeld) || Input::GetKey(Keyboard::D, Input::KeyHeld))
	{
		rb.velocity.x = 1;
	}
	if (Input::GetKey(Keyboard::Up, Input::KeyHeld) || Input::GetKey(Keyboard::W, Input::KeyHeld))
	{
		rb.velocity.y = -1;
	}
	if (Input::GetKey(Keyboard::Down, Input::KeyHeld) || Input::GetKey(Keyboard::S, Input::KeyHeld))
	{
		rb.velocity.y = 1;
	}

	//rb2.velocity = Mathf::Normalize(velocity) * moveSpeed;
	rb.velocity = Mathf::Normalize(rb.velocity) * moveSpeed;
}

void CollisionChecking() // this should be moved else where
{
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

void Draw() // window clear vs background should be changeable 
{
	window.clear(Color(255, 204, 92)); // could be equal to camera solid backround, how would you lerp between colors over time? it's almost like you need an update or coroutine loop^^
	myCanvas->DrawCanvas();
	window.display();
}