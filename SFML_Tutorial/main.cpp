#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Canvas.h"
#include "Scene.h"
#include "SceneWindow.h"
#include "Physics.h"
#include "Monobehaviour.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "ParticleSystem.h"
#include "Input.h"
#include "Mathf.h"
#include "Debug.h"
#include "Rigidbody.h"
#include "MyParticleSystem.h"

using namespace sf;
using namespace std;

void Draw();
void KeyBoardInput();
void MouseInput();
void CollisionChecking();
void PlayerAnimState();
void SceneWindow();

uint32_t windowHeight = 1200;
uint32_t windowWidth = 1200;

RenderWindow window(VideoMode(windowWidth, windowHeight), "Dragon game", Style::Close | Style::Titlebar | Style::Resize);

RectangleShape _player(Vector2f(100, 200));

float moveSpeed = 50.0f; // MoveSpeed was 5.0f as default
Vector2f velocity;

Texture hero, particle, snowyForest, astroid;

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

//View camera; // usable if you need to move, scale and rotate the view, like any camera in Unity


RectangleShape _background, _rock;
BoxCollider bx;

GameObject Background, Rock;

Rigidbody rb2;

void GameStart()
{
	// this will initiate the game with it's positions and activate game objects to be visible and whatnot

	// dude i absolutely wasted my time, making useless ai images, fooling around with anim, making a game with unity?!?!?

	// gonna make a game with the dragon, that already has anim
	// it's gonna shoot projectiles, maybe with particle effect's at the mouse pos
	// things, idk a rock or something comes at the player
	// the things shrink and die and explode
	// there are a few objects in the scene and a background

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

	BoxCollider box_ = BoxCollider(_player, _player.getPosition()); 
	box_.rect->setOrigin(_player.getOrigin().x / 2, _player.getOrigin().y / 2);
	go = GameObject(_player, rb, box_);

	Renderer->ChangeDrawableLayer(_player, 9); // the higher the layer the further in front

	// background

	snowyForest.loadFromFile("Dream_Background.jpg");

	auto size = Vector2f(snowyForest.getSize().x, snowyForest.getSize().y);
	_background.setOrigin(_background.getSize().x, _background.getSize().y);
	_background.setPosition(windowWidth / size.x - 5, windowHeight / size.y - 800);

	_background.setSize(size * 1.28f);
	_background.setTexture(&snowyForest);
	Background = GameObject(_background);

	Renderer->ChangeDrawableLayer(_background, 0);

	// rock
	astroid.loadFromFile("Rock.png");

	size = Vector2f(astroid.getSize().x, astroid.getSize().y);
	_rock.setOrigin(_rock.getSize().x / 2, _rock.getSize().y / 2);
	_rock.setPosition(windowWidth / 2 + 200, windowHeight / 2 + 200);

	_rock.setTexture(&astroid);
	_rock.setSize(size);

	_rock.setSize(_rock.getSize() * 0.5f); // when changing size, you must always change origin as well
	_rock.setOrigin(_rock.getSize().x / 2.0f, _rock.getSize().y / 2.0f);

	bx = BoxCollider(_rock, _rock.getPosition());
	bx.rect->setOrigin(_rock.getOrigin().x / 2, _rock.getOrigin().y / 2);

	// the boxcollider can go out of scope, and the physics still expect the rigidbody to exist
	// when things go out of scope(gets destroyed) they have to be removed from what ever list they were added to

	Rock = GameObject(_rock, rb2); // immediately after, delete on gameObject gets called, it doesn't even go out of scope yet?!? is it making a copy or something?
	rb2.velocity = Vector2f(10, 0);
	// even if the game object gets deleted the rigidbody should still exist and be accessible

	Renderer->ChangeDrawableLayer(_rock, 1);

	// what I really need is a sprite renderer class to handle all this png data
}

int main() // main should literally be empty!!
{
	//camera.setCenter(Vector2f(600, 600)); // making use of a camera
	//camera.setSize(Vector2f(1200, 1200));
	//window.getViewport(camera);

	// I think that I need to try and make a simple game and see what obstacles I face in doing so
	// then I can find possible features that will improve the experience of developing games

	// conclusion, making a game with the engine is terribly difficult, takes alot of code to handle images
	// tons of access violation, gameObject deletion for some reason, can't reset the lvl, that would have to be done manually
	// would have to directly go and make changes to the engine to add wished functionallity 
	// hard to place images and set appropriate size of things without having to compile and run the engine
	// can't delete objects or components at the moment, nor even disable gameObjects, would have to relly on a pool system for reuse of objects
	// can't easily make prefabs
	// any and all changes has to be done through code, that then needs to be compiled
	//
	// the scenewindow would really be handy as well as well as a config file for simple settings
	// not to mention the seperation of concerns with having the game engine and the game seperate, the game code should really not be within main
	// would have a horrible time trying to use the engine with another project
	//
	// Scene serialization and the seperation of the game engine and the game would be huge benefits for the engine
	// Though both things that are very hard to implement and would take a ton of time
	// first the errors withing the game engine would have to be resolved, everything tested
	// really tempted to start fresh to then also get release build setup and pch again and clean the headers
	//
	// Hmm a game I could see would be possible to create without issue would be a simple highscore game
	// the only things fall or what ever, coming at the player and if he is hit he dies(invoke) and we place him at a start position
	// the time is then measured upon dead and you would display the current time while playing and the highscore

	Renderer->AddWindow(window);
	switch (myApplication)
	{
	case Running_GameWindow:
		break;
	case Running_SceneWindow:
		SceneWindow(); // this will run it's own loop
		break;
	}

	Science->InitializePhysicsUpdate();

	GameStart();

	//hero.loadFromFile("_sprites_heroes.png");
	//particle.loadFromFile("ParticleDefault.png");
	//Vector2u textureSize = hero.getSize(); // 9 * 8
	//textureSize.x /= 9;
	//textureSize.y /= 8;

	//_player.setPosition(600, 600);
	//_player.setOrigin(textureSize.x * (float)3.5, hero.getSize().y / 2); // check what these values give,  the character in the image are not centered in their so called box, need a better image to do this with
	//_player.setTexture(&hero);
	//_player.setTextureRect(IntRect(textureSize.x * 1, textureSize.y * 7, textureSize.x, textureSize.y));

	//_playerAnim = Animation(&hero, Vector2u(9, 8), 0.15f); 

	////ParticleSystem particlesPlayer(5000, Color::Blue, 500); // try not and go over 50.000 particles, preferably under 40k, 40k will just about give 120 fps

	//BoxCollider box_ = BoxCollider(_player, _player.getPosition()); 
	//box_.rect->setOrigin(_player.getOrigin().x / 2, _player.getOrigin().y / 2);
	//go = GameObject(_player, rb, box_);

	//Renderer->ChangeDrawableLayer(_player, 4); // the higher the layer the further in front

	//BoxCollider aboxC = BoxCollider(*new RectangleShape(Vector2f(100, 100)), Vector2f(1100, 1000)); 
	//GameObject g = GameObject(*aboxC.shape, aboxC, *new Rigidbody());

	//CircleCollider circle = CircleCollider(*new CircleShape(50, 50), Vector2f(400, 400));
	//circleThing = GameObject(*circle.shape, circle);
	//circleThing.AddComponent(g2rb); 

	//unique_ptr<CircleShape> d(new CircleShape(50, 50)); // hmm i don't like that i manually have to make sure that i use a unique ptr
	//CircleCollider circle2 = CircleCollider(*d, Vector2f(100, 200), Color::Red);
	//GameObject c = GameObject(*circle2.shape, circle2, *new Rigidbody()); // carefull we have memory leak with new

	//Renderer->ChangeDrawableLayer(*circle2.shape, 7); // dude it works, no trouble, just worked first try

	//MyParticleSystem p(&_player, 2000, 5, particle, 50, seconds(1), Color::Black); // can currently only emit 1.5k before the fps goes below the minimum required fps
	//go.AddComponent(p);

	window.setFramerateLimit(120); // this should also be changeable
	while (window.isOpen()) // checking window events
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

		//window.setView(camera); // setting the view of the camera, this is not a expensive call, don't worry

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

void Shoot() // make objects with monobehaviour so that objects don't go out of scope, here the drawable and rigidbody ref are added to their management system
{
	CircleShape* s = new CircleShape(10, 50); 
	s->setPosition(_player.getPosition() + Vector2f(0, 100) + Vector2f(+40, 0) + (Vector2f(70, 0) * _player.getScale().x));
	s->setOrigin(_player.getOrigin());
	s->setFillColor(Color::Magenta);

	Rigidbody* r = new Rigidbody();
	GameObject g = GameObject(*s, *r);
	r->useGravity = true;
	r->velocity = Vector2f(_player.getScale().x * 60, 0);
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
	//camera.move(Mathf::Normalize(velocity) * moveSpeed); // moves the camera
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