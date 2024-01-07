#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>

const int SCREEN_HEIGHT = 800;
const int SCREEN_WIDTH = 640;

const float FPS = 60.0;

class entity
{
	public:
		entity(int xi, int yi, int size_xi, int size_yi, uint anglei)
		{
			x = xi;
			y = yi;
			size_x = size_xi;
			size_y = size_yi;
			angle = anglei;
		}
		int x;
		int y;
		int size_x;
		int size_y;
		int vel_x;
		int vel_y;
		int accel_x;
		int accel_y;
		uint angle;
	
};


bool Setup();
bool Input(auto l_ptr_ev);
void UpdateLogic(auto l_ptr_ev, auto l_ptr_p, auto l_ptr_stage);
void DrawAndPresent(auto l_renderer, auto l_ptr_p, auto l_bg);
void CleanUp(auto l_window, auto l_renderer, auto l_bg);

int main(int argc, char* argv[])
{
	std::vector<entity*> entity_vec;
	bool running = true;
	SDL_Event ev;
	SDL_Event* ptr_ev = &ev;
	SDL_Texture *bg_texture = NULL;
	entity player(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 20, 20, 0);
	entity* ptr_player = &player;
	uint32_t FrameTime = 0;
	std::vector<char*> bg_vec = {"menu.png", "background.png"};
	int stage = 1;
	int* ptr_stage = &stage;

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
	bg_texture = IMG_LoadTexture(renderer, bg_vec[stage]);
	
	while(running)
	{
		//snapshot frame start
		FrameTime = SDL_GetTicks();
		//
		running = Input(ptr_ev);
		UpdateLogic(ptr_ev, ptr_player, ptr_stage);
		DrawAndPresent(renderer, ptr_player, bg_texture);
		//delay next frame if it finished faster than desired
		FrameTime = SDL_GetTicks() - FrameTime;
		if(1000.0/FPS > (float)FrameTime)
		{
			SDL_Delay((float)1000.0/FPS - (float)FrameTime);
		}
		//
	}
	CleanUp(window, renderer, bg_texture);
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


void UpdateLogic(auto l_ptr_ev, auto l_ptr_p, auto l_ptr_stage)
{
	if(l_ptr_ev->key.keysym.sym == SDLK_RIGHT)
	{
		l_ptr_p->x += 10;
	}
	if(l_ptr_ev->key.keysym.sym == SDLK_LEFT)
	{
		l_ptr_p->x -= 10;
	}
		
	if(l_ptr_ev->key.keysym.sym == SDLK_UP)
	{
		l_ptr_p->y -= 10;
	}
	if(l_ptr_ev->key.keysym.sym == SDLK_DOWN)
	{
		l_ptr_p->y += 10;
	}
}


void DrawAndPresent(auto l_renderer, auto l_ptr_p, auto l_bg)
{
	SDL_RenderClear(l_renderer);
	//Draw Background
	SDL_RenderCopy(l_renderer, l_bg, NULL, NULL);
	//Draw Player
	SDL_Rect rect{l_ptr_p->x, l_ptr_p->y, l_ptr_p->size_x, l_ptr_p->size_y};
	SDL_SetRenderDrawColor(l_renderer, 200, 100, 255, 255);
	SDL_RenderFillRect(l_renderer, &rect);
	

	SDL_RenderPresent(l_renderer);
}


void CleanUp(auto l_window, auto l_renderer, auto l_bg)
{
	SDL_DestroyTexture(l_bg);
	SDL_DestroyRenderer(l_renderer);
	SDL_DestroyWindow(l_window);
	IMG_Quit();
	SDL_Quit();
}
