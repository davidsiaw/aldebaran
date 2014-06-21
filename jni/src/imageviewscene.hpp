#ifndef IMAGEVIEWSCENE_HPP
#define IMAGEVIEWSCENE_HPP

#include "viewscene_interface.hpp"

#include <boost/tr1/memory.hpp>
#include <boost/tr1/functional.hpp>

#include "imagedesc.hpp"

// Scene that displays an image
class ImageViewScene : public ViewSceneInterface
{
    std::tr1::shared_ptr<ImageDesc> image;
	bool running;

public:
	ImageViewScene(std::tr1::shared_ptr<ImageDesc> image);
	virtual ~ImageViewScene();
	
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
	virtual void Update(const InputState& inputs, Uint32 timestamp);
	virtual void Render(SDL_Renderer *renderer);
	virtual bool Running() const;

	void Stop();
    
	// TODO
	// image position/width/height

	// menu

};

#endif // IMAGESCENE_HPP
