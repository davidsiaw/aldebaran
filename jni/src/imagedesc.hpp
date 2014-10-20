#ifndef IMAGEDESC_HPP
#define IMAGEDESC_HPP

#include <memory>
#include <SDL.h>

class ImageDesc
{
    std::shared_ptr<SDL_Texture> texture;
    SDL_Rect rect;
    
public:
    ImageDesc(std::shared_ptr<SDL_Texture> texture, SDL_Rect rect) :
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
