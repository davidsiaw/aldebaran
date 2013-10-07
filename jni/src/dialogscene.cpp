#include "dialogscene.hpp"

#include <SDL_image.h>

DialogScene::DialogScene(std::tr1::shared_ptr<TTF_Font> font, std::tr1::shared_ptr<SDL_Texture> windowSkin, std::vector<std::wstring> text)
	: font(font), windowSkin(windowSkin), text(text), running(true)
{
}

DialogScene::~DialogScene()
{
}

void DialogScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
	tx=4;
	ty=600-164;
	tw=792;
	th=160;
	updateDelay = 100;
	lastUpdate = 0;
	displayedText.clear();
	displayedText.push_back(std::wstring());

	showAllText = false;
	showNextArrow = false;
	row = 0;
	complete = false;
}

void DialogScene::Update(const InputState& inputs, Uint32 timestamp)
{
	if (complete && inputs.GetButtonState(InputState::A))
	{
		running = false;
	}

	int timeDiff = timestamp - lastUpdate;
	if (timeDiff > updateDelay && !complete)
	{
		while (row < text.size() && this->displayedText[row].size() == this->text[row].size())
		{
			row++;
			displayedText.push_back(std::wstring());
		}
		
		if (this->showAllText)
		{
			row = text.size();
			displayedText = this->text;
		}

		if (row >= text.size())
		{
			this->showNextArrow = true;
			complete = true;
			return;
		}

		do
		{
			displayedText[row] += text[row][displayedText[row].size()];
		}
		while (
			this->displayedText[row].size() != this->text[row].size() &&
			text[row][displayedText[row].size()] == ' ');

		lastUpdate = timestamp;
	}
}

void drawImage(std::tr1::shared_ptr<SDL_Texture> src, int srcX, int srcY, int srcW, int srcH, SDL_Renderer* renderer, int dstX, int dstY, int dstW, int dstH)
{
	SDL_Rect srcRect;
	srcRect.x = srcX;
	srcRect.y = srcY;
	srcRect.w = srcW;
	srcRect.h = srcH;

	SDL_Rect dstRect;
	dstRect.x = dstX;
	dstRect.y = dstY;
	dstRect.w = dstW;
	dstRect.h = dstH;

	SDL_RenderCopy(renderer, src.get(), &srcRect, &dstRect);
}

void drawImage(std::tr1::shared_ptr<SDL_Texture> src, SDL_Renderer* renderer, int dstX, int dstY)
{
	Uint32 format;
	int access, w, h;

	SDL_QueryTexture(src.get(), &format, &access, &w, &h);
	drawImage(src, 0, 0, w, h, renderer, dstX, dstY, w, h);
}

void DialogScene::Render(SDL_Renderer *renderer)
{
	drawImage(windowSkin, 0, 0, 64, 64, renderer, tx+2, ty+2, tw-4, th-4);
	
	drawImage(windowSkin, 64, 0, 16, 16, renderer, tx, ty, 16, 16);
	drawImage(windowSkin, 128-16, 0, 16, 16, renderer, tx+tw-16, ty, 16, 16);
	drawImage(windowSkin, 64, 64-16, 16, 16, renderer, tx, ty+th-16, 16, 16);
	drawImage(windowSkin, 128-16, 64-16, 16, 16, renderer, tx+tw-16, ty+th-16, 16, 16);

	drawImage(windowSkin, 64+16, 0, 32, 16, renderer, tx+16, ty, tw-32, 16);
	drawImage(windowSkin, 64+16, 64-16, 32, 16, renderer, tx+16, ty+th-16, tw-32, 16);

	drawImage(windowSkin, 64, 16, 16, 32, renderer, tx, ty+16, 16, th-32);
	drawImage(windowSkin, 128-16, 16, 16, 32, renderer, tx+tw-16, ty+16, 16, th-32);
	
	if (showNextArrow)
	{
		Uint32 arrowAnimInt = (SDL_GetTicks() / 100) % 4;

		drawImage(windowSkin, 96+(arrowAnimInt % 2)*16, 64+(arrowAnimInt >> 1)*16, 16, 16, renderer, tx+tw-32, ty+th-32, 16, 16);
	}

	SDL_Color c;
	c.r = 0xff;
	c.g = 0xff;
	c.b = 0xff;
	for(size_t i = 0; i < displayedText.size(); i++)
	{
		auto txt = std::tr1::shared_ptr<SDL_Surface>(TTF_RenderUNICODE_Solid(font.get(), (const Uint16*)displayedText[i].c_str(), c), SDL_FreeSurface);
		auto texture = std::tr1::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, txt.get()), SDL_DestroyTexture);
		if (txt)
		{
			drawImage(texture, renderer, tx+16, ty+16+i*32);
		}
	}

	// render press arrow
}

bool DialogScene::Running() const
{
	return running;
}
