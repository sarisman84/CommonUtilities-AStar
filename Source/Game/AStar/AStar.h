#pragma once
#include <vector>
#include <Custom/Heap.hpp>
#include <Custom/Vector2.h>
#include <functional>


static const int MapWidth = 20;
static const int MapHeight = 20;
static const int TileCount = MapWidth * MapHeight;
enum class Tile
{
	Impassable,
	Passable
};

enum class Access
{
	Read, Write, Both
};





struct Node
{
private:
	int myFCost;
public:

	Node() = default;
	Node(int anXCoord, int anYCoord)
	{
		myPos.x = anXCoord;
		myPos.y = anYCoord;

		myDadPos.x = -1;
		myDadPos.y = -1;
	}

	CommonUtilities::Vector2<int> myPos;
	CommonUtilities::Vector2<int> myDadPos;
	int myHCost, myGCost;

	int FCost()
	{
		myFCost = myGCost + myHCost;
		return myFCost;
	}

	bool operator<(Node& someOther)
	{
		return FCost() < someOther.FCost();
	}

	bool operator==(const Node& someOther)
	{
		return myPos == someOther.myPos;
	}


	bool operator!=(const Node& someOther)
	{
		return !(myPos == someOther.myPos);
	}

	void operator=(const Node& someOther)
	{
		myHCost = someOther.myHCost;
		myGCost = someOther.myGCost;

		myPos = someOther.myPos;
		myDadPos = someOther.myDadPos;
	}



};


inline std::vector<int> AStar(const std::vector<Tile>& aMap, int aStartIndex, int anEndIndex, bool aUseEuclidianDistance = false)
{
	std::vector<int> path;
	using namespace CommonUtilities;

	if (aStartIndex < 0 || aStartIndex >= TileCount || anEndIndex < 0 || anEndIndex >= TileCount) return path;
	if (aMap[aStartIndex] == Tile::Impassable || aMap[anEndIndex] == Tile::Impassable) return path;



	Vector2<int> endPos = { anEndIndex % MapWidth , anEndIndex / MapHeight };

	std::vector<Node> myNodes;
	for (int i = 0; i < TileCount; i++)
	{
		int x = i % MapWidth;
		int y = i / MapHeight;
		myNodes.emplace_back(x, y);
		myNodes[i].myGCost = INT_MAX;
		myNodes[i].myHCost = aUseEuclidianDistance ?
			static_cast<int>(std::sqrt(std::pow((x - endPos.x), 2) + std::pow((y - endPos.y), 2))) :
			std::abs(x - endPos.x) + std::abs(y - endPos.y);
	}

	Node& startingNode = myNodes[aStartIndex];
	startingNode.myGCost = 0;
	Heap<Node*, HeapType::Min> myOpenNodes;
	std::vector<Node> myClosedNodes;

	//std::vector<Node> myOpenNodes;

	//std::make_heap(myOpenNodes.begin(), myOpenNodes.end());

	myOpenNodes.Enqueue(&startingNode);

	while (myOpenNodes.GetSize() > 0)
	{

		/*std::sort(myOpenNodes.begin(), myOpenNodes.end(), [](Node aLhs, Node aRhs)->bool { return aLhs.FCost() > aRhs.FCost(); });*/
		Node* owner = myOpenNodes.Dequeue();



		myClosedNodes.push_back(*owner);
		for (int xNeighbour = -1; xNeighbour <= 1; xNeighbour++)
		{
			for (int yNeighbour = -1; yNeighbour <= 1; yNeighbour++)
			{
				int offsetX = owner->myPos.x + xNeighbour;
				int offsetY = owner->myPos.y + yNeighbour;

				if (offsetX < 0 || offsetX >= MapWidth) continue; //Skip incase of out of bounds
				if (offsetY < 0 || offsetY >= MapHeight) continue; //Skip incase of out of bounds
				if (xNeighbour != 0 && yNeighbour != 0) continue; //Skip diagonals
				if (xNeighbour == 0 && yNeighbour == 0) continue; //Skip center
				Node& neighbour = myNodes[offsetX + MapWidth * offsetY];

				if (std::find(myClosedNodes.begin(), myClosedNodes.end(), neighbour) != myClosedNodes.end()) continue; //Skip if node is closed
				if (aMap[offsetX + MapWidth * offsetY] == Tile::Impassable) continue; //Skip if node is impassable

				int gCost = owner->myGCost + static_cast<int>(Vector2<int>::Distance(neighbour.myPos, owner->myPos));

				if (gCost < neighbour.myGCost)
				{
					neighbour.myGCost = gCost;
					neighbour.myDadPos = owner->myPos;
					neighbour.FCost();
					myOpenNodes.Enqueue(&neighbour);
				}



			}
		}
	}
	Node currentNode = myNodes[anEndIndex];
	while (currentNode != myNodes[aStartIndex])
	{
		int i = currentNode.myDadPos.x + MapWidth * currentNode.myDadPos.y;
		currentNode = myNodes[i];
		path.push_back(i);
	}

	std::reverse(path.begin(), path.end());

	return path;

}
