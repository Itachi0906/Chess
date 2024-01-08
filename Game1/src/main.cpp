#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include "fen_parse.h"

fen_parse* fp = nullptr;

int boxsize = 50;
std::vector<SDL_Surface*> pieces;
std::vector<SDL_Texture*> pieces_texture;
std::unordered_map<int, SDL_Texture* > mp;
std::vector<const char*> pieces_name = { "vendor/images/bB.png","vendor/images/bK.png","vendor/images/bN.png","vendor/images/bP.png","vendor/images/bQ.png","vendor/images/bR.png","vendor/images/wB.png","vendor/images/wK.png","vendor/images/wN.png","vendor/images/wP.png","vendor/images/wQ.png","vendor/images/wR.png" };
std::vector<int> piece_values = { 20 , 22 , 18 , 17 , 21 , 19 , 12 , 14 , 10 , 9 , 13 , 11 };
SDL_Rect drect;
int r, c,st_row,st_col,end_row,end_col;
int piece_moved;
std::pair <int, int> pos = { -1,-1 };
std::vector<std::pair<int,int>> clicks;
std::vector<std::vector<int>> piece_pos;


void move_click() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	r = y / 50;
	c = x / 50;
	if (pos == std::make_pair(r, c)) {
		pos = { -1,-1 };
		clicks = {};
	}
	else {
		pos =std:: make_pair(r, c);
		clicks.push_back(pos);
	}
	if (clicks.size() == 2) {
		st_row = clicks[0].first;
		st_col = clicks[0].second;
		end_row = clicks[1].first;
		end_col = clicks[1].second;
		if (fp->board[st_row][st_col] != 0) {
			piece_moved = fp->board[st_row][st_col];
			fp->board[end_row][end_col] = piece_moved;
			fp->board[st_row][st_col] = 0;
			if (piece_moved == 9) {
				piece_pos = fp->pawn_w;
			}
			else if (piece_moved == 10) {
				piece_pos = fp->knight_w;
			}
			else if (piece_moved == 11) {
				piece_pos = fp->rook_w;
			}
			else if (piece_moved == 12) {
				piece_pos = fp->bishop_w;
			}
			else if (piece_moved == 13) {
				piece_pos = fp->queen_w;
			}
			else if (piece_moved == 14) {
				piece_pos = fp->king_w;
			}
			else if (piece_moved == 17) {
				piece_pos = fp->pawn_b;
			}
			else if (piece_moved == 18) {
				piece_pos = fp->knight_b;
			}
			else if (piece_moved == 19) {
				piece_pos = fp->rook_b;
			}
			else if (piece_moved == 20) {
				piece_pos = fp->bishop_b;
			}
			else if (piece_moved == 21) {
				piece_pos = fp->queen_b;
			}
			else if (piece_moved == 22) {
				piece_pos = fp->king_b;
			}
			for (auto square : piece_pos) {
				if (clicks[0] == std::make_pair(square[0], square[1])) {
					square[0] = clicks[1].first;
					square[1] = clicks[1].second;
				}
			}
		}
			pos = { -1,-1 };
			clicks = {};
	}
}

void board_pieces(SDL_Renderer* renderer) {
	drect.w = 49;
	drect.h = 49;
	drect.x = 0;
	drect.y = 0;
	for (auto v: fp->board) {
		for (auto k:v) {
			if (k != 0) {
				SDL_RenderCopy(renderer, mp[k], NULL, &drect);
				
			}
			drect.x += 50;
		}
		drect.y += 50;
		drect.x = 0;
	}
	SDL_RenderPresent(renderer);
}

SDL_Texture* load_texture(const char* path , SDL_Renderer* renderer)
{
	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL) {
		std::cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
		return NULL;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	if (texture == NULL) {
		std::cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
	}

	SDL_FreeSurface(loadedSurface);
	return texture;

}
void load_images(SDL_Renderer* renderer)
{
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
		return;
	}
	for (int i = 0;i < pieces_name.size();i++)
	{
		const char* path = pieces_name[i];
		SDL_Texture* player_tex = load_texture(path, renderer);
		if (player_tex != NULL)
		{
			pieces_texture.push_back(player_tex);
		}
		mp[piece_values[i]] = player_tex;
	}
	
}


void createBoard(SDL_Renderer* renderer)
{
	SDL_RenderClear(renderer);
	bool white = true;
	for(int i = 0;i<8;i++)
	{
		for(int j = 0;j<8;j++)
		{
			SDL_Rect square = { j * boxsize, i * boxsize, boxsize, boxsize };
			if(white)
			{
				SDL_SetRenderDrawColor(renderer, 255, 206, 158, 255);
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 209, 139, 71, 255);
			}
			SDL_RenderFillRect(renderer, &square);
			white = !white;
		}
		white = !white;
	}
	
}

int main(int argc, char* argv[]) {

	fp = new fen_parse();
	fp->init("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");

	if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		int flag = SDL_WINDOW_SHOWN;
		SDL_Window* window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 400, flag);
		SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
		bool running = true;
		while (running)
		{
			SDL_Event event;
			while(SDL_PollEvent(&event) != 0)
			{
				if (event.type == SDL_QUIT)
				{
					running = false;
				}
				else if (event.type == SDL_MOUSEBUTTONDOWN) {
					move_click();
				}
			}
			createBoard(renderer);
			load_images(renderer);
			board_pieces(renderer);
		}
	}
	return 0;
}
