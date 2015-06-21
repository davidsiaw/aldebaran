#ifndef QUADVBO_HPP
#define QUADVBO_HPP

#include <vector>
#include "primitives.hpp"
#include "vbo_interface.hpp"

class QuadVbo : public VboInterface
{
    Quad quad;
    
    float x, y, z, w, h, texX, texY, texW, texH, wholeTexW, wholeTexH, offsetW, offsetH, numtiles;
    
    void init(float x, float y, float z,
              float w, float h,
              float texX, float texY,
              float texW, float texH,
              float wholeTexW, float wholeTexH,
              float offsetW, float offsetH, float numtiles)
    {
        quad = {{
            {{x  , y  , z}, {255,255,  0,255}, {(texX     )/wholeTexW, (texY     )/wholeTexH}, offsetW/wholeTexW, offsetH/wholeTexH, numtiles},
            {{x+w, y  , z}, {  0,255,255,255}, {(texX+texW)/wholeTexW, (texY     )/wholeTexH}, offsetW/wholeTexW, offsetH/wholeTexH, numtiles},
            {{x  , y+h, z}, {255,  0,255,255}, {(texX     )/wholeTexW, (texY+texH)/wholeTexH}, offsetW/wholeTexW, offsetH/wholeTexH, numtiles},
            {{x+w, y+h, z}, {255,  0,  0,255}, {(texX+texW)/wholeTexW, (texY+texH)/wholeTexH}, offsetW/wholeTexW, offsetH/wholeTexH, numtiles}
        }};
        
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        this->texX = texX;
        this->texY = texY;
        this->texW = texW;
        this->texH = texH;
        this->wholeTexW = wholeTexW;
        this->wholeTexH = wholeTexH;
        this->offsetW = offsetW;
        this->offsetH = offsetH;
        this->numtiles = numtiles;
    }
    
public:
    QuadVbo(float x, float y, float z,
            float w, float h,
            float texX = 0.0f, float texY = 0.0f,
            float texW = 1.0f, float texH = 1.0f,
            float wholeTexW = 1.0f, float wholeTexH = 1.0f,
            float offsetW = 0.0f, float offsetH = 0.0f, float numtiles = 1.0f)
    {
        init(x,y,z,w,h,texX,texY,texW,texH,wholeTexW,wholeTexH,offsetW,offsetH,numtiles);
    }
    
    QuadVbo(QuadVbo& original, float x, float y, float z)
    {
        init(x,y,z,original.w,original.h,
             original.texX,original.texY,original.texW,original.texH,
             original.wholeTexW,original.wholeTexH,original.offsetW,original.offsetH,
             original.numtiles);
    }
    
    static QuadVbo NullQuad()
    {
        return QuadVbo(0,0,0,0,0,0,0,0,0,0,0,0,0,0);
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
