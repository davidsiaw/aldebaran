#ifndef SPRITEDESC_HPP
#define SPRITEDESC_HPP

#include <vector>
#include "imagedesc.hpp"

class SpriteDesc
{
    std::vector< std::shared_ptr<ImageDesc> > images;
    SDL_Rect rect;
    unsigned int delay;
    
public:
    SpriteDesc(std::vector< std::shared_ptr<ImageDesc> > images, SDL_Rect rect, unsigned int delay) :
    images(images), rect(rect), delay(delay)
    { }
    
    std::shared_ptr<ImageDesc> GetImageForTick(unsigned int tick)
    {
        return images[tick % images.size()];
    }
    
    unsigned int GetDelay() const
    {
        return delay;
    }
    
    const SDL_Rect* GetRect() const
    {
        return &rect;
    }
    
};

#endif // SPRITEDESC_HPP

