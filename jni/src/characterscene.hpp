//
//  characterscene.hpp
//  aldebaran
//
//  Created by David Siaw on 2015/06/23.
//  Copyright (c) 2015年 David Siaw. All rights reserved.
//

#ifndef CHARACTERSCENE_HPP
#define CHARACTERSCENE_HPP

#include "characterset.hpp"
#include "charactersceneelement.hpp"

class CharacterScene : public ComposableSceneInterface
{
    std::shared_ptr<ShaderInterface> shader;
    std::shared_ptr<QuadCollectionVbo> quads;
    std::shared_ptr<VboScene> scene;
    std::shared_ptr<Characterset> charset;
    
    std::map<int, std::shared_ptr<CharacterSceneElement>> elements;
    
    std::vector<size_t> quadCollection;
    
    bool changed;
    int count;
    
    int chartilesize;
    
    // For chars bigger than the tile, we need to centralize it on the tile itself
    int centralizerX;
    int centralizerY;
    
    
public:
    CharacterScene(std::shared_ptr<Characterset> charset, int tilesize) :
    shader(new DefaultShader()),
    quads(new QuadCollectionVbo),
    charset(charset),
    changed(true),
    count(0),
    chartilesize(tilesize * 2),
    centralizerX(-(charset->GetCharTileWidth() - chartilesize) / 2),
    centralizerY(-(charset->GetCharTileHeight() - chartilesize))
    {
        scene = std::make_shared<VboScene>(shader, quads, charset->GetSurface());
    }
    
    int AddCharacter(int charIdx, int x, int y)
    {
        int id = count++;
        elements[id] = std::make_shared<CharacterSceneElement>(x, y, charIdx, 0, false);
        changed = true;
        
        while (elements.size() > quadCollection.size())
        {
            size_t quadId = quads->Add(QuadVbo::NullQuad());
            quadCollection.push_back(quadId);
        }
        
        return id;
    }
    
    void RemoveCharacter(int id)
    {
        elements.erase(id);
        changed = true;
    }
    
    void SetCharacterPosition(int id, int x, int y)
    {
        elements[id]->SetPosition(x, y);
        changed = true;
    }
    
    int GetCharacterX(int id) const
    {
        return elements.find(id)->second->GetX();
    }
    
    int GetCharacterY(int id) const
    {
        return elements.find(id)->second->GetY();
    }
    
    void SetCharacterAnimation(int id, bool animated, int variation)
    {
        elements[id]->SetAnimated(animated);
        elements[id]->SetVariation(variation);
        changed = true;
    }
    
    void ClearCharacters()
    {
        elements.clear();
        changed = true;
    }
    
    virtual void Init(SDL_Window* window)
    {
        scene->Init(window);
    }
    
    virtual void Update(const InputState& inputs, Uint32 timestamp)
    {
        if (changed)
        {
            std::vector<std::shared_ptr<CharacterSceneElement>> elems;
            for (auto elem : elements)
            {
                elems.push_back(elem.second);
            }
            
            std::sort(elems.begin(), elems.end(),
                      [](const std::shared_ptr<CharacterSceneElement> a, const std::shared_ptr<CharacterSceneElement> b) -> bool {
                          return a->GetY() < b->GetY();
                      });
            
            for (size_t i=0; i<elems.size(); i++)
            {
                if (elems[i]->IsAnimated())
                {
                    quads->Modify(quadCollection[i],
                                  charset->GetAnimated(elems[i]->GetCharacterIdx(),
                                                       elems[i]->GetVariationIdx(),
                                                       elems[i]->GetX() + centralizerX,
                                                       elems[i]->GetY() + centralizerY, 0));
                }
                else
                {
                    quads->Modify(quadCollection[i],
                                  charset->GetStill(elems[i]->GetCharacterIdx(),
                                                    elems[i]->GetVariationIdx(),
                                                    elems[i]->GetX() + centralizerX,
                                                    elems[i]->GetY() + centralizerY, 0));
                }
            }
            
            for (size_t i=elems.size(); i<quadCollection.size(); i++)
            {
                quads->Modify(quadCollection[i], QuadVbo::NullQuad());
            }
            
            changed = false;
            
        }
        scene->Update(inputs, timestamp);
    }
    
    virtual void Render(std::shared_ptr<RenderContextInterface> renderContext)
    {
        scene->SetMatrixTo2DRectangle(0, 0, renderContext->GetScreenWidth(), renderContext->GetScreenHeight());
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


#endif // CHARACTERSCENE_HPP
