#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>



const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 640;
const int BOARD_OFFSET = 80;
const int BOARD_SIZE = 480;

const float FPS = 60.0F;


class Entity
{
	public:
		SDL_Rect rect{};
		Entity(int x, int y, int w, int h)
		{
			rect.x = x;
			rect.y = y;
			rect.w = w;
			rect.h = h;
		}
		Entity() = default;
};

class Player : public Entity
{
	public:
		Entity::Entity; 
		SDL_Texture * texture = NULL;
		SDL_Texture* liv_texture = NULL;
		SDL_Rect liv_rect{SCREEN_HEIGHT, 60, 30, 30};
		SDL_Texture* liv_text = NULL;
		SDL_Rect liv_text_rect{};
		uint32_t movdelay = 0;
		int lives = 3;
		bool boundcheck_x_l()
		{
			if( rect.x > BOARD_OFFSET)
				return true;
			return false;
		}
		bool boundcheck_x_r()
		{
			if( rect.x < BOARD_SIZE)
				return true;
			return false;
		}
		bool boundcheck_y_u()
		{
			if( rect.y > BOARD_OFFSET)
				return true;
			return false;
		}
		bool boundcheck_y_d()
		{
			if( rect.y < BOARD_SIZE)
				return true;
			return false;
		}
		bool delaycheck()
		{
			if( SDL_GetTicks() > movdelay + 128)
				return true;
			return false;
		}
};

class Enemy : public Entity
{
	public:
		Enemy(int x, int y, int w, int h, int l){
			rect.x = x;
			rect.y = y;
			rect.w = w;
			rect.h = h;
			lifetime = l;
		}
		unsigned int lifetime;
		int damage;
};


bool Setup();
void Input(auto l_ptr_ev, auto l_state);
bool UpdateLogic(auto l_ptr_p, auto l_ptr_stage, auto l_state, auto en_vec, auto l_ptr_ev, auto l_field, auto l_timer);
void DrawAndPresent(auto l_renderer, auto l_ptr_p, auto l_bg, auto l_window, auto l_timer);
void CleanUp(auto l_window, auto l_renderer, auto l_bg);

int main(int argc, char* argv[])
{
	unsigned int timer = 0;
	std::srand(std::time(nullptr));
	std::vector<Enemy*> enemy_vec;
	int field[5][5];
	//TTF_Font* font = NULL;
	// font source: https://fontmeme.com/fonts/freemono-font/
	/*
	TTF_Font* font = TTF_OpenFont("FreeMono.ttf", 16);
	if (font == NULL) {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }
	*/
	bool running = true;
	SDL_Event ev;
	SDL_Event* ptr_ev = &ev;
	SDL_Texture *bg_texture = NULL;
	Player player(80, 80, 80, 80);
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
	//SDL_Surface *window_surface = SDL_GetWindowSurface(window);

	player.texture = IMG_LoadTexture(renderer, "player.png");
	player.liv_texture = IMG_LoadTexture(renderer, "health.png");
	bg_texture = IMG_LoadTexture(renderer, bg_vec[stage]);
	

	while(running)
	{
		//snapshot frame start
		FrameTime = SDL_GetTicks();
		//
		Input(ptr_ev, state);
		running = UpdateLogic(ptr_player, ptr_stage, state, enemy_vec, ptr_ev, field, timer);
		
		DrawAndPresent(renderer, ptr_player, bg_texture, window, timer);
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
	/*if(TTF_Init() < 0)
	{
		std::cout << "TTF_Init error: " << TTF_GetError() << std::endl;
	}*/
	return false;
}


void Input(auto l_ptr_ev, auto l_state)
{
	SDL_PollEvent(l_ptr_ev);
	l_state = SDL_GetKeyboardState(nullptr);
}


bool UpdateLogic(auto l_ptr_p, auto l_ptr_stage, auto l_state, auto en_vec, auto l_ptr_ev, auto l_field, auto l_timer)
{
	l_timer = l_timer + 1;
	if(l_state[SDL_SCANCODE_RIGHT])
	{
		if(l_ptr_p->boundcheck_x_r() && l_ptr_p->delaycheck() )
		{
			l_ptr_p->rect.x += 100;
			l_ptr_p->movdelay = SDL_GetTicks();
		}
	}
	if(l_state[SDL_SCANCODE_LEFT])
	{
		if(l_ptr_p->boundcheck_x_l() && l_ptr_p->delaycheck() )
		{
			l_ptr_p->rect.x -= 100;
			l_ptr_p->movdelay = SDL_GetTicks();
		}
	}
	if(l_state[SDL_SCANCODE_UP])	

	{
		if(l_ptr_p->boundcheck_y_u() && l_ptr_p->delaycheck() )
		{
			l_ptr_p->rect.y -= 100;
			l_ptr_p->movdelay = SDL_GetTicks();
		}
	}
	if(l_state[SDL_SCANCODE_DOWN])
	{
		if(l_ptr_p->boundcheck_y_d() && l_ptr_p->delaycheck() )
		{
			l_ptr_p->rect.y += 100;
			l_ptr_p->movdelay = SDL_GetTicks();
		}
	}
	for( long long unsigned int i = 0; i < del_vec.size(); i++)
	{
		if(del_vec[i]->lifetime <= 0)
		{
			del_vec.erase(del_vec.begin() + i);
			i--;
		}
	}

	//closing the program
	if(l_ptr_ev->type == SDL_QUIT)
	{
		return false;
	}
	if(l_ptr_p->lives <= 0)
	{
		return false;
	}
	return true;
}


void DrawAndPresent(auto l_renderer, auto l_ptr_p, auto l_bg, auto l_window, auto l_timer)
{
	SDL_RenderClear(l_renderer);
	//Draw Background
	SDL_RenderCopy(l_renderer, l_bg, NULL, NULL);
	//Draw Player
	//SDL_SetRenderDrawColor(l_renderer, 200, 100, 255, 255);
	//SDL_RenderFillRect(l_renderer, &l_ptr_p->rect);
	SDL_RenderCopyEx(l_renderer, l_ptr_p->texture, NULL, &l_ptr_p->rect, 0, NULL, SDL_FLIP_NONE);
	

	//SDL_RenderCopyEx(l_renderer, text_texture, NULL, &text_rec, 0, NULL, SDL_FLIP_NONE  );
	//render lives
	for(int i = 0; i < l_ptr_p->lives; i++)
	{
		l_ptr_p->liv_rect.x += 30;
		SDL_RenderCopyEx(l_renderer, l_ptr_p->liv_texture, NULL, &l_ptr_p->liv_rect, 0, NULL, SDL_FLIP_NONE );
	}
	l_ptr_p->liv_rect.x -= 30 * l_ptr_p->lives;


	//SDL_BlitSurface(l_ptr_p->texture, NULL, l_window_surface, NULL);
	//SDL_UpdateWindowSurface(l_window);
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