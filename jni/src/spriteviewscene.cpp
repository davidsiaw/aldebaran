#include <SDL_image.h>

#include "spriteviewscene.hpp"

SpriteViewScene::SpriteViewScene(std::tr1::shared_ptr<SpriteDesc> sprite) : sprite(sprite), lastUpdate(0), tick(0)
{
    origin.x = 0;
    origin.y = 0;
}

SpriteViewScene::~SpriteViewScene()
{
}

void SpriteViewScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
    running = true;
    lastUpdate = 0;
}

void SpriteViewScene::Update(const InputState& inputs, Uint32 timestamp)
{
    if (timestamp - lastUpdate > sprite->GetDelay())
    {
        lastUpdate = timestamp;
        tick++;
    }
}

void SpriteViewScene::Render(SDL_Renderer *renderer)
{
    if (!running)
    {
        return;
    }
    
    std::tr1::shared_ptr<ImageDesc> image = sprite->GetImageForTick(tick);
    
	SDL_Rect dstRect;
	dstRect.x = origin.x;
	dstRect.y = origin.y;
	dstRect.w = image->GetRect()->w;
	dstRect.h = image->GetRect()->h;
    
	SDL_RenderCopy(renderer, image->GetTexture(), image->GetRect(), &dstRect);
}

bool SpriteViewScene::Running() const
{
	return running;
}

void SpriteViewScene::Stop()
{
	running = false;
}
