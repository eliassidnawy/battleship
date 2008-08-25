#pragma once
#include "d3dUtil.h"

class Ball
{
	private:
		//param: ID3DXMesh ballMesh is the variable that represents the mesh of the ball
		ID3DXMesh* ballMesh;
		
		//param: float radius, uint slices, uint stacks, variables used to create the sphere
		float radius;
		unsigned int slices;
		unsigned int stack;

		//param: float posX, posY, posZ, represent the position of the ball
		float posX;
		float posY;
		float posZ;

	public:
		Ball();
		Ball(float, unsigned int, unsigned int, float, float, float);
		~Ball();

		void createBall(float, unsigned int, unsigned int);

		void draw();

		void updatePosX(float dx) { posX += dx; }
		void updatePosY(float dy) { posY += dy; }
		void updatePosZ(float dz) { posZ += dz; }

		float getPosX() { return posX;}
		float getPosY() {return posY;}
		float getPosZ() {return posZ;}

};