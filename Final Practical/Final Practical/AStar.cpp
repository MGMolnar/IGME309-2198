#include "AStar.h"

using namespace Simplex;


AStar::AStar()
{
	
}

AStar::~AStar()
{
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			delete cubes[i][j];
			cubes[i][j] = nullptr;
		}
	}

}

//sets an entity to the variable current node
void Simplex::AStar::SetCurrentNode(MyEntity* vCurrentNode)
{
	currentNode = vCurrentNode;
}

//method that will loop through all the blocks
//in the scene and render them to the screen
void Simplex::AStar::AddToRenderList()
{
	//nested for loop that will go through
	//the 2D array and add everything that isn't a null pointer
	//and render them
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			if (cubes[i][j] != nullptr)
			{
				cubes[i][j]->AddToRenderList();
			}
		}
	}
}

//method that will add the neighbors to each Entity in the 2D array
//This will check to see if the entitys are on the edges of the 2D array
//to make sure the correct amount of connections are added and to add
//the path values to that connection
void Simplex::AStar::AddNeighbors()
{
	//nested for loop that will check to make sure that
	//each cubes is not a null pointer
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			if (cubes[i][j] != nullptr)
			{
				if (j == 0)
				{
					//will check for the corner
					//and add two connections
					//and their path values
					if (i == 0)
					{
						cubes[i][j]->connections.push_back(cubes[i + 1][j]);
						cubes[i][j]->connections.push_back(cubes[i][j + 1]);
						cubes[i][j]->paths.push_back(glm::linearRand(0, 10));
						cubes[i][j]->paths.push_back(glm::linearRand(0, 10));

					}
					//checks the next corner and adds
					//the two connections.
					else if (i == 19)
					{
						cubes[i][j]->connections.push_back(cubes[i - 1][j]);
						cubes[i][j]->connections.push_back(cubes[i][j + 1]);
						cubes[i][j]->paths.push_back(glm::linearRand(0, 10));
						cubes[i][j]->paths.push_back(glm::linearRand(0, 10));
					}
					//anything else along this edge
					//will get three connections
					//and add their path values
					else
					{
						cubes[i][j]->connections.push_back(cubes[i + 1][j]);
						cubes[i][j]->connections.push_back(cubes[i][j + 1]);
						cubes[i][j]->connections.push_back(cubes[i - 1][j]);
						cubes[i][j]->paths.push_back(glm::linearRand(0, 10));
						cubes[i][j]->paths.push_back(glm::linearRand(0, 10));
						cubes[i][j]->paths.push_back(glm::linearRand(0, 10));
					}
				}
				//checks the other edge case
				else if (j == 19)
				{
					//checks a corner which only gets
					//two connections and path values
					if (i == 0)
					{
						cubes[i][j]->connections.push_back(cubes[i + 1][j]);
						cubes[i][j]->connections.push_back(cubes[i][j - 1]);
						cubes[i][j]->paths.push_back(glm::linearRand(0, 10));
						cubes[i][j]->paths.push_back(glm::linearRand(0, 10));
					}
					//checks for the last corner of the 2D array
					//and adds the connections and path values
					else if (i == 19)
					{
						cubes[i][j]->connections.push_back(cubes[i - 1][j]);
						cubes[i][j]->connections.push_back(cubes[i][j - 1]);
						cubes[i][j]->paths.push_back(glm::linearRand(0, 10));
						cubes[i][j]->paths.push_back(glm::linearRand(0, 10));
					}
					//checks the rest of the edge and adds their connections and path values
					else
					{
						cubes[i][j]->connections.push_back(cubes[i + 1][j]);
						cubes[i][j]->connections.push_back(cubes[i][j - 1]);
						cubes[i][j]->connections.push_back(cubes[i - 1][j]);
						cubes[i][j]->paths.push_back(glm::linearRand(0, 10));
						cubes[i][j]->paths.push_back(glm::linearRand(0, 10));
						cubes[i][j]->paths.push_back(glm::linearRand(0, 10));
					}
				}
				//this is the third edge but doesn't check for the corner as that has
				//already been checked for. Adds their connections and path values
				else if (i == 0)
				{
					cubes[i][j]->connections.push_back(cubes[i + 1][j]);
					cubes[i][j]->connections.push_back(cubes[i][j - 1]);
					cubes[i][j]->connections.push_back(cubes[i][j + 1]);
					cubes[i][j]->paths.push_back(glm::linearRand(0, 10));
					cubes[i][j]->paths.push_back(glm::linearRand(0, 10));
					cubes[i][j]->paths.push_back(glm::linearRand(0, 10));
				}
				//Last edge case and adds the connections and path values
				else if (i == 19)
				{
					cubes[i][j]->connections.push_back(cubes[i - 1][j]);
					cubes[i][j]->connections.push_back(cubes[i][j - 1]);
					cubes[i][j]->connections.push_back(cubes[i][j + 1]);
					cubes[i][j]->paths.push_back(glm::linearRand(0, 10));
					cubes[i][j]->paths.push_back(glm::linearRand(0, 10));
					cubes[i][j]->paths.push_back(glm::linearRand(0, 10));
				}
				//This is any cube in the middle of the 2D array
				//that will have 4 connections and each given their own path values
				else
				{
					cubes[i][j]->connections.push_back(cubes[i - 1][j]);
					cubes[i][j]->connections.push_back(cubes[i + 1][j]);
					cubes[i][j]->connections.push_back(cubes[i][j - 1]);
					cubes[i][j]->connections.push_back(cubes[i][j + 1]);
					cubes[i][j]->paths.push_back(glm::linearRand(0, 10));
					cubes[i][j]->paths.push_back(glm::linearRand(0, 10));
					cubes[i][j]->paths.push_back(glm::linearRand(0, 10));
					cubes[i][j]->paths.push_back(glm::linearRand(0, 10));
				}
			}
		}
	}
}

//Method to find the shortest path from the currentNode given and the given end node
//Will check everything around a single node adding them to an active list if they
//havene't been tested already. Then will find the shortest path out of all the active nodes
//before changing the current node to the next shortest path and adding the current node to the tested path values.
void Simplex::AStar::ShortestPath(MyEntity* vCurrentNode)
{

	//for loop that goes through all the connections of the current node.
	for (int i = 0; i < currentNode->connections.size(); i++)
	{
		//makes sure that this connections isn't a null pointer
		if (currentNode->connections[i] != nullptr)
		{
			//nested for loop to make sure that this connection has not already been tested
			for (int j = 0; j < testedNodes.size(); j++)
			{
				if (currentNode->connections[i] == testedNodes[j])
				{
					inTested = true;
				}
			}

			//if the connection has not been teseted before and the path to that connection
			//plus the current nodes distance is smaller than the current shortest path for that connection
			if ((currentNode->paths[i] + currentNode->GetDistanceLength()) 
				< currentNode->connections[i]->GetShortestPath()
				&& inTested == false)
			{
				//then update the shortest path, the previous node to the current one, and add it to the active nodes list.
				currentNode->connections[i]->SetShortestPath(currentNode->paths[i] + currentNode->GetDistanceLength());
				currentNode->connections[i]->previousNode = currentNode;
				activeNodes.push_back(currentNode->connections[i]);
			}

			//if this connection was in tested, set the boolean in tested back
			//to false for the next connection
			inTested = false;
		}
	}

	//removes any nodes in the activeNodes that are equal to the currentNode
	remove(activeNodes.begin(), activeNodes.end(), currentNode);

	//moves the currentNode into tested nodes
	testedNodes.push_back(currentNode);
	
	//a temporary large number
	int tempShortestPath = 10000000;

	//for loop to loop through all the active nodes
	for (int i = 0; i < activeNodes.size(); i++)
	{
		//will check all the active nodes and see if they are smaller
		//than the integer value
		if (activeNodes[i]->GetShortestPath() < tempShortestPath)
		{
			//once found the shorter path update the temporary integer value 
			//and have current node be that node.
			tempShortestPath = activeNodes[i]->GetShortestPath();
			currentNode = activeNodes[i];
		}
	}
	
	//run the program again until the end is hit
	if (currentNode != cubes[19][19])
	{
		ShortestPath(currentNode);
	}

	
}

//Method that takes in a current node and add all the previous nodes starting
//from the end into an array
void Simplex::AStar::SetFinalNodes(MyEntity* vCurrentNode)
{
	//grab the current node
	currentNode = vCurrentNode;

	//add this node to an array for the shortest path to the end
	shortestPath.push_back(currentNode);

	//now if the previous from this current node
	//is not a null point run this method again until 
	//the node does not have a previous node
	if (currentNode->previousNode != nullptr)
	{
		SetFinalNodes(currentNode->previousNode);
	}
	else
	{
		return;
	}
}



