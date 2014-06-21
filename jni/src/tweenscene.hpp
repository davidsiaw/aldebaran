#ifndef TWEENSCENE_HPP
#define TWEENSCENE_HPP

#include <vector>
#include "transformableviewscene.hpp"

// Scene that performs a series of animations on a transformable view scene
class TweenScene : public SceneInterface
{
public:
    enum Easing
    {
        NO_EASING,
        EASE_IN,
        EASE_OUT
    };

private:
    struct Animation
    {
        SDL_Rect rect;
        double opacity;
        double rotation;
        double time;
        Easing easing;
    };
    
public:
    
    enum Behaviour
    {
        TRANSIENT,  // stop running after animations are complete
        PERMANENT  // continue running after animations are complete
    };
	
	TweenScene(std::tr1::shared_ptr<TransformableViewScene> scene);
	virtual ~TweenScene();
	
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
	virtual void Update(const InputState& inputs, Uint32 timestamp);
	virtual void Render(SDL_Renderer *renderer);
	virtual bool Running() const;
    
    virtual void PushTargetState(int x, int y, int w, int h, double rotation, double opacity, Uint32 time, Easing easing);
    virtual void ClearAnimation();
    
    virtual void SetBehaviour(Behaviour behaviour);
    virtual Behaviour GetBehaviour() const;
    
    virtual void SetLoops(int loops);
    virtual int GetLoops() const;
    
private:
	std::tr1::shared_ptr<TransformableViewScene> scene;
    std::vector<Animation> animations;
    
    Animation initialState;
    Animation startState;
    int currentAnimation;
    Uint32 currentAnimationStartTime;
    Uint32 lastUpdateTime;
    int currentLoop;
    Behaviour behaviour;
    int loops;  // number of repeats
    bool running;
};

#endif // TWEENSCENE_HPP
