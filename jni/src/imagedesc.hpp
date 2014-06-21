#ifndef IMAGEDESC_HPP
#define IMAGEDESC_HPP

#include <boost/tr1/memory.hpp>
#include <SDL.h>

class ImageDesc
{
    std::tr1::shared_ptr<SDL_Texture> texture;
    SDL_Rect rect;
    
public:
    ImageDesc(std::tr1::shared_ptr<SDL_Texture> texture, SDL_Rect rect) :
    texture(texture), rect(rect)
    { }
    
    SDL_Texture* GetTexture() const
    {
        return texture.get();
    }
    
    const SDL_Rect* GetRect() const
    {
        return &rect;
    }
    
};

#endif // IMAGEDESC_HPP

