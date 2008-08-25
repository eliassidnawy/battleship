// 6/09/08 Cai
#ifndef __SHIP_H
#define __SHIP_H

#include "d3dUtil.h"

class Ship
{
private:
	
public:
	// probably private

	char * name;
	int length;
	int damage;
	int dir;
	char face;
	
	bool placed;
	

	// probably public

	int col, row;
	D3DXVECTOR3 Pos;

	Ship(int size, char * n, char f);
	~Ship();

	void setPlaced(bool value);
	bool isPlaced();

	char * getName() { return name; }

	int getSize();

	bool checkDead(int damage);
};

#endif