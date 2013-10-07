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
	text.push_back(L"東中出身、涼宮ハルヒ。ただの人間には");
	text.push_back(L"興味ありません。この中に宇宙人、未来人、");
	text.push_back(L"異世界人、超能力者がいたら、");
	text.push_back(L"あたしのところに来なさい。以上。");

	currentDialog = std::tr1::shared_ptr<DialogScene>(new DialogScene(font, windowSkin, text));
	currentDialog->Init(window, renderer);

	running = true;
}

void VisualNovelScene::Update(const InputState& inputs, Uint32 timestamp)
{
	if (inputs.GetButtonState(InputState::A))
	{
		running = false;
	}

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
	return running;
}
