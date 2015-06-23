//
//  tileset.hpp
//  aldebaran
//
//  Created by David Siaw on 2015/06/23.
//  Copyright (c) 2015年 David Siaw. All rights reserved.
//

#ifndef TILESET_HPP
#define TILESET_HPP

#include <memory>
#include <vector>

#include <SDL.h>

#include "primitives.hpp"

class Tileset
{
    std::shared_ptr<SDL_Surface> tex;
    std::vector<std::shared_ptr<QuadVbo>> tiles;
    int tilesize;
    
public:
    Tileset(std::string textureFile, int tilesize) : tex(LoadImage(textureFile)), tilesize(tilesize)
    {
        for (int y=0; y<(tex->w / tilesize); y++)
            for (int x=0; x<(tex->h / tilesize); x++)
            {
                auto quad = std::make_shared<QuadVbo>(0, 0, 0, tilesize, tilesize,
                                                      x*tilesize, y*tilesize, tilesize, tilesize,
                                                      tex->w, tex->h);
                
                tiles.push_back(quad);
            }
    }
    
    std::shared_ptr<SDL_Surface> GetSurface() const
    {
        return tex;
    }
    
    int GetTilesize() const
    {
        return tilesize;
    }
    
    size_t GetTileCount() const
    {
        return tiles.size();
    }
    
    QuadVbo GetTile(int index, int x, int y) const
    {
        QuadVbo quad(*tiles[index], x*tilesize, y*tilesize, 0);
        return quad;
    }
};


#endif //TILESET_HPP

