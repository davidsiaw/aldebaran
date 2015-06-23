//
//  rpgmapscene.hpp
//  aldebaran
//
//  Created by David Siaw on 2015/06/23.
//  Copyright (c) 2015年 David Siaw. All rights reserved.
//

#ifndef RPGMAPSCENE_HPP
#define RPGMAPSCENE_HPP

#include "tilemapscene.hpp"
#include "characterscene.hpp"

class RPGMapScene : public ComposableSceneInterface
{
    class CharacterDetails
    {
    public:
        CharacterDetails(int id, int x, int y) :
        charid(id),
        tilex(x),
        tiley(y),
        x(x),
        y(y),
        speed(0),
        lastUpdate(0),
        direction(0),
        dx(0),
        dy(0),
        movementStep(0)
        {
            
        }
        
        int charid;
        int tilex;
        int tiley;
        
        double x;
        double y;
        
        int speed;
        int lastUpdate;
        
        double dx;
        double dy;
        
        int direction;
        int movementStep;
    };
    
    int w,h;
    
    std::vector<std::shared_ptr<TileMapScene>> layers;
    std::shared_ptr<CharacterScene> characters;
    
    std::map<int, std::shared_ptr<CharacterDetails>> characterRegister;
    
    TwoDArray<std::shared_ptr<CharacterDetails>> characterGrid;
    
    bool showDebugScene;
    std::shared_ptr<QuadCollectionVbo> vbo;
    std::shared_ptr<ShaderInterface> ds;
    std::shared_ptr<VboScene> debugScene;
    
public:
    RPGMapScene(int w, int h, std::shared_ptr<Tileset> tileset, std::shared_ptr<Characterset> charset) :
    characters(std::make_shared<CharacterScene>(charset, tileset->GetTilesize())),
    characterGrid(w,h),
    w(w),
    h(h),
    vbo(new QuadCollectionVbo()),
    ds(new WireframeShader()),
    debugScene(new VboScene(ds, vbo, nullptr))
    {
        for (int i=0; i<4; i++)
        {
            layers.push_back(std::make_shared<TileMapScene>(tileset, w * 2, h * 2));
        }
    }
    
    virtual void Init(SDL_Window* window)
    {
        for (int i=0; i<layers.size(); i++)
        {
            layers[i]->Init(window);
        }
        characters->Init(window);
        debugScene->Init(window);
    }
    
    bool CanMoveTo(std::shared_ptr<CharacterDetails> character, int mapx, int mapy)
    {
        if (mapx < 0 || mapx >= w || mapy < 0 || mapy >= h)
        {
            return false;
        }
        
        if (!characterGrid(mapx, mapy))
        {
            return true;
        }
        
        if (characterGrid(mapx, mapy) == character)
        {
            return true;
        }
        
        return false;
    }
    
    virtual void Update(const InputState& inputs, Uint32 timestamp)
    {
        auto tilesize = layers[0]->GetTilesize() * 2;
        
        for (int i=0; i<layers.size(); i++)
        {
            layers[i]->Update(inputs, timestamp);
        }
        
        
        for (auto kvpair : characterRegister)
        {
            
            auto id = kvpair.first;
            auto info = kvpair.second;
            
            bool move = false;
            
            
            if (info->dx > 0) { info->direction = 1; }
            if (info->dx < 0) { info->direction = 3; }
            if (info->dy > 0) { info->direction = 2; }
            if (info->dy < 0) { info->direction = 0; }
            
            if (info->dx == 0 && info->dy == 0)
            {
                characters->SetCharacterAnimation(id, false, info->direction);
                continue;
            }
            
            if (info->dx != 0 || info->dy != 0)
            {
                move = true;
            }
            
            if (timestamp - info->lastUpdate > 1000 - info->speed)
            {
                int framesAdvanced = 1;
                
                for (int i=0; i<framesAdvanced; i++)
                {
                    double resultingx = info->x + info->dx;
                    double resultingy = info->y + info->dy;
                    
                    if (resultingx == std::floor(resultingx))
                    {
                        info->x = resultingx;
                        info->dx = 0;
                    }
                    else
                    {
                        info->x = resultingx;
                    }
                    
                    if (std::abs(info->x - info->tilex) >= 1)
                    {
                        info->x = info->tilex;
                        info->dx = 0;
                    }
                    
                    info->y = resultingy;
                    if (resultingy == std::floor(resultingy))
                    {
                        info->y = resultingy;
                        info->dy = 0;
                    }
                    else
                    {
                        info->y = resultingy;
                    }
                    
                    if (std::abs(info->y - info->tiley) >= 1)
                    {
                        info->y = info->tiley;
                        info->dy = 0;
                    }
                    
                }
                
                characters->SetCharacterPosition(id,
                                                 info->x * (double)tilesize,
                                                 info->y * (double)tilesize);
                info->lastUpdate = timestamp;
                characters->SetCharacterAnimation(id, move, info->direction);
                
                vbo->Modify(kvpair.first, QuadVbo(info->tilex * tilesize,
                                                  info->tiley * tilesize, 0,
                                                  tilesize,
                                                  tilesize));
                
            }
            
        }
        
        
        for (int i=vbo->QuadCount(); i<characterRegister.size(); i++)
        {
            vbo->Add(QuadVbo::NullQuad());
        }
        
        debugScene->Update(inputs, timestamp);
        
        characters->Update(inputs, timestamp);
    }
    
    virtual void Render(std::shared_ptr<RenderContextInterface> renderContext)
    {
        for (int i=0; i<layers.size(); i++)
        {
            layers[i]->Render(renderContext);
            if (i == 1) { characters->Render(renderContext); }
        }
        
        debugScene->SetMatrixTo2DRectangle(0, 0, renderContext->GetScreenWidth(), renderContext->GetScreenHeight());
        //debugScene->Render(renderContext);
    }
    
    virtual bool Running() const
    {
        return true;
    }
    
    virtual void SetOrigin(Uint16 x, Uint16 y, Uint16 z)
    {
        for (int i=0; i<layers.size(); i++)
        {
            layers[i]->SetOrigin(x, y, 0);
        }
        
        characters->SetOrigin(x, y, 0);
        
        debugScene->SetOrigin(x, y, 0);
    }
    
    virtual Uint16 GetOriginX() const
    {
        return layers[0]->GetOriginX();
    }
    
    virtual Uint16 GetOriginY() const
    {
        return layers[0]->GetOriginY();
    }
    
    virtual Uint16 GetOriginZ() const
    {
        return layers[0]->GetOriginZ();
    }
    
    void Set(int layer, int x, int y, int value)
    {
        layers[layer]->Set(x, y, value);
    }
    
    int SetCharacter(int idx, int mapx, int mapy)
    {
        auto tilesize = layers[0]->GetTilesize() * 2;
        auto id = characters->AddCharacter(idx, mapx * tilesize, mapy * tilesize);
        
        auto md = std::make_shared<CharacterDetails>(id, mapx, mapy);
        characterRegister[id] = md;
        characterGrid(mapx,mapy) = md;
        
        return id;
    }
    
    void GetDirection(InputState::Key direction, int* dx, int* dy)
    {
        *dx = 0;
        *dy = 0;
        
        if (direction == InputState::DOWN)
        {
            *dy = 1;
        }
        if (direction == InputState::LEFT)
        {
            *dx = -1;
        }
        if (direction == InputState::RIGHT)
        {
            *dx = 1;
        }
        if (direction == InputState::UP)
        {
            *dy = -1;
        }
    }
    
    void MoveTo(std::shared_ptr<CharacterDetails> character, int x, int y)
    {
        characterGrid(character->tilex, character->tiley) = nullptr;
        characterGrid(character->x, character->y) = character;
    }
    
    void SetCharMovement(std::shared_ptr<CharacterDetails> character, InputState::Key direction)
    {
        double movementStep = 1.0 / 16.0;
        
        int dx, dy;
        GetDirection(direction, &dx, &dy);
        
        if (dy)
        {
            character->dy = (double)dy * movementStep;
        }
        if (dx)
        {
            character->dx = (double)dx * movementStep;
        }
        
    }
    
    void MoveCharacter(int id, InputState::Key direction, int amount, int speed)
    {
        if (characterRegister.find(id) == characterRegister.end())
        {
            return;
        }
        
        auto character = characterRegister[id];
        
        character->movementStep = amount;
        character->speed = speed;
        
        int dx, dy;
        GetDirection(direction, &dx, &dy);
        
        if (!character->dx && dx && character->tilex == character->x)
        {
            if (CanMoveTo(character, character->tilex + dx, character->tiley))
            {
                MoveTo(character, character->tilex+dx, character->tiley);
                character->tilex += dx;
                SetCharMovement(character, direction);
            }
            else
            {
                // collide
            }
        }
        
        if (!character->dy && dy && character->tiley == character->y)
        {
            if (CanMoveTo(character, character->tilex, character->tiley + dy))
            {
                MoveTo(character, character->tilex, character->tiley+dy);
                character->tiley += dy;
                SetCharMovement(character, direction);
            }
            else
            {
                // collide
            }
        }
        
        
    }
};

#endif // RPGMAPSCENE_HPP
