#include <SFML/Graphics.hpp>
#include "Pch.h"
#include "Animation.h"
#include "Canvas.h"
#include "Physics.h"
#include "Component.h"
#include "Monobehaviour.h"
#include "ParticleSystem.h"
#include "Collider.h"
#include "CircleCollider.h"
#include "Input.h"
#include "Mathf.h"
#include "Debug.h"
#include "GameObject.h"
#include "Rigidbody.h"


using namespace sf;
using namespace std;

void Draw();
void KeyBoardInput();
void MouseInput();
void CollisionChecking();
void PlayerAnimState();
void Shoot();

int windowHeight = 1200;
int windowWidth = 1200;

RenderWindow window(VideoMode(windowWidth, windowHeight), "Dragon game", Style::Close | Style::Titlebar | Style::Resize);

RectangleShape _player(Vector2f(100, 200));

float moveSpeed = 5.0f;
float velocityX = 0;
float velocityY = 0;

int frame = 0;

Texture hero;
Vector2u textureSize;

float deltaTime = 0;
Clock _clock;

Animation _playerIdle;

bool idle = true;

vector<CircleShape> proj = vector<CircleShape>();

Canvas* myCanvas = Canvas::GetInstance("MyFirstCanvas");
Physics* myPhysics = Physics::GetInstance("MyFirstPhysicsSystem");
Debug* debug = Debug::GetInstance("MyFirstDebugLog");

// first you move then check collision and based on collision you will change the position on rigidbodies                        
// in the future, physics simulation should happen on it's own thread

GameObject go, zoro2, g;
Rigidbody rb, rb2;

int main()
{
	// When used in declaration(string * ptr), it creates a pointer variable.
	// When not used in declaration, it act as a dereference operator.
	debug->Log(myCanvas->value() + "\n" + myPhysics->value());

	hero.loadFromFile("_sprites_heroes.png");
	textureSize = hero.getSize(); // 9 * 8
	textureSize.x /= 9;
	textureSize.y /= 8;

	_player.setPosition(600, 600);
	_player.setOrigin((double)textureSize.x * 3.5, (double)hero.getSize().y / 2); // check what these values give,  the character in the image are not centered in their so called box, need a better image to do this with
	_player.setTexture(&hero);
	_player.setTextureRect(IntRect(textureSize.x * 1, textureSize.y * 7, textureSize.x, textureSize.y));

	_playerIdle = Animation(&hero, Vector2u(9, 8), 0.15f); // the whole tileset is now the same framerate :/

	ParticleSystem particles(10000, Color::Black);
	ParticleSystem particlesPlayer(10000, Color::Black); // try not and go over 100.000 particles, preferably under 50k

	go = Monobehaviour::Instantiate(GameObject(_player, rb)); 

	CircleCollider cool = CircleCollider();
	cool.size = 50;
	cool.offsetPos = Vector2f(0, 0);
	go.AddComponent(cool);

	RectangleShape Supp(Vector2f(1, 1));
	Supp.setSize(Vector2f(50,50));
	Supp.setOrigin(25,25);
	Supp.setPosition(Vector2f(500,500));
	Supp.setFillColor(Color::Blue);

	//CircleCollider circle = CircleCollider();
	//circle.offsetPos = Vector2f(0, 0);             
	//circle.size = 25;     

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

	debug->Log("g has this many components" + to_string(g.GetComponents().size()));
	debug->Log("g's collider has this many components" + to_string(box2.gameObject->GetComponents().size()));
	
	window.setFramerateLimit(120); // smooth constant fps
	while (window.isOpen()) // checking window events
	{
		Time t = _clock.getElapsedTime();
		_clock.restart();
		deltaTime = t.asSeconds(); // this will get the time between frames

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

		myPhysics->PhysicsUpdate(); //* is a pointer, & is a reference,    how would you have a physics loop that loops at a fixed time?
		myPhysics->PhysicsCollisionUpdate();

		//Vector2i mouse = Mouse::getPosition(window);
		//particles.setEmitter(Vector2f(static_cast<float>(mouse.x), static_cast<float>(mouse.y))); // would like for it to update itself on it's own, once instantiated
		//particles.update(t); // would really just like to set the speed, my dude!!

		//particlesPlayer.setEmitter(_player.getPosition()); // player origin doesn't move with the player?
		//particlesPlayer.update(t);

		PlayerAnimState(); // then animate based on input
		CollisionChecking(); // last check for collisions
		Draw();
	}
	return 0;
}

void PlayerAnimState() // would like to set the animation from the outside, this func will just call the animation
{
	if (velocityX < 0 && _player.getScale().x > 0) { // what way are we facing?
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

		_playerIdle.Update(7, 0, 3, deltaTime);
	}
	_player.setTextureRect(_playerIdle.uvRect);
}

void Yomama() 
{
	cout << "shoot";
}

void Shoot() // set it so you can destroy after an interval, basicly an invoke from unity or a coroutine 
{
	CircleShape s = CircleShape(5, 50);
	s.setPosition(_player.getPosition()); // really can't seem to initialize gameobjects without them having acces violation the same frame, even with the use of threads
	s.setOrigin(_player.getOrigin());
	s.setFillColor(Color::Blue);

	thread t(Yomama);
	t.join();

	proj.push_back(s);

	if (proj.size() == 125) { // do something with particles and noise
		proj.erase(proj.begin());
	}
}

void MouseInput()
{
	if (Mouse::isButtonPressed(Mouse::Left))
	{
		Vector2i mousePos = Mouse::getPosition(window);
		zoro2.transform->setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
		//_player.setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)); // can also be put outside the if statement for constant follow of mouse, love it
	}
}

void KeyBoardInput()
{
	if (Input::GetKey(Keyboard::Escape, Input::KeyDown)) window.close();

	if (Input::GetKey(Keyboard::Space, Input::KeyDown)) Shoot();

	if (Input::GetKey(Keyboard::Left, Input::KeyHeld) || Input::GetKey(Keyboard::A, Input::KeyHeld))
	{
		velocityX = -1;
	}
	if (Input::GetKey(Keyboard::Right, Input::KeyHeld) || Input::GetKey(Keyboard::D, Input::KeyHeld))
	{
		velocityX = 1;
	}
	if (Input::GetKey(Keyboard::Up, Input::KeyHeld) || Input::GetKey(Keyboard::W, Input::KeyHeld)) 
	{
		velocityY = -1;
	}
	if (Input::GetKey(Keyboard::Down, Input::KeyHeld) || Input::GetKey(Keyboard::S, Input::KeyHeld))
	{
		velocityY = 1;
	}

	Vector2f v = Vector2f(velocityX, velocityY);

	rb2.velocity = Mathf::Normalize(v) * moveSpeed;
	//rb.velocity = Mathf::Normalize(Vector2f(velocityX, velocityY)) * moveSpeed; // why the helll can i multiply a vector here!?
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

void Draw()
{
	window.clear(Color(255, 204, 92)); // could be equal to camera solid backround, how would you lerp between colors over time? it's almost like you need an update or coroutine loop^^

	myCanvas->DrawCanvas(window);

	for (size_t i = 0; i < proj.size(); i++) // can't safe drawables, instantiated at runtime too canvas and render them, can draw them here though
	{
		window.draw(proj[i]);
	}

	window.display();
}