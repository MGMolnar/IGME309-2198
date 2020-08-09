#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(0.0f, 10.0f, 18.0f), //Position
		vector3(25.0f, -10.0f, 0.0f),	//Target
		AXIS_Y);					//Up

	aStar = new AStar();

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			if (!((i >= 3 && i <= 18) && j == glm::linearRand(3,18)))
			{
				vector3 v3Position = vector3(i, -1.0f, j);
				matrix4 m4Position = glm::translate(v3Position);
				aStar->cubes[i][j] = new MyEntity("Minecraft\\Cube.obj", "Cube");
				aStar->cubes[i][j]->SetModelMatrix(m4Position);
				aStar->cubes[i][j]->SetPositon(v3Position);
				aStar->cubes[i][j]->SetDistanceLength((20 * 20) - (i * j));
			}
			else
			{
				aStar->cubes[i][j] = nullptr;
			}
		}
	}

	//make sure the final cubes distacne is equal to 0
	aStar->cubes[19][19]->SetDistanceLength(0);

	//set the current node for the astar to be the very first node
	//then go through and add the neighbors to all the nodes
	aStar->SetCurrentNode(aStar->cubes[0][0]);
	aStar->AddNeighbors();

	//run the shortest path algorithm and gather
	//the shortest paths node usign both functions
	aStar->ShortestPath(aStar->cubes[0][0]);
	aStar->SetFinalNodes(aStar->cubes[19][19]);

	steve = new MyEntity("Minecraft\\Steve.obj");
	matrix4 m4Position = glm::translate(vector3(19.5f, 0.0f, 19.5f));
	steve->SetModelMatrix(m4Position);

	creeper = new MyEntity("Minecraft\\Creeper.obj");
	m4Position = glm::translate(vector3(0.5f, 0.0f, 0.5f));
	creeper->SetModelMatrix(m4Position);

	//set the starting node for the creeper to be the very first node
	creeperNode = aStar->shortestPath.size() - 1;

	m_pEntityMngr->Update();
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
	
	//if statement to make sure the current node is 
	//within the bounds
	if (creeperNode > 0)
	{
		//check to see if the creeper and the currenet node are colliding
		if (creeper->IsColliding(aStar->shortestPath[creeperNode]))
		{
			//set the creepers movement to the current node reset the model matrix and then set the current node to the next node
			creeperMovement = (aStar->shortestPath[creeperNode]->GetRigidBody()->GetCenterGlobal());
			creeper->SetModelMatrix(IDENTITY_M4);
			creeperNode--;
		}
	}

	//checks to make sure that the current node for the creeeper is still within the range.
	if (creeperNode >= 0)
	{
		//find the next node position for the creeper to move to then add that position to the movement vector for the creeper.
		creeperMovement += (aStar->shortestPath[creeperNode]->GetRigidBody()->GetCenterGlobal() - aStar->shortestPath[creeperNode + 1]->GetRigidBody()->GetCenterGlobal()) * .01f;


		//make sure the creeper is on top of the cubes
		creeperMovement.y = 0.0f;

		//translate the movement and add to the creepers model matrix
		move = glm::translate(creeperMovement);
		creeper->SetModelMatrix(move);
	}

	//once the node for the creeper hits
	//steve end the movement by setting the node equal to -1
	if (creeper->IsColliding(steve))
	{
		creeperNode = -1;
	}

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	steve->AddToRenderList();
	creeper->AddToRenderList();
	
	aStar->AddToRenderList();

	for (int i = 0; i < aStar->shortestPath.size(); i++)
	{
		aStar->shortestPath[i]->GetRigidBody()->AddToRenderList();
	}

	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	
	//draw gui,
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	delete aStar;
	aStar = nullptr;

	//release GUI
	ShutdownGUI();
}