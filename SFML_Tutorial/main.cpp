#include <SFML/Graphics.hpp>
#include "Pch.h"
#include "Animation.h"
#include "Canvas.h"
#include "Scene.h"
#include "Physics.h"
#include "Component.h"
#include "Monobehaviour.h"
#include "ParticleSystem.h"
#include "CircleCollider.h"
#include "Input.h"
#include "Mathf.h"
#include "Debug.h"
#include "Rigidbody.h"
#include "MyParticle.h"

using namespace sf;
using namespace std;

void Draw();
void KeyBoardInput();
void MouseInput();
void CollisionChecking();
void PlayerAnimState();
void SceneWindow();

int windowHeight = 1200;
int windowWidth = 1200;

RenderWindow window(VideoMode(windowWidth, windowHeight), "Dragon game", Style::Close | Style::Titlebar | Style::Resize);

RectangleShape _player(Vector2f(100, 200));

float moveSpeed = 1.0f; // MoveSpeed was 5.0f as default
Vector2f velocity;

Texture hero, particle;

Time _time;
double deltaTime = 0; // these should be publicly available to everyone 
Clock _clock;

Animation _playerAnim;

enum ApplicationState
{
	Running_GameWindow,
	RunningSceneWindow
};

ApplicationState myApplication = ApplicationState::Running_GameWindow;

bool idle = true;

Canvas* myCanvas = Canvas::GetInstance();
Physics* myPhysics = Physics::GetInstance(); // in the future, physics simulation should happen on it's own thread
Debug* debug = Debug::GetInstance();

Rigidbody rb, g2rb, crb;
GameObject go, g1, circleThing;
RectangleShape rect1, rect2;

int main() // When used in declaration(string * ptr), it creates a pointer variable, when not used in declaration, it act as a dereference operator.
{
	switch (myApplication)
	{
	case Running_GameWindow:
		break;
	case RunningSceneWindow:
		SceneWindow(); // this will run it's own loop, and we will never continue in main here, well I mean technically we could return
		break;
	}

	myPhysics->InitializePhysicsUpdate();

	myCanvas->AddWindow(window); // could you somehow send it on initialize!?
	hero.loadFromFile("_sprites_heroes.png"); // unsigned int means the int can only be positive 
	particle.loadFromFile("ParticleDefault.png");
	Vector2u textureSize = hero.getSize(); // 9 * 8
	textureSize.x /= 9;
	textureSize.y /= 8;

	_player.setPosition(600, 600);
	_player.setOrigin(textureSize.x * (float)3.5, hero.getSize().y / 2); // check what these values give,  the character in the image are not centered in their so called box, need a better image to do this with
	_player.setTexture(&hero);
	_player.setTextureRect(IntRect(textureSize.x * 1, textureSize.y * 7, textureSize.x, textureSize.y));

	_playerAnim = Animation(&hero, Vector2u(9, 8), 0.15f); // the whole tileset is now the same framerate :/

	//ParticleSystem particles(10000, Color::Blue);
	ParticleSystem particlesPlayer(10000, Color::Black); // try not and go over 50.000 particles, preferably under 40k, 40k will just about give 120 fps

	BoxCollider box_ = BoxCollider(_player, _player.getPosition()); // the pos should be set to the gameObject pos if no pos is given
	box_.rect->setOrigin(_player.getOrigin().x / 2, _player.getOrigin().y / 2);
	go = GameObject(_player, rb, box_); // you should be able to make a gameobject with collider, shape and rigid body in one line
	go.AddComponent(particlesPlayer);

	BoxCollider aboxC = BoxCollider(*new RectangleShape(Vector2f(100, 100)), Vector2f(1100, 1000)); // there we go, a gameObject in 2 lines, though not super pretty
	GameObject g = GameObject(*aboxC.shape, aboxC, *new Rigidbody());

	CircleCollider circle = CircleCollider(*new CircleShape(50, 50), Vector2f(400, 400));
	circleThing = GameObject(*circle.shape, circle);
	circleThing.AddComponent(g2rb); 

	CircleCollider circle2 = CircleCollider(*new CircleShape(50, 50), Vector2f(100, 200), Color::Red);
	GameObject c = GameObject(*circle2.shape, circle2, *new Rigidbody());

	//debug->Log(c.transform->getPosition().x);

	//c.MyPos = c.transform->getPosition();
	//ofstream file_obj;
	//file_obj.open("Scene01.txt", ios::app);
	//file_obj.write((char*)&c, sizeof(c));

	//ifstream file_obj;
	//file_obj.open("Scene01.txt", ios::in);

	//// Object of class contestant to input data in file
	//GameObject obj;

	//// Reading from file into object "obj"
	//file_obj.read((char*)&obj, sizeof(obj));

	//while (!file_obj.eof()) {
	//	// Checking further
	//	file_obj.read((char*)&obj, sizeof(obj));
	//}
	//file_obj.close();

	//c.transform->setPosition(obj.MyPos);
	//debug->Log(obj.MyPos); // whenever we save the scene we save all the pos and ref and all the objects


	// might need scene classes 
	// need to know how to make copy(instantiation) of class from a file
	// need to clear when writing again to scene file
	// need like a scene window system, where you choose to run the scene

	// the big question is, how hard would it be to save a gameObject to a file and create it from a scene
	// for creation them again we cannot be using their pointers
	// currently have nothing that keeps track of gameObjects in the scene

	//Texture t;
	//t.loadFromFile("_sprites_heroes.png");
	//
	//CircleShape myR(50,50);
	////myR.setTexture(_player);

	/*Particle p(10, particle);
	p.setVelocity(1, 1);*/
	//myCanvas->AddDrawable(p.dot);

	MyParticleSystem p(&_player, 1500, 5, particle, 2, Color::Black); // can currently only emit 1.5k before the fps goes below the minimum required fps

	// need to be able to set the mouse pos as a transformable and move/inject it to other classes, like the particle system

	window.setFramerateLimit(120); // smooth constant fps, this should also be changeable
	while (window.isOpen()) // checking window events
	{
		_time = _clock.getElapsedTime(); // show fps, with debug info or something
		_clock.restart();
		deltaTime = _time.asSeconds(); // this will get the time between frames
		Monobehaviour::GetInstance()->DeltaTime = deltaTime;

		debug->DisplayFrameRate(_time);
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

		//Vector2i mouse = Mouse::getPosition(window);
		//particles.SetEmitterVector(Vector2f(static_cast<float>(mouse.x), static_cast<float>(mouse.y)));

		PlayerAnimState(); // then animate based on input
		CollisionChecking(); // last check for collisions
		p.Update();
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
	s->setPosition(_player.getPosition() + Vector2f(0, 100) + Vector2f(+40, 0) + (Vector2f(50, 0) * _player.getScale().x));
	s->setOrigin(_player.getOrigin());
	s->setFillColor(Color::Blue); // + is down, lol, dude it confuses me every time, have you guys not had math?

	Rigidbody* r = new Rigidbody();
	GameObject g = GameObject(*s, *r);
	r->velocity = Vector2f(_player.getScale().x * 8, 0);
}

void MouseInput() // gameObject should have a destroy method you can call
{
	if (Mouse::isButtonPressed(Mouse::Left))
	{
		Vector2i mousePos = Mouse::getPosition(window);
		//circleThing.transform->setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
		_player.setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)); // can also be put outside the if statement for constant follow of mouse, love it
	}
}

void KeyBoardInput()
{
	velocity = Mathf::Zero();
	if (Input::GetKey(Keyboard::Escape, Input::KeyDown)) window.close();

	if (Input::GetKey(Keyboard::Space, Input::KeyDown)) Shoot();

	if (Input::GetKey(Keyboard::Left, Input::KeyHeld) || Input::GetKey(Keyboard::A, Input::KeyHeld))
	{
		velocity.x = -1;
	}
	if (Input::GetKey(Keyboard::Right, Input::KeyHeld) || Input::GetKey(Keyboard::D, Input::KeyHeld))
	{
		velocity.x = 1;
	}
	if (Input::GetKey(Keyboard::Up, Input::KeyHeld) || Input::GetKey(Keyboard::W, Input::KeyHeld))
	{
		velocity.y = -1;
	}
	if (Input::GetKey(Keyboard::Down, Input::KeyHeld) || Input::GetKey(Keyboard::S, Input::KeyHeld))
	{
		velocity.y = 1;
	}
	//rb2.velocity = Mathf::Normalize(velocity) * moveSpeed;
	rb.velocity = Mathf::Normalize(velocity) * moveSpeed;
}

void CollisionChecking() // this should be moved into physics
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

void SceneWindow() // should be putting this elsewhere
{

}