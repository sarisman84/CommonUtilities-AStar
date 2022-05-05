#include "stdafx.h"
#include "AStarTest.h"
#include <tga2d/sprite/sprite.h>
#include <tga2d/texture/TextureManager.h>
#include <tga2d/drawers/SpriteDrawer.h>
#include <tga2d/graphics/GraphicsEngine.h>
#include <Custom/Input.h>


void AStarTest::Awake()
{
	myStartPosition = -1;
	myEndPosition = -1;



	myTileMap.resize(TileCount);

	auto targetRez = Tga2D::Engine::GetInstance()->GetWindowRatio();

	myTileTexture = Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(L"Sprites/tile.png");
	for (int x = 0; x < MapWidth; x++)
	{
		for (int y = 0; y < MapHeight; y++)
		{
			float xSize = myTileSize.x / targetRez;
			float ySize = myTileSize.y;


			float xPos = ((x * xSize) + (myTileSize.x / 2.f) / targetRez);
			float yPos = ((y * ySize) + (myTileSize.y / 2.f));

			mySpriteMap[x][y] = Transform2D
			{
				Tga2D::Vector2<float>(xPos, yPos),
				myTileSize
			};


			myTileMap[x * MapWidth + y] = Tile::Passable;
			myColliderMap[x][y] = BoxCollider(mySpriteMap[x][y]);

			/*myColliderMap[x][y].OnCollisionEnter() += [this, x, y]()
			{

				myTileMap[x * MapWidth + y] = Tile::Impassable;

			};


			myColliderMap[x][y].OnCollisionExit() += [this, x, y]()
			{

				myTileMap[x * MapWidth + y] = Tile::Passable;

			};*/


			myColliderMap[x][y].OnCollisionClick() += [this, x, y]()
			{
				using namespace CommonUtilities;
				using MKey = Mouse::Key;
				using CKey = Keyboard::Key;





				if (Mouse::GetButton(MKey::LeftMouseButton))
				{
					if (Keyboard::GetButton(CKey::Q))
					{
						myTileMap[x * MapWidth + y] = Tile::Impassable;
					}
					else if (Keyboard::GetButton(CKey::E))
					{
						myTileMap[x * MapWidth + y] = Tile::Passable;
					}

				}

				if (Mouse::GetButtonDown(MKey::LeftMouseButton) && !Keyboard::GetButton(CKey::Q) && !Keyboard::GetButton(CKey::E))
				{
					if (myFoundPath.size() != 0)
					{
						myFoundPath.clear();
						myStartPosition = -1;
						myEndPosition = -1;
					}
					else if (myStartPosition == -1)
					{
						myStartPosition = x * MapWidth + y;
					}
					else if (myEndPosition == -1)
					{
						myEndPosition = x * MapWidth + y;
						myFoundPath = AStar(myTileMap, myStartPosition, myEndPosition);
					}
				}

			};



		}
	}
}
void AStarTest::Update(const float& /*aTimeDelta*/)
{
	auto targetSize = Tga2D::Engine::GetInstance()->GetWindowSize();
	auto pos = CommonUtilities::Mouse::GetMousePosition();
	myMousePosition = { static_cast<float>(pos.x) / static_cast<float>(targetSize.x), static_cast<float>(pos.y) / static_cast<float>(targetSize.y) };


	std::cout << '\r' << myMousePosition << std::flush;

	for (int x = 0; x < MapWidth; x++)
	{
		for (int y = 0; y < MapHeight; y++)
		{
			/*float dist = (myMousePosition - myColliderMap[x][y].Transform().myPosition).Length();

			if (dist >= 0.015f) continue;*/

			bool result = myColliderMap[x][y].CheckCollision(Transform2D{ myMousePosition,  Tga2D::Vector2f(1.f, 1.f) });

			bool hasCollidedBefore = myColliderMap[x][y].HasCollidedAlready();
			if (result)
			{
				if (!hasCollidedBefore)
				{
					myColliderMap[x][y].OnCollisionEnter().Invoke();
					myColliderMap[x][y].HasCollidedAlready() = true;
				}




				myColliderMap[x][y].OnCollisionClick().Invoke();


			}
			else if (hasCollidedBefore)
			{
				myColliderMap[x][y].OnCollisionExit().Invoke();
				myColliderMap[x][y].HasCollidedAlready() = false;
			}





		}
	}


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
			ins.myPosition = transform.myPosition;
			ins.mySize = transform.mySize;
			ins.myPivot = { 0.5f, 0.5f };

			ins.myColor = myTileMap[x * MapWidth + y] == Tile::Passable ? Tga2D::Color(0.f, 1.f, 0.f, 1.f) : Tga2D::Color(1.f, 0.f, 0.f, 1.f);

			if (std::find(myFoundPath.begin(), myFoundPath.end(), x * MapWidth + y) != myFoundPath.end())
				ins.myColor = Tga2D::Color(0.f, 0.f, 1.f, 1.f);

			if (x * MapWidth + y == myStartPosition || x * MapWidth + y == myEndPosition)
				ins.myColor = Tga2D::Color(0.f, 1.f, 1.f, 1.f);
			batch.Draw(ins);
		}
	}
}

void AStarTest::OnClick()
{
}
