#ifndef NDFSCENE_HPP
#define NDFSCENE_HPP

#include "scene_interface.hpp"

#include <boost/tr1/memory.hpp>

// Scene that darkens all the scenes drawn before it
class NDFScene : public SceneInterface
{
	int w,h;
	std::tr1::shared_ptr<SDL_Texture> filter;
	int opacity;

public:
	NDFScene(int opacity);
	virtual ~NDFScene();
	
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
	virtual void Update(const InputState& inputs, Uint32 timestamp);
	virtual void Render(SDL_Renderer *renderer);
	virtual bool Running();
};

#endif // NDFSCENE_HPP
