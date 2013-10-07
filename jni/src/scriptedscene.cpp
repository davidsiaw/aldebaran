#include "scriptedscene.hpp"

#include <algorithm>
#include <vector>
#include <boost/foreach.hpp>

#include "dialogscene.hpp"
#include "printlog.hpp"


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

void ScriptedScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
	currentWindowSkin = std::tr1::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "midJQ.png"), SDL_DestroyTexture);
	currentFont = std::tr1::shared_ptr<TTF_Font>(TTF_OpenFont("HuiFont29.ttf", 32), TTF_CloseFont);

	// prepare scene objects
	auto globals = lua.GetGlobalEnvironment();
	lua.LoadStandardLibraries();
	
	auto scriptWaitFor = lua.CreateYieldingFunction<void(LuaUserdata<SceneInterface>)>(
	[&](LuaUserdata<SceneInterface> scene)
	{
		scriptBlocker = scene.GetPointer();
	});

	auto sceneClear = lua.CreateFunction<void()>(
	[&]()
	{
		scenes.clear();
	});

	auto sceneImage = lua.CreateFunction<void(std::string, int)>(
	[&](std::string filename, int level)
	{

	});

	auto sceneDialog = lua.CreateFunction<LuaUserdata<SceneInterface>(LuaTable, int) >(
	[&](LuaTable text , int level) -> LuaUserdata<SceneInterface>
	{
		std::vector<std::wstring> textVector;
		textVector.push_back(text.Get<std::wstring>(1));
		textVector.push_back(text.Get<std::wstring>(2));
		textVector.push_back(text.Get<std::wstring>(3));
		textVector.push_back(text.Get<std::wstring>(4));

		auto userdata = lua.CreateUserdata<SceneInterface>(new DialogScene(currentFont, currentWindowSkin, textVector));
		userdata.GetPointer()->Init(window, renderer);
		
		scenes[level] = userdata.GetPointer();

		return userdata;
	});

	auto inputAssign = lua.CreateFunction<void(int, int)>(
	[&](int key, int level)
	{
		inputLevelAssignment[(InputState::Key)key] = level;
	});

	auto scriptTable = lua.CreateTable();
	auto inputTable = lua.CreateTable();
	auto sceneTable = lua.CreateTable();

	inputTable.Set("A", (int)InputState::A);
	inputTable.Set("B", (int)InputState::B);
	inputTable.Set("X", (int)InputState::X);
	inputTable.Set("Y", (int)InputState::Y);

	inputTable.Set("UP", (int)InputState::UP);
	inputTable.Set("DOWN", (int)InputState::DOWN);
	inputTable.Set("LEFT", (int)InputState::LEFT);
	inputTable.Set("RIGHT", (int)InputState::RIGHT);
	
	inputTable.Set("L1", (int)InputState::L1);
	inputTable.Set("L2", (int)InputState::L2);
	inputTable.Set("R1", (int)InputState::R1);
	inputTable.Set("R2", (int)InputState::R2);

	inputTable.Set("assign", inputAssign);
	
	sceneTable.Set("image", sceneImage);

	sceneTable.Set("dialog", sceneDialog);
	sceneTable.Set("clear", sceneClear);
	
	scriptTable.Set("waitfor", scriptWaitFor);

	globals.Set("script", scriptTable);
	globals.Set("input", inputTable);
	globals.Set("scene", sceneTable);

	
	SDL_RWops *rw = SDL_RWFromFile(scriptFile.c_str(), "r");

	auto size = rw->size(rw);
	char* script = new char[size + 1];
	memset(script, 0, size + 1);
	rw->read(rw, script, sizeof(char), size);
	rw->close(rw);
	SDL_FreeRW(rw);

	auto res = coroutine.RunScript(script);
	if (res != "No errors")
	{
		printlog("Script ended: %s\n", res.c_str());
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

		m.second->Update(theInput, timestamp);

		if (scriptBlocker == m.second && !m.second->Running())
		{
			scriptBlocker = NULL;
		}
	}

	if (scriptBlocker == NULL)
	{
		auto res = coroutine.Resume();
		if (res != "No errors")
		{
			printlog("Script ended: %s\n", res.c_str());
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
		m.second->Render(renderer);
	}
}

bool ScriptedScene::Running() const
{
	return running;
}