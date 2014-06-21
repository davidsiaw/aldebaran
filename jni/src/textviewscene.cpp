#include "textviewscene.hpp"

TextViewScene::TextViewScene(std::tr1::shared_ptr<FontDesc> font, std::string text, int width) :
font(font), text(text), textTexture(NULL), width(width)
{
    size.x = 0;
    size.y = 0;
    origin.x = 0;
    origin.y = 0;
    SetText(text);
}

TextViewScene::~TextViewScene()
{
    
}

void TextViewScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
    running = true;
}

void TextViewScene::Update(const InputState& inputs, Uint32 timestamp)
{
}

void TextViewScene::Render(SDL_Renderer *renderer)
{
    if (!textTexture)
    {
        textTexture = font->DrawString(renderer, text, width);
    }
    
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = size.x;
    src.h = size.y;
    
    SDL_Rect r;
    r.x = origin.x;
    r.y = origin.y;
    r.w = size.x;
    r.h = size.y;
    
	SDL_RenderCopy(renderer, textTexture.get(), &src, &r);
}

bool TextViewScene::Running() const
{
    return running;
}

void TextViewScene::SetText(std::string text)
{
    this->text = text;
    size = font->StringSize(text, width);
    textTexture = NULL;
}

std::string TextViewScene::GetText() const
{
    return text;
}

SDL_Point TextViewScene::GetSize() const
{
    return size;
}

void TextViewScene::Stop()
{
    running = false;
}

