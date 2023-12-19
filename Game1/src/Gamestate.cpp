#include "Gamestate.h"

Gamestate::Gamestate()
{
}

Gamestate::~Gamestate()
{
}

std::vector<std::string> split(std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	for (char c : s)
	{
		if (c == delimiter)
		{
			tokens.push_back(token);
			token = "";
		}
		else
		{
			token += c;
		}
	}
	tokens.push_back(token);
	return tokens;
}

int get_piece_value(char c)
{
	switch (c)
	{
	case 'K':return 1;
	case 'P':return 2;
	case 'N':return 3;
	case 'B':return 4;
	case 'R':return 5;
	case 'Q':return 6;
	default:
		return 0;
	}
}

std::vector<std::vector<int>> create_board(std::string& fen_board, bool whiteToMove)
{
	std::vector<std::vector<int>> board(8, std::vector<int>(8, 0));
	int row = 7;
	int col = 0;
	for (char c : fen_board)
	{
		if (c == '/')
		{
			row--;
			col = 0;
		}
		else if (isdigit(c))
		{
			col += c - '0';
		}
		else
		{
			int x = isupper(c) ? 16 : 8;
			board[row][col++] = get_piece_value(toupper(c)) | x;
		}
	}
	return board;
}

std::vector<bool> create_castling_rights(std::string& fen_castling_rights)
{
	std::vector<bool> castling_rights(4, false);
	for (char c : fen_castling_rights)
	{
		if (c == 'K')castling_rights[0] = true;
		if (c == 'Q')castling_rights[1] = true;
		if (c == 'k')castling_rights[2] = true;
		if (c == 'q')castling_rights[3] = true;
	}
	return castling_rights;
}

int get_square_index(std::string& square_name)
{
	int file = square_name[0] - 'a';
	int rank = square_name[1] - '0';
	return rank * 8 + file;
}

void Gamestate::init(std::string fen)
{
	//split into components
	std::vector<std::string> fen_parts = split(fen, ' ');
	//turn to move
	whiteToMove = fen_parts[1] == "w" ? true : false;
	//Create board
	board = create_board(fen_parts[0],whiteToMove);
	//castling rights
	castling_rights = create_castling_rights(fen_parts[2]);
	//enpassant possible or not
	enpassant_target_square = fen_parts[3] == "-" ? -1 : get_square_index(fen_parts[3]);
	//half move count for 50 move rule
	halfmoveCounter = std::stoi(fen_parts[4]);
	// total moves;
	moveCounter = std::stoi(fen_parts[5]);
}

void Gamestate::makemove(Move* move)
{
	board[move->start_col][move->start_row] = 0;
	board[move->end_col][move->end_row] = move->piece_moved;
	movelog.push_back(move);
}

void Gamestate::undomove(std::vector<Move*>& movelog,std::vector<std::vector<int>> &board)
{
	if (movelog.size() != 0)
	{
		Move* move = movelog[movelog.size() - 1];
		movelog.pop_back();
		board[move->start_col][move->start_row] = move->piece_moved;
		board[move->end_col][move->end_row] = move->piece_captured;
	}
}

std::set<Move*> Gamestate::GenerateAllMoves()
{
	std::set<Move*> possible_moves;
	for (int i = 0;i < 8;i++)
	{
		for (int j = 0;j < 8;j++)
		{
			if (whiteToMove)
			{
				if (board[i][j] - 8 >= 4 && board[i][j] < 16 )
				{
					generateSlidingMoves(possible_moves, i, j,board[i][j] - 8);
				}
				else if (board[i][j] - 8 == 3)
				{
					generateKnightMoves(possible_moves, i, j);
				}
				else if (board[i][j] - 8 == 2)
				{
					generatePawnMoves(possible_moves, i, j);
				}
				else if (board[i][j] - 8 == 1)
				{
					generateKingMoves(possible_moves, i, j);
				}
			}
			else
			{
				if (board[i][j] - 16 >= 4)
				{
					generateSlidingMoves(possible_moves, i, j,board[i][j] - 16);
				}
				else if (board[i][j] - 16 == 3)
				{
					generateKnightMoves(possible_moves, i, j);
				}
				else if (board[i][j] - 16 == 2)
				{
					generatePawnMoves(possible_moves, i, j);
				}
				else if (board[i][j] - 16 == 1)
				{
					generateKingMoves(possible_moves, i, j);
				}
			}
		}
	}
	return possible_moves;
}

void Gamestate::generatePawnMoves(std::set<Move*>& possible_moves, int r, int c)
{
	int moveAmount, startRow, enemyColor;
	if (whiteToMove)
	{
		moveAmount = -1;
		startRow = 6;
		enemyColor = 16;
	}
	else
	{
		moveAmount = 1;
		startRow = 1;
		enemyColor = 8;
	}
	Move* move = new Move();
	std::pair<int, int> p, p2;
	if (board[r + moveAmount][c] == 0) // 1 pawn advance move
	{
		p = { r,c }, p2 = { r + moveAmount , c };
		move->register_move(p, p2, board);
		possible_moves.insert(move);
		if (r == startRow && board[r + 2 * moveAmount][c] == 0) // 2 pawn advance move
		{
			p2 = { r + 2 * moveAmount , c };
			move->register_move(p, p2, board);
			possible_moves.insert(move);
		}
		if (c - 1 >= 0)
		{
			if (board[r + moveAmount][c - 1] - enemyColor >= 0)
			{
				p = { r,c };
				p2 = { r + moveAmount, c - 1 };
				move->register_move(p, p2, board);
				possible_moves.insert(move);
			}
		}
		if (c + 1 < board.size())
		{
			if (board[r + moveAmount][c + 1] - enemyColor >= 0)
			{
				p = { r,c };
				p2 = { r + moveAmount, c + 1 };
				move->register_move(p, p2, board);
				possible_moves.insert(move);
			}
		}
	}

}

void Gamestate::generateSlidingMoves(std::set<Move*>& possible_moves, int r, int c, int type)
{
	const std::vector<std::pair<int, int>> dir = { {0,1},{0,-1},{1,0},{-1,0},{1,1},{1,-1},{-1,1},{-1,-1}};
	int start_ind = 0 , end_ind = dir.size() - 1;
	if (type == 4)
	{
		start_ind = 4;
		end_ind = dir.size() - 1;
	}
	else if (type == 5)
	{
		start_ind = 0;
		end_ind = 3;
	}
	std::cout << whiteToMove << std::endl;
	std::cout << type << " " << start_ind<<" "<<end_ind<< std::endl;
	for (int i = start_ind;i <= end_ind;i++)
	{
		for (int j = 1;j < 8;j++)
		{
			int endrow = r + dir[i].first * j;
			int endcol = c + dir[i].second * j;

			if (0 <= endrow && endrow < 8 && endcol >= 0 && endcol < 8)
			{
				std::pair<int, int> p1 = { r,c }, p2 = {endrow,endcol};
				int endpiece = board[endrow][endcol];
				if (endpiece == 0)
				{
					Move* move = new Move();
					move->register_move(p1, p2, board);
					possible_moves.insert(move);
				}
				else if (abs(endpiece - board[r][c]) > 6)
				{
					Move* move = new Move();
					move->register_move(p1, p2, board);
					possible_moves.insert(move);
					break;
				}
				else
					break;
			}
		}
	}

}

void Gamestate::generateKnightMoves(std::set<Move*>& possible_moves, int r, int c)
{
}

void Gamestate::generateKingMoves(std::set<Move*>& possible_moves,int r,int c)
{
}
