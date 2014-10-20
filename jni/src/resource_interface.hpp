#ifndef RESOURCE_INTERFACE_HPP
#define RESOURCE_INTERFACE_HPP

#include <SDL.h>
#include <SDL_ttf.h>

#include <string>

#include "imagedesc.hpp"
#include "spritedesc.hpp"
#include "fontdesc.hpp"

typedef unsigned int ResourceID;

class ResourceInterface
{
public:
    virtual std::shared_ptr<ImageDesc> GetImage(ResourceID num) const = 0;
    virtual std::shared_ptr<SpriteDesc> GetSprite(ResourceID num) const = 0;
    virtual std::shared_ptr<FontDesc> GetFont(ResourceID num) const = 0;
};

#endif // RESOURCE_INTERFACE_HPP
