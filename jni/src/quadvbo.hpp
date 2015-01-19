#ifndef QUADVBO_HPP
#define QUADVBO_HPP

#include <vector>
#include "primitives.hpp"
#include "vbo_interface.hpp"

class QuadVbo : public VboInterface
{
    Quad quad;
    
public:
    QuadVbo(float x, float y, float w, float h, float texX = 0.0f, float texY = 0.0f, float texW = 1.0f, float texH = 1.0f)
    {
        quad = {{
            {{x  , y  , 0}, {255,255,  0,255}, {texX     , texY     }, 0.0f, 1.0f},
            {{x+w, y  , 0}, {  0,255,255,255}, {texX+texW, texY     }, 0.0f, 1.0f},
            {{x  , y+h, 0}, {255,  0,255,255}, {texX     , texY+texH}, 0.0f, 1.0f},
            {{x+w, y+h, 0}, {255,  0,  0,255}, {texX+texW, texY+texH}, 0.0f, 1.0f}
        }};
    }
    
    virtual Element* GetElements() const
    {
        return (Element*)quad.e;
    }
    
    virtual int GetElementCount() const
    {
        return 4;
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

#endif // QUADVBO_HPP
