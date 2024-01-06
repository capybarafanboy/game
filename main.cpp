#include <SDL2/SDL.h>
#include <iostream>



void InitialiseSDL()
{
	SDL_Init(SDL_INIT_EVERYTHING);
}



void DrawAndPresent(auto l_renderer, auto l_r)
{
	SDL_SetRenderDrawColor(l_renderer, 0, 0, 0, 255);
	SDL_RenderClear(l_renderer);

	SDL_SetRenderDrawColor(l_renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(l_renderer, &l_r);

	SDL_RenderPresent(l_renderer);
}

int main(int argc, char* argv[])
{
	SDL_Event ev;
	bool running = true;
	
	InitialiseSDL();

	SDL_Rect Player{10, 10, 250, 250};

	auto window = SDL_CreateWindow
	(
		"Boom",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800, 640, 0
	);

	auto renderer = SDL_CreateRenderer(window, -1, 0);


	while(running)
	{
		while(SDL_PollEvent(&ev))
		{
			if(ev.type == SDL_QUIT)
			{
				running = false;
			}
			if(ev.type == SDL_KEYDOWN)
			{
				switch(ev.key.keysym.sym)
				{
					case SDLK_RIGHT:
						Player.x += 50;
						break;
					case SDLK_LEFT:
						Player.x -= 50;
						break;
					case SDLK_UP:
						Player.y -= 50;
						break;
					case SDLK_DOWN:
						Player.y += 50;
						break;
				}
			}
			else if(ev.type == SDL_KEYUP)
			{
				switch(ev.key.keysym.sym)
				{
				//	case SDLK_RIGHT:
				//	break;
				}
			}

			DrawAndPresent(renderer, Player);
			SDL_Delay(10);
		}
	}
	
    return 0;
}