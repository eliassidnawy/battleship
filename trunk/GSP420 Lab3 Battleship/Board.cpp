#include "Board.h"

Board::Board():boardHeight(10), boardWidth(10)
{
	board = new char * [boardHeight];

	for(int row = 0; row < boardHeight; ++row)
	{
		board[row] = new char [boardWidth];

		for(int col = 0; col < boardWidth; ++col)
		{
			board[row][col] = '.';
		}
	}
}
		
Board::~Board()
{
	for(int row = 0; row < boardHeight; ++row)
		delete [] board[row];

	delete [] board;
}

int Board::getBoardWidth()
{
	return boardWidth;
}

int Board::getBoardHeight()
{
	return boardHeight;
}

char Board::getAt(int x, int y)
{
	return board[y][x];
}

void Board::setAt(int x, int y, char face)
{
	board[y][x] = face;
	return;
}