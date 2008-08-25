#ifndef __BOARD_H
#define __BOARD_H

#include "defines.h"

class Board
{
	private:

	public:
		// probably private
		int boardWidth;
		int boardHeight;

		char **board;

		// probably public
		Board();
		~Board();

		int getBoardWidth();
		int getBoardHeight();
		
		char getAt(int x, int y);
		void setAt(int x, int y, char face);		
};
#endif