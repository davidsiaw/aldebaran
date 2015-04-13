//
//  quadcollectionvbo.h
//  aldebaran
//
//  Created by David Siaw on 2015/01/18.
//  Copyright (c) 2015年 David Siaw. All rights reserved.
//

#ifndef QUADCOLLECTIONVBO_H
#define QUADCOLLECTIONVBO_H

#include <vector>
#include "primitives.hpp"
#include "vbo_interface.hpp"
#include "quadvbo.hpp"

class QuadCollectionVbo : public VboInterface
{
    std::vector<Element> elements;
    
    bool changed;
    
public:
    QuadCollectionVbo() : changed(true)
    {
    }
    
    virtual size_t Add(QuadVbo quad)
    {
        size_t quadId = elements.size();
        
        if (elements.size() != 0)
        {
            elements.push_back(elements[elements.size() - 1]);
            elements.push_back(quad.GetElements()[0]);
        }
        
        for (int i=0; i<4; i++)
        {
            elements.push_back(quad.GetElements()[i]);
        }
        
        changed = true;
        
        return quadId;
    }
    
    virtual void Modify(size_t index, QuadVbo quad)
    {
        if (index >= elements.size())
        {
            return;
        }
        
        int elemIdx = 1;
        if (index != 0)
        {
            elements[elemIdx++] = quad.GetElements()[0];
        }
        
        for (int i=0; i<4; i++)
        {
            elements[elemIdx++] = quad.GetElements()[i];
        }

        changed = true;
    }
    
    virtual int Count() const
    {
        return (int)elements.size();
    }
    
    virtual Element* GetElements() const
    {
        return (Element*)&elements[0];
    }
    
    virtual int GetElementCount() const
    {
        return (int)elements.size();
    }
    
    virtual GLenum GetBufferType() const
    {
        return GL_TRIANGLE_STRIP;
    }
    
    virtual bool Changed() const
    {
        return changed;
    }
    
    virtual void ChangeCommitted()
    {
        changed = false;
    }
    
};

#endif // QUADCOLLECTIONVBO_H
