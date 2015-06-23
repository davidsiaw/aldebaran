
//
//  characterscene.hpp
//  aldebaran
//
//  Created by David Siaw on 2015/06/23.
//  Copyright (c) 2015年 David Siaw. All rights reserved.
//

#ifndef CHARACTERSET_HPP
#define CHARACTERSET_HPP

class Characterset
{
    std::shared_ptr<SDL_Surface> tex;
    int w,h;
    
public:
    Characterset(std::string bitmap, int w, int h) : tex(LoadImage(bitmap)), w(w), h(h)
    {
    }
    
    QuadVbo GetAnimated(int characterIdx, int spriteIdx, int x, int y, int z) const
    {
        QuadVbo quad(x, y, z, w, h,
                     w * characterIdx * 4, h * spriteIdx, w, h,
                     tex->w, tex->h, w, 0, 4);
        return quad;
    }
    
    QuadVbo GetStill(int characterIdx, int spriteIdx, int x, int y, int z) const
    {
        QuadVbo quad(x, y, z, w, h,
                     w * characterIdx * 4, h * spriteIdx, w, h,
                     tex->w, tex->h);
        return quad;
    }
    
    std::shared_ptr<SDL_Surface> GetSurface() const
    {
        return tex;
    }
    
    int GetCharTileWidth() const
    {
        return w;
    }
    
    int GetCharTileHeight() const
    {
        return h;
    }
};

#endif // CHARACTERSET_HPP
