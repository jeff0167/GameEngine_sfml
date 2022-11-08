#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Animation
{
public:
	IntRect uvRect;

	Animation();
	Animation(Texture* texture, Vector2u imageCount, float switchTime);

	void Update(int row, float deltaTime);
	void Update(int row, int rowStartFrame, int fowEndFrame, float deltaTime);
	void NextAnim();

private:
	Vector2u imageCount;
	Vector2u currentImage;

	float totalTime;
	float switchTime;
};