#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue,-fValue, fValue); //0
	vector3 point1( fValue,-fValue, fValue); //1
	vector3 point2( fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue,-fValue,-fValue); //4
	vector3 point5( fValue,-fValue,-fValue); //5
	vector3 point6( fValue, fValue,-fValue); //6
	vector3 point7(-fValue, fValue,-fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	float twoPI = PI * 2;
	std::vector<vector3> vertexList;


	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();


	//center point
	vertexList.push_back(vector3(0.0f, 0.0f, (a_fHeight / 2)));

	//for loop that calculates where all the vertexs will be
	for (int i = 0; i < a_nSubdivisions + 1; i++)
	{
		vertexList.push_back(vector3((a_fRadius * cos(i * twoPI / a_nSubdivisions)), (a_fRadius * sin(i * twoPI / a_nSubdivisions)), (-a_fHeight / 2)));
	}

	//for loop to add all the sub divisions into triangles
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		AddTri(vertexList[0], vertexList[i + 1], vertexList[i + 2]);
	}

	//for loop that will flip all the points in the y direction to show the circle
	for (int i = 0; i < a_nSubdivisions + 1; i++)
	{
		vertexList[i].y *= -1;
	}

	//move the center point to be in the center of the base
	vertexList[0] = (vector3(0.0f, 0.0f, (-a_fHeight / 2)));
	
	//for loop to add all the sub divisions into triangles
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		AddTri(vertexList[0], vertexList[i + 1], vertexList[i + 2]);
	} 


	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	//double pi
	float twoPI = PI * 2;

	//vertex list
	std::vector<vector3> vertexList;

	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//center point
	vertexList.push_back(vector3(0.0f, 0.0f, (a_fHeight / 2)));

	//for loop that calculates where all the vertexs will be
	for (int i = 0; i < a_nSubdivisions + 1; i++)
	{
		vertexList.push_back(vector3((a_fRadius * cos(i * twoPI / a_nSubdivisions)), (a_fRadius * sin(i * twoPI / a_nSubdivisions)), (a_fHeight/ 2)));
	}

	//for loop to add all the sub divisions into triangles
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		AddTri(vertexList[0], vertexList[i + 1], vertexList[i + 2]);
	}

	vertexList.clear();

	//center point
	vertexList.push_back(vector3(0.0f, 0.0f, (-a_fHeight / 2)));

	//for loop that calculates where all the vertexs will be
	for (int i = 0; i < a_nSubdivisions + 1; i++)
	{
		vertexList.push_back(vector3((a_fRadius * cos(i * twoPI / a_nSubdivisions)), (a_fRadius * sin(i * twoPI / a_nSubdivisions)), (-a_fHeight / 2)));
	}

	//for loop to add all the sub divisions into triangles
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		AddTri(vertexList[0], vertexList[i + 2], vertexList[i + 1]);
	}
	vertexList.clear();

	//for loop to create the two vertical verticies one for at the top and one at the bottom
	for (int i = 0; i < (a_nSubdivisions * 2) + 2; i++)
	{
		vertexList.push_back(vector3((a_fRadius * cos(i * twoPI / a_nSubdivisions)), (a_fRadius * sin(i * twoPI / a_nSubdivisions)), (a_fHeight / 2)));
		vertexList.push_back(vector3((a_fRadius * cos(i * twoPI / a_nSubdivisions)), (a_fRadius * sin(i * twoPI / a_nSubdivisions)), (-a_fHeight / 2)));
	}

	//3--2
	//|  |
	//0--1

	//adds the quads to create the cylinder
	for (int i = 0; i < (a_nSubdivisions * 2) + 2; i++)
	{
		AddQuad(vertexList[i], vertexList[i + 1], vertexList[i + 2], vertexList[i + 3]);
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	//double pi
	float twoPI = PI * 2;

	//vertex list
	std::vector<vector3> vertexList;

	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;


	Release();
	Init();

	//for loop that calculates where all the verteices for the top of the tube
	for (int i = 0; i < a_nSubdivisions * 2; i++)
	{
		vertexList.push_back(vector3((a_fInnerRadius * cos(i * twoPI / a_nSubdivisions)), (a_fInnerRadius * sin(i * twoPI / a_nSubdivisions)), (a_fHeight / 2)));
		vertexList.push_back(vector3((a_fOuterRadius * cos(i * twoPI / a_nSubdivisions)), (a_fOuterRadius * sin(i * twoPI / a_nSubdivisions)), (a_fHeight / 2)));
	}

	//for loop to add all the vertices for the top of the tube
	for (int i = 0; i < a_nSubdivisions * 2; i++)
	{
		AddQuad(vertexList[i], vertexList[i + 1], vertexList[i + 2], vertexList[i + 3]);
	}

	vertexList.clear();

	//for loop that will create the verticies for the bottom of the tube
	for (int i = 0; i < a_nSubdivisions * 2; i++)
	{
		vertexList.push_back(vector3((a_fInnerRadius * cos(i * twoPI / a_nSubdivisions)), (a_fInnerRadius * sin(i * twoPI / a_nSubdivisions)), (-a_fHeight / 2)));
		vertexList.push_back(vector3((a_fOuterRadius * cos(i * twoPI / a_nSubdivisions)), (a_fOuterRadius * sin(i * twoPI / a_nSubdivisions)), (-a_fHeight / 2)));
	}

	//for loop to add all the inner tube vertices
	for (int i = 0; i < a_nSubdivisions * 2; i++)
	{
		AddQuad(vertexList[i], vertexList[i + 1], vertexList[i + 2], vertexList[i + 3]);
	}

	vertexList.clear();

	//for loop to create the two vertical verticies for the otuer wall
	for (int i = 0; i < (a_nSubdivisions * 2) + 2; i++)
	{
		vertexList.push_back(vector3((a_fOuterRadius * cos(i * twoPI / a_nSubdivisions)), (a_fOuterRadius * sin(i * twoPI / a_nSubdivisions)), (a_fHeight / 2)));
		vertexList.push_back(vector3((a_fOuterRadius * cos(i * twoPI / a_nSubdivisions)), (a_fOuterRadius * sin(i * twoPI / a_nSubdivisions)), (-a_fHeight / 2)));
	}

	//adds the quads to create the outer cylinder
	for (int i = 0; i < (a_nSubdivisions * 2) + 2; i++)
	{
		AddQuad(vertexList[i], vertexList[i + 1], vertexList[i + 2], vertexList[i + 3]);
	}


	vertexList.clear();

	//for loop to create the vertices for the inner radius
	for (int i = 0; i < (a_nSubdivisions * 2) + 2; i++)
	{
		vertexList.push_back(vector3((a_fInnerRadius * cos(i * twoPI / a_nSubdivisions)), (a_fInnerRadius * sin(i * twoPI / a_nSubdivisions)), (a_fHeight / 2)));
		vertexList.push_back(vector3((a_fInnerRadius * cos(i * twoPI / a_nSubdivisions)), (a_fInnerRadius * sin(i * twoPI / a_nSubdivisions)), (-a_fHeight / 2)));
	}

	//adds the quads to create the inner cylinder
	for (int i = 0; i < (a_nSubdivisions * 2) + 2; i++)
	{
		AddQuad(vertexList[i], vertexList[i + 1], vertexList[i + 2], vertexList[i + 3]);
	}

	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	//double pi
	float twoPI = PI * 2;

	//angle from top to bottom on the object
	float yAngle = 0.0f;

	//angle from left to right on the object.
	float xAngle = 0.0f;

	//vertex list
	std::vector<vector3> vertexList;

	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();
	

	//this is a nested for loop first to calculate the torus
	//first for loop is to calculate from top to bottom the angle that the vertices will be placed on
	//the second for loop will calculate where teh vertices go in the x direction and will also account for
	//the middle of the torus as well.
	for (int i = 0; i <= a_nSubdivisionsB * 3; i++)
	{
		yAngle = (PI / 2) - PI * (i / (float)a_nSubdivisionsB);

		for (int j = 0; j <= a_nSubdivisionsA; j++)
		{
			xAngle = twoPI * (j / (float)a_nSubdivisionsA);

			vertexList.push_back(vector3((a_fOuterRadius + (a_fInnerRadius * cos(yAngle))) * cos(xAngle), (a_fOuterRadius + (a_fInnerRadius * cos(yAngle))) * sin(xAngle), a_fInnerRadius * sin(yAngle)));
		}
	}

	//this will add the vertices and add them into a square before sending them to be drawn
	for (size_t i = 0; i <= ((a_nSubdivisionsB * 3) * a_nSubdivisionsA); i++)
	{
		AddQuad(vertexList[i + 1], vertexList[i], vertexList[(i + a_nSubdivisionsA) + 1], vertexList[(i + a_nSubdivisionsA)]);
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, float a_nSubdivisions, vector3 a_v3Color)
{	
	//double pi
	float twoPI = PI * 2;
	
	//angle from top to bottom on the object
	float yAngle = 0.0f;

	//angle from left to right on the object.
	float xAngle = 0.0f;

	//vertex list
	std::vector<vector3> vertexList;


	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	//this is a nested for loop first to calculate how how far down on the circle
	//the vertexes have to go then the second for loop will calculate where the vertices go in the x direction
	//or around the circle on that y level.
	for (int i = 0; i <= a_nSubdivisions; i++)
	{
		yAngle = (PI / 2) - PI * (i / a_nSubdivisions);

		for (int j = 0; j <= a_nSubdivisions; j++)
		{
			xAngle = twoPI * (j / a_nSubdivisions);

			vertexList.push_back(vector3(((a_fRadius * cos(yAngle)) * cos(xAngle)), ((a_fRadius * cos(yAngle)) * sin(xAngle)), a_fRadius * sin(yAngle)));
		}
	}

	//this will add the vertices and add them into a square before sending them to be drawn
	for (size_t i = 0; i <= (a_nSubdivisions * a_nSubdivisions) + 4; i++)
	{
		AddQuad(vertexList[i + 1], vertexList[i], vertexList[(i + a_nSubdivisions) + 1], vertexList[(i + a_nSubdivisions)]);
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}