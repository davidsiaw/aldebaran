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

template<typename T>
class TwoDArray
{
    std::vector<T> array;
    unsigned w,h;
public:
    
    TwoDArray(unsigned w, unsigned h) : array(w*h), w(w), h(h)
    {
    }
    
    ~TwoDArray()
    {
        
    }
    
    T Get(int x, int y) const
    {
        return array[x + y * w];
    }
    
    void Set(int x, int y, T value)
    {
        array[x + y * w] = value;
    }
    
    unsigned GetWidth() const
    {
        return w;
    }
    
    unsigned GetHeight() const
    {
        return h;
    }
    
    T& operator() (int x, int y)
    {
        return array[x + y * w];
    }
};

#endif // TWODARRAY_HPP
