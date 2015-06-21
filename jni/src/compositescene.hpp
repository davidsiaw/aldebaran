//
//  compositescene.hpp
//  aldebaran
//
//  Created by David Siaw on 2015/04/14.
//  Copyright (c) 2015年 David Siaw. All rights reserved.
//

#ifndef COMPOSITESCENE_HPP
#define COMPOSITESCENE_HPP

#include "composablescene_interface.hpp"

class CompositeScene : public ComposableSceneInterface
{
    class SceneEntry
    {
    public:
        std::shared_ptr<ComposableSceneInterface> scene;
        Uint16 x,y,z;
        
        SceneEntry(std::shared_ptr<ComposableSceneInterface> scene, Uint16 x, Uint16 y, Uint16 z)
        : scene(scene), x(x), y(y), z(z)
        {}
    };
    
    Uint16 x,y,z;
    std::vector<SceneEntry> sceneEntries;
    
public:
    
    CompositeScene() : x(0), y(0), z(0)
    {
        
    }
    
    virtual void AddScene(std::shared_ptr<ComposableSceneInterface> scene, Uint16 x, Uint16 y, Uint16 z = 0)
    {
        sceneEntries.push_back(SceneEntry(scene, x, y, z));
    }
    
    virtual void Init(SDL_Window* window)
    {
        for (auto sceneEntry : sceneEntries)
        {
            sceneEntry.scene->Init(window);
        }
    }
    
    virtual void Update(const InputState& inputs, Uint32 timestamp)
    {
        for (auto sceneEntry : sceneEntries)
        {
            sceneEntry.scene->Update(inputs, timestamp);
        }
    }
    
    virtual void Render(std::shared_ptr<RenderContextInterface> renderContext)
    {
        for (auto sceneEntry : sceneEntries)
        {
            sceneEntry.scene->SetOrigin(x + sceneEntry.x, y + sceneEntry.y, z + sceneEntry.z);
            sceneEntry.scene->Render(renderContext);
        }
    }
    
    virtual bool Running() const
    {
        return true;
    }
    
    virtual void SetOrigin(Uint16 x, Uint16 y, Uint16 z)
    {
        this->x = x;
        this->y = y;
    }
    
    virtual Uint16 GetOriginX() const
    {
        return x;
    }
    
    virtual Uint16 GetOriginY() const
    {
        return y;
    }
    
    virtual Uint16 GetOriginZ() const
    {
        return z;
    }

    
};

#endif // COMPOSITESCENE_HPP
