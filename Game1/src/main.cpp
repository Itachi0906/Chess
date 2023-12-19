#include "Game.h"
#include "Gamestate.h"
//#include "Move.h"

Game *game = nullptr;
Gamestate* gs = nullptr;

int main(int argc , char *argv[])
{
	//initialize the game and gamestate
	game = new Game();
	gs = new Gamestate();
	game->init("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 400, false); // Ye sdl ka use h window create ho jaati isse 
	game->loadimages();
	gs->init("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

	//initialize the variables

	bool running = game->running();
	std::vector<std::pair<int,int>> clicks;
	std::pair<int,int> sq_selected;
	bool movemade = false;
	std::set<Move*> possible_moves = gs->GenerateAllMoves();
	while (running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				int col = x / 50, row = y / 50;
				if (sq_selected == std::make_pair(row , col))
				{
					sq_selected = {};
					clicks = {};
				}
				else
				{
					sq_selected = {row , col};
					clicks.push_back(sq_selected);
				}
				if (clicks.size() == 2)
				{

					// yha error h 
					// mene pointers ka vector bna dia without realising do pointers equal ni hote T-T
					Move* move = new Move();
					move->register_move(clicks[0], clicks[1], gs->board);
					std::cout << move->start_row << " " << move->start_col << " " << move->end_row << " " << move->end_col << std::endl;
					//if(possible_moves.find(move) != possible_moves.end())
					//{
						std::cout << "here" << std::endl;
						gs->makemove(move);
						movemade = true;
					//}
					if (!movemade)
					{
						sq_selected = {};
						clicks = {};
					}
					//std::cout << gs->whiteToMove << std::endl;
				}
			}
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_z)
				{
					gs->undomove(gs->movelog, gs->board);
					movemade = true;
					std::cout << gs->whiteToMove << std::endl;
				}
			}
		}

		if (movemade)
		{
			possible_moves = gs->GenerateAllMoves();
			/*for (auto move:possible_moves)
			{
				std::cout << move->start_row << " " << move->start_col << " " << move->end_row << " " << move->end_col << std::endl;
			}*/
			clicks = {};
			sq_selected = {};
			movemade = false;
			gs->whiteToMove = !gs->whiteToMove;
		}


		game->update();
		game->render();
		game->create_fen(gs->board);
	}
	game->clean();
	return 0;
}