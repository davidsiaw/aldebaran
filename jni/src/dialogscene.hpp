#ifndef DIALOGSCENE_HPP
#define DIALOGSCENE_HPP

#include "scene_interface.hpp"

#include <vector>
#include <string>
#include <SDL_ttf.h>
#include <boost/tr1/memory.hpp>
#include <boost/tr1/functional.hpp>

class DialogScene : public SceneInterface
{
	std::tr1::shared_ptr<TTF_Font> font;
	std::tr1::shared_ptr<SDL_Texture> windowSkin;
    int tx,ty,tw,th;
	Uint32 updateDelay;
	std::vector<std::wstring> text;
	bool showAllText;
	std::tr1::function<void()> completionFunc;
	
	int row;
	int complete;
	bool showNextArrow;
	std::vector<std::wstring> displayedText;
	Uint32 lastUpdate;

public:
	DialogScene(std::tr1::shared_ptr<TTF_Font> font, std::tr1::shared_ptr<SDL_Texture> windowSkin, std::vector<std::wstring> text);
	virtual ~DialogScene();
	
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
	virtual void Update(const InputState& inputs, Uint32 timestamp);
	virtual void Render(SDL_Renderer *renderer);
	virtual bool Running();
	
	void SetShowAllText(bool show);
	bool GetShowAllText() const;

	bool Complete() const;

	// TODO
	// dialog position/width/height

	// menu

};

#endif // DIALOGSCENE_HPP
