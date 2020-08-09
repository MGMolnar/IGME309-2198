#ifndef  _ASAR_H_
#define  _ASAR_H_


#include "MyEntity.h"
#include "Definitions.h"
#include <vector>

namespace Simplex
{
	class AStar
	{
	private:

		std::vector<MyEntity*> testedNodes;
		std::vector<MyEntity*> activeNodes;
		MyEntity* currentNode = nullptr;

		bool inTested = false;

	public:

		std::vector<MyEntity*> shortestPath;

		MyEntity* cubes[20][20];

		AStar();
		~AStar();

		void AddToRenderList();
		void AddNeighbors();
		void ShortestPath(MyEntity* vCurrentNode);

		void SetCurrentNode(MyEntity* vCurrentNode);

		void SetFinalNodes(MyEntity* vCurrentNode);

	};

}

#endif
