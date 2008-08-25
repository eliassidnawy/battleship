#include "Entity.h"
#include <iostream>


Entity::Entity()
{
	bShip = new Ship(4, "Battleship", 'B');
	carrierShip = new Ship(5, "Carrier", 'C');
	cruiserShip = new Ship(3, "Destroyer", 'D');
	submarineShip = new Ship(3, "Submarine", 'S');
	patrolShip = new Ship(2, "Patrol Boat", 'P');
}

Entity::~Entity()
{
	delete bShip;
	delete carrierShip;
	delete cruiserShip;
	delete submarineShip;
	delete patrolShip;
}

void Entity::setPlaced(SHIPTYPE sType, bool value)
{
	switch(sType)
	{
		case BS_CARRIER:
			carrierShip->setPlaced(value);
			break;

		case BS_BATTLESHIP:
			bShip->setPlaced(value);
			break;

		case BS_CRUISER:
			cruiserShip->setPlaced(value);
			break;

		case BS_SUBMARINE:
			submarineShip->setPlaced(value);
			break;

		case BS_PATROL:
			patrolShip->setPlaced(value);
			break;
	}
}

bool Entity::isPlaced(SHIPTYPE sType)
{
	switch(sType)
	{
		case BS_CARRIER:
			return carrierShip->isPlaced();

		case BS_BATTLESHIP:
			return bShip->isPlaced();

		case BS_CRUISER:
			return cruiserShip->isPlaced();

		case BS_SUBMARINE:
			return submarineShip->isPlaced();

		case BS_PATROL:
			return patrolShip->isPlaced();
	}
}

string Entity::getName(SHIPTYPE sType)
{
	switch(sType)
	{
		case BS_CARRIER:
			return carrierShip->getName();
			

		case BS_BATTLESHIP:
			return bShip->getName();

		case BS_CRUISER:
			return cruiserShip->getName();

		case BS_SUBMARINE:
			return submarineShip->getName();

		case BS_PATROL:
			return patrolShip->getName();
	}
}

int Entity::getSize(SHIPTYPE sType)
{
	switch(sType)
	{
		case BS_CARRIER:
			return carrierShip->getSize();
			
		case BS_BATTLESHIP:
			return bShip->getSize();

		case BS_CRUISER:
			return cruiserShip->getSize();
			
		case BS_SUBMARINE:
			return submarineShip->getSize();

		case BS_PATROL:
			return patrolShip->getSize();
	}
}

char Entity::getFace(SHIPTYPE sType)
{
	switch(sType)
	{
		case BS_CARRIER:
			return carrierShip->face;
			

		case BS_BATTLESHIP:
			return bShip->face;
		

		case BS_CRUISER:
			return cruiserShip->face;
			

		case BS_SUBMARINE:
			return submarineShip->face;
			

		case BS_PATROL:
			return patrolShip->face;
			
	}
}

SHIPTYPE Entity::getShip(char c)
{/*
 #define CARRIER	'@'
#define BATTLSHIP '#'
#define CRUISER	'$'
#define SUBMARINE	'%'
#define PATROL	'+'
 */

	if(c == '@')
		return BS_CARRIER;
	if(c == '#')
		return BS_BATTLESHIP;
	if(c == '$')
		return BS_CRUISER;
	if(c == '%')
		return BS_SUBMARINE;
	if(c == '+')
		return BS_PATROL;
}

bool Entity::shipHit(char ship, int damage)
{
	

	switch(getShip(ship))
	{
		case BS_CARRIER:
			return carrierShip->checkDead(damage);
			

		case BS_BATTLESHIP:
			return bShip->checkDead(damage);
		

		case BS_CRUISER:
			return cruiserShip->checkDead(damage);
			

		case BS_SUBMARINE:
			return submarineShip->checkDead(damage);
			

		case BS_PATROL:
			return patrolShip->checkDead(damage);
			
	}

}