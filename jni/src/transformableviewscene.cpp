#include "transformableviewscene.hpp"

TransformableViewScene::TransformableViewScene(std::tr1::shared_ptr<SceneInterface> scene, int width, int height)
: scene(scene), opacity(1), rotation(0)
{
    origin.x = 0;
    origin.y = 0;
    
    targetSize.x = size.x = width;
    targetSize.y = size.y = height;
    
    axis.x = size.x/2;
    axis.y = size.y/2;
}

TransformableViewScene::~TransformableViewScene()
{
}

void TransformableViewScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
    scene->Init(window, renderer);
    
	renderTarget = std::tr1::shared_ptr<SDL_Texture>(
                                                     SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, size.x, size.y),
                                                     SDL_DestroyTexture);
    
	// make it so that the texture can be transparent
	SDL_SetTextureBlendMode(renderTarget.get(), SDL_BLENDMODE_BLEND);
}

void TransformableViewScene::Update(const InputState& inputs, Uint32 timestamp)
{
	scene->Update(inputs, timestamp);
}

void TransformableViewScene::Render(SDL_Renderer *renderer)
{
	SDL_Rect srcrect;
	srcrect.x = 0;
	srcrect.y = 0;
	srcrect.w = targetSize.x;
	srcrect.h = targetSize.y;
    
	SDL_Rect dstrect;
	dstrect.x = origin.x;
	dstrect.y = origin.y;
	dstrect.w = size.x;
	dstrect.h = size.y;
    
	auto target = SDL_GetRenderTarget(renderer);
    
	int res = SDL_SetRenderTarget(renderer, renderTarget.get());
    
    Uint8 r,g,b,a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    
	scene->Render(renderer);
    
	res = SDL_SetRenderTarget(renderer, target);
	
	res = SDL_SetTextureAlphaMod(renderTarget.get(), opacity * 255);
    
	//SDL_RenderCopy(renderer, renderTarget.get(), &srcrect, &dstrect);
	SDL_RenderCopyEx(renderer, renderTarget.get(), &srcrect, &dstrect, rotation, &axis, SDL_FLIP_NONE);
}

bool TransformableViewScene::Running() const
{
    return scene->Running();
}

void TransformableViewScene::SetSize(int w, int h)
{
    // scale the axis center out
    axis.x = w / 2;
    axis.y = h / 2;
    
    size.x = w;
    size.y = h;
}

SDL_Point TransformableViewScene::GetSize() const
{
    return size;
}

void TransformableViewScene::SetOpacity(double opacity)
{
    this->opacity = opacity;
}

double TransformableViewScene::GetOpacity() const
{
    return opacity;
}

void TransformableViewScene::SetAxis(int x, int y)
{
    axis.x = x;
    axis.y = y;
}

SDL_Point TransformableViewScene::GetAxis() const
{
    return axis;
}

void TransformableViewScene::SetRotation(double rot)
{
    rotation = rot;
}

double TransformableViewScene::GetRotation() const
{
    return rotation;
}

