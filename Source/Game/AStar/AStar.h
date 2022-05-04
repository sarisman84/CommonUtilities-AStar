#pragma once
#include <vector>
#include "Custom/Heap.hpp"
#include "Custom/Vector2.h"



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

template<class T>
struct Prop
{
	T myValue;
	Prop(std::function<void(T& aValue)> aSet, std::function<T()> aGet)
	{
		mySet = aSet;
		myGet = aGet;
	}

	void operator=(const T& someOtherValue)
	{
		if (mySet)
			mySet(someOtherValue);
	}

	void operator=(const Prop<T>& someOtherProp)
	{
		mySet = someOtherProp.mySet;
		myGet = someOtherProp.myGet;
	}

	bool operator<(const Prop<T>& someOther) const
	{
		if (myGet && someOther.myGet)
			return myGet() < someOther.myGet();
		return false;
	}

private:

	std::function<void(T& aValue)> mySet;
	std::function<T()> myGet;

};

template<class T>
void operator<<(T& aValue, const Prop<T>& aProp)
{
	if (aProp.myGet)
	{
		aProp.myValue = aProp.myGet(aValue);
		aValue = aProp.myValue;
	}


}

struct Node
{
	Node() = default;
	Node(int anXCoord, int anYCoord)
	{
		myPos.x = anXCoord;
		myPos.y = anYCoord;
	}

	CommonUtilities::Vector2<int> myPos;
	CommonUtilities::Vector2<int> myPathPos;
	int myHCost, myGCost;





	const Prop<int> myFCost = { nullptr, [this]() -> int
	{
			return myGCost + myHCost;
	} };

	bool operator<(const Node& someOther)
	{
		return myFCost < someOther.myFCost;
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
		//myFCost.myValue << someOther.myFCost;

		myPos = someOther.myPos;
		myPathPos = someOther.myPathPos;
	}

};


std::vector<int> AStar(const std::vector<Tile>& aMap, int aStartIndex, int anEndIndex, bool aUseEuclidianDistance = false)
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
		myNodes[i].myHCost = aUseEuclidianDistance ? static_cast<int>(std::sqrt(std::pow((x - endPos.x), 2) + std::pow((y - endPos.y), 2))) : std::abs(x - endPos.x) + std::abs(y - endPos.y);
	}

	Node startingNode = myNodes[aStartIndex];
	startingNode.myGCost = 0;
	Node endNode = Node(-1, -1);
	Heap<Node, HeapType::Min> myOpenNodes;
	std::vector<Node> myClosedNodes;


	myOpenNodes.Enqueue(startingNode);

	while (myOpenNodes.GetSize() > 0)
	{
		Node targetNode = myOpenNodes.Dequeue();
		myClosedNodes.push_back(targetNode);
		for (int xNeighbour = 0; xNeighbour < 3; xNeighbour++)
		{
			for (int yNeighbour = 0; yNeighbour < 3; yNeighbour++)
			{
				int offsetX = targetNode.myPos.x + xNeighbour;
				int offsetY = targetNode.myPos.y + yNeighbour;

				if (offsetX < 0 || offsetX >= MapWidth) continue; //Skip incase of out of bounds
				if (offsetY < 0 || offsetY >= MapHeight) continue; //Skip incase of out of bounds
				if (xNeighbour != 0 && yNeighbour != 0) continue; //Skip diagonals
				if (xNeighbour == 0 && yNeighbour == 0) continue; //Skip center
				Node neighbour = myNodes[offsetX + MapWidth * offsetY];

				if (std::find(myClosedNodes.begin(), myClosedNodes.end(), neighbour) != myClosedNodes.end()) continue; //Skip if node is closed
				if (aMap[offsetX + MapWidth * offsetY] == Tile::Impassable) continue; //Skip if node is impassable

				int gCost = targetNode.myGCost + static_cast<int>(Vector2<int>::Distance(neighbour.myPos, targetNode.myPos));

				if (gCost < neighbour.myGCost)
				{
					neighbour.myGCost = gCost;
					neighbour.myPathPos = targetNode.myPos;
				}

				myOpenNodes.Enqueue(neighbour);

			}
		}
	}
	Node currentNode = myNodes[endPos.x + MapWidth * endPos.y];
	while (currentNode != startingNode)
	{
		int i = currentNode.myPathPos.x + MapWidth * currentNode.myPathPos.y;
		currentNode = myNodes[i];
		path.push_back(i);
	}

	std::reverse(path.begin(), path.end());

	return path;

}
