#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>
#include <string>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 640;
const int BOARD_OFFSET = 80;
const int BOARD_SIZE = 480;

const float FPS = 60.0F;


class Entity
{
	public:
		Entity(int xi, int yi, int size_xi, int size_yi)
		{
			x = xi;
			y = yi;
			size_x = size_xi;
			size_y = size_yi;
		}
		int x;
		int y;
		int size_x;
		int size_y;
};

class Player : public Entity
{
	public:
		Entity::Entity; 
		SDL_Texture * texture;
		uint32_t movdelay;
		bool boundcheck_x_l()
		{
			if( x > BOARD_OFFSET)
				return true;
			return false;
		}
		bool boundcheck_x_r()
		{
			if( x < BOARD_SIZE)
				return true;
			return false;
		}
		bool boundcheck_y_u()
		{
			if( y > BOARD_OFFSET)
				return true;
			return false;
		}
		bool boundcheck_y_d()
		{
			if( y < BOARD_SIZE)
				return true;
			return false;
		}
};

bool Setup();
bool Input(auto l_ptr_ev, auto l_state);
void UpdateLogic(auto l_ptr_p, auto l_ptr_stage, auto l_state);
void DrawAndPresent(auto l_renderer, auto l_ptr_p, auto l_bg);
void CleanUp(auto l_window, auto l_renderer, auto l_bg);

int main(int argc, char* argv[])
{
	std::vector<Entity*> entity_vec;
	bool running = true;
	SDL_Event ev;
	SDL_Event* ptr_ev = &ev;
	SDL_Texture *bg_texture = NULL;
	Player player(80, 80, 20, 20);
	Player* ptr_player = &player;
	uint32_t FrameTime = 0;
	std::vector<char*> bg_vec = {"menu.png", "background.png"};
	int stage = 1;
	int* ptr_stage = &stage;

	
	const Uint8* state = SDL_GetKeyboardState(nullptr);

	if(Setup())
	{
		return 1;
	}

	auto window = SDL_CreateWindow
	(
		"Boom",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
	);

	auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	bg_texture = IMG_LoadTexture(renderer, bg_vec[stage]);
	

	while(running)
	{
		//snapshot frame start
		FrameTime = SDL_GetTicks();
		//
		running = Input(ptr_ev, state);
		UpdateLogic(ptr_player, ptr_stage, state);
		DrawAndPresent(renderer, ptr_player, bg_texture);
		//delay next frame if it finished faster than desired
		FrameTime = SDL_GetTicks() - FrameTime;
		if(1000.0F/FPS > (float)FrameTime)
		{
			SDL_Delay(1000.0F/FPS - (float)FrameTime);
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


bool Input(auto l_ptr_ev, auto l_state)
{
	SDL_PollEvent(l_ptr_ev);
		if(l_ptr_ev->type == SDL_QUIT)
		{
				return false;
		}
	l_state = SDL_GetKeyboardState(nullptr);
	return true;
}


void UpdateLogic(auto l_ptr_p, auto l_ptr_stage, auto l_state)
{
	if(l_state[SDL_SCANCODE_RIGHT])
	{
		if(l_ptr_p->boundcheck_x_r())
			l_ptr_p->x += 100;
	}
	if(l_state[SDL_SCANCODE_LEFT])
	{
		if(l_ptr_p->boundcheck_x_l())
			l_ptr_p->x -= 100;
	}
	if(l_state[SDL_SCANCODE_UP])	

	{
		if(l_ptr_p->boundcheck_y_u())
			l_ptr_p->y -= 100;
	}
	if(l_state[SDL_SCANCODE_DOWN])
	{
		if(l_ptr_p->boundcheck_y_d())
			l_ptr_p->y += 100;
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
