#include <SFML/Graphics.hpp>
#include "Pch.h"
#include "Animation.h"
#include "Canvas.h"
#include "Scene.h"
#include "SceneWindow.h"
#include "Physics.h"
#include "Component.h"
#include "Monobehaviour.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "ParticleSystem.h"
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

uint32_t windowHeight = 1200; // better then unsigned int, shorter and clearer
uint32_t windowWidth = 1200;

RenderWindow window(VideoMode(windowWidth, windowHeight), "Dragon game", Style::Close | Style::Titlebar | Style::Resize);

RectangleShape _player(Vector2f(100, 200));

float moveSpeed = 1.0f; // MoveSpeed was 5.0f as default
Vector2f velocity;

Texture hero, particle;

Animation _playerAnim;

enum ApplicationState
{
	Running_GameWindow,
	Running_SceneWindow
};

ApplicationState myApplication = ApplicationState::Running_GameWindow;

bool idle = true;

Rigidbody rb, g2rb;
GameObject go, circleThing;

int main() // main should literally be empty!!
{
	// What I have learned
	// I cannot load the scene when in game mode
	// ParticleSystem does not work cause of timing issues

	// I think that I need to try and make a simple game and see what obstacles I face in doing so
	// then I can find possible features that will improve the experience of developing games

	// I really think that I am missing a layer system on the canvas for one ^

	Renderer->AddWindow(window);
	switch (myApplication)
	{
	case Running_GameWindow:
		break;
	case Running_SceneWindow:
		SceneWindow(); // this will run it's own loop, and we will never continue in main here, well I mean technically we could return
		break;
	}

	Science->InitializePhysicsUpdate();

	hero.loadFromFile("_sprites_heroes.png");
	particle.loadFromFile("ParticleDefault.png");
	Vector2u textureSize = hero.getSize(); // 9 * 8
	textureSize.x /= 9;
	textureSize.y /= 8;

	_player.setPosition(600, 600);
	_player.setOrigin(textureSize.x * (float)3.5, hero.getSize().y / 2); // check what these values give,  the character in the image are not centered in their so called box, need a better image to do this with
	_player.setTexture(&hero);
	_player.setTextureRect(IntRect(textureSize.x * 1, textureSize.y * 7, textureSize.x, textureSize.y));

	_playerAnim = Animation(&hero, Vector2u(9, 8), 0.15f); 

	//ParticleSystem particles(10000, Color::Blue); // currently not working correctly
	//ParticleSystem particlesPlayer(10000, Color::Blue, 10000); // try not and go over 50.000 particles, preferably under 40k, 40k will just about give 120 fps

	BoxCollider box_ = BoxCollider(_player, _player.getPosition()); // the pos should be set to the gameObject pos if no pos is given
	box_.rect->setOrigin(_player.getOrigin().x / 2, _player.getOrigin().y / 2);
	go = GameObject(_player, rb, box_);
	//go.AddComponent(particlesPlayer);

	BoxCollider aboxC = BoxCollider(*new RectangleShape(Vector2f(100, 100)), Vector2f(1100, 1000)); 
	GameObject g = GameObject(*aboxC.shape, aboxC, *new Rigidbody());

	CircleCollider circle = CircleCollider(*new CircleShape(50, 50), Vector2f(400, 400));
	circleThing = GameObject(*circle.shape, circle);
	circleThing.AddComponent(g2rb); 

	unique_ptr<CircleShape> d(new CircleShape(50, 50)); // hmm i don't like that i manually have to make sure that i use a unique ptr
	CircleCollider circle2 = CircleCollider(*d, Vector2f(100, 200), Color::Red);
	GameObject c = GameObject(*circle2.shape, circle2, *new Rigidbody()); // carefull we have memory leak with new

	MyParticleSystem p(&_player, 50, 5, particle, 0.5f, seconds(1), Color::Black); // can currently only emit 1.5k before the fps goes below the minimum required fps
	go.AddComponent(p);

	window.setFramerateLimit(120); // this should also be changeable
	while (window.isOpen()) // checking window eventss
	{
		Mono->UpdateTime();

		DebugFrameRate(Mono->_time);
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

		PlayerAnimState(); // animate based on input
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
		_playerAnim.Update(7, 3, 6, Mono->DeltaTime); // there was a comment about it being a bit unresponsive but I don't currently see the problem
	}
	else
	{
		if (!idle)
		{
			idle = true;
			_playerAnim.NextAnim();
		}

		_playerAnim.Update(7, 0, 3, Mono->DeltaTime);
	}
	_player.setTextureRect(_playerAnim.uvRect);
}

void Shoot() // make objects with monobehaviour so that objects don't go out of scope, here the drawable and rigidbody ref are added to their management system and still
{
	CircleShape* s = new CircleShape(10, 50); 
	s->setPosition(_player.getPosition() + Vector2f(0, 100) + Vector2f(+40, 0) + (Vector2f(70, 0) * _player.getScale().x));
	s->setOrigin(_player.getOrigin());
	s->setFillColor(Color::Blue);

	Rigidbody* r = new Rigidbody();
	GameObject g = GameObject(*s, *r);
	r->useGravity = true;
	r->velocity = Vector2f(_player.getScale().x * 2, 0);
}

void MouseInput() 
{
	if (Mouse::isButtonPressed(Mouse::Left))
	{
		Vector2i mousePos = Mouse::getPosition(window);
		_player.setPosition((mousePos.x), (mousePos.y));
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

void Draw() 
{
	window.clear(Color(255, 204, 92)); 
	Renderer->DrawCanvas();
	window.display();
}

void SceneWindow() // should be putting this elsewhere
{
	//SceneManager->SetActiveScene("Scene02");
	SceneManager->LoadScene();
	SceneManager->DisplaySceneWindow(window);
}