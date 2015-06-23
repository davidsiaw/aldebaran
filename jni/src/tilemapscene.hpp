//
//  tilemapscene.hpp
//  aldebaran
//
//  Created by David Siaw on 2015/06/23.
//  Copyright (c) 2015年 David Siaw. All rights reserved.
//

#ifndef TILEMAPSCENE_HPP
#define TILEMAPSCENE_HPP

#include <memory>

#include "tileset.hpp"
#include "defaultshader.hpp"
#include "twodarray.hpp"

class TileMapScene : public ComposableSceneInterface
{
    std::shared_ptr<ShaderInterface> shader;
    std::shared_ptr<QuadCollectionVbo> quads;
    std::shared_ptr<VboScene> scene;
    std::shared_ptr<Tileset> tiles;
    TwoDArray<size_t> quadIndices;
    int w, h;
    
public:
    TileMapScene(std::shared_ptr<Tileset> tiles, int w, int h) :
    shader(new DefaultShader()), quads(new QuadCollectionVbo()), tiles(tiles), w(w), h(h), quadIndices(w,h)
    {
        for (int y=0; y<h; y++)
            for (int x=0; x<w; x++)
            {
                quadIndices(x,y) = quads->Add(tiles->GetTile(0, x, y));
            }
        
        scene = std::make_shared<VboScene>(shader, quads, tiles->GetSurface());
        
    }
    
    virtual void Init(SDL_Window* window)
    {
        scene->Init(window);
    }
    
    virtual void Update(const InputState& inputs, Uint32 timestamp)
    {
        scene->Update(inputs, timestamp);
    }
    
    virtual void Render(std::shared_ptr<RenderContextInterface> renderContext)
    {
        scene->SetMatrixTo2DRectangle(0, 0, renderContext->GetScreenWidth(), renderContext->GetScreenHeight());
        scene->Render(renderContext);
    }
    
    virtual bool Running() const
    {
        return true;
    }
    
    virtual void SetOrigin(Uint16 x, Uint16 y, Uint16 z)
    {
        scene->SetOrigin(x, y, z);
    }
    
    virtual Uint16 GetOriginX() const
    {
        return scene->GetOriginX();
    }
    
    virtual Uint16 GetOriginY() const
    {
        return scene->GetOriginY();
    }
    
    virtual Uint16 GetOriginZ() const
    {
        return scene->GetOriginZ();
    }
    
    virtual void Set(int x, int y, int value)
    {
        quads->Modify(quadIndices(x,y), tiles->GetTile(value, x, y));
    }
    
    int GetTilesize() const
    {
        return tiles->GetTilesize();
    }
};

#endif // TILEMAPSCENE_HPP
