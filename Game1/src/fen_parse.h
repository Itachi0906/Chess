#pragma once
#include <string>
#include <vector>
#include <iostream>

class fen_parse
{
public:
	std::vector <std::vector<int>> board_pos(std::string &positions);

	std::vector <std::vector<int>> pawn_w,pawn_b,knight_w,knight_b,rook_w,rook_b,queen_w,queen_b,king_w,king_b,bishop_w,bishop_b;
	std::string to_move;
	int half_moves;
	int full_moves;
	std::vector<std::vector<int>> board;
	std::vector<bool> castle;
	int en_passant;
	void init(std::string fen);
};

