#ifndef __AI_H
#define __AI_H

#include "entity.h"
#include "board.h"

struct Tile
{
	int x;
	int y;
	int state;
	bool possible;
};

#define AI_WIDTH	10
#define AI_HEIGHT	10

#define	UNFIRED		1
#define	MISSED		2
#define	RESOLVED	3
#define	UNRESOLVED	4
#define	SUSPECTED	5
#define	UNRESOLVEDTRAPPED	6

class AI:public Entity
{
	private:
		Board * aiBoard;

		//Cai's AI

		Tile ** board;
		bool HCarrierLives;
		bool HBattleshipLives;
		bool HDestroyerLives;
		bool HSubmarineLives;
		bool HPatrolBoatLives;

	public:
		AI();
		~AI();

		void getBoard(Board * b);
		void placeShips();

		bool placeShip(int shipType, int row, int col, char direction);
		bool isOpen(int ship, int row, int col, char direction);

		//Cai's AI

		void receiveSuccess(int x, int y, bool hit);
		void clearPossibleTargets();
		void getFiringSolution(int &x, int&y);
		int getNumPosShips(int x, int y);
		void fireNearThatSpot(int &x, int&y);
		int getOpenWaterRank(int x, int y);
		int getNumShipsFitIn(int n);
		void updateHumanStatus(/*Ship hShip[]*/);
		void print();
};

#endif