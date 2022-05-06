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




enum class NodeStatus
{
	Open, Closed, NotVisited
};


struct Node
{

	int myFCost, myGCost, myHCost;
	NodeStatus myStatus;
	CommonUtilities::Vector2<int> myPos;
	Node* myPredecessor;
};


inline bool operator<(Node& aLhs, Node& aRhs)
{
	aLhs.myFCost = aLhs.myGCost + aLhs.myHCost;
	aRhs.myFCost = aRhs.myGCost + aLhs.myHCost;
	return aLhs.myFCost < aRhs.myFCost;
}

inline bool operator==(Node aLhs, Node aRhs)
{
	return aLhs.myPos == aRhs.myPos;
}


inline std::vector<int> AStar(const std::vector<Tile>& aMap, int aStartIndex, int anEndIndex, std::vector<int>* aDebugPath = nullptr, bool aUseEuclidianDistance = false)
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
		myNodes.emplace_back();
		myNodes[i].myPos = { x,y };
		myNodes[i].myGCost = INT_MAX;
		myNodes[i].myHCost = aUseEuclidianDistance ?
			static_cast<int>(std::sqrt(std::pow((x - endPos.x), 2) + std::pow((y - endPos.y), 2))) :
			std::abs(x - endPos.x) + std::abs(y - endPos.y);
		myNodes[i].myStatus = NodeStatus::NotVisited;
	}

	Node& startingNode = myNodes[aStartIndex];
	startingNode.myGCost = 0;
	Heap<Node, HeapType::Min> myOpenNodes;

	//std::vector<Node> myOpenNodes;

	//std::make_heap(myOpenNodes.begin(), myOpenNodes.end());

	myOpenNodes.Enqueue(startingNode);

	while (myOpenNodes.GetSize() > 0)
	{

		/*std::sort(myOpenNodes.begin(), myOpenNodes.end(), [](Node aLhs, Node aRhs)->bool { return aLhs.FCost() > aRhs.FCost(); });*/
		Node* currentNode = &myOpenNodes.Dequeue();

		if (currentNode == &myNodes[anEndIndex])
		{
			while (currentNode != &myNodes[aStartIndex] && currentNode->myPredecessor)
			{
				int i = currentNode->myPredecessor->myPos.x + MapWidth * currentNode->myPredecessor->myPos.y;
				currentNode = currentNode->myPredecessor;
				path.push_back(i);
			}

			std::reverse(path.begin(), path.end());
			return path;
		}

		currentNode->myStatus = NodeStatus::Closed;

		for (int yNeighbour = -1; yNeighbour <= 1; yNeighbour++)
		{
			for (int xNeighbour = -1; xNeighbour <= 1; xNeighbour++)
			{
				int offsetX = currentNode->myPos.x + xNeighbour;
				int offsetY = currentNode->myPos.y + yNeighbour;

				if (offsetX < 0 || offsetX >= MapWidth) continue; //Skip incase of out of bounds
				if (offsetY < 0 || offsetY >= MapHeight) continue; //Skip incase of out of bounds
				if (xNeighbour != 0 && yNeighbour != 0) continue; //Skip diagonals
				if (xNeighbour == 0 && yNeighbour == 0) continue; //Skip center
				Node& neighbour = myNodes[offsetX + MapWidth * offsetY];

				if (neighbour.myStatus == NodeStatus::Closed) continue; //Skip if node is closed
				if (aMap[offsetX + MapWidth * offsetY] == Tile::Impassable) continue; //Skip if node is impassable

				int gCost = currentNode->myGCost + static_cast<int>(Vector2<int>::Distance(neighbour.myPos, currentNode->myPos));



				if (gCost < neighbour.myGCost)
				{
					neighbour.myGCost = gCost;
					neighbour.myPredecessor = currentNode;

					//myOpenNodes.Reset();
					if (neighbour.myStatus != NodeStatus::Open)
					{
						neighbour.myStatus = NodeStatus::Open;
						myOpenNodes.Enqueue(neighbour);
					}

				}


				aDebugPath->push_back(neighbour.myPos.x + MapWidth * neighbour.myPos.y);



			}
		}
	}


	return path;

}
