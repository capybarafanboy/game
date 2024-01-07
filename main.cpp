#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

const int SCREEN_HEIGHT = 800;
const int SCREEN_WIDTH = 640;

const float FPS = 60.0;


bool Setup();
bool Input(auto l_ptr_ev);
void UpdateLogic(auto l_ptr_ev, auto l_ptr_P, auto l_ptr_PI);
void DrawAndPresent(auto l_renderer, auto l_ptr_P, auto l_ptr_PI, auto l_background);
void CleanUp(auto l_window, auto l_renderer, auto l_background);

int main(int argc, char* argv[])
{
	bool running = true;
	SDL_Event ev;
	SDL_Event* ptr_ev = &ev;
	SDL_Texture *background_texture = NULL;
	SDL_Rect Player{SCREEN_HEIGHT/2, SCREEN_WIDTH/2+60, 60, 60};
	SDL_Rect PlayerInside{(SCREEN_HEIGHT/2)+10, (SCREEN_WIDTH/2)+10+60, 40, 40};
	SDL_Rect* ptr_Player = &Player;
	SDL_Rect* ptr_PlayerInside = &PlayerInside;
	uint32_t FrameTime = 0;

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
	/*set variables for first level 
	stage 0 potential menu */
	int stage = 1;
	background_texture = IMG_LoadTexture(renderer, "background.png");
	//
	while(running)
	{
		//snapshot frame start
		FrameTime = SDL_GetTicks();
		//
		running = Input(ptr_ev);
		UpdateLogic(ptr_ev, ptr_Player, ptr_PlayerInside);
		DrawAndPresent(renderer, ptr_Player, ptr_PlayerInside, background_texture);
		//delay next frame if it finished faster than desired
		FrameTime = SDL_GetTicks() - FrameTime;
		if(1000.0/FPS > (float)FrameTime)
		{
			SDL_Delay((float)1000.0/FPS - (float)FrameTime);
		}
		//
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


bool Input(auto l_ptr_ev)
{
	SDL_PollEvent(l_ptr_ev);
		if(l_ptr_ev->type == SDL_QUIT)
		{
				return false;
		}
	return true;
}


void UpdateLogic(auto l_ptr_ev, auto l_ptr_P, auto l_ptr_PI)
{
	if(l_ptr_ev->key.keysym.sym == SDLK_RIGHT)
	{
		l_ptr_P->x += 10;
		l_ptr_PI->x += 10;
	}
	if(l_ptr_ev->key.keysym.sym == SDLK_LEFT)
	{
		l_ptr_P->x -= 10;
		l_ptr_PI->x -= 10;
	}
		
	if(l_ptr_ev->key.keysym.sym == SDLK_UP)
	{
		l_ptr_P->y -= 10;
		l_ptr_PI->y -= 10;
	}
	if(l_ptr_ev->key.keysym.sym == SDLK_DOWN)
	{
		l_ptr_P->y += 10;
		l_ptr_PI->y += 10;
	}
}


void DrawAndPresent(auto l_renderer, auto l_ptr_P, auto l_ptr_PI, auto l_background)
{
	SDL_RenderClear(l_renderer);
	//Draw Background
	SDL_RenderCopy(l_renderer, l_background, NULL, NULL);
	//Draw Player
	SDL_SetRenderDrawColor(l_renderer, 200, 100, 255, 255);
	SDL_RenderFillRect(l_renderer, l_ptr_P);
	SDL_SetRenderDrawColor(l_renderer, 255, 100, 0, 255);
	SDL_RenderFillRect(l_renderer, l_ptr_PI);

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