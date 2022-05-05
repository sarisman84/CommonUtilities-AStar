#pragma once
#include "../AStar/AStar.h"
#include <tga2d/texture/texture.h>
#include "Transform2D.h"
#include "BoxCollider.h"
#include "../AStar/AStar.h"
class AStarTest
{
public:
	AStarTest() = default;
	void Awake();
	void Update(const float& aTimeDelta);
	void Render();
private:
	void OnClick();

	Transform2D mySpriteMap[MapWidth][MapHeight];
	BoxCollider myColliderMap[MapWidth][MapHeight];
	std::vector<Tile> myTileMap;
	Tga2D::Texture* myTileTexture;
	const Tga2D::Vector2<float> myTileSize = { .05f, .05f };
	Tga2D::Vector2<float> myMousePosition;
	std::vector<int> myFoundPath;


	int myStartPosition;
	int myEndPosition;

	
};

