//
//  twodarray_interface.h
//  aldebaran
//
//  Created by David Siaw on 2015/05/15.
//  Copyright (c) 2015年 David Siaw. All rights reserved.
//

#ifndef TWODARRAY_INTERFACE_HPP
#define TWODARRAY_INTERFACE_HPP

// Interface for any kind of two-dimensional array

template<typename T>
class TwoDArrayInterface
{
public:
    virtual T Get(int x, int y) const = 0;
    virtual void Set(int x, int y, T value) = 0;
    virtual unsigned GetWidth() const = 0;
    virtual unsigned GetHeight() const = 0;
    virtual T& operator() (int x, int y) = 0;
};

#endif // TWODARRAY_INTERFACE_HPP
