#include "imageviewscene.hpp"

#include <SDL_image.h>

ImageViewScene::ImageViewScene(std::tr1::shared_ptr<ImageDesc> image) : image(image)
{
    origin.x = 0;
    origin.y = 0;
}

ImageViewScene::~ImageViewScene()
{
}

void ImageViewScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
    running = true;
}

void ImageViewScene::Update(const InputState& inputs, Uint32 timestamp)
{
}

void ImageViewScene::Render(SDL_Renderer *renderer)
{
    if (!running)
    {
        return;
    }
    
	SDL_Rect dstRect;
	dstRect.x = origin.x;
	dstRect.y = origin.y;
	dstRect.w = image->GetRect()->w;
	dstRect.h = image->GetRect()->h;
    
	SDL_RenderCopy(renderer, image->GetTexture(), image->GetRect(), &dstRect);
}

bool ImageViewScene::Running() const
{
	return running;
}

void ImageViewScene::Stop()
{
	running = false;
}
