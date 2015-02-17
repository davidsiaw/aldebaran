//
//  emptyscene.hpp
//  aldebaran
//
//  Created by David Siaw on 2014/10/16.
//  Copyright (c) 2014年 David Siaw. All rights reserved.
//

#ifndef VBOSCENE_HPP
#define VBOSCENE_HPP

#include "vbo_interface.hpp"
#include "printlog.hpp"

class VboScene : public SceneInterface
{
    std::shared_ptr<VboInterface> vbo;
    std::shared_ptr<ShaderInterface> shader;
    std::shared_ptr<SDL_Surface> texSurface;
    std::shared_ptr<GLuint> texture;
    bool texturePresent;
    GLuint buffer;
    int tile;
    Uint32 animationDelay;
    Uint32 lastUpdate;
    float transparency;

    glm::mat4 matrix;
    
    void Upload()
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        
        glVertexAttribPointer(shader->GetPositionAttribute(), 3, GL_FLOAT, GL_FALSE, sizeof(Element), OFFSET_OF(Element, v));
        
        if (shader->HasColorAttribute())
        {
            glVertexAttribPointer(shader->GetColorAttribute(), 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Element), OFFSET_OF(Element, c));
        }
        
        if (shader->HasTexCoordAttribute())
        {
            glVertexAttribPointer(shader->GetTexCoordAttribute(), 2, GL_FLOAT, GL_FALSE, sizeof(Element), OFFSET_OF(Element, t));
        }
        
        if (shader->HasTileAttribute())
        {
            glVertexAttribPointer(shader->GetTileOffsetWAttribute(), 1, GL_FLOAT, GL_FALSE, sizeof(Element), OFFSET_OF(Element, tileOffsetW));
            glVertexAttribPointer(shader->GetTileOffsetHAttribute(), 1, GL_FLOAT, GL_FALSE, sizeof(Element), OFFSET_OF(Element, tileOffsetH));
        }
        
        if (shader->HasTileNumAttribute())
        {
            glVertexAttribPointer(shader->GetTilenumAttribute(), 1, GL_FLOAT, GL_FALSE, sizeof(Element), OFFSET_OF(Element, numtiles));
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
public:
    VboScene(
        std::shared_ptr<ShaderInterface> shader, 
        std::shared_ptr<VboInterface> vbo, 
        std::shared_ptr<SDL_Surface> texSurface,
        Uint32 animationDelay = 200)
            : vbo(vbo),
            shader(shader),
            texSurface(texSurface),
            texturePresent(texSurface.get() != nullptr),
            tile(0),
            animationDelay(animationDelay),
            lastUpdate(0),
            transparency(1.0f)
    {
        int bufferSize = vbo->GetElementCount() * sizeof(Element);
        
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, vbo->GetElements());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        Upload();
    }
    
    virtual ~VboScene()
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &buffer);
    }

    void SetMatrixToIdentity()
    {
        matrix = glm::mat4();
    }

    void SetMatrixTo2DView(int width, int height)
    {
        matrix = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
    }
    
    void SetMatrixTo2DRectangle(int x, int y, int width, int height)
    {
        matrix = glm::ortho(x, width + x, height + y, y);
    }
    
    virtual void Init(SDL_Window* window)
    {
        if (texSurface && !texture)
        {
            GLuint textureIndex;
            /* Create The Texture */
            glGenTextures( 1, &textureIndex );
            
            /* Typical Texture Generation Using Data From The Bitmap */
            glBindTexture( GL_TEXTURE_2D, textureIndex );
            
            /* Generate The Texture */
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texSurface->w,
                         texSurface->h, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, texSurface->pixels );
            glGetError();
            
            /* Linear Filtering */
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
            
            texture = std::shared_ptr<GLuint>(new GLuint(textureIndex));
            if (texture)
            {
                texturePresent = true;
            }
        }
    }
    
    virtual void Update(const InputState& inputs, Uint32 timestamp)
    {
        if (timestamp > lastUpdate + animationDelay)
        {
            lastUpdate = timestamp;
            tile++;
        }
        
        if (vbo->Changed())
        {
            Upload();
            vbo->ChangeCommitted();
        }
    }
    
    virtual void Render()
    {
        glUseProgram(shader->GetProgram());
        
        if (shader->HasTexCoordAttribute())
        {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, *texture);
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        
        shader->SetMatrix(matrix);
        shader->SetActiveTileUniform(tile);
        
        glEnableVertexAttribArray(shader->GetPositionAttribute());
        if (shader->HasTexCoordAttribute()) glEnableVertexAttribArray(shader->GetTexCoordAttribute());
        if (shader->HasColorAttribute()) glEnableVertexAttribArray(shader->GetColorAttribute());
        if (shader->HasTileAttribute())
        {
            glEnableVertexAttribArray(shader->GetTileOffsetWAttribute());
            glEnableVertexAttribArray(shader->GetTileOffsetHAttribute());
        }
        if (shader->HasTileNumAttribute()) glEnableVertexAttribArray(shader->GetTilenumAttribute());
        
        if (shader->HasTransparencyUniform()) shader->SetTransparencyUniform(transparency);
        if (shader->HasActiveTileUniform())  shader->SetActiveTileUniform(tile);
        
        if (shader->HasTexCoordAttribute())
        {
            shader->SetTexturePresentUniform(texturePresent);
            shader->SetTextureUniform(0);
        }
        
        glDrawArrays(vbo->GetBufferType(), 0, vbo->GetElementCount());
        
        glDisableVertexAttribArray(shader->GetPositionAttribute());
        if (shader->HasTexCoordAttribute()) glDisableVertexAttribArray(shader->GetTexCoordAttribute());
        if (shader->HasColorAttribute()) glDisableVertexAttribArray(shader->GetColorAttribute());
        if (shader->HasTileAttribute())
        {
            glDisableVertexAttribArray(shader->GetTileOffsetWAttribute());
            glDisableVertexAttribArray(shader->GetTileOffsetHAttribute());
        }
        if (shader->HasTileNumAttribute()) glDisableVertexAttribArray(shader->GetTilenumAttribute());
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        if (shader->HasTexCoordAttribute())
        {
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_BLEND);
        }
        
        int a = glGetError();
        if (a != GL_NO_ERROR)
        {
            printlog("%x\n", a);
            exit(EXIT_FAILURE);
        }
    }
    
    virtual bool Running() const
    {
        return true;
    }
    
};



#endif //GUISCENE_HPP
