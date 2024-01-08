#pragma once
#include <string>
#include <vector>
#include <iostream>

class fen_parse
{
public:
	
	std::string to_move;
	int half_moves;
	int full_moves;
	std::vector<std::vector<int>> board;
	std::vector<bool> castle;
	int en_passant;
	void init(std::string fen);
};

