#ifndef CELVIEWSCENE_HPP
#define CELVIEWSCENE_HPP

#include "viewscene_interface.hpp"

#include <boost/tr1/memory.hpp>

// Scene that displays its contents clipped in a particular box
class CelViewScene : public ViewSceneInterface
{
	std::tr1::shared_ptr<SceneInterface> scene;
	SDL_Point size;
    
public:
	CelViewScene(std::tr1::shared_ptr<SceneInterface> scene) : scene(scene)
	{
		origin.x = 0;
		origin.y = 0;
		size.x = 0;
		size.y = 0;
	}
    
	virtual ~CelViewScene() { }
	
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer)
	{
		scene->Init(window, renderer);
	}
    
	virtual void Update(const InputState& inputs, Uint32 timestamp)
	{
		scene->Update(inputs, timestamp);
	}
    
	virtual void Render(SDL_Renderer *renderer)
	{
		SDL_Rect originalOrigin;
		SDL_RenderGetViewport(renderer, &originalOrigin);
        
		SDL_Rect originalClipRect;
		SDL_RenderGetClipRect(renderer, &originalClipRect);
        
		SDL_Rect newOrigin;
		newOrigin.x = originalOrigin.x + origin.x;
		newOrigin.y = originalOrigin.y - origin.y;
		newOrigin.w = originalOrigin.w;
		newOrigin.h = originalOrigin.h;
        
		SDL_Rect newClipRect;
		newClipRect.x = originalOrigin.x + origin.x;
		newClipRect.y = originalOrigin.x + origin.y;
		newClipRect.w = size.x;
		newClipRect.h = size.y;
        
		SDL_RenderSetViewport(renderer, &newOrigin);
		SDL_RenderSetClipRect(renderer, &newClipRect);
        
		scene->Render(renderer);
		SDL_RenderSetViewport(renderer, &originalOrigin);
		SDL_RenderSetClipRect(renderer, &originalClipRect);
	}
    
	virtual bool Running() const
	{
		return scene->Running();
	}
    
	virtual SDL_Rect GetFrame() const
	{
        SDL_Rect frame;
        frame.x = origin.x;
        frame.y = origin.y;
        frame.w = size.x;
        frame.h = size.y;
		return frame;
	}
    
	virtual void SetFrame(int x, int y, int w, int h)
	{
		origin.x = x;
		origin.y = y;
		size.x = w;
		size.y = h;
	}
};

#endif // CELVIEWSCENE_HPP
