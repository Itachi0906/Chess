#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <set>
#include "Move.h"

class Gamestate
{
public:
	Gamestate();
	~Gamestate();

	void init(std::string fen);
	void makemove(Move* move);
	void undomove(std::vector<Move*>& movelog, std::vector<std::vector<int>>& board);
	std::set<Move*> GenerateAllMoves();
	void generatePawnMoves(std::set<Move*>& possible_moves,int r, int c);
	void generateSlidingMoves(std::set<Move*>& possible_moves, int r, int c,int type);
	void generateKingMoves(std::set<Move*>& possible_moves, int r, int c);
	void generateKnightMoves(std::set<Move*>& possible_moves, int r, int c);

	bool whiteToMove;
	int halfmoveCounter;
	int moveCounter;
	std::vector<std::vector<int>> board;
	std::vector<bool> castling_rights;
	int enpassant_target_square;
	std::vector<Move*> movelog;
};

