#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <boost/tr1/memory.hpp>
#include "scene_interface.hpp"
#include "inputstate.hpp"

#include "printlog.hpp"

#include "inputmachine.hpp"

#include "testscene.hpp"
#include "visualnovelscene.hpp"

SDL_Window* window = NULL;

void run(std::tr1::shared_ptr<SceneInterface> scene)
{
	InputState inputState;
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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
			CaptureInputState(&inputState, &e);
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
	std::tr1::shared_ptr<VisualNovelScene> ts(new VisualNovelScene());
	
	run(ts);
}

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);   // Initialize SDL2
	TTF_Init();

	SDL_DisplayMode mode;
	int WIDTH = 800, HEIGHT = 600;

	//if (SDL_GetCurrentDisplayMode(0, &mode)==0)
	//{
	//	/* I read that android ignores these so you can just as well set
	//	them to 0 */
	//	WIDTH=mode.w;
	//	HEIGHT=mode.h;
	//}

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
		game();
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
	SDL_Quit(); 
	
	printlog("Success!\n");
	
	return 0;
}
