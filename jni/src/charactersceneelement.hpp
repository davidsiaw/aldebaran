//
//  charactersceneelement.hpp
//  aldebaran
//
//  Created by David Siaw on 2015/06/23.
//  Copyright (c) 2015年 David Siaw. All rights reserved.
//

#ifndef CHARACTERSCENEELEMENT_HPP
#define CHARACTERSCENEELEMENT_HPP


class CharacterSceneElement
{
    int x,y;
    int charIdx;
    int variation;
    bool animated;
    
public:
    CharacterSceneElement(int x, int y, int charIdx, int variation, bool animated)
    :  x(x), y(y), charIdx(charIdx), variation(variation), animated(animated)
    {
    }
    
    bool IsAnimated() const
    {
        return animated;
    }
    
    int GetCharacterIdx() const
    {
        return charIdx;
    }
    
    int GetVariationIdx() const
    {
        return variation;
    }
    
    void SetPosition(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
    
    void SetX(int value)
    {
        x = value;
    }
    
    void SetY(int value)
    {
        y = value;
    }
    
    int GetX() const
    {
        return x;
    }
    
    int GetY() const
    {
        return y;
    }
    
    void SetVariation(int variation)
    {
        this->variation = variation;
    }
    
    void SetAnimated(bool animated)
    {
        this->animated = animated;
    }
};

#endif // CHARACTERSCENEELEMENT_HPP
