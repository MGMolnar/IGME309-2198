#include "AppClass.h"
void Application::InitVariables(void)
{
	//init the mesh
	m_pMesh = new MyMesh();
	m_pMesh->GenerateCube(1.0f, C_BLACK);

	//m_pMesh->GenerateSphere(1.0f, 5, C_WHITE);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	
	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(2.0f,2.0f,2.0f));
	matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(value, 2.0f, 3.0f));

	//once the value has reached a certain point value changer will
	//make it move in the opposite way
	if (value >= 2.0f || value <= -2.0f)
	{
		valueChanger *= -1;
	}

	value += valueChanger;

	//trasnlates for x, will be used to move the model in the x direction using a grid
	matrix4 xTranslate0 = glm::translate(IDENTITY_M4, vector3(value + 0.0f, 0.0f, 3.0f));
	matrix4 xTranslate1 = glm::translate(IDENTITY_M4, vector3(value + 1.0f, 0.0f, 3.0f));
	matrix4 xTranslate2 = glm::translate(IDENTITY_M4, vector3(value + 2.0f, 0.0f, 3.0f));
	matrix4 xTranslate3 = glm::translate(IDENTITY_M4, vector3(value + 3.0f, 0.0f, 3.0f));
	matrix4 xTranslate4 = glm::translate(IDENTITY_M4, vector3(value + 4.0f, 0.0f, 3.0f));
	matrix4 xTranslate5 = glm::translate(IDENTITY_M4, vector3(value + 5.0f, 0.0f, 3.0f));
	matrix4 xTranslate6 = glm::translate(IDENTITY_M4, vector3(value + 6.0f, 0.0f, 3.0f));
	matrix4 xTranslate7 = glm::translate(IDENTITY_M4, vector3(value + 7.0f, 0.0f, 3.0f));
	matrix4 xTranslate8 = glm::translate(IDENTITY_M4, vector3(value + 8.0f, 0.0f, 3.0f));
	matrix4 xTranslate9 = glm::translate(IDENTITY_M4, vector3(value + 9.0f, 0.0f, 3.0f));
	matrix4 xTranslate10 = glm::translate(IDENTITY_M4, vector3(value + 10.0f, 0.0f, 3.0f));
	matrix4 xTranslate11 = glm::translate(IDENTITY_M4, vector3(value + 11.0f, 0.0f, 3.0f));
	
	//trasnlates for y, will be used to move the model in the x direction using a grid
	matrix4 yTranslate1 = glm::translate(IDENTITY_M4, vector3(0.0f, 1.0f, 0.0f));
	matrix4 yTranslate2 = glm::translate(IDENTITY_M4, vector3(0.0f, 2.0f, 0.0f));
	matrix4 yTranslate3 = glm::translate(IDENTITY_M4, vector3(0.0f, 3.0f, 0.0f));
	matrix4 yTranslate4 = glm::translate(IDENTITY_M4, vector3(0.0f, 4.0f, 0.0f));
	matrix4 yTranslate5 = glm::translate(IDENTITY_M4, vector3(0.0f, 5.0f, 0.0f));
	matrix4 yTranslate6 = glm::translate(IDENTITY_M4, vector3(0.0f, 6.0f, 0.0f));
	matrix4 yTranslate7 = glm::translate(IDENTITY_M4, vector3(0.0f, 7.0f, 0.0f));
	matrix4 yTranslate8 = glm::translate(IDENTITY_M4, vector3(0.0f, 8.0f, 0.0f));

	//matrix4 m4Model = m4Translate * m4Scale;
	matrix4 m4Model = m4Scale * xTranslate3;
	m_pMesh->Render(m4Projection, m4View, m4Model);

	//all the translations below are multiplied by the scale and set to the M4Model. 
	//Each one is multiplied by either a x or y translate then rendered below that. 
	//The translates are set in a grid pattern. EX: 6 x translates and 5 y translates 
	//means the object will be found in the 6th column over and 5 rows up.

	m4Model = m4Scale * xTranslate4;
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * xTranslate6;
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * xTranslate7;
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate1 * xTranslate0);
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate1 * xTranslate2);
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate1 * xTranslate8);
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate1 * xTranslate10);
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate2 * xTranslate0 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate2 * xTranslate2 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate2 * xTranslate3);
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate2 * xTranslate4);
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate2 * xTranslate5);
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate2 * xTranslate6);
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate2 * xTranslate7);
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate2 * xTranslate8);
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate2 * xTranslate10);
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate3 * xTranslate0);
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate3 * xTranslate1);
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate3 * xTranslate2 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate3 * xTranslate3 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate3 * xTranslate4 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate3 * xTranslate5 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate3 * xTranslate6 );
	m_pMesh->Render(m4Projection, m4View, m4Model);


	m4Model = m4Scale * (yTranslate3 * xTranslate7 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate3 * xTranslate8 );
	m_pMesh->Render(m4Projection, m4View, m4Model);


	m4Model = m4Scale * (yTranslate3 * xTranslate9 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate3 * xTranslate10);
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate4 * xTranslate1 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate4 * xTranslate2 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate4 * xTranslate4 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate4 * xTranslate5 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate4 * xTranslate6 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate4 * xTranslate8 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate4 * xTranslate9 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate5 * xTranslate2 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate5 * xTranslate3 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate5 * xTranslate4 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate5 * xTranslate5 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate5 * xTranslate6 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate5 * xTranslate7 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate5 * xTranslate8 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate6 * xTranslate3 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate6 * xTranslate7 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate7 * xTranslate2 );
	m_pMesh->Render(m4Projection, m4View, m4Model);

	m4Model = m4Scale * (yTranslate7 * xTranslate8 );
	m_pMesh->Render(m4Projection, m4View, m4Model);


	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}