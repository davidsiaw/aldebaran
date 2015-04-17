//
//  emptyscene.hpp
//  aldebaran
//
//  Created by David Siaw on 2014/10/16.
//  Copyright (c) 2014年 David Siaw. All rights reserved.
//

#ifndef EMPTYSCENE_HPP
#define EMPTYSCENE_HPP

class EmptyScene : public SceneInterface
{
public:
    EmptyScene() {}
    virtual ~EmptyScene() {}
    
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



#endif //EMPTYSCENE_HPP
