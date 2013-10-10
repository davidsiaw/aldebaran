#include <SDL_image.h>

#include "imagescene.hpp"

ImageScene::ImageScene(std::string filename) : filename(filename)
{
}

ImageScene::~ImageScene()
{
}

void ImageScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
	image = std::tr1::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, filename.c_str()), SDL_DestroyTexture);
}

void ImageScene::Update(const InputState& inputs, Uint32 timestamp)
{
}

void ImageScene::Render(SDL_Renderer *renderer)
{
	SDL_Rect dstRect;
	dstRect.x = 0;
	dstRect.y = 0;
	dstRect.w = 800;
	dstRect.h = 600;
	SDL_RenderCopy(renderer, image.get(), &dstRect, &dstRect);
}

bool ImageScene::Running() const
{
	return running;
}

void ImageScene::SetRunning(bool yesno)
{
	running = yesno;
}
