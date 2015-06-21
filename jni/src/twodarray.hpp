//
//  twodarray.hpp
//  aldebaran
//
//  Created by David on 4/22/15.
//  Copyright (c) 2015 David Siaw. All rights reserved.
//

#ifndef TWODARRAY_HPP
#define TWODARRAY_HPP

#include <vector>

#include "twodarray_interface.hpp"

template<typename T>
class TwoDArray : public TwoDArrayInterface<T>
{
    std::vector<T> array;
    unsigned w,h;
    T defaultValue;
public:
    
    TwoDArray(unsigned w, unsigned h) : array(w*h), w(w), h(h)
    {
    }
    
    ~TwoDArray()
    {
        
    }
    
    virtual T Get(int x, int y) const
    {
        return array[x + y * w];
    }
    
    virtual void Set(int x, int y, T value)
    {
        if (x >= 0 && x < w && y >=0 && y < h)
        {
            array[x + y * w] = value;
        }
    }
    
    virtual unsigned GetWidth() const
    {
        return w;
    }
    
    virtual unsigned GetHeight() const
    {
        return h;
    }
    
    virtual T& operator() (int x, int y)
    {
        if (x < 0 || x >= w || y < 0 || y >= h)
        {
            return defaultValue;
        }
        return array[x + y * w];
    }
    
    void Write2x2(int x, int y, T tl, T tr, T bl, T br)
    {
        Set(x  ,y,  tl);
        Set(x+1,y,  tr);
        Set(x  ,y+1,bl);
        Set(x+1,y+1,br);
    }
};

#endif // TWODARRAY_HPP
