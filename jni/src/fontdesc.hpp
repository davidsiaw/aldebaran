#ifndef FONTDESC_HPP
#define FONTDESC_HPP

#include <SDL.h>
#include <boost/tr1/memory.hpp>

class FontDesc
{
public:
    virtual SDL_Point StringSize(std::string text, int width) const = 0;
    virtual std::tr1::shared_ptr<SDL_Texture> DrawString(SDL_Renderer* renderer, std::string text, int width) const = 0;
};


#endif // FONTDESC_HPP
