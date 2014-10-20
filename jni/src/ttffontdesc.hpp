#ifndef TTFFONTDESC_HPP
#define TTFFONTDESC_HPP

#include "fontdesc.hpp"

#include <SDL_ttf.h>

class TTFFontDesc : public FontDesc
{
    std::shared_ptr<TTF_Font> ttfFont;
    SDL_Color color;
    
public:
    TTFFontDesc(std::shared_ptr<TTF_Font> ttfFont, SDL_Color color) :
    color(color), ttfFont(ttfFont)
    {
        
    }
    
    virtual ~TTFFontDesc()
    {
        
    }
    
    virtual SDL_Point StringSize(std::string text, int width) const
    {
        SDL_Point p;
        int numLines;
        if (width < 0)
        {
            TTF_SizeUTF8(ttfFont.get(), text.c_str(), &p.x, &p.y);
        }
        else
        {
            TTF_SizeUTF8_Wrapped(ttfFont.get(), text.c_str(), width, &p.x, &p.y, &numLines);
        }
        return p;
    }
    
    virtual std::shared_ptr<SDL_Texture> DrawString(SDL_Renderer* renderer, std::string text, int width) const
    {
        SDL_Surface* surf;
        if (width < 0)
        {
            surf = TTF_RenderUTF8_Blended(ttfFont.get(), text.c_str(), color);
        }
        else
        {
            surf = TTF_RenderUTF8_Blended_Wrapped(ttfFont.get(), text.c_str(), color, width);
        }
        
        auto tex = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, surf), SDL_DestroyTexture);
        
        SDL_FreeSurface(surf);
        
        return tex;
    }
};


#endif // TTFFONTDESC_HPP
