#ifndef TEXTVIEWSCENE_HPP
#define TEXTVIEWSCENE_HPP

#include "viewscene_interface.hpp"
#include "fontdesc.hpp"

#include <string>
#include <SDL_ttf.h>

// Scene that displays text
class TextViewScene : public ViewSceneInterface
{
    std::tr1::shared_ptr<FontDesc> font;
    std::string text;
    std::tr1::shared_ptr<SDL_Texture> textTexture;
    SDL_Point size;
    int width;
    bool running;
    
public:
	TextViewScene(std::tr1::shared_ptr<FontDesc> font, std::string text, int width = -1);
	virtual ~TextViewScene();
	
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
	virtual void Update(const InputState& inputs, Uint32 timestamp);
	virtual void Render(SDL_Renderer *renderer);
	virtual bool Running() const;
    
    virtual void SetText(std::string text);
    virtual std::string GetText() const;
    
    virtual SDL_Point GetSize() const;
    
	void Stop();
    
};

#endif // TEXTVIEWSCENE_HPP
