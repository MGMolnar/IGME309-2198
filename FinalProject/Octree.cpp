#include "Octree.h"
using namespace Simplex;
//Octree()

//Simplex

Simplex::Octree::Octree(uint vOctantLevel, uint vNodeNumber, float vExtentSize, MyEntityManager* vEntityManger)
{
	maxSub = 4;
	octantLevel = vOctantLevel;
	nodeNumber = vNodeNumber;
	m4ToWorld = IDENTITY_M4;
	extentSize = vExtentSize;
	entityManger = vEntityManger;
	
	if (octantLevel == 0)
	{
		uniqueID = 0;
	}
}

//Destructor
Simplex::Octree::~Octree()
{
	delete rigidBody;
	rigidBody = nullptr;
}

//Returns the matrix 4 to world
matrix4 Simplex::Octree::GetModelMatrix()
{
	return m4ToWorld;
}

//Uses the sub levels to calculate the amount of current octants
int Simplex::Octree::GetOctantCount()
{
	int total = 0;
	
	//for loop that will add to the total based on an equation of powers.
	for (int i = 0; i < subLevels; i++)
	{
		total += (8 * pow(8, i));
	}

	return total;
}

//method that will return the children of an octant
std::vector<Octree*> Simplex::Octree::GetNodes()
{
	return nodes;
}

//method that will set the parent and also create the unique ID 
//based on the uniqueID of the parent and the octant's node number
void Simplex::Octree::SetParent(Octree* vParent)
{
	parent = vParent;
	uniqueID = (parent->uniqueID * 8) + nodeNumber;
}


//Method that will take an interger value and loop through all the 
//octants to either display the specified value or to display everything if the incoming
//integer value is 0
void Simplex::Octree::Display(uint octreeID)
{
	//checks to see if the incoming integer is less than or equal to zero
	//if so it will display everything
	if (octreeID <= 0)
	{
		//Once it gets to an octant with no children
		//stop looping there and add that to the render list
		if (children == false)
		{
			rigidBody->AddToRenderList();
		}
		else
		{
			//if the octant has children loop through them and display them
			for (int i = 0; i < nodes.size(); i++)
			{
				nodes[i]->Display(octreeID);
			}

			//once that is done add this octant to the renderlist
			rigidBody->AddToRenderList();
		}
	}
	else
	{
		//once the correct id is found add that rigidbody to the render list
		if (uniqueID == octreeID)
		{
			rigidBody->AddToRenderList();
		}
		//otherwise keep looping through the children to find the correct octant to render
		else
		{
			if (children)
			{
				for (int i = 0; i < nodes.size(); i++)
				{
					nodes[i]->Display(octreeID);
				}
			}
		}
	}
}

//Method that configure the current octant based on the node number
//This method will calculate the matrix 4, the center, the max and min points.
//Lastly once that is done it will loop through the models of the parent and
//find the ones that are inside the octant and add that to the list of models
void Simplex::Octree::ConfigureOctree()
{
	//checks to make sure this is not the 
	//very first octant.
	if (octantLevel != 0)
	{
		//takes the incoming size of the sides and halves them.
		extentSize = extentSize / 2.0f;


		//if statements that will figure out which node number the incoming octant is based out of the 8 children
		if (nodeNumber == 1)
		{
			//calculates the center min and max points based on the extention size, parent center and the octant's center
			//this set of vectors will create the front top left octant
			center = vector3(parent->center.x - extentSize / 2.0f, parent->center.y + extentSize / 2.0f, parent->center.z + extentSize / 2.0f);
			maxPoints = vector3(center.x + extentSize, center.y + extentSize, center.z + extentSize);
			minPoints = vector3(center.x - extentSize, center.y - extentSize, center.z - extentSize);

			//creates the matrix4 based upon the transformation of the center.
			m4ToWorld = glm::translate(IDENTITY_M4, center);

		}
		else if (nodeNumber == 2)
		{
			//calculates the center min and max points based on the extention size, parent center and the octant's center
			//this set of vectors will create the back top left octant
			center = vector3(parent->center.x - extentSize / 2.0f, parent->center.y + extentSize / 2.0f, parent->center.z - extentSize / 2.0f);
			maxPoints = vector3(center.x + extentSize, center.y + extentSize, center.z + extentSize);
			minPoints = vector3(center.x - extentSize, center.y - extentSize, center.z - extentSize);

			//creates the matrix4 based upon the transformation of the center.
			m4ToWorld = glm::translate(IDENTITY_M4, center);
		}
		else if (nodeNumber == 3)
		{
			//calculates the center min and max points based on the extention size, parent center and the octant's center
			//this set of vectors will create the back top right octant
			center = vector3(parent->center.x + extentSize / 2.0f, parent->center.y + extentSize / 2.0f, parent->center.z - extentSize / 2.0f);
			maxPoints = vector3(center.x + extentSize, center.y + extentSize, center.z + extentSize);
			minPoints = vector3(center.x - extentSize, center.y - extentSize, center.z - extentSize);
		
			//creates the matrix4 based upon the transformation of the center.
			m4ToWorld = glm::translate(IDENTITY_M4, center);
		}
		else if (nodeNumber == 4)
		{
			//calculates the center min and max points based on the extention size, parent center and the octant's center
			//this set of vectors will create the front top right octant
			center = vector3(parent->center.x + extentSize / 2.0f, parent->center.y + extentSize / 2.0f, parent->center.z + extentSize / 2.0f);
			maxPoints = vector3(center.x + extentSize, center.y + extentSize, center.z + extentSize);
			minPoints = vector3(center.x - extentSize, center.y - extentSize, center.z - extentSize);
			
			//creates the matrix4 based upon the transformation of the center.
			m4ToWorld = glm::translate(IDENTITY_M4, center);
		}
		else if (nodeNumber == 5)
		{
			//calculates the center min and max points based on the extention size, parent center and the octant's center
			//this set of vectors will create the front bottom left octant
			center = vector3(parent->center.x - extentSize / 2.0f, parent->center.y - extentSize / 2.0f, parent->center.z + extentSize / 2.0f);
			maxPoints = vector3(center.x + extentSize, center.y + extentSize, center.z + extentSize);
			minPoints = vector3(center.x - extentSize, center.y - extentSize, center.z - extentSize);
		
			//creates the matrix4 based upon the transformation of the center.
			m4ToWorld = glm::translate(IDENTITY_M4, center);
		}
		else if (nodeNumber == 6)
		{
			//calculates the center min and max points based on the extention size, parent center and the octant's center
			//this set of vectors will create the back bottom left octant
			center = vector3(parent->center.x - extentSize / 2.0f, parent->center.y - extentSize / 2.0f, parent->center.z - extentSize / 2.0f);
			maxPoints = vector3(center.x + extentSize, center.y + extentSize, center.z + extentSize);
			minPoints = vector3(center.x - extentSize, center.y - extentSize, center.z - extentSize);
		
			//creates the matrix4 based upon the transformation of the center.
			m4ToWorld = glm::translate(IDENTITY_M4, center);
		}
		else if (nodeNumber == 7)
		{
			//calculates the center min and max points based on the extention size, parent center and the octant's center
			//this set of vectors will create the back bottom right octant
			center = vector3(parent->center.x + extentSize / 2.0f, parent->center.y - extentSize / 2.0f, parent->center.z - extentSize / 2.0f);
			maxPoints = vector3(center.x + extentSize, center.y + extentSize, center.z + extentSize);
			minPoints = vector3(center.x - extentSize, center.y - extentSize, center.z - extentSize);
		
			//creates the matrix4 based upon the transformation of the center.
			m4ToWorld = glm::translate(IDENTITY_M4, center);
		}
		else if (nodeNumber == 8)
		{
			//calculates the center min and max points based on the extention size, parent center and the octant's center
			//this set of vectors will create the front bottom right octant
			center = vector3(parent->center.x + extentSize / 2.0f, parent->center.y - extentSize / 2.0f, parent->center.z + extentSize / 2.0f);
			maxPoints = vector3(center.x + extentSize, center.y + extentSize, center.z + extentSize);
			minPoints = vector3(center.x - extentSize, center.y - extentSize, center.z - extentSize);
		
			//creates the matrix4 based upon the transformation of the center.
			m4ToWorld = glm::translate(IDENTITY_M4, center);
		}

		//calculates the max and min global point values
		vector3 maxPointsGlobal = vector3(vector4(maxPoints, 1.0f) * m4ToWorld);
		vector3 minPointsGlobal = vector3(vector4(minPoints, 1.0f) * m4ToWorld);

		//now for loop to loop through all the models that the parent holds
		for (int i = 0; i < parent->models.size(); i++)
		{
			//finds the model's global max, min, and center vector values
			vector3 modelMax = parent->models[i]->GetRigidBody()->GetMaxGlobal();
			vector3 modelMin = parent->models[i]->GetRigidBody()->GetMinGlobal();
			vector3 modelCenter = parent->models[i]->GetRigidBody()->GetCenterGlobal();
			
			//checks to see if the center is within the bounds of the octant if so add it to the list of models
			if ((maxPointsGlobal.x > modelCenter.x && maxPointsGlobal.y > modelCenter.y && maxPointsGlobal.z > modelCenter.z)
				&& (minPointsGlobal.x < modelCenter.x && minPointsGlobal.y < modelCenter.y && minPointsGlobal.z < modelCenter.z))
			{
				models.push_back(parent->models[i]);
			}
			//checks to see if the bounds of the model are within the octant bounds and if so add them to the list of models
			else if((maxPointsGlobal.x < modelMax.x && maxPointsGlobal.y < modelMax.y && maxPointsGlobal.z < modelMax.z)
				|| (minPointsGlobal.x > modelMax.x && minPointsGlobal.y > modelMax.y && minPointsGlobal.z > modelMax.z))
			{
				models.push_back(parent->models[i]);
			}
		}
	}
	//Now if this octant is the very first octant create the center min and max points
	//then add all the models into the model list
	else
	{
		//creates the center min and max points based on the middle of the screen and the extention of the sides
		center = vector3(0.0f, 0.0f, 0.0f);
		maxPoints = vector3(center.x + extentSize, center.y + extentSize, center.z + extentSize);
		minPoints = vector3(center.x - extentSize, center.y - extentSize, center.z - extentSize);

		//for loop that will add all the models in the world and add them to the model list
		for (int i = 0; i < entityManger->GetEntityCount(); i++)
		{
			models.push_back(entityManger->GetEntity(i));
		}

		//update the rigidbody for the very first octant
		UpdateRigidBodies();
		
	}
}

//Method that will go through and add a subdivision to the octree
void Simplex::Octree::AddDimension()
{
	//checks to make sure that the max amount of subdivisions is created
	if (octantLevel == maxSub)
	{
		return;
	}

	//adds to the sub levels but only to the very first octant
	if (octantLevel == 0)
	{
		subLevels++;
	}

	//checks to see whther this current octant has children or not
	//if it doesn't have children cretae them or loop through the octree to find
	//one that doesn't have children
	if (children == false)
	{
		//for loop to create 8 node children for the current octant
		 for (int i = 0; i < 8; i++)
		{
			nodes.push_back(new Octree(uint(octantLevel + 1), uint(i + 1), extentSize, entityManger));

			//set the parent and configure that octant to the screen
			nodes[i]->SetParent(this);
			nodes[i]->ConfigureOctree();

			children = true;

		}

		 //update the rigidbodies of the new nodes
		 UpdateRigidBodies();
	}
	//if this octant has children loop through those children to find 
	//one without any children
	else
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->AddDimension();
		}
	}

}

//loops through the octree and find the lowest level children to be removed
//from the octree
void Simplex::Octree::RemoveDimension()
{
	//will stop the function if there are no children on the very first octant
	if (octantLevel == 0 && children == false)
	{
		return;
	}

	//remove a sub division level but only on the 
	//very first octant
	if (octantLevel == 0)
	{
		subLevels--;
	}

	//checks to see if the current octant has children or not
	if (children)
	{
		//checks to see if the nodes have children is so then loop through
		//and remove those children instead
		if (nodes[0]->children)
		{
			for (int i = 0; i < nodes.size(); i++)
			{
				nodes[i]->RemoveDimension();
			}
		}
		//else if they do not remove the children of this octant
		else
		{
			//for loop to delete all the nodes and clear the node list
			for (int i = 0; i < nodes.size(); i++)
			{
				delete nodes[i];
				nodes[i] = nullptr;

				nodes.clear();
			}

			
			children = false;
		}
	}

}

//Method that will check the collision of the models
void Simplex::Octree::CheckCollision()
{
	//checks to make sure this octant doesn't have children
	if (children == false)
	{
		//nested for loop to have each model check collision upon the other models
		//Makes sure that two models are never checked twice.
		for (int i = 0; i < models.size(); i++)
		{
			if (i != models.size())
			{
				for (int j = i + 1; j < models.size(); j++)
				{
					models[i]->IsColliding(models[j]);
				}
			}
		}
	}
	//if the octant has children then 
	//check the collision of those children
	else
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->CheckCollision();
		}
	}
}

//Updates the rigid bodies within all the octants
void Simplex::Octree::UpdateRigidBodies()
{
	//if this octant has no children
	//create the rigid body for this octant
	if (children == false)
	{
		points.push_back(maxPoints);
		points.push_back(minPoints);

		delete rigidBody;
		rigidBody = new MyRigidBody(points);
		rigidBody->SetModelMatrix(m4ToWorld);

		points.clear();
	}
	//else if this octant has children
	//then loop through those children to update their rigid bodies
	else
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->UpdateRigidBodies();
		}

		//afterwards update the rigidbody of the current octant so then every 
		//octant has a rigidbody
		points.push_back(maxPoints);
		points.push_back(minPoints);

		delete rigidBody;
		rigidBody = new MyRigidBody(points);
		rigidBody->SetModelMatrix(m4ToWorld);

		points.clear();
	}
}

