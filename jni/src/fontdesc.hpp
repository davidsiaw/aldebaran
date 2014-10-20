#ifndef FONTDESC_HPP
#define FONTDESC_HPP

#include <SDL.h>
#include <memory>

class FontDesc
{
public:
    virtual SDL_Point StringSize(std::string text, int width) const = 0;
    virtual std::shared_ptr<SDL_Texture> DrawString(SDL_Renderer* renderer, std::string text, int width) const = 0;
};


#endif // FONTDESC_HPP
