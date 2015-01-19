#ifndef TRIANGLEVBO_HPP
#define TRIANGLEVBO_HPP

#include <vector>
#include "primitives.hpp"
#include "vbo_interface.hpp"

class TriangleVbo : public VboInterface
{
    Triangle triangle;
    
public:
    TriangleVbo()
    {
        triangle = {{
            {{-1, -1, 0}, {255,255,  0,255}, {0.0f,1.0f}, 0.0f, 1.0f},
            {{ 1, -1, 0}, {  0,255,255,255}, {0.0f,0.0f}, 0.0f, 1.0f},
            {{ 0,  1, 0}, {255,  0,255,255}, {1.0f,0.0f}, 0.0f, 1.0f}
        }};
    }
    
    virtual Element* GetElements() const
    {
        return (Element*)triangle.e;
    }
    
    virtual int GetElementCount() const
    {
        return 3;
    }
    
    virtual GLenum GetBufferType() const
    {
        return GL_TRIANGLES;
    }
    
    virtual bool Changed() const
    {
        return false;
    }
    
    virtual void ChangeCommitted()
    {
    }

};

#endif // TRIANGLEVBO_HPP
