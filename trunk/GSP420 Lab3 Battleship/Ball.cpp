#include "Ball.h"

Ball::Ball()
{

}

Ball::Ball(float radius, unsigned int slices, unsigned int stack, float x, float y, float z)
{
	//set the ball params
	this->radius = radius;
	this->slices = slices;
	this->stack = stack;

	//set the ball pos in the world
	this->posX = x;
	this->posY = y;
	this->posZ = z;

	//create the ball 
	createBall(this->radius, this->slices, this->stack);


}


void Ball::createBall(float radius, unsigned int slices, unsigned int stack)
{
	//provide the radius, slices, and stack, the mesh we have, null for adjacency
    D3DXCreateSphere(gd3dDevice, radius, slices, stack, &ballMesh, NULL);
}  
 
void Ball::draw()
{
  HR(ballMesh->DrawSubset(0));
}
