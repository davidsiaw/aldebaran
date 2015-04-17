//
//  emptyscene.hpp
//  aldebaran
//
//  Created by David Siaw on 2014/10/16.
//  Copyright (c) 2014年 David Siaw. All rights reserved.
//

#ifndef GUISCENE_HPP
#define GUISCENE_HPP

class GuiScene : public SceneInterface
{
public:
    GuiScene() {}
    virtual ~GuiScene() {}
    
    virtual void Init(SDL_Window* window)
    {
        
    }
    
    virtual void Update(const InputState& inputs, Uint32 timestamp)
    {
        
    }
    
    virtual void Render(std::shared_ptr<RenderContextInterface> renderContext)
    {
        
    }
    
    virtual bool Running() const
    {
        return true;
    }
    
};



#endif //GUISCENE_HPP
