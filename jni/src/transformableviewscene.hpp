#ifndef TRANSFORMABLEVIEWSCENE_HPP
#define TRANSFORMABLEVIEWSCENE_HPP

#include "viewscene_interface.hpp"

#include <boost/tr1/memory.hpp>

// Scene that displays its contents clipped in a particular box
// which has adjustable location, size, rotation and opacity
class TransformableViewScene : public ViewSceneInterface
{
public:
	
	TransformableViewScene(std::tr1::shared_ptr<SceneInterface> scene, int width, int height);
	virtual ~TransformableViewScene();
	
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
	virtual void Update(const InputState& inputs, Uint32 timestamp);
	virtual void Render(SDL_Renderer *renderer);
	virtual bool Running() const;
    
    virtual void SetSize(int w, int h);
    virtual SDL_Point GetSize() const;
    
    virtual void SetOpacity(double opacity);
    virtual double GetOpacity() const;
    
    virtual void SetAxis(int x, int y);
    virtual SDL_Point GetAxis() const;
    
    virtual void SetRotation(double rot);
    virtual double GetRotation() const;
    
private:
	std::tr1::shared_ptr<SceneInterface> scene;
	std::tr1::shared_ptr<SDL_Texture> renderTarget;
	SDL_Point size;
	SDL_Point axis;
    double rotation;
    SDL_Point targetSize;
    double opacity;
    
};



#endif /* TRANSFORMABLEVIEWSCENE_HPP */
