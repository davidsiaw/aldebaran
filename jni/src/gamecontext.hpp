//
//  rendercontext.hpp
//  aldebaran
//
//  Created by David Siaw on 2015/04/14.
//  Copyright (c) 2015年 David Siaw. All rights reserved.
//

#ifndef GAMECONTEXT_HPP
#define GAMECONTEXT_HPP

#include <vector>
#include <memory>
#include "rendercontext_interface.hpp"

class GameContext : public RenderContextInterface
{
    SDL_Window* window;
    std::map<int, InputState::Key> keyMap;
    std::vector< std::shared_ptr<SDL_Joystick> > joysticks;
    
    int screenWidth, screenHeight;
    
public:
    GameContext(SDL_Window* window, std::map<int, InputState::Key> keyMap, int screenWidth, int screenHeight)
    : window(window), keyMap(keyMap), screenWidth(screenWidth), screenHeight(screenHeight)
    {
        
    }

    void AddJoystick(std::shared_ptr<SDL_Joystick> joystick)
    {
        joysticks.push_back(joystick);
    }
    
    virtual int GetScreenWidth() const
    {
        return screenWidth;
    }
    
    virtual int GetScreenHeight() const
    {
        return screenHeight;
    }
    
    virtual SDL_Window* GetWindow() const
    {
        return window;
    }
    
    virtual const std::map<int, InputState::Key>& GetKeyMap() const
    {
        return keyMap;
    }
};


#endif // RENDERCONTEXT_HPP
