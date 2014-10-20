#ifndef ONETRIANGLEVBO_HPP
#define ONETRIANGLEVBO_HPP

#include <vector>
#include "primitives.hpp"
#include "vbo_interface.hpp"

class OneTriangleVbo : public VboInterface
{
    Triangle triangle;
public:
    OneTriangleVbo()
    {
        triangle = {{
            {{0,  0,0}, {255,255,  0  ,255}, {0.0f,1.0f}, 0.0f, 1.0f},
            {{1, 1,0}, {0  ,255,255  ,255}, {0.0f,0.0f}, 0.0f, 1.0f},
            {{500.5, -500.5,0}, {255,  0,  255,255}, {1.0f,0.0f}, 0.0f, 1.0f}
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
};

#endif // DISCRETETRIANGLEVBO_HPP
