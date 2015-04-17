//
//  rendercontext.hpp
//  aldebaran
//
//  Created by David Siaw on 2015/04/14.
//  Copyright (c) 2015年 David Siaw. All rights reserved.
//

#ifndef RENDERCONTEXTINTERFACE_HPP
#define RENDERCONTEXTINTERFACE_HPP

class RenderContextInterface
{
public:
    virtual int GetScreenWidth() const = 0;
    virtual int GetScreenHeight() const = 0;
};

#endif // RENDERCONTEXTINTERFACE_HPP
