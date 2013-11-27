#ifndef RPGRESOURCEINTERFACE_HPP
#define RPGRESOURCEINTERFACE_HPP

#include <SDL.h>

typedef unsigned int RPGResourceID;
class RPGResourceInterface
{

public:
	virtual std::pair<std::tr1::shared_ptr<SDL_Texture>, SDL_Rect> GetImage(RPGResourceID num) const = 0;
};

#endif // RPGRESOURCEINTERFACE_HPP
