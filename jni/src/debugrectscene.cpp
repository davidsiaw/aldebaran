#include "debugrectscene.hpp"

DebugRectScene::DebugRectScene(int x, int y, int w, int h)
{
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
}

DebugRectScene::~DebugRectScene()
{

}

void DebugRectScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{

}

void DebugRectScene::Update(const InputState& inputs, Uint32 timestamp)
{

}

void DebugRectScene::Render(SDL_Renderer *renderer)
{
	Uint8 r,g,b,a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

bool DebugRectScene::Running() const
{
	return true;
}
