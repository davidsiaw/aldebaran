#ifndef SCENE_INTERFACE_HPP
#define SCENE_INTERFACE_HPP

#include <memory>
#include <SDL.h>
#include "inputstate.hpp"
#include "rendercontext_interface.hpp"

class SceneInterface
{
    bool initialized;
    
public:
    SceneInterface() : initialized(false) {}
    virtual ~SceneInterface() {}
	
	virtual void Init(SDL_Window* window) = 0;
	virtual void Update(const InputState& inputs, Uint32 timestamp) = 0;
    virtual void Render(std::shared_ptr<RenderContextInterface> renderContext) = 0;
	virtual bool Running() const = 0;
    
    void FirstInitialize(SDL_Window* window)
    {
        if (!initialized)
        {
            Init(window);
        }
    }
};

#endif // SCENE_INTERFACE_HPP
