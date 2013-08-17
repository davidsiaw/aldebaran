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
	kagamin = std::tr1::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "kagamin.jpg"), SDL_DestroyTexture);
	
	if(!kagamin) 
	{
		printf("IMG_LoadTexture: %s\n", IMG_GetError());
	}

	windowSkin = std::tr1::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, "midJQ.png"), SDL_DestroyTexture);
	font = std::tr1::shared_ptr<TTF_Font>(TTF_OpenFont("HuiFont29.ttf", 32), TTF_CloseFont);

	std::vector<std::wstring> text;
	text.push_back(L"Hello!");
	text.push_back(L"This is the world!");
	text.push_back(L"I am not interested in normal humans");
	text.push_back(L"âFíàêlÇ»Ç«éÑÇÃÇ∆Ç±ÇÎÇ÷óàÇ»Ç≥Ç¢ÅI");

	currentDialog = std::tr1::make_shared<DialogScene>(font, windowSkin, text);
	currentDialog->Init(window, renderer);
}

void VisualNovelScene::Update(const InputState& inputs, Uint32 timestamp)
{
	if (currentDialog)
	{
		currentDialog->Update(inputs, timestamp);
	}
}

void VisualNovelScene::Render(SDL_Renderer *renderer)
{
	SDL_Rect dstRect;
	dstRect.x = 0;
	dstRect.y = 0;
	dstRect.w = 800;
	dstRect.h = 600;
	SDL_RenderCopy(renderer, kagamin.get(), &dstRect, &dstRect);

	if (currentDialog)
	{
		currentDialog->Render(renderer);
	}
}

bool VisualNovelScene::Running()
{
	return true;
}
