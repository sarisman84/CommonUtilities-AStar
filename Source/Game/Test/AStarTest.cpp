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
	for (int y = 0; y < MapHeight; y++)
	{
		for (int x = 0; x < MapWidth; x++)
		{
			float xSize = myTileSize.x / targetRez;
			float ySize = myTileSize.y;


			float xPos = ((x * xSize) + (myTileSize.x / 2.f) / targetRez);
			float yPos = ((y * ySize) + (myTileSize.y / 2.f));

			mySpriteMap[y][x] = Transform2D
			{
				Tga2D::Vector2<float>(xPos, yPos),
				myTileSize
			};


			myTileMap[x + MapWidth * y] = Tile::Passable;
			myColliderMap[y][x] = BoxCollider(mySpriteMap[y][x]);

			/*myColliderMap[x][y].OnCollisionEnter() += [this, x, y]()
			{

				myTileMap[x * MapWidth + y] = Tile::Impassable;

			};


			myColliderMap[x][y].OnCollisionExit() += [this, x, y]()
			{

				myTileMap[x * MapWidth + y] = Tile::Passable;

			};*/


			myColliderMap[y][x].OnCollisionClick() += [this, x, y]()
			{
				using namespace CommonUtilities;
				using MKey = Mouse::Key;
				using CKey = Keyboard::Key;





				if (Mouse::GetButton(MKey::LeftMouseButton))
				{
					if (Keyboard::GetButton(CKey::Q))
					{
						myTileMap[x + MapWidth * y] = Tile::Impassable;
					}
					else if (Keyboard::GetButton(CKey::E))
					{
						myTileMap[x + MapWidth * y] = Tile::Passable;
					}

				}

				if (Mouse::GetButtonDown(MKey::LeftMouseButton) && !Keyboard::GetButton(CKey::Q) && !Keyboard::GetButton(CKey::E))
				{
					if (myStartPosition != -1 && myEndPosition != -1)
					{
						mySearchArea.clear();
						myFoundPath.clear();
						myStartPosition = -1;
						myEndPosition = -1;
					}
					else if (myStartPosition == -1)
					{
						myStartPosition = x + MapWidth * y;
					}
					else if (myEndPosition == -1 && myStartPosition != myEndPosition)
					{
						myEndPosition = x + MapWidth * y;
						myFoundPath = AStar(myTileMap, myStartPosition, myEndPosition, &mySearchArea);
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

	for (int y = 0; y < MapHeight; y++)
	{
		for (int x = 0; x < MapWidth; x++)
		{
			/*float dist = (myMousePosition - myColliderMap[x][y].Transform().myPosition).Length();

			if (dist >= 0.015f) continue;*/

			bool result = myColliderMap[y][x].CheckCollision(Transform2D{ myMousePosition,  Tga2D::Vector2f(1.f, 1.f) });

			bool hasCollidedBefore = myColliderMap[y][x].HasCollidedAlready();
			if (result)
			{
				if (!hasCollidedBefore)
				{
					myColliderMap[y][x].OnCollisionEnter().Invoke();
					myColliderMap[y][x].HasCollidedAlready() = true;
				}




				myColliderMap[y][x].OnCollisionClick().Invoke();


			}
			else if (hasCollidedBefore)
			{
				myColliderMap[y][x].OnCollisionExit().Invoke();
				myColliderMap[y][x].HasCollidedAlready() = false;
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


	for (size_t y = 0; y < MapHeight; y++)
	{
		for (size_t x = 0; x < MapWidth; x++)
		{
			Tga2D::Sprite2DInstanceData ins;
			auto transform = mySpriteMap[y][x];
			ins.myPosition = transform.myPosition;
			ins.mySize = transform.mySize;
			ins.myPivot = { 0.5f, 0.5f };

			ins.myColor = myTileMap[x + MapWidth * y] == Tile::Passable ? Tga2D::Color(0.f, 1.f, 0.f, 1.f) : Tga2D::Color(1.f, 0.f, 0.f, 1.f);


			if (std::find(mySearchArea.begin(), mySearchArea.end(), x + MapWidth * y) != mySearchArea.end())
				ins.myColor = Tga2D::Color(1.f, 1.f, 0.f, 1.f);
			if (std::find(myFoundPath.begin(), myFoundPath.end(), x + MapWidth * y) != myFoundPath.end())
				ins.myColor = Tga2D::Color(0.f, 0.f, 1.f, 1.f);

			if (x + MapWidth * y == myStartPosition || x + MapWidth * y == myEndPosition)
				ins.myColor = Tga2D::Color(0.f, 1.f, 1.f, 1.f);



			batch.Draw(ins);
		}
	}
}

void AStarTest::OnClick()
{
}
