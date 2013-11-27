#include "fadescene.hpp"

FadeScene::FadeScene(SceneInterface* scene, FadeScene::Mode mode, double speed)
	: scene(scene), mode(mode), speed(speed)
{
}

FadeScene::~FadeScene()
{
}
	
void FadeScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_GetWindowSize(window, &w, &h);
	renderTarget = std::tr1::shared_ptr<SDL_Texture>(
		SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, w, h),
		SDL_DestroyTexture);

	// make it so that the texture can be transparent
	SDL_SetTextureBlendMode(renderTarget.get(), SDL_BLENDMODE_BLEND);

	opacity = mode == FadeIn ? 0 : 255;
}

void FadeScene::Update(const InputState& inputs, Uint32 timestamp)
{
	scene->Update(inputs, timestamp);
}

void FadeScene::Render(SDL_Renderer *renderer)
{
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = w;
	rect.h = h;

	auto target = SDL_GetRenderTarget(renderer);

	int res = SDL_SetRenderTarget(renderer, renderTarget.get());
	scene->Render(renderer);
	res = SDL_SetRenderTarget(renderer, target);
	
	if (mode == FadeIn)
	{
		opacity = std::min(opacity + speed, 255.0);
	}
	else if (mode == FadeOut)
	{
		opacity = std::max(opacity - speed, 0.0);
	}

	res = SDL_SetTextureAlphaMod(renderTarget.get(), opacity);

	SDL_RenderCopy(renderer, renderTarget.get(), &rect, &rect);
}

bool FadeScene::Running() const
{
	if (mode == FadeIn)
	{
		return opacity != 255;
	}
	else if (mode == FadeOut)
	{
		return opacity != 0;
	}
}

void FadeScene::SetSpeed(double speed)
{
	this->speed = speed;
}

double FadeScene::GetSpeed() const
{
	return speed;
}
