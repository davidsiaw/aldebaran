#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <memory>
#include <functional>

#include <Box2D/Box2D.h>

#include "opengl.hpp"

#include "scene_interface.hpp"
#include "inputstate.hpp"

#include "printlog.hpp"

#include "inputmachine.hpp"

#include "manifestbasedresources.hpp"

#include "notifier.hpp"

#include "emptyscene.hpp"
#include "guiscene.hpp"
#include "vboscene.hpp"
#include "fpsscene.hpp"

#include "utils.hpp"

#include "defaultshader.hpp"
#include "colorshader.hpp"
#include "trianglevbo.hpp"
#include "quadvbo.hpp"
#include "quadcollectionvbo.hpp"
#include "viewscene.hpp"
#include "compositescene.hpp"
#include "gamecontext.hpp"

SDL_Surface* flipVert(SDL_Surface* sfc)
{
    SDL_Surface* result = SDL_CreateRGBSurface(sfc->flags, sfc->w, sfc->h,
                                               sfc->format->BytesPerPixel * 8, sfc->format->Rmask, sfc->format->Gmask,
                                               sfc->format->Bmask, sfc->format->Amask);
    if (result == NULL) return NULL;
    
    Uint8* pixels = (Uint8*) sfc->pixels;
    Uint8* rpixels = (Uint8*) result->pixels;
    
    Uint32 pitch = sfc->pitch;
    Uint32 pxlength = pitch*sfc->h;
    
    for(int line = 0; line < sfc->h; ++line) {
        Uint32 pos = line * pitch;
        memcpy(&rpixels[pos], &pixels[(pxlength-pos)-pitch], pitch);
    }
    
    return result;
}

void takeScreenShot(std::string filename, SDL_Window* window)
{
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	SDL_Surface* surf = SDL_CreateRGBSurface(
		SDL_SWSURFACE, 
		windowWidth,
		windowHeight,
		24, 
		0x000000FF, 
		0x0000FF00, 
		0x00FF0000, 
		0);
    
	if (surf == NULL) return;

	glReadPixels(0, 0, windowWidth, windowHeight, GL_RGB, GL_UNSIGNED_BYTE, surf->pixels);

    SDL_Surface * flip = flipVert(surf);
    if (flip == NULL) return;
    SDL_FreeSurface(surf);
    
    SDL_SaveBMP(flip, filename.c_str());
    
    SDL_FreeSurface(flip);
}

int exists(const char *fname)
{
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

void run(std::shared_ptr<SceneInterface> scene, std::shared_ptr<GameContext> context)
{
	InputState inputState;
	Uint32 time = SDL_GetTicks();
	scene->Init(context->GetWindow());
	
	while (scene->Running())
	{
		SDL_Event e;
		if ( SDL_PollEvent(&e) )
		{
			if (e.type == SDL_QUIT)
			{
				break;
			}
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_1 && (e.key.keysym.mod & (KMOD_CTRL | KMOD_SHIFT)) != 0)
                {
                    int count = 0;
                    std::string fname = "screenshot";
                    do
                    {
                        std::stringstream ss;
                        ss << "screenshot" << count << ".bmp";
                        fname = ss.str();
                        count++;
                    }
                    while (exists(fname.c_str()));
                    takeScreenShot(fname, context->GetWindow());
                }
            }
			CaptureInputState(context->GetKeyMap(), &inputState, &e);
		}
		
		Uint32 now = SDL_GetTicks();
		scene->Update(inputState, now);
		if (now - time > 15)
        {
            glClearColor ( 1.0, 1.0, 1.0, 1.0 );
            glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            scene->Render(context);
            SDL_GL_SwapWindow(context->GetWindow());
			time = SDL_GetTicks();
		}
	}
}

struct TextureItem
{
    int x,y,w,h;
    int animOffX, animOffY;
    int animFrameCount;
};

class SuperTexture
{
    std::vector<TextureItem> items;
    std::shared_ptr<SDL_Surface> texture;
public:
    SuperTexture()
    {
        
    }
};





void game(std::shared_ptr<GameContext> context)
{
    std::shared_ptr<ShaderInterface> shader(new DefaultShader());

    //std::shared_ptr<VboInterface> vbo(new QuadVbo(100,100,200,300));
    
    std::shared_ptr<QuadCollectionVbo> vbo(new QuadCollectionVbo());
    
    vbo->Add(QuadVbo(  0,  0, 64, 64,  0, 32, 32, 32, 256, 1124, 4, 4, 32));
    for (int y=0;y<10;y++)
    for (int x=0;x<10;x++)
    {
        vbo->Add(QuadVbo(x*48,y*48,48,48,  32*x, 32*y, 32, 32, 256, 1124, 0, 32, 6));
    }
    vbo->Add(QuadVbo(330, 100, 128, 128, 0, 96, 32, 32, 256, 1124));
    //vbo->Add(QuadVbo(460, 100, 128, 128));
    
    
    std::shared_ptr<SDL_Surface> tex(IMG_Load("tiles/pokemontiles.png"), SDL_FreeSurface);

    std::shared_ptr<VboScene> scene(new VboScene(shader, vbo, tex));
    
    //scene->SetMatrixTo2DView(960, 640);
    scene->SetMatrixTo2DRectangle(0, 0, context->GetScreenWidth(), context->GetScreenHeight());
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    
    std::shared_ptr<CompositeScene> comp = std::make_shared<CompositeScene>();
    
    std::shared_ptr<FPSScene> txtscene(new FPSScene());
    
    std::shared_ptr<ViewScene> view(new ViewScene(scene, 0, 0, 256, 256));
    
    comp->AddScene(scene, 300, 300);
    //comp->AddScene(scene, 0, 0);
    comp->AddScene(txtscene, 0, 0);
    comp->AddScene(view, 25, 25);
    
    //run(view);
    
    run(comp, context);
    
}

int main(int argc, char** argv)
{
    std::map<int, InputState::Key> keyMap;

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
    

	int WIDTH = 960, HEIGHT = 640;
    
    if (argc == 2)
    {
        printlog("Configuration file provided: %s\n", argv[1]);
        LuaTable config = LoadLuaConfiguration(argv[1]);
    }
    
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
	SDL_Window* window = SDL_CreateWindow(
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
        /* Request opengl 3.2 context.
         * SDL doesn't have the ability to choose which profile at this time of writing,
         * but it should default to the core profile */
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        
        /* Turn on double buffering with a 24bit Z buffer.
         * You may need to change this to 16 or 32 for your system */
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        
        SDL_GLContext context = SDL_GL_CreateContext(window);
        
        printlog("OpenGL context created\n");
        
        glViewport(0, 0, WIDTH, HEIGHT);
        
        std::shared_ptr<GameContext> gameContext = std::make_shared<GameContext>(window, keyMap, WIDTH, HEIGHT);
        
		game(gameContext);
        
		SDL_GL_DeleteContext(context);
	}
	catch(std::exception e)
	{
		printlog("Caught a std::exception! %s\n", e.what());
        return EXIT_FAILURE;
	}
	catch(...)
	{
		printlog("Caught an unknown exception!\n");
        return EXIT_FAILURE;
	}
    
	SDL_DestroyWindow(window);
    
	// Clean up
	Mix_CloseAudio();
	Mix_Quit();
    
	TTF_Quit();
    
	SDL_Quit(); 
	
	printlog("Program Successfully Terminated!\n");
	
	return EXIT_SUCCESS;
}
