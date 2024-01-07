#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

const int SCREEN_HEIGHT = 800;
const int SCREEN_WIDTH = 640;

SDL_Event ev;
SDL_Rect Player{SCREEN_HEIGHT/2, SCREEN_WIDTH/2+60, 60, 60};
SDL_Rect PlayerInside{(SCREEN_HEIGHT/2)+10, (SCREEN_WIDTH/2)+10+60, 40, 40};

bool Setup();
bool Input();
void UpdateLogic();
void DrawAndPresent(auto l_renderer, auto l_r, auto l_background);
void CleanUp(auto l_window, auto l_renderer, auto l_background);

int main(int argc, char* argv[])
{
	bool running = true;
	SDL_Texture *background_texture = NULL;
	
	if(Setup())
	{
		return 1;
	}

	auto window = SDL_CreateWindow
	(
		"Boom",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_HEIGHT, SCREEN_WIDTH, SDL_WINDOW_SHOWN
	);

	auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	//set variables for first level stage 0 potential menu
	int stage = 1;
	background_texture = IMG_LoadTexture(renderer, "background.png");
	while(running)
	{
		running = Input();
		UpdateLogic();
		DrawAndPresent(renderer, Player, background_texture);
		SDL_Delay((int)(1/60));
	}
	CleanUp(window, renderer, background_texture);
    return 0;
}


//return true to close program on error
bool Setup()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) > 0)
	{
		std::cout << "SDL_Init error: " << SDL_GetError() << std::endl;
		return true;
	}
	if(!(IMG_Init(IMG_INIT_PNG)))
	{
		std::cout << "IMG_Init error: " << SDL_GetError() << std::endl;
	}
	return false;
}


bool Input()
{
	SDL_PollEvent(&ev);
		if(ev.type == SDL_QUIT)
		{
				return false;
		}
	return true;
}


void UpdateLogic()
{
	if(ev.key.keysym.sym == SDLK_RIGHT)
	{
		Player.x += 10;
		PlayerInside.x += 10;
	}
	if(ev.key.keysym.sym == SDLK_LEFT)
	{
		Player.x -= 10;
		PlayerInside.x -= 10;
	}
		
	if(ev.key.keysym.sym == SDLK_UP)
	{
		Player.y -= 10;
		PlayerInside.y -= 10;
	}
	if(ev.key.keysym.sym == SDLK_DOWN)
	{
		Player.y += 10;
		PlayerInside.y += 10;
	}
}


void DrawAndPresent(auto l_renderer, auto l_r, auto l_background)
{
	SDL_RenderClear(l_renderer);
	SDL_RenderCopy(l_renderer, l_background, NULL, NULL);
	SDL_SetRenderDrawColor(l_renderer, 200, 100, 255, 255);
	SDL_RenderFillRect(l_renderer, &l_r);
	SDL_SetRenderDrawColor(l_renderer, 255, 100, 0, 255);
	SDL_RenderFillRect(l_renderer, &PlayerInside);
	SDL_RenderPresent(l_renderer);
}


void CleanUp(auto l_window, auto l_renderer, auto l_background)
{
	SDL_DestroyTexture(l_background);
	SDL_DestroyRenderer(l_renderer);
	SDL_DestroyWindow(l_window);
	IMG_Quit();
	SDL_Quit();
}
