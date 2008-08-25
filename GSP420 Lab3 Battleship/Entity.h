#ifndef __ENTITY_H_
#define __ENTITY_H_

#include "Ship.h"

#include "defines.h"
#include <string>
using namespace std;

class Entity
{
	private:
		Ship * bShip;
		Ship *carrierShip;
		Ship *cruiserShip;
		Ship * submarineShip;
		Ship * patrolShip;
		

	public:
		Entity();
		~Entity();

		//wrapper for access to ship functions
		void setPlaced(SHIPTYPE sType, bool value);
		bool isPlaced(SHIPTYPE sType);
		string getName(SHIPTYPE sType);
		int getSize(SHIPTYPE sType);
		char getFace(SHIPTYPE sType);
		SHIPTYPE getShip(char c);
		bool shipHit(char ship, int damage);
};
#endif