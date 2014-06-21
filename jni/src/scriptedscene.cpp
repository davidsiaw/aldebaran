#include "scriptedscene.hpp"

#include <algorithm>
#include <vector>
#include <boost/foreach.hpp>

#include "imageviewscene.hpp"
#include "dialogscene.hpp"
#include "printlog.hpp"
#include "fadescene.hpp"
#include "utils.hpp"

ScriptedScene::ScriptedScene(std::string scriptFile) : 
	  lua(),
	  coroutine(lua.CreateCoroutine()),
	  scriptBlocker(NULL), 
	  running(true),
	  scriptFile(scriptFile)
{

}

ScriptedScene::~ScriptedScene()
{
}

void ScriptedScene::WaitFor(LuaUserdata<SceneInterface> scene)
{
	scriptBlocker = scene.GetPointer();
	printlog("waiting for scene: %x\n", scriptBlocker);
}

void ScriptedScene::Clear()
{
	scenes.clear();
}

void ScriptedScene::Display(LuaUserdata<SceneInterface> scene, int level)
{
	(*((scenes.insert(std::make_pair(level, scene))).first)).second = scene;
}

LuaUserdata<SceneInterface> ScriptedScene::FadeIn(int level)
{
	auto theScene = scenes.find(level)->second;
		
	auto userdata = lua.CreateUserdata<SceneInterface>(new FadeScene(theScene.GetPointer(), FadeScene::FadeIn));
	userdata.GetPointer()->Init(window, renderer);
	Display(userdata, level);

	return userdata;
}

LuaUserdata<SceneInterface> ScriptedScene::FadeOut(int level)
{
	auto theScene = scenes.find(level)->second;
		
	auto userdata = lua.CreateUserdata<SceneInterface>(new FadeScene(theScene.GetPointer(), FadeScene::FadeOut));
	userdata.GetPointer()->Init(window, renderer);
	Display(userdata, level);

	return userdata;
}

LuaUserdata<SceneInterface> ScriptedScene::NewImage(std::string filename)
{
	auto userdata = lua.CreateUserdata<SceneInterface>(NULL); //lua.CreateUserdata<SceneInterface>(new ImageScene(filename));
	userdata.GetPointer()->Init(window, renderer);
	printlog("created image scene: %x\n", userdata.GetPointer());

	return userdata;
}

LuaUserdata<SceneInterface> ScriptedScene::NewDialog(LuaTable text)
{
	std::vector<std::wstring> textVector;
	textVector.push_back(text.Get<std::wstring>(1));
	textVector.push_back(text.Get<std::wstring>(2));
	textVector.push_back(text.Get<std::wstring>(3));
	textVector.push_back(text.Get<std::wstring>(4));

	auto userdata = lua.CreateUserdata<SceneInterface>(new DialogScene(currentFont, currentWindowSkin, textVector));
	userdata.GetPointer()->Init(window, renderer);
	printlog("created dialog scene: %x\n", userdata.GetPointer());
	return userdata;
}

void ScriptedScene::Assign(int key, int level)
{
	inputLevelAssignment[(InputState::Key)key] = level;
}

void ScriptedScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
	currentWindowSkin = std::tr1::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "midJQ.png"), SDL_DestroyTexture);
	currentFont = std::tr1::shared_ptr<TTF_Font>(TTF_OpenFont("HuiFont29.ttf", 32), TTF_CloseFont);

	this->window = window;
	this->renderer = renderer;

	// prepare scene objects
	auto globals = lua.GetGlobalEnvironment();
	lua.LoadStandardLibraries();
	
	auto gameTable = lua.CreateUserdata<ScriptedScene>(this, [](ScriptedScene*){});

	gameTable.Set("A", (int)InputState::A);
	gameTable.Set("B", (int)InputState::B);
	gameTable.Set("X", (int)InputState::X);
	gameTable.Set("Y", (int)InputState::Y);

	gameTable.Set("UP", (int)InputState::UP);
	gameTable.Set("DOWN", (int)InputState::DOWN);
	gameTable.Set("LEFT", (int)InputState::LEFT);
	gameTable.Set("RIGHT", (int)InputState::RIGHT);
	
	gameTable.Set("L1", (int)InputState::L1);
	gameTable.Set("L2", (int)InputState::L2);
	gameTable.Set("R1", (int)InputState::R1);
	gameTable.Set("R2", (int)InputState::R2);

	gameTable.Bind("assign", &ScriptedScene::Assign);
	
	gameTable.Bind("newImage", &ScriptedScene::NewImage);
	gameTable.Bind("newDialog", &ScriptedScene::NewDialog);

	gameTable.Bind("display", &ScriptedScene::Display);
	gameTable.Bind("clear", &ScriptedScene::Clear);
	gameTable.Bind("fadein", &ScriptedScene::FadeIn);
	gameTable.Bind("fadeout", &ScriptedScene::FadeOut);

	gameTable.BindYield("waitfor", &ScriptedScene::WaitFor);

	globals.Set("game", gameTable);

	std::string script = LoadAllText(scriptFile);

	auto res = coroutine.RunScript(script);
	if (res != "No errors")
	{
		printlog("Script ended: %s\n", res.c_str());
		scenes.clear();
		running = false;
	}
}

void ScriptedScene::Update(const InputState& inputs, Uint32 timestamp)
{
	BOOST_FOREACH(auto m, scenes)
	{
		InputState theInput;
		BOOST_FOREACH(auto kvpair, inputLevelAssignment)
		{
			if (kvpair.second == m.first)
			{
				theInput.SetButtonPressed(kvpair.first, inputs.GetButtonPressed(kvpair.first));
				theInput.SetButtonReleased(kvpair.first, inputs.GetButtonReleased(kvpair.first));
				theInput.SetButtonState(kvpair.first, inputs.GetButtonState(kvpair.first));
			}
		}

		m.second.GetPointer()->Update(theInput, timestamp);

		if (scriptBlocker == m.second.GetPointer() && !m.second.GetPointer()->Running())
		{
			scriptBlocker = NULL;
		}
	}

	if (scriptBlocker == NULL)
	{
		if (coroutine.CanResume())
		{
			auto res = coroutine.Resume();
			if (res != "No errors")
			{
				printlog("Script ended: %s\n", res.c_str());
				scenes.clear();
				running = false;
			}
		}
		else
		{
			scenes.clear();
			running = false;
		}
	}
}

void ScriptedScene::Render(SDL_Renderer *renderer)
{
	typedef std::pair<sceneMap::key_type, sceneMap::mapped_type> item;
	std::vector<item> s;

	BOOST_FOREACH(auto m, scenes)
	{
		s.push_back(m);
	}

	std::sort(s.begin(), s.end(), [](item a, item b){ return a.first < b.first; });

	BOOST_FOREACH(auto m, s)
	{
		m.second.GetPointer()->Render(renderer);
	}
}

bool ScriptedScene::Running() const
{
	return running;
}
