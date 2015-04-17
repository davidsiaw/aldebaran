//
//  viewscene.hpp
//  aldebaran
//
//  Created by David Siaw on 2015/02/26.
//  Copyright (c) 2015年 David Siaw. All rights reserved.
//

#ifndef VIEWSCENE_HPP
#define VIEWSCENE_HPP

#include "scene_interface.hpp"
#include "composablescene_interface.hpp"

class FloatColor
{
    float r,g,b,a;

public:
    FloatColor(float r, float b, float g, float a) : r(r), b(b), g(g), a(a) {}
    
    float R() const
    {
        return r;
    }
    
    float B() const
    {
        return b;
    }
    
    float G() const
    {
        return g;
    }
    
    float A() const
    {
        return a;
    }
};

class ViewScene : public ComposableSceneInterface
{
    GLuint frameBuffer;
    GLuint renderedTexture;
    
    std::shared_ptr<VboInterface> quad;
    std::shared_ptr<ShaderInterface> shader;
    std::shared_ptr<SDL_Surface> dummy;
    
    std::shared_ptr<VboScene> vboScene;
    
    std::shared_ptr<ComposableSceneInterface> scene;
    
    Uint16 x,y,w,h;
    
    FloatColor background;
    
public:
    ViewScene(std::shared_ptr<ComposableSceneInterface> scene, Uint16 x, Uint16 y, Uint16 w, Uint16 h)
    : scene(scene),
    quad(new QuadVbo(x, y, w, h, 0, 0, w, -h, w, h, 0, 0, 1)),
    shader(new ColorShader()),
    dummy(MakeSurface(w,h)),
    vboScene(new VboScene(shader, quad, dummy)),
    x(x), y(y), w(w), h(h),
    background(0.0, 0.0, 0.0, 0.0)
    {
        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        
        glGenTextures(1, &renderedTexture);
        glBindTexture(GL_TEXTURE_2D, renderedTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);
        
        
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(status != GL_FRAMEBUFFER_COMPLETE)
        {
            printlog("problem");
            exit(1);
        }
        
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
    }
    
    virtual ~ViewScene()
    {
        glDeleteFramebuffers(1, &frameBuffer);
        glDeleteTextures(1, &renderedTexture);
    }
    
    virtual void SetBackgroundColor(float r, float g, float b, float a)
    {
        background = FloatColor(r, g, b, a);
    }
    
    virtual FloatColor GetBackgroundColor() const
    {
        return background;
    }
    
    virtual void Init(SDL_Window* window)
    {
        scene->Init(window);
    }
    
    virtual void Update(const InputState& inputs, Uint32 timestamp)
    {
        scene->Update(inputs, timestamp);
        vboScene->Update(inputs, timestamp);
    }
    
    virtual void Render(std::shared_ptr<RenderContextInterface> renderContext)
    {
        vboScene->SetMatrixTo2DRectangle(-x, -y, renderContext->GetScreenWidth(), renderContext->GetScreenHeight());
        scene->SetOrigin(0, 0);
        vboScene->_SetGLTexture(renderedTexture);
        
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        
        glClearColor ( background.R(), background.B(), background.G(), background.A() );
        glClear ( GL_COLOR_BUFFER_BIT );

        glViewport(0, 0, w, h);
        scene->Render(renderContext);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        glViewport(0, 0, renderContext->GetScreenWidth(), renderContext->GetScreenHeight());
        
        //vboScene->UpdateTexture();
        vboScene->Render(renderContext);
    }
    
    virtual bool Running() const
    {
        return true;
    }
    
    virtual void SetOrigin(Uint16 x, Uint16 y)
    {
        this->x = x;
        this->y = y;
    }
    
    virtual Uint16 GetOriginX() const
    {
        return x;
    }
    
    virtual Uint16 GetOriginY() const
    {
        return y;
    }
    
};


#endif // VIEWSCENE_HPP
