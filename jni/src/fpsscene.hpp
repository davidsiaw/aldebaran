#ifndef FPSSCENE_HPP
#define FPSSCENE_HPP

#include "utils.hpp"
#include "defaultshader.hpp"
#include "quadcollectionvbo.hpp"

class FPSScene : public ComposableSceneInterface
{
    std::shared_ptr<ShaderInterface> shader;
    std::shared_ptr<QuadCollectionVbo> vbo;
    std::shared_ptr<SDL_Surface> tex;
    std::shared_ptr<VboScene> scene;
    std::shared_ptr<TTF_Font> font;
    
    Uint32 timeStamp;
    Uint32 lastTimeStamp;
    float fps;
    
public:
    
    FPSScene()
    : shader(new DefaultShader()),
    vbo(new QuadCollectionVbo()),
    tex(MakeSurface(256, 256)),
    scene(new VboScene(shader, vbo, tex)),
    font(MakeFont("fonts/sazanami-gothic.ttf", 20))
    {
        
    }
    
    virtual void Init(SDL_Window* window)
    {
        vbo->Add(QuadVbo(0, 0, 0, 256, 256,  0, 0, 256, 256, 256, 256, 0, 0, 1));
        scene->Init(window);
    }
    
    virtual void Update(const InputState& inputs, Uint32 timestamp)
    {
        this->timeStamp = timestamp;
        scene->Update(inputs, timestamp);
    }
    
    virtual void Render(std::shared_ptr<RenderContextInterface> renderContext)
    {
        scene->SetMatrixTo2DView(renderContext->GetScreenWidth(), renderContext->GetScreenHeight());
        SDL_Color clear;
        clear.a = 0x00;
        clear.r = 0x00;
        clear.g = 0x00;
        clear.b = 0x00;
        
        
        SDL_Color black;
        black.a = 0xff;
        black.r = 0x00;
        black.g = 0x00;
        black.b = 0x00;
        
        fps = 1.0f/(timeStamp - lastTimeStamp)*1000;
        
        std::stringstream ss;
        ss << "FPS: " << fps;
        
        SDL_FillRect(tex.get(), nullptr, 0);
        SDL_Surface* text = TTF_RenderUTF8_Blended(font.get(), ss.str().c_str(), black);
        SDL_BlitSurface(text, nullptr, tex.get(), nullptr);
        SDL_FreeSurface(text);
        
        lastTimeStamp = timeStamp;
        
        scene->UpdateTexture();
        
        scene->Render(renderContext);
    }
    
    virtual bool Running() const
    {
        return true;
    }
    
    virtual void SetOrigin(Uint16 x, Uint16 y, Uint16 z)
    {
        scene->SetOrigin(x, y, z);
    }
    
    virtual Uint16 GetOriginX() const
    {
        return scene->GetOriginX();
    }
    
    virtual Uint16 GetOriginY() const
    {
        return scene->GetOriginY();
    }
    
    virtual Uint16 GetOriginZ() const
    {
        return scene->GetOriginZ();
    }


};


#endif // FPSSCENE_HPP
