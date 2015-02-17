#ifndef QUADVBO_HPP
#define QUADVBO_HPP

#include <vector>
#include "primitives.hpp"
#include "vbo_interface.hpp"

class QuadVbo : public VboInterface
{
    Quad quad;
    
public:
    QuadVbo(float x, float y,
            float w, float h,
            float texX = 0.0f, float texY = 0.0f,
            float texW = 1.0f, float texH = 1.0f,
            float wholeTexW = 1.0f, float wholeTexH = 1.0f,
            float offsetW = 0.0f, float offsetH = 0.0f, float numtiles = 1.0f)
    {
        quad = {{
            {{x  , y  , 0}, {255,255,  0,255}, {(texX     )/wholeTexW, (texY     )/wholeTexH}, offsetW/wholeTexW, offsetH/wholeTexH, numtiles},
            {{x+w, y  , 0}, {  0,255,255,255}, {(texX+texW)/wholeTexW, (texY     )/wholeTexH}, offsetW/wholeTexW, offsetH/wholeTexH, numtiles},
            {{x  , y+h, 0}, {255,  0,255,255}, {(texX     )/wholeTexW, (texY+texH)/wholeTexH}, offsetW/wholeTexW, offsetH/wholeTexH, numtiles},
            {{x+w, y+h, 0}, {255,  0,  0,255}, {(texX+texW)/wholeTexW, (texY+texH)/wholeTexH}, offsetW/wholeTexW, offsetH/wholeTexH, numtiles}
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
