#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <boost/tr1/memory.hpp>
#include <boost/tr1/functional.hpp>

#include <Box2D/Box2D.h>

#include "scene_interface.hpp"
#include "inputstate.hpp"

#include "printlog.hpp"

#include "inputmachine.hpp"

#include "manifestbasedresources.hpp"

#include "notifier.hpp"

SDL_Window* window = NULL;
SDL_Renderer *renderer = NULL;
static std::map<int, InputState::Key> keyMap;

void run(std::tr1::shared_ptr<SceneInterface> scene)
{
	InputState inputState;
	Uint32 time = SDL_GetTicks();
	scene->Init(window, renderer);
	
	while (scene->Running())
	{
		SDL_Event e;
		if ( SDL_PollEvent(&e) )
		{
			if (e.type == SDL_QUIT)
			{
				break;
			}
			CaptureInputState(keyMap, &inputState, &e);
		}
		
		Uint32 now = SDL_GetTicks();
		scene->Update(inputState, now);
		if (now - time > 15)
		{
			SDL_RenderClear(renderer);
			scene->Render(renderer);
			SDL_RenderPresent(renderer);
			time = SDL_GetTicks();
		}
	}
}

void game()
{
}

int main(int argc, char** argv)
{
	keyMap[SDLK_UP] = InputState::UP;
	keyMap[SDLK_DOWN] = InputState::DOWN;
	keyMap[SDLK_LEFT] = InputState::LEFT;
	keyMap[SDLK_RIGHT] = InputState::RIGHT;
	keyMap[SDLK_a] = InputState::A;
	keyMap[SDLK_s] = InputState::B;
	keyMap[SDLK_z] = InputState::X;
	keyMap[SDLK_x] = InputState::Y;
	keyMap[SDLK_1] = InputState::L1;
	keyMap[SDLK_3] = InputState::L2;
	keyMap[SDLK_2] = InputState::R1;
	keyMap[SDLK_4] = InputState::R2;
	keyMap[SDLK_SPACE] = InputState::START;
    
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);
    
	// Something to note here: We don't initialize mp3 because there are too many
	// issues with it. Instead, simply make FLAC or OGG files. They are free
	// FLAC is better too because of its higher quality.
	Mix_Init(MIX_INIT_FLAC|MIX_INIT_MOD|MIX_INIT_OGG);
    
	SDL_DisplayMode mode;
	int WIDTH = 800, HEIGHT = 600;
    
	//if (SDL_GetCurrentDisplayMode(0, &mode)==0)
	//{
	//	/* I read that android ignores these so you can just as well set
	//	them to 0 */
	//	WIDTH=mode.w;
	//	HEIGHT=mode.h;
	//}
    
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1)
	{
		printlog("Mix_OpenAudio: %s\n", Mix_GetError());
	}
    
	printlog("Window size: %d x %d!\n", WIDTH, HEIGHT);
    
	// Create an application window with the following settings:
	window = SDL_CreateWindow(
                              "Aldebaran",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WIDTH,
                              HEIGHT,
                              SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL
                              );
    
	// Check that the window was successfully made
	if(window == NULL)
	{
		// In the event that the window could not be made...
		printlog("Could not create window: %s\n", SDL_GetError());
		return 1;
	}
	
	printlog("Beginning game with window=%p\n", window);
	try
	{
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
		game();
		SDL_DestroyRenderer(renderer);
	}
	catch(std::exception e)
	{
		printlog("Caught a std::exception! %s\n", e.what());
	}
	catch(...)
	{
		printlog("Caught an unknown exception!\n");
	}
    
	SDL_DestroyWindow(window);
    
	// Clean up
	Mix_CloseAudio();
	Mix_Quit();
    
	TTF_Quit();
    
	SDL_Quit(); 
	
	printlog("Success!\n");
	
	return 0;
}
