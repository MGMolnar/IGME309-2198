#pragma once

#include "Definitions.h"
#include "MyEntity.h"
#include "MyEntityManager.h"
#include "MyRigidBody.h"

#include <vector>

namespace Simplex
{
	class Octree
	{
	private:
		std::vector<Octree*> nodes; //holds all the children nodes that connect to the parent
		uint octantLevel; //holds the subdivision the current octant is at
		uint maxSub; //holds the maximum amount of subdivisions I can have
		Octree* parent = nullptr; //Holds the parent to the children
		uint nodeNumber; //holds the node number in the sequence of children

		vector3 center; //holds the center of the octant
		vector3 maxPoints; //holds the maximum x y and z values of the octant
		vector3 minPoints; //holds the minimum x y and z values of the octant
		float extentSize; //holds the value of the length of the sides of the octant

		float subLevels = 0; //holds the current amount of subdivisions made

		MyRigidBody* rigidBody; //holds the octants rigidbody which will be used to show the octant on the screen
		std::vector<vector3> points; //holds the maximum and minimum points of the octant for the rigidbody

		int uniqueID; //holds the unique number id for each octant

		matrix4 m4ToWorld; //holds the transformation to the world for the octant

		MyEntityManager* entityManger = nullptr; //holds the entity manger to get all the models in the scene

	public:
		std::vector<MyEntity*> models; //holds the models that are in the current scene
		bool children = false; //boolean that holds whether the current octant has a child or not
		Octree(uint vOctantLevel, uint vNodeNumber, float vExtentSize, MyEntityManager* vEntityManger); //construct octree
		
		~Octree();

		//method that will get the model matrix to the world
		matrix4 GetModelMatrix();

		//method that will return the amount of octants currently active
		int GetOctantCount();

		//returns the nodes of an octant
		std::vector<Octree*> GetNodes();

		//method that will set the parent for the octant
		void SetParent(Octree* vParent);

		//method that will display all the octants to the screen
		void Display(uint octreeID);

		//method that will configure where each of the octants are in the screen and which models go inside them
		void ConfigureOctree();

		//Method that will add a sub division of children to the octree
		void AddDimension();

		//Method that will remove a sub division to the octree
		void RemoveDimension();

		//method that will check the collision of the models in the scene
		void CheckCollision();

		//Method that will update the Rigid bodies of all the octants
		void UpdateRigidBodies();

		//method to delete all heap associated variables
		void safeDelete();
	};
}
