#include "AI.h"
#include <stdlib.h>

#define NUM_SHIPS	5

AI::AI():Entity()
{
	//Cai's AI
	board = new Tile*[AI_HEIGHT];
	for(int i = 0; i < AI_HEIGHT; ++i)
	{
		board[i] = new Tile[AI_WIDTH];
	}

	for(int r = 0; r < AI_HEIGHT; ++r)
	{
		for(int c = 0; c < AI_HEIGHT; ++c)
		{
			board[r][c].x = c;
			board[r][c].y = r;
			board[r][c].state = UNFIRED;
			board[r][c].possible = false;
		}
	}

	HCarrierLives = true;
	HBattleshipLives = true;
	HDestroyerLives = true;
	HSubmarineLives = true;
	HPatrolBoatLives = true;
}

AI::~AI()
{
	//Cai's AI
	for(int i = 0; i < AI_HEIGHT; ++i)
	{
		delete board[i];
	}
	delete board;
}

void AI::getBoard(Board * b)
{
	aiBoard = b;
}

void AI::placeShips()
{
	int row, col, dir, ship = 0;
    char d = 0;
	bool goodData = false;

	while(ship < NUM_SHIPS)
	{
		//get row and col to place ship
		col = rand() % 10;
		row = rand() % 10;
		dir = rand() % 2;

		d = ( (dir == 0)?'h':'v');

		//if BS_FAIL, a ship is already in location
		if( (placeShip(ship, row, col, d)) == true )
		{	
			//printf("\nship: %i, col: %i, row: %i, direction: %c", ship, col, row, d);
			++ship;
		}
	}
}

bool AI::isOpen(int ship, int row, int col, char direction)
{
	for(int r = 0; r < getSize( (SHIPTYPE)ship); ++r)
	{	
		if(col < aiBoard->getBoardWidth() && row < aiBoard->getBoardHeight())
		{
			if( aiBoard->getAt(col, row) != '.' )
				return false;
		}
		else
		{
			return false;
		}
		
		(direction == 'h' || direction == 'H')?++col:++row;
	}

	return true;
}

bool AI::placeShip(int shipType, int row, int col, char direction)
{
	
	if( (isOpen(shipType, row, col, direction)) != true )
		return false;

	for(int r = 0; r < getSize( (SHIPTYPE)shipType ); ++r)
	{			
		aiBoard->setAt(col, row, getFace( (SHIPTYPE)shipType ) );
		(direction == 'h' || direction == 'H')?++col:++row;
	}
		
	return true;
}

// Cai's AI

void AI::receiveSuccess(int x, int y, bool hit)
{
	if(hit == true)
	{
		board[y][x].state = UNRESOLVED;
	}
	else
	{
        board[y][x].state = MISSED;
	}
}

void AI::clearPossibleTargets()
{
	for(int r = 0; r < AI_HEIGHT; ++r)
	{
		for(int c = 0; c < AI_HEIGHT; ++c)
		{
			board[r][c].possible = false;
		}
	}
}

void AI::getFiringSolution(int &x, int&y)
{
	int tempX;
	int tempY;
	// clear possible targets
	clearPossibleTargets();

	// determine what state I should be in

	int unresolved;
	unresolved = 0;
	bool stillUnresolved;

	// count unresolved hits and find any trapped unresolved hits
	for(int r = 0; r < AI_HEIGHT; ++r)
	{
		for(int c = 0; c < AI_WIDTH; ++c)
		{
			if(board[r][c].state == UNRESOLVED)
			{
				stillUnresolved = false;
				if(r != 0)
				{
					if(board[r-1][c].state == UNFIRED)
					{
						stillUnresolved = true;
					}
				}
				if(stillUnresolved == false)
				{
					if(r != 9)
					{
						if(board[r+1][c].state == UNFIRED)
						{
							stillUnresolved = true;
						}
					}
				}
				if(stillUnresolved == false)
				{
					if(c != 0)
					{
						if(board[r][c-1].state == UNFIRED)
						{
							stillUnresolved = true;
						}
					}
				}
				if(stillUnresolved == false)
				{
					if(c != 9)
					{
						if(board[r][c+1].state == UNFIRED)
						{
							stillUnresolved = true;
						}
					}
				}

				// if after all that it is still unresolved
				if(stillUnresolved)
				{
					++unresolved;
				}
				else
				{
					// else it's a trapped unresolved
					board[r][c].state = UNRESOLVEDTRAPPED;
				}
			}
		}
	}

	// test case
	//unresolved = 0;

	if(unresolved != 0)
	{
		// if unresolved is larger than 0, then I have a hit that I need to investigate
		// RESOLVEHIT State

		// choose first unresolved hit
		for(int r = 0; r < AI_HEIGHT; ++r)
		{
			for(int c = 0; c < AI_WIDTH; ++c)
			{
				if(board[r][c].state == UNRESOLVED)
				{
					tempX = c;
					tempY = r;
					c = AI_WIDTH;
					r = AI_HEIGHT;	// get me out of this loop
				}
			}
		}

		// look around it for a hit then hit the opposite side if possible
		// check in the x direction
		if(tempX != 0 && tempX != 9)
		{
			
			if	(// check the left
				(board[tempY][tempX - 1].state == UNRESOLVED || board[tempY][tempX - 1].state == UNRESOLVEDTRAPPED)
				&&
				(board[tempY][tempX + 1].state == UNFIRED)
				)
			{
				x = tempX + 1;
				y = tempY;
				return;
			}
			else if
				(// check the right
				(board[tempY][tempX + 1].state == UNRESOLVED || board[tempY][tempX + 1].state == UNRESOLVEDTRAPPED)
				&&
				(board[tempY][tempX -1].state == UNFIRED)
				)
			{
				x = tempX - 1;
				y = tempY;
				return;
			}
		}
		if(tempY != 0 && tempY != 9)// check in the y direction
		{
			if	(// check above
				(board[tempY - 1][tempX].state == UNRESOLVED || board[tempY - 1][tempX].state == UNRESOLVEDTRAPPED)
				&&
				(board[tempY + 1][tempX].state == UNFIRED)
				)
			{
				x = tempX;
				y = tempY + 1;
				return;
			}
			else if
				(// check below
				(board[tempY + 1][tempX].state == UNRESOLVED || board[tempY + 1][tempX].state == UNRESOLVEDTRAPPED)
				&&
				(board[tempY - 1][tempX].state == UNFIRED)
				)
			{
				x = tempX;
				y = tempY - 1;
				return;
			}
		}
		if(tempX == 0 && board[tempY][tempX + 1].state == UNFIRED)	// if it's on the left edge, see if we can hit the right
		{
			x = tempX + 1;
			y = tempY;
			return;
		}
		if(tempX == 9 && board[tempY][tempX - 1].state == UNFIRED)	// if it's on the right edge, see if we can hit the left
		{
			x = tempX - 1;
			y = tempY;
			return;
		}
		if(tempY == 0 && board[tempY + 1][tempX].state == UNFIRED)	// if it's on the top edge, see if we can hit below
		{
			x = tempX;
			y = tempY + 1;
			return;
		}
		if(tempY == 9 && board[tempY - 1][tempX].state == UNFIRED)	// if it's on the bottom edge, see if we can hit above
		{
			x = tempX;
			y = tempY - 1;
			return;
		}
		else
		{
			// there are no hits to work off of, so hit in a random direction
			x = tempX;
			y = tempY;
			fireNearThatSpot(x, y);
			return;
		}

	}
	//else if()
	//{
	//	// try torpedoes
	//}
	//else if()
	//{
	//	// try sonar
	//}
	else
	{
		// if I have no unresolved hits to investigate, I need to go ship hunting
		// SHIPHUNT State

		// rank all spots according to maximum number of possible ships in that spot

		int numPossibleTargets = 0;
		int MostNumPosShips = -1;

		for(int r = 0; r < AI_HEIGHT; ++r)
		{
			for(int c = 0; c < AI_HEIGHT; ++c)
			{
				if(board[r][c].state == UNFIRED)
				{
					if(MostNumPosShips < getNumPosShips(c, r))
					{
						MostNumPosShips = getNumPosShips(c, r);
						tempX = c;
						tempY = r;
						clearPossibleTargets();
						board[r][c].possible = true;
						numPossibleTargets = 1;
					}
					else if(MostNumPosShips == getNumPosShips(c, r))
					{
						board[r][c].possible = true;
						++numPossibleTargets;
					}
				}
			}
		}

		// fire at a spot with the highest possibility
		
		if(numPossibleTargets == 1)
		{
			// if only one highest rank, hit it
			x = tempX;
			y = tempY;
			return;
		}
		else
		{
			// if multiple highest ranks, use least open water ranking to break tie
			int mostOpenWater = -1;
			MostNumPosShips = 0;

			for(int r = 0; r < AI_HEIGHT; ++r)
			{
				for(int c = 0; c < AI_WIDTH; ++c)
				{
					if(board[r][c].possible == true)
					{
						if(mostOpenWater < getOpenWaterRank(c, r))
						{
							mostOpenWater = getOpenWaterRank(c, r);
							tempX = c;
							tempY = r;
							numPossibleTargets = 1;
						}
						else if(mostOpenWater == getOpenWaterRank(c, r))
						{
							++numPossibleTargets;
						}
					}
				}
			}

			if(numPossibleTargets == 1)
			{
				// if only one lowest water ranking, hit it
				x = tempX;
				y = tempY;
				return;
			}
			else
			{
				// else choose a random number and use that to act as tiebreaker
				int randNum;
				randNum = rand() % numPossibleTargets; 

				for(int r = 0; r < AI_HEIGHT; ++r)
				{
					for(int c = 0; c < AI_WIDTH; ++c)
					{
						if(mostOpenWater == getOpenWaterRank(c, r))
						{
							if(randNum != 0)
							{
								--randNum;
							}
							else
							{
								x = c;
								y = r;
								return;
							}
						}
					}
				}
			}
		}
	}
}

int AI::getNumPosShips(int x, int y)
{
	int tempX = x;
	int tempY = y;
	// these counters start at 1 to include that starting spot
	int vertical = 1, horizontal = 1;
	// count vertical
	while(tempY > 0 && board[tempY - 1][x].state == UNFIRED)
	{
		--tempY;
		++vertical;
	}
	tempY = y;
	while(tempY < 9 && board[tempY + 1][x].state == UNFIRED)
	{
		++tempY;
		++vertical;
	}
	// count horizontal
	while(tempX > 0 && board[y][tempX - 1].state == UNFIRED)
	{
		--tempX;
		++vertical;
	}
	tempX = x;
	while(tempX < 9 && board[y][tempX + 1].state == UNFIRED)
	{
		++tempX;
		++vertical;
	}

	if(vertical < horizontal)
	{
		vertical = horizontal;
	}

	return getNumShipsFitIn(vertical);
}

int AI::getNumShipsFitIn(int n)
{
	// get num ships left that will fit in n spaces
	int numPos = 0;
	if (n > 5)
	{
		n = 5;
	}
	switch(n)
	{
	case 5:
		if(HCarrierLives == true)
		{
			++numPos;
		}
	case 4:
		if(HBattleshipLives == true)
		{
			++numPos;
		}
	case 3:
		if(HDestroyerLives == true)
		{
			++numPos;
		}
		if(HSubmarineLives == true)
		{
			++numPos;
		}
	case 2:
		if(HPatrolBoatLives == true)
		{
			++numPos;
		}
	}
	return numPos;
}

int AI::getOpenWaterRank(int x, int y)
{
	int tempX = x;
	int tempY = y;
	// these counters start at 1 to include that starting spot
	int vertical = 1, horizontal = 1;
	// count vertical
	while(tempY > 0 && board[tempY][x].state == UNFIRED)
	{
		--tempY;
		++vertical;
	}
	tempY = y;
	while(tempY < 9 && board[tempY][x].state == UNFIRED)
	{
		++tempY;
		++vertical;
	}
	// count horizontal
	while(tempX > 0 && board[y][tempX].state == UNFIRED)
	{
		--tempX;
		++vertical;
	}
	tempX = x;
	while(tempX < 9 && board[y][tempX].state == UNFIRED)
	{
		++tempX;
		++vertical;
	}
	return vertical + horizontal - 1;
}

void AI::fireNearThatSpot(int &x, int&y)
{
	int direction;
	direction = rand() % 4;
	bool valid = false;

	while(valid == false)
	{
		switch(direction)
		{
		case 0:
			if(y - 1 < 0)
			{
				valid = false;
			}
			else if(board[y - 1][x].state != UNFIRED)
			{
				valid = false;
			}
			else
			{
				valid = true;
				y = y - 1;
			}
			break;
		case 1:
			if(x + 1 < 0)
			{
				valid = false;
			}
			else if(board[y][x + 1].state != UNFIRED)
			{
				valid = false;
			}
			else
			{
				valid = true;
				x = x + 1;
			}
			break;
		case 2:
			if(y + 1 < 0)
			{
				valid = false;
			}
			else if(board[y + 1][x].state != UNFIRED)
			{
				valid = false;
			}
			else
			{
				valid = true;
				y = y + 1;
			}
			break;
		case 3:
			if(x - 1 < 0)
			{
				valid = false;
			}
			else if(board[y][x - 1].state != UNFIRED)
			{
				valid = false;
			}
			else
			{
				valid = true;
				x = x - 1;
			}
			break;
		}
		++direction;
	}

	return;
}

void AI::updateHumanStatus(/*Ship hShip[]*/)
{
    int maxShips = 5;

	/*
	HCarrierLives = hShip[0];
	HBattleshipLives = hShip[1];
	HDestroyerLives = hShip[2];
	HSubmarineLives = hShip[3];
	HPatrolBoatLives = hShip[4];
	*/

	return;
}

void AI::print()
{
	/*for(int r = 0; r < HEIGHT; ++r)
	{
		for(int c = 0; c < HEIGHT; ++c)
		{
			cout << board[r][c].state;
		}
		cout << endl;
	}
	cout << endl;*/
}