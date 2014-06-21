#ifndef DEBUGRECTSCENE_HPP
#define DEBUGRECTSCENE_HPP

#include "scene_interface.hpp"

// Scene that displays a dialog
class DebugRectScene : public SceneInterface
{
	SDL_Rect rect;

public:
	DebugRectScene(int x, int y, int w, int h);
	virtual ~DebugRectScene();
	
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
	virtual void Update(const InputState& inputs, Uint32 timestamp);
	virtual void Render(SDL_Renderer *renderer);
	virtual bool Running() const;
	

};

#endif // DEBUGRECTSCENE_HPP
