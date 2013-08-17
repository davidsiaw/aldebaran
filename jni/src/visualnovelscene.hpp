#ifndef VISUALNOVELSCENE_HPP
#define VISUALNOVELSCENE_HPP

#include "scene_interface.hpp"

#include <SDL_ttf.h>
#include <boost/tr1/memory.hpp>

#include "dialogscene.hpp"

class VisualNovelScene : public SceneInterface
{
	std::tr1::shared_ptr<TTF_Font> font;
	std::tr1::shared_ptr<SDL_Texture> windowSkin;

	std::tr1::shared_ptr<SDL_Texture> kagamin;

	std::tr1::shared_ptr<DialogScene> currentDialog;

public:

	// TODO
	// constructor that takes a script
	// 

	VisualNovelScene();
	virtual ~VisualNovelScene();
	
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
	virtual void Update(const InputState& inputs, Uint32 timestamp);
	virtual void Render(SDL_Renderer *renderer);
	virtual bool Running();
};

#endif // VISUALNOVELSCENE_HPP
