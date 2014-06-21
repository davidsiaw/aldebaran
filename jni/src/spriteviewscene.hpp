#ifndef SPRITEVIEWSCENE_HPP
#define SPRITEVIEWSCENE_HPP

#include "viewscene_interface.hpp"

#include <boost/tr1/memory.hpp>
#include <boost/tr1/functional.hpp>

#include "spritedesc.hpp"

// Scene that displays a sequence of images one at a time
class SpriteViewScene : public ViewSceneInterface
{
    std::tr1::shared_ptr<SpriteDesc> sprite;
	bool running;
    Uint32 lastUpdate;
    Uint32 tick;
    
public:
	SpriteViewScene(std::tr1::shared_ptr<SpriteDesc> sprite);
	virtual ~SpriteViewScene();
	
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
	virtual void Update(const InputState& inputs, Uint32 timestamp);
	virtual void Render(SDL_Renderer *renderer);
	virtual bool Running() const;
    
	void Stop();
    
	// TODO
	// image position/width/height
    
	// menu
    
};

#endif // SPRITEVIEWSCENE_HPP
