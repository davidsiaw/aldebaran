//
//  composablescene_interface.hpp
//  aldebaran
//
//  Created by David Siaw on 2015/02/26.
//  Copyright (c) 2015年 David Siaw. All rights reserved.
//

#ifndef COMPOSABLESCENEINTERFACE_HPP
#define COMPOSABLESCENEINTERFACE_HPP

#include "scene_interface.hpp"

class ComposableSceneInterface : public SceneInterface
{
public:
    virtual void SetOrigin(Uint16 x, Uint16 y, Uint16 z) = 0;
    virtual Uint16 GetOriginX() const = 0;
    virtual Uint16 GetOriginY() const = 0;
    virtual Uint16 GetOriginZ() const = 0;
};


#endif // COMPOSABLESCENEINTERFACE_HPP
