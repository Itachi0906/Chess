#include "fen_parse.h"

std::vector <std::string> split(std::string& fen) {
	std::vector <std::string> parts;
	std::string temp = "";
	for (char c : fen) {
		if (c == ' ') {
			parts.emplace_back(temp);
			temp = "";
		}
		else {
			temp += c;
		}
	}
	parts.emplace_back(temp);
	return parts;
}

int piece_value(char piece) {
	int val = 16;
	if (isupper(piece)) {
		val = 8;
	}
	switch (toupper(piece)) {
	case 'P': return 1+val;
	case 'N': return 2+val;
	case 'R': return 3+val;
	case 'B': return 4+val;
	case 'Q': return 5+val;
	case 'K': return 6+val;
	default: return 0;
	}
}

std::vector <std::vector<int>> fen_parse::board_pos(std::string& positions) {
	std::vector <std::vector<int>> board(8, std::vector<int>(8, 0));
	int row = 0, col = 0;
	for (char c : positions) {
		if (isdigit(c)) {
			col += int(c) - '0';
		}
		else if (c == '/') {
			col = 0;
			row++;
		}
		else {
			board[row][col] = piece_value(c);
			if (piece_value(c) == 9) {
				pawn_w.push_back({row,col});
			}
			else if (piece_value(c) == 10) {
				knight_w.push_back({ row,col });
			}
			else if (piece_value(c) == 11) {
				rook_w.push_back({ row,col });
			}
			else if (piece_value(c) == 12) {
				bishop_w.push_back({ row,col });
			}
			else if (piece_value(c) == 13) {
				queen_w.push_back({ row,col });
			}
			else if (piece_value(c) == 14) {
				king_w.push_back({ row,col });
			}
			else if (piece_value(c) == 17) {
				pawn_b.push_back({ row,col });
			}
			else if (piece_value(c) == 18) {
				knight_b.push_back({ row,col });
			}
			else if (piece_value(c) == 19) {
				rook_b.push_back({ row,col });
			}
			else if (piece_value(c) == 20) {
				bishop_b.push_back({ row,col });
			}
			else if (piece_value(c) == 21) {
				queen_b.push_back({ row,col });
			}
			else if (piece_value(c) == 22) {
				king_b.push_back({ row,col });
			}
			col++;
		}
	}

	for (auto v : board) {
		for (auto k : v) {
			std::cout << k << ' ';
		}
		std::cout << std::endl;
	}



	return board;
}

std::vector<bool> rights(std::string& castling) {
	std::vector <bool> castle(4, false);
	for (char c : castling) {
		if (c == 'k') {
			castle[0] = true;
		}
		else if (c == 'q') {
			castle[1] = true;
		}
		else if (c == 'K') {
			castle[2] = true;
		}
		else if (c == 'Q') {
			castle[3] = true;
		}
	}
	return castle;
}

int square_ind(std::string& square) {
	int row = '8' - square[1];
	int col = square[0] - 'a';
	return (row * 8) + col;
}

void fen_parse::init(std::string fen) {
	
	//fen components
	std::vector <std::string> fen_parts = split(fen);

	//position of pieces
	board = board_pos(fen_parts[0]);

	//turn
	to_move = (fen_parts[1] == "w") ? "White" : "Black";
	std::cout << to_move << std::endl;

	//castling
	castle = rights(fen_parts[2]);

	for (bool x : castle) {
		std::cout << x << ' ';
	}
	std::cout << std::endl;

	//en-passant
	en_passant = (fen_parts[3] == "-") ? -1 : square_ind(fen_parts[3]);
	std::cout << en_passant << std::endl;

	//halfmove
	half_moves = stoi(fen_parts[4]);
	std::cout << half_moves << std::endl;

	//fullmoves
	full_moves = stoi(fen_parts[5]);
	std::cout << full_moves << std::endl;
}
