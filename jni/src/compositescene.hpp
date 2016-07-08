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
        Uint16 x,y,z;
    public:
        std::shared_ptr<ComposableSceneInterface> scene;
        
        SceneEntry(std::shared_ptr<ComposableSceneInterface> scene, Uint16 x, Uint16 y, Uint16 z)
        : scene(scene), x(x), y(y), z(z)
        {}

        Uint16 GetX() const { return x; }
        Uint16 GetY() const { return y; }
        Uint16 GetZ() const { return z; }

        void SetX(Uint16 value) { x = value; }
        void SetY(Uint16 value) { y = value; }
        void SetZ(Uint16 value) { z = value; }
    };
    
    Uint16 x,y,z;
    std::vector<SceneEntry> sceneEntries;
    
public:
    
    CompositeScene() : x(0), y(0), z(0)
    {
        
    }
    
    virtual size_t AddScene(std::shared_ptr<ComposableSceneInterface> scene, Uint16 x = 0, Uint16 y = 0, Uint16 z = 0)
    {
        sceneEntries.push_back(SceneEntry(scene, x, y, z));
        return sceneEntries.size() - 1;
    }

    virtual void MoveScene(size_t index, Uint16 x, Uint16 y, Uint16 z)
    {
        sceneEntries[index].SetX(x);
        sceneEntries[index].SetY(y);
        sceneEntries[index].SetZ(z);
    }

    virtual Uint16 GetSceneX(size_t index) const
    {
        return sceneEntries.at(index).GetX();
    }

    virtual Uint16 GetSceneY(size_t index) const
    {
        return sceneEntries.at(index).GetY();
    }

    virtual Uint16 GetSceneZ(size_t index) const
    {
        return sceneEntries.at(index).GetZ();
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
            sceneEntry.scene->SetOrigin(x + sceneEntry.GetX(), y + sceneEntry.GetY(), z + sceneEntry.GetZ());
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
