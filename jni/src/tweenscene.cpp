#include "tweenscene.hpp"

TweenScene::TweenScene(std::tr1::shared_ptr<TransformableViewScene> scene) :
scene(scene), currentAnimation(-1), currentAnimationStartTime(0), lastUpdateTime(0), behaviour(PERMANENT), loops(0)
{
    
}

TweenScene::~TweenScene()
{
    
}

void TweenScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{
    scene->Init(window, renderer);
    currentAnimation = -1;
    currentAnimationStartTime = 0;
    lastUpdateTime = 0;
    currentLoop = 0;
    running = true;
    
    initialState.rect.x = scene->GetOrigin().x;
    initialState.rect.y = scene->GetOrigin().y;
    initialState.rect.w = scene->GetSize().x;
    initialState.rect.h = scene->GetSize().y;
    initialState.opacity = scene->GetOpacity();
    initialState.rotation = scene->GetRotation();
}

double Fraction(Uint32 startTime, Uint32 timeLength, Uint32 currentTime)
{
    double soFar = currentTime - startTime;
    return soFar / timeLength;
}

double InterpolateLinear(double startValue, double endValue, double fraction)
{
    return (endValue - startValue) * fraction + startValue;
}

double InterpolateEaseIn(double startValue, double endValue, double fraction)
{
    double t = 1 - fraction;
    return (startValue - endValue) * t * t + endValue;
}

double InterpolateEaseOut(double startValue, double endValue, double fraction)
{
    return (endValue - startValue) * fraction * fraction + startValue;
}

void TweenScene::Update(const InputState& inputs, Uint32 timestamp)
{
    if (currentAnimation == -1 || (timestamp - currentAnimationStartTime) > animations[currentAnimation].time)
    {
        // check if finished animating
        if (currentAnimation + 1 >= animations.size())
        {
            if (loops == -1)
            {
                currentAnimation = 0;
            }
            else if (currentLoop < loops)
            {
                currentLoop++;
            }
            else
            {
                if (behaviour == TRANSIENT)
                {
                    running = false;
                }
                return;
            }
            currentAnimation = -1;
            currentAnimationStartTime = 0;
            lastUpdateTime = 0;
        }
        
        if (currentAnimation == -1)
        {
            currentAnimationStartTime = timestamp;
            
            startState = initialState;
        }
        else
        {
            currentAnimationStartTime = currentAnimationStartTime + animations[currentAnimation].time;
            
            startState.rect.x = scene->GetOrigin().x;
            startState.rect.y = scene->GetOrigin().y;
            startState.rect.w = scene->GetSize().x;
            startState.rect.h = scene->GetSize().y;
            startState.opacity = scene->GetOpacity();
            startState.rotation = scene->GetRotation();
        }
        
        // move to the next animation
        currentAnimation++;
        
    }
    
    // Now we interpolate our state
    Animation* curr = &animations[currentAnimation];
    
    double frac = Fraction(currentAnimationStartTime, curr->time, timestamp);
    double(*interpolatefunc)(double, double, double) = InterpolateLinear;
    
    switch (curr->easing) {
        case EASE_IN:
            interpolatefunc = InterpolateEaseIn;
            break;
            
        case EASE_OUT:
            interpolatefunc = InterpolateEaseOut;
            break;

        default:
            break;
    }
    
    int x = interpolatefunc(startState.rect.x, curr->rect.x, frac);
    int y = interpolatefunc(startState.rect.y, curr->rect.y, frac);
    int w = interpolatefunc(startState.rect.w, curr->rect.w, frac);
    int h = interpolatefunc(startState.rect.h, curr->rect.h, frac);
    
    double rotation = interpolatefunc(startState.rotation, curr->rotation, frac);
    double opacity = interpolatefunc(startState.opacity, curr->opacity, frac);
    
    scene->SetOrigin(x, y);
    scene->SetSize(w, h);
    scene->SetRotation(rotation);
    scene->SetOpacity(opacity);
    
    scene->Update(inputs, timestamp);
}

void TweenScene::Render(SDL_Renderer *renderer)
{
    scene->Render(renderer);
}

bool TweenScene::Running() const
{
    return scene && scene->Running() && running;
}

void TweenScene::PushTargetState(int x, int y, int w, int h, double rotation, double opacity, Uint32 time, Easing easing)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    
    Animation anim;
    anim.rect = rect;
    anim.rotation = rotation;
    anim.opacity = opacity;
    anim.time = time;
    anim.easing = easing;
    
    animations.push_back(anim);
}

void TweenScene::ClearAnimation()
{
    animations.clear();
}

void TweenScene::SetBehaviour(Behaviour behaviour)
{
    this->behaviour = behaviour;
}

TweenScene::Behaviour TweenScene::GetBehaviour() const
{
    return this->behaviour;
}

void TweenScene::SetLoops(int loops)
{
    this->loops = loops;
}

int TweenScene::GetLoops() const
{
    return loops;
}
