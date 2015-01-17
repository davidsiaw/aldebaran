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

class VboScene : public SceneInterface
{
    std::shared_ptr<VboInterface> vbo;
    std::shared_ptr<ShaderInterface> shader;
    std::shared_ptr<SDL_Texture> texture;
    bool texturePresent;
    GLuint buffer;
    int tile;
    float transparency;
    
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
            glVertexAttribPointer(shader->GetTileAttribute(), 1, GL_FLOAT, GL_FALSE, sizeof(Element), OFFSET_OF(Element, tileOffset));
        }
        
        if (shader->HasTileNumAttribute())
        {
            glVertexAttribPointer(shader->GetTilenumAttribute(), 1, GL_FLOAT, GL_FALSE, sizeof(Element), OFFSET_OF(Element, numtiles));
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
public:
    VboScene(std::shared_ptr<ShaderInterface> shader, std::shared_ptr<VboInterface> vbo, std::shared_ptr<SDL_Texture> texture)
    : vbo(vbo), shader(shader), texture(texture), texturePresent(texture.get() != nullptr), tile(0), transparency(1.0f)
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
    
    virtual void Init(SDL_Window* window)
    {
        
    }
    
    virtual void Update(const InputState& inputs, Uint32 timestamp)
    {
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
            glActiveTexture(GL_TEXTURE0);
            SDL_GL_BindTexture(texture.get(), nullptr, nullptr);
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        
        shader->SetMatrix(glm::mat4());
        
        glEnableVertexAttribArray(shader->GetPositionAttribute());
        if (shader->HasTexCoordAttribute()) glEnableVertexAttribArray(shader->GetTexCoordAttribute());
        if (shader->HasColorAttribute()) glEnableVertexAttribArray(shader->GetColorAttribute());
        if (shader->HasTileAttribute()) glEnableVertexAttribArray(shader->GetTileAttribute());
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
        if (shader->HasTileAttribute()) glDisableVertexAttribArray(shader->GetTileAttribute());
        if (shader->HasTileNumAttribute()) glDisableVertexAttribArray(shader->GetTilenumAttribute());
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        if (shader->HasTexCoordAttribute())
        {
            glBindTexture(GL_TEXTURE_2D, 0);
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
