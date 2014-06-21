#ifndef VIEWSCENE_INTERFACE_HPP
#define VIEWSCENE_INTERFACE_HPP

#include "scene_interface.hpp"

#include <boost/tr1/memory.hpp>

// Scene that displays its contents at a particular offset
class ViewSceneInterface : public SceneInterface
{
protected:
	SDL_Point origin;
    
public:
	virtual SDL_Point GetOrigin() const { return origin; }
	virtual void SetOrigin(int x, int y) { origin.x = x; origin.y = y; };
};

#endif // VIEWSCENE_INTERFACE_HPP
