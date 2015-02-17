#ifndef RECTANGLESVBO_HPP
#define RECTANGLESVBO_HPP

#include <vector>

#include "vbo_interface.hpp"

class RectanglesVbo : public VboInterface
{
    std::vector<Element> elements;
    
public:
    RectanglesVbo()
    {
        triangle = {{
            {{-1, -1, 0}, {255,255,  0,255}, {0.0f, 1.0f}, 0.0f, 1.0f},
            {{ 1, -1, 0}, {  0,255,255,255}, {0.0f, 0.0f}, 0.0f, 1.0f},
            {{ 0,  1, 0}, {255,  0,255,255}, {1.0f, 0.0f}, 0.0f, 1.0f}
        }};
    }
    
    virtual Element* GetElements() const
    {
        return &elements[0];
    }
    
    virtual int GetElementCount() const
    {
        return elements.size();
    }
    
    virtual GLenum GetBufferType() const
    {
        return GL_TRIANGLE_STRIP;
    }
    
    virtual bool Changed() const
    {
        return false;
    }
    
    virtual void ChangeCommitted()
    {
    }

};

#endif // RECTANGLESVBO_HPP
