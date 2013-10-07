#include "ndfscene.hpp"

NDFScene::NDFScene(int opacity) : opacity(opacity)
{
}

NDFScene::~NDFScene()
{
}
	
void NDFScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_GetWindowSize(window, &w, &h);
	filter = std::tr1::shared_ptr<SDL_Texture>(
		SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, w, h),
		SDL_DestroyTexture);

	// make it so that the texture can be transparent
	SDL_SetTextureBlendMode(filter.get(), SDL_BLENDMODE_BLEND);

	opacity = 0;
}

void NDFScene::Update(const InputState& inputs, Uint32 timestamp)
{
}

void NDFScene::Render(SDL_Renderer *renderer)
{
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = w;
	rect.h = h;

	SDL_SetTextureAlphaMod(filter.get(), opacity);

	SDL_RenderCopy(renderer, filter.get(), &rect, &rect);
}

bool NDFScene::Running()
{
	return true;
}
