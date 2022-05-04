#pragma once
#include "../AStar/AStar.h"
#include <tga2d/texture/texture.h>

struct Transform2D
{
	CommonUtilities::Vector2<float> myPosition;
	CommonUtilities::Vector2<float> mySize;
};

class AStarTest
{
public:
	AStarTest() = default;
	void Awake();
	void Update(const float& aTimeDelta);
	void Render();
private:
	Transform2D mySpriteMap[MapWidth][MapHeight];
	Tga2D::Texture* myTileTexture;
	const CommonUtilities::Vector2<float> myTileSize;
};

