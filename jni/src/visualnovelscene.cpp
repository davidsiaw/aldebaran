#include "visualnovelscene.hpp"

#include <vector>
#include <string>
#include <SDL_image.h>

VisualNovelScene::VisualNovelScene()
{
}

VisualNovelScene::~VisualNovelScene()
{
}

void VisualNovelScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
	windowSkin = std::tr1::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "midJQ.png"), SDL_DestroyTexture);
	font = std::tr1::shared_ptr<TTF_Font>(TTF_OpenFont("HuiFont29.ttf", 32), TTF_CloseFont);
}

void VisualNovelScene::Update(const InputState& inputs, Uint32 timestamp)
{
	if (dialog)
	{
		dialog->Update(inputs, timestamp);
	}
}

void VisualNovelScene::Render(SDL_Renderer *renderer)
{
	if (dialog)
	{
		dialog->Render(renderer);
	}
}

bool VisualNovelScene::Running()
{
	return true;
}
