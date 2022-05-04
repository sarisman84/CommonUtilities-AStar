#include "stdafx.h"
#include "AStarTest.h"
#include <tga2d/sprite/sprite.h>
#include <tga2d/texture/TextureManager.h>
#include <tga2d/drawers/SpriteDrawer.h>
#include <tga2d/graphics/GraphicsEngine.h>

void AStarTest::Awake()
{
	myTileTexture = Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(L"Sprites/tile.png");
	for (size_t x = 0; x < MapWidth; x++)
	{
		for (size_t y = 0; y < MapHeight; y++)
		{
			mySpriteMap[x][y] = Transform2D
			{
				CommonUtilities::Vector2<float>(-(MapWidth / 2.f) + (myTileSize.x * x), -(MapHeight / 2.f) + (myTileSize.y * y)),
				myTileSize
			};
		}
	}
}

void AStarTest::Update(const float& /*aTimeDelta*/)
{
}

void AStarTest::Render()
{

	Tga2D::SpriteSharedData data;
	data.myTexture = myTileTexture;

	if (!data.myTexture)return;

	Tga2D::SpriteDrawer& drawer = Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetSpriteDrawer();

	auto batch = drawer.BeginBatch(data);


	for (size_t x = 0; x < MapWidth; x++)
	{
		for (size_t y = 0; y < MapHeight; y++)
		{
			Tga2D::Sprite2DInstanceData ins;
			auto transform = mySpriteMap[x][y];
			ins.myPosition = { transform.myPosition.x, transform.myPosition.y };
			ins.mySize = { transform.mySize.x, transform.mySize.y };


			batch.Draw(ins);
		}
	}
}
