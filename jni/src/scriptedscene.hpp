#ifndef SCRIPTEDSCENE_HPP
#define SCRIPTEDSCENE_HPP

#include "scene_interface.hpp"

#include <string>
#include <map>
#include <boost/tr1/memory.hpp>
#include <luacppinterface.h>
#include <SDL_image.h>
#include <SDL_ttf.h>


// Scene that fades in a child scene and then fades out when the scene ends
class ScriptedScene : public SceneInterface
{
	typedef std::map<int, SceneInterface* > sceneMap;

	sceneMap scenes;
	std::map<InputState::Key, int> inputLevelAssignment;
	std::string scriptFile;

	std::tr1::shared_ptr<TTF_Font> currentFont;
	std::tr1::shared_ptr<SDL_Texture> currentWindowSkin;

	Lua lua;
	LuaCoroutine coroutine;
	SceneInterface* scriptBlocker;
	bool running;

public:
	ScriptedScene(std::string scriptFile);
	virtual ~ScriptedScene();
	
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
	virtual void Update(const InputState& inputs, Uint32 timestamp);
	virtual void Render(SDL_Renderer *renderer);
	virtual bool Running() const;
};

#endif // SCRIPTEDSCENE_HPP
