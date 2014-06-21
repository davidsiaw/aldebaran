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

#include "scriptedscene.hpp"

#include "manifestbasedresources.hpp"

#include "compositescene.hpp"
#include "debugrectscene.hpp"
#include "containerviewscene.hpp"
#include "imageviewscene.hpp"
#include "spriteviewscene.hpp"
#include "celviewscene.hpp"

#include "transformableviewscene.hpp"
#include "tweenscene.hpp"
#include "textviewscene.hpp"

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


class PhysicalRigidBodyScene : public SceneInterface
{
    std::tr1::shared_ptr<b2Body> body;
    std::tr1::shared_ptr<TransformableViewScene> texture;
    
public:
    PhysicalRigidBodyScene(std::tr1::shared_ptr<b2Body> body, std::tr1::shared_ptr<TransformableViewScene> texture) : body(body), texture(texture)
    {
    }
    
    virtual ~PhysicalRigidBodyScene()
    {
        
    }
    
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer)
    {
        texture->Init(window, renderer);
    }
    
	virtual void Update(const InputState& inputs, Uint32 timestamp)
    {
        body->SetAwake(true);
        texture->Update(inputs, timestamp);
    };
    
	virtual void Render(SDL_Renderer *renderer)
    {
        auto pos = body->GetPosition();
        texture->SetOrigin(pos.x, pos.y);
        texture->SetRotation(body->GetAngle() * 180 / M_PI);
        texture->Render(renderer);
    }
    
	virtual bool Running() const { return body->IsActive(); }
};

class PhysicalWorldScene : public SceneInterface
{
    b2Vec2 gravity;
    std::tr1::shared_ptr<b2World> world;
    
public:
    PhysicalWorldScene() : gravity(0.0f, 0.0f), world(new b2World(gravity))
    {
        
    }
    
    virtual ~PhysicalWorldScene()
    {
        
    }
    
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer) { }
    
	virtual void Update(const InputState& inputs, Uint32 timestamp)
    {
        float32 timeStep = 1.0f / 60.0f;
        int32 velocityIterations = 6;
        int32 positionIterations = 2;
        
        world->Step(timeStep, velocityIterations, positionIterations);
    };
    
	virtual void Render(SDL_Renderer *renderer) { }
    
	virtual bool Running() const { return true; }
    
    virtual std::tr1::shared_ptr<PhysicalRigidBodyScene> AddBox(std::tr1::shared_ptr<ViewSceneInterface> texture, int x ,int y, int w, int h)
    {
        std::tr1::shared_ptr<TransformableViewScene> tvs(new TransformableViewScene(texture, w, h));
        
        b2PolygonShape shape;
        shape.SetAsBox(w/2.0, h/2.0);
        
        b2BodyDef def;
        def.type = b2_dynamicBody;
        def.position.Set(x, y);
        
        std::tr1::shared_ptr<b2Body> body(world->CreateBody(&def), [=](b2Body* body){ world->DestroyBody(body); });
        
        body->CreateFixture(&shape, 1.0f);
        
        std::tr1::shared_ptr<PhysicalRigidBodyScene> theBody(new PhysicalRigidBodyScene(body, tvs));
        
        return theBody;
    }
    
    virtual std::tr1::shared_ptr<PhysicalRigidBodyScene> AddFixedBox(std::tr1::shared_ptr<ViewSceneInterface> texture, int x, int y, int w, int h)
    {
        std::tr1::shared_ptr<TransformableViewScene> tvs(new TransformableViewScene(texture, w, h));
        
        b2PolygonShape shape;
        shape.SetAsBox(w/2.0, h/2.0);
        
        b2BodyDef def;
        def.position.Set(x, y);
        
        std::tr1::shared_ptr<b2Body> body(world->CreateBody(&def), [=](b2Body* body){ world->DestroyBody(body); });
        
        body->CreateFixture(&shape, 1.0f);
        
        std::tr1::shared_ptr<PhysicalRigidBodyScene> theBody(new PhysicalRigidBodyScene(body, tvs));
        
        return theBody;
    }
    
    b2Vec2 GetGravity() const { return world->GetGravity(); }
    
    void SetGravity(b2Vec2 grav) { return world->SetGravity(grav); }
};


class ProcessorScene : public SceneInterface
{
    std::tr1::function<bool(const InputState& inputs, Uint32 timestamp)> proc;
    bool running;
    
public:
    ProcessorScene(std::tr1::function<bool(const InputState& inputs, Uint32 timestamp)> proc) : proc(proc)
    {
    }
    
	virtual ~ProcessorScene() {}
	
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer) { running = true; };
    
	virtual void Update(const InputState& inputs, Uint32 timestamp)
    {
        running = proc(inputs, timestamp);
    };
    
	virtual void Render(SDL_Renderer *renderer) { };
	virtual bool Running() const { return running; };
    
};

void game()
{
	//std::tr1::shared_ptr<SceneInterface> ts(new VisualNovelScene());
	//
	//Mix_Music* music = Mix_LoadMUS("01 Connect.flac");
	//if(!music)
	//{
	//	printf("Mix_LoadMUS: %s\n", Mix_GetError());
	//}
	//auto res = Mix_PlayMusic(music, 1);
	//if(res==-1)
	//{
	//	printf("Mix_PlayMusic: %s\n", Mix_GetError());
	//}
	//
	//std::tr1::shared_ptr<FadeScene> fs(new FadeScene(ts));
	//run(fs);
    
    
	
	std::tr1::shared_ptr<ResourceInterface> resources(new ManifestBasedResources("manifest.lua", renderer));
    
	std::tr1::shared_ptr<ImageViewScene> debugRect(new ImageViewScene(resources->GetImage(7)));
    
    std::tr1::shared_ptr<ImageViewScene> staticImage(new ImageViewScene(resources->GetImage(3)));
    staticImage->SetOrigin(200, 0);
    
    std::tr1::shared_ptr<SpriteViewScene> dynamicImage(new SpriteViewScene(resources->GetSprite(10001)));
    dynamicImage->SetOrigin(250, 0);
    
	std::tr1::shared_ptr<CompositeScene> topLevel(new CompositeScene());
    
	std::tr1::shared_ptr<CompositeScene> shiftedScene(new CompositeScene());
	std::tr1::shared_ptr<ContainerViewScene> shiftedStuff(new ContainerViewScene(shiftedScene));
    shiftedStuff->SetOrigin(200, 200);
    
    shiftedScene->AddScene(debugRect);
    shiftedScene->AddScene(staticImage);
    shiftedScene->AddScene(dynamicImage);
    
    std::tr1::shared_ptr<ImageViewScene> img(new ImageViewScene(resources->GetImage(3)));
	std::tr1::shared_ptr<TransformableViewScene> ball(new TransformableViewScene(img, 48, 48));
    std::tr1::shared_ptr<TweenScene> tw(new TweenScene(ball));
    tw->PushTargetState(0, 0, 400, 400, 360, 1, 1000, TweenScene::EASE_OUT);
    tw->PushTargetState(0, 0, 200, 200, 360, 1, 1000, TweenScene::EASE_IN);
    
    std::tr1::shared_ptr<TextViewScene> text(new TextViewScene(resources->GetFont(20002), "Helvetica Light", 100));

    tw->SetLoops(3);
    //topLevel->AddScene(text);
    topLevel->AddScene(tw);
    
    
    
    
    
    
    std::tr1::shared_ptr<TextViewScene> xyz(new TextViewScene(resources->GetFont(20002), "meow"));
    
	std::tr1::shared_ptr<TransformableViewScene> textbox(new TransformableViewScene(xyz,750, 80));
    textbox->SetOrigin(500, 500);
    
    topLevel->AddScene(textbox);
    
    
    
    
    
    topLevel->AddScene(shiftedStuff);
    //topLevel->AddScene(debugRect);
    
    
    
    std::tr1::shared_ptr<PhysicalWorldScene> world(new PhysicalWorldScene());
    
    auto someimage = std::tr1::shared_ptr<ImageViewScene>(new ImageViewScene(resources->GetImage(1)));
    
    auto box = world->AddBox(std::tr1::shared_ptr<ImageViewScene>(new ImageViewScene(resources->GetImage(8))), 0, 0, 300, 294);
    auto topborder = world->AddFixedBox(someimage, -1, -1, 2050, 100);
    auto leftborder = world->AddFixedBox(someimage, -1, -1, 100, 1538);
    auto bottomborder = world->AddFixedBox(someimage, -1, 1536, 2050, 100);
    auto rightborder = world->AddFixedBox(someimage, 2048, -1, 1538, 100);
    
    topLevel->AddScene(world);
    topLevel->AddScene(box);
    
    topLevel->AddScene(topborder);
    topLevel->AddScene(leftborder);
    topLevel->AddScene(bottomborder);
    topLevel->AddScene(rightborder);
    
    
    
    
    
    std::tr1::shared_ptr<ProcessorScene> proc(new ProcessorScene([=](const InputState& inputs, Uint32 timestamp) -> bool
     {
         
         auto joy = SDL_JoystickOpen(0);
         
         auto x = SDL_JoystickGetAxis(joy, 0) * SDL_IPHONE_MAX_GFORCE / 0x7FFF;
         auto y = SDL_JoystickGetAxis(joy, 1) * SDL_IPHONE_MAX_GFORCE / 0x7FFF;
         auto z = SDL_JoystickGetAxis(joy, 2) * SDL_IPHONE_MAX_GFORCE / 0x7FFF;
         
         std::stringstream str;
         str << "x = " << x << ", y = " << y << ", z = " << z;
         
         double angle = atan2(y,x);
         textbox->SetRotation(angle * 180 / M_PI);
         
         b2Vec2 grav(-y, x);
         world->SetGravity(grav);
         
         xyz->SetText(str.str());
         return true;
     }));
    
    topLevel->AddScene(proc);
    
    
    
	run(topLevel);
    
	//std::tr1::shared_ptr<ScriptedScene> scene(new ScriptedScene("script/main.lua"));
	//run(scene);
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
