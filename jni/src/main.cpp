#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <memory>
#include <functional>
#include <list>

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
#include "wireframeshader.hpp"
#include "colorshader.hpp"
#include "trianglevbo.hpp"
#include "quadvbo.hpp"
#include "quadcollectionvbo.hpp"
#include "viewscene.hpp"
#include "compositescene.hpp"
#include "gamecontext.hpp"

#include "twodarray.hpp"

#include "rpgmapscene.hpp"


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
		if (e.key.keysym.sym == SDLK_c && (e.key.keysym.mod & KMOD_CTRL) != 0)
		{
			break;
		}
            }
			CaptureInputState(context->GetKeyMap(), &inputState, &e);
		}
		
		Uint32 now = SDL_GetTicks();
		scene->Update(inputState, now);
		if (now - time > 15)
        {
            glClearColor( 1.0, 1.0, 0.0, 1.0 );
            glClear ( GL_COLOR_BUFFER_BIT );
            scene->Render(context);
            SDL_GL_SwapWindow(context->GetWindow());
			time = SDL_GetTicks();
		}
	}
}

class ProcessingScene : public ComposableSceneInterface
{
    std::function<void(const InputState&, Uint32)> update;
    std::function<bool()> running;
    
public:
    
    ProcessingScene(std::function<void(const InputState&, Uint32)> update, std::function<bool()> running) :
    update(update), running(running)
    {
        
    }
    
    virtual void Init(SDL_Window* window) { }

    virtual void Update(const InputState& inputs, Uint32 timestamp)
    {
        update(inputs, timestamp);
    }

    virtual void Render(std::shared_ptr<RenderContextInterface> renderContext) { }

    virtual bool Running() const
    {
        return running();
    }
    
    virtual void SetOrigin(Uint16 x, Uint16 y, Uint16 z) { }
    virtual Uint16 GetOriginX() const { return 0; }
    virtual Uint16 GetOriginY() const { return 0; }
    virtual Uint16 GetOriginZ() const { return 0; }
};

class Bullet
{
public:
    int x;
    int y;
    size_t id;
    bool active;
    int speed;
    Uint32 lastmove;
    int updateDelay;

    Bullet(int x, int y, size_t id) : 
        x(x), 
        y(y), 
        id(id), 
        active(true),
        lastmove(0),
        updateDelay(1)
    {

    }

};

class ShooterScene : public ComposableSceneInterface
{
    std::shared_ptr<CompositeScene> composite;
    bool running;

    Uint32 lastUpdate;
    double accel;
    double speed;
    double x;
    double y;
    int max;
    double maxspeed;
    double thrust;
    double slow;

    int lastShoot;
    int shootCooldown;

    size_t shipId;


    std::shared_ptr<VboScene> scene;
    std::shared_ptr<QuadCollectionVbo> vbo;

    std::shared_ptr<SDL_Surface> arrow;
    std::vector<std::shared_ptr<Bullet>> bullets;

public:
    ShooterScene() : 
        composite(new CompositeScene()),
        running(true),
        lastUpdate(0),
        accel(0),
        speed(0),
        x(0),
        y(500),
        max(700),
        maxspeed(10),
        thrust(1.0),
        slow(0.5),
        shootCooldown(100)
    {
        //auto shader = std::make_shared<WireframeShader>();
        auto shader = std::make_shared<DefaultShader>();
        vbo = std::make_shared<QuadCollectionVbo>();
        arrow = LoadImage("images/arrow.png");
        scene = std::make_shared<VboScene>(shader, vbo, arrow);
        auto quad = QuadVbo(x, y, 0, 50, 50);
        shipId = vbo->Add(quad);
        auto arrowscene = composite->AddScene(scene, 0, 0);

    }
    
    virtual void Init(SDL_Window* window)
    {   
        int w,h;
        SDL_GetWindowSize(window, &w, &h);
        scene->SetMatrixTo2DRectangle(0, 0, w, h);
        composite->Init(window);
    }

    virtual void Update(const InputState& inputs, Uint32 timestamp)
    {
        Uint32 now = timestamp;

        if (inputs.GetButtonState(InputState::DOWN))
        {
        }
        
        if (inputs.GetButtonState(InputState::UP))
        {
            if (now - lastShoot > shootCooldown)
            {
                auto quad = QuadVbo(x+15, y, 0, 20, 20);
                auto quadId = vbo->Add(quad);

                printlog("shoot bullet %d\n", bullets.size());
                auto bullet = std::make_shared<Bullet>(x+15, y, quadId);

                bool pushed = false;
                for (int i=0;i<bullets.size();i++)
                {
                    if (!bullets[i]->active)
                    {
                        bullets[i] = bullet;
                        pushed = true;
                        break;
                    }
                }
                if (!pushed)
                {
                    bullets.push_back(bullet);
                }

                lastShoot = now;
            }
        }

        if (inputs.GetButtonState(InputState::LEFT))
        {
            accel = -thrust;
        }
        else if (inputs.GetButtonState(InputState::RIGHT))
        {
            accel = thrust;
        }
        else
        {
            accel = 0;
        }

        for (size_t i=0;i<bullets.size();i++)
        {
            auto bullet = bullets[i];
            if (bullet->active)
            {
                if (now - bullet->lastmove > bullet->updateDelay)
                {
                    bullet->y -= 1;

                    auto quad = QuadVbo(bullet->x, bullet->y, 0, 20, 20);
                    vbo->Modify(bullet->id, quad);

                    if (bullet->y < -40)
                    {
                        bullet->active = false;
                        printlog("bullet %d dies\n", i);
                    }
                    bullet->lastmove = now;
                }
            }
        }

        if (now - lastUpdate > 10)
        {
            if (accel == 0)
            {
                speed -= sgn(speed) * slow;
            }

            speed += accel;
            if (speed < -maxspeed)
            {
                speed = -maxspeed;
            }
            if (speed > maxspeed)
            {
                speed = maxspeed;
            }

            x += speed;
            if (x < 0)
            {
                x = 0;
                speed = 0;
            }
            if (x > max)
            {
                x = max;
                speed = 0;
            }
            auto quad = QuadVbo(x, y, 0, 50, 50);
            vbo->Modify(shipId, quad);
            lastUpdate = timestamp;
        }
        composite->Update(inputs, timestamp);


    }

    virtual void Render(std::shared_ptr<RenderContextInterface> renderContext)
    {
        composite->Render(renderContext);
    }

    virtual bool Running() const
    {
        return running;
    }
    
    virtual void SetOrigin(Uint16 x, Uint16 y, Uint16 z) { }
    virtual Uint16 GetOriginX() const { return 0; }
    virtual Uint16 GetOriginY() const { return 0; }
    virtual Uint16 GetOriginZ() const { return 0; }
};


void game(std::shared_ptr<GameContext> context)
{

    auto shooter = std::make_shared<ShooterScene>();
    run(shooter, context);
    
}

void set_gl_attribute(SDL_GLattr attr, int value)
{
	if (SDL_GL_SetAttribute(attr, value) != 0)
	{
		printlog("SDL_GL_SetAttribute(%d, %d) failed: %s\n", attr, value, SDL_GetError());
	}
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
    
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printlog("Failed to initialize SDL: %s\n", SDL_GetError());
	}
	TTF_Init();
	IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);
    
	// Something to note here: We don't initialize mp3 because there are too many
	// issues with it. Instead, simply make FLAC or OGG files. They are free
	// FLAC is better too because of its higher quality.
	Mix_Init(MIX_INIT_FLAC|MIX_INIT_MOD|MIX_INIT_OGG);
    

	int WIDTH = 960, HEIGHT = 540;
    
    if (argc == 2)
    {
        printlog("Configuration file provided: %s\n", argv[1]);
        LuaTable config = LoadLuaConfiguration(argv[1]);
    }
    
	for (int i=0; i<SDL_GetNumVideoDisplays(); ++i)
	{
		SDL_DisplayMode current;
		int should_be_zero = SDL_GetCurrentDisplayMode(i, &current);

		if(should_be_zero != 0)
		{
			printlog("Could not get display mode for video display #%d: %s\n", i, SDL_GetError());
		}
		else
		{
			printlog("Display #%d: current display mode is %dx%dpx @ %dhz.\n", i, current.w, current.h, current.refresh_rate);
		}
	}
    
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
                              SDL_WINDOW_OPENGL
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
#ifdef __RASPBERRYPI__
		set_gl_attribute(SDL_GL_RED_SIZE, 5);
		set_gl_attribute(SDL_GL_GREEN_SIZE, 6);
		set_gl_attribute(SDL_GL_BLUE_SIZE, 5);
		//set_gl_attribute(SDL_GL_DEPTH_SIZE, 8);
		
		set_gl_attribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		set_gl_attribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		set_gl_attribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#else // __RASPBERRYPI__
		set_gl_attribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		set_gl_attribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif // __RASPBERRYPI__

		
		set_gl_attribute(SDL_GL_DOUBLEBUFFER, 1);
		
		SDL_GLContext context = SDL_GL_CreateContext(window);
		if(context == NULL)
		{
			printlog("Could not create GL context: %s\n", SDL_GetError());
			return 1;
		}
		
		
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
