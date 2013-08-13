#ifndef DIALOGSCENE_HPP
#define DIALOGSCENE_HPP

#include "scene_interface.hpp"

#include <vector>
#include <string>
#include <SDL_ttf.h>
#include <boost/tr1/memory.hpp>

class DialogScene : public SceneInterface
{
	std::tr1::shared_ptr<TTF_Font> font;
	std::tr1::shared_ptr<SDL_Texture> windowSkin;
    int tx,ty,tw,th;
	std::vector<std::wstring> displayedText;

public:
	DialogScene(std::tr1::shared_ptr<TTF_Font> font, std::tr1::shared_ptr<SDL_Texture> windowSkin);
	virtual ~DialogScene();
	
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
	virtual void Update(const InputState& inputs, Uint32 timestamp);
	virtual void Render(SDL_Renderer *renderer);
	virtual bool Running();

	// TODO
	// dialog position/width/height

	// menu

};

#endif // DIALOGSCENE_HPP
