#ifndef CONTAINERVIEWSCENE_HPP
#define CONTAINERVIEWSCENE_HPP

#include "viewscene_interface.hpp"

#include <boost/tr1/memory.hpp>

// Scene that displays its contents at a particular offset
class ContainerViewScene : public ViewSceneInterface
{
	std::tr1::shared_ptr<SDL_Texture> renderTarget;
	std::tr1::shared_ptr<SceneInterface> scene;

public:
	ContainerViewScene(std::tr1::shared_ptr<SceneInterface> scene) : scene(scene)
	{
		origin.x = 0;
		origin.y = 0;
	}

	virtual ~ContainerViewScene() { }
	
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
		newClipRect.x = originalOrigin.x - origin.x;
		newClipRect.y = originalOrigin.y - origin.y;
		newClipRect.w = originalOrigin.w;
		newClipRect.h = originalOrigin.h;

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
};

#endif // CONTAINERVIEWSCENE_HPP
