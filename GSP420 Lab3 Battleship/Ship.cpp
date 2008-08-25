#include "Ship.h"

Ship::Ship(int size, char * n, char f)
{
	length = size;
	name = n;
	face = f;
	damage = 0;
	dir = 0;
	
	placed = false;
}

Ship::~Ship()
{
}

void Ship::setPlaced(bool value)
{
	placed = value;
}

bool Ship::isPlaced()
{
	return placed;
}

int Ship::getSize()
{
	return length;
}

bool Ship::checkDead(int damage)
{
	damage += damage;
	if(damage >= length)
		return true;
	else
		return false;
}