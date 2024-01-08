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
			}
			createBoard(renderer);
			load_images(renderer);
			board_pieces(renderer);
		}
	}
	return 0;
}
