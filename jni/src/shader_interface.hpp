#ifndef SHADER_INTERFACE_H
#define SHADER_INTERFACE_H

#include "opengl.hpp"

class ShaderInterface
{
public:
    virtual GLuint GetProgram()=0;
    
    virtual GLuint GetPositionAttribute()=0;
    virtual GLuint GetColorAttribute()=0;
    virtual GLuint GetTileOffsetWAttribute()=0;
    virtual GLuint GetTileOffsetHAttribute()=0;
    virtual GLuint GetTilenumAttribute()=0;
    virtual GLuint GetTexCoordAttribute()=0;
    
    virtual void SetTransparencyUniform(GLfloat transp)=0;
    virtual void SetActiveTileUniform(GLfloat activetile)=0;
    virtual void SetTexturePresentUniform(bool hastexture)=0;
    virtual void SetTextureUniform(GLuint tex)=0;
    virtual void SetMatrix(glm::mat4 matrix)=0;
    
    virtual bool HasColorAttribute()=0;
    virtual bool HasTileAttribute()=0;
    virtual bool HasTileNumAttribute()=0;
    virtual bool HasTexCoordAttribute()=0;
    
    virtual bool HasTransparencyUniform()=0;
    virtual bool HasActiveTileUniform()=0;
};

#endif // SHADER_INTERFACE_H
