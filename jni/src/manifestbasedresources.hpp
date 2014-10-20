#ifndef MANIFESTBASEDRESOURCES_HPP
#define MANIFESTBASEDRESOURCES_HPP

#include <map>
#include <sstream>
#include <luacppinterface.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "resource_interface.hpp"
#include "utils.hpp"
#include "ttffontdesc.hpp"

class ManifestBasedResources : public ResourceInterface
{
    std::map<std::string, std::shared_ptr<TTF_Font> > fontsByName;
    std::map<std::string, std::shared_ptr<SDL_Texture> > texturesByName;
    std::map<ResourceID, std::shared_ptr<ImageDesc> > imageResources;
    std::map<ResourceID, std::shared_ptr<SpriteDesc> > spriteResources;
    std::map<ResourceID, std::shared_ptr<FontDesc> > fontResources;

public:
    ManifestBasedResources(std::string manifest, SDL_Renderer* renderer)
    {
        auto configuration = LoadLuaConfiguration(manifest);
        
        auto images = configuration.Get<LuaTable>("images");
        
        images.ForAllIntegerKeys([&](int key, LuaType::Value type)
                                 {
                                     if (type == LuaType::table)
                                     {
                                         auto value = images.Get<LuaTable>(key);
                                         auto id = value.Get<int>("id");
                                         auto path = value.Get<std::string>("path");
                                         auto x = value.Get<int>("x");
                                         auto y = value.Get<int>("y");
                                         auto w = value.Get<int>("w");
                                         auto h = value.Get<int>("h");
                                         
                                         SDL_Rect rect;
                                         rect.x = x;
                                         rect.y = y;
                                         rect.w = w;
                                         rect.h = h;
                                         
                                         auto texture = texturesByName.find(path);
                                         if (texture == texturesByName.end())
                                         {
                                             texturesByName[path] = std::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, path.c_str()), SDL_DestroyTexture);
                                             
                                             
                                             texture = texturesByName.find(path);
                                             
                                             if (!texture->second)
                                             {
                                                 printlog("Image '%s' IMG:%s\n", path.c_str(), IMG_GetError());
                                             }
                                         }
                                         
                                         imageResources[id] = std::shared_ptr<ImageDesc>(new ImageDesc(texture->second, rect));
                                     }
                                 });
        
        
        auto sprites = configuration.Get<LuaTable>("sprites");
        
        sprites.ForAllIntegerKeys([&](int key, LuaType::Value type)
                                 {
                                     if (type == LuaType::table)
                                     {
                                         auto sprite = sprites.Get<LuaTable>(key);
                                         
                                         auto id = sprite.Get<int>("id");
                                         auto imageIds = sprite.Get<LuaTable>("imageIds");
                                         unsigned int delay = sprite.Get<int>("delay");
                                         
                                         std::vector< std::shared_ptr<ImageDesc> > imageList;
                                         imageIds.ForAllIntegerKeys([&](int key, LuaType::Value type)
                                                                    {
                                                                        auto imageId = imageIds.Get<int>(key);
                                                                        imageList.push_back(GetImage(imageId));
                                                                   });
                                         
                                         SDL_Rect rect = *(imageList[0]->GetRect());
                                         
                                         spriteResources[id] = std::shared_ptr<SpriteDesc>(new SpriteDesc(imageList, rect, delay));
                                         
                                     }
                                 });
        
        auto fonts = configuration.Get<LuaTable>("fonts");
        
        fonts.ForAllIntegerKeys([&](int key, LuaType::Value type)
                                  {
                                      if (type == LuaType::table)
                                      {
                                          auto font = fonts.Get<LuaTable>(key);
                                          
                                          auto id = font.Get<int>("id");
                                          auto path = font.Get<std::string>("path");
                                          auto color = font.Get<LuaTable>("color");
                                          auto ptsize = font.Get<int>("ptsize");
                                          
                                          std::stringstream ss;
                                          ss << path << "@" << ptsize;
                                          
                                          auto fontFile = fontsByName.find(ss.str());
                                          if (fontFile == fontsByName.end())
                                          {
                                              fontsByName[path] = std::shared_ptr<TTF_Font>(TTF_OpenFont(path.c_str(), ptsize), TTF_CloseFont);
                                              fontFile = fontsByName.find(path);
                                              TTF_SetFontHinting(fontFile->second.get(), 1);
                                              TTF_SetFontKerning(fontFile->second.get(), 1);
                                          }
                                          
                                          SDL_Color c;
                                          c.a = 0xff;
                                          c.r = (Uint8)color.Get<int>("r");
                                          c.g = (Uint8)color.Get<int>("g");
                                          c.b = (Uint8)color.Get<int>("b");

                                          fontResources[id] = std::shared_ptr<TTFFontDesc>(new TTFFontDesc(fontFile->second, c));

                                      }
                                  });
    }
    
    virtual std::shared_ptr<ImageDesc> GetImage(ResourceID num) const
    {
        return imageResources.find(num)->second;
    }
    
    virtual std::shared_ptr<SpriteDesc> GetSprite(ResourceID num) const
    {
        return spriteResources.find(num)->second;
    }
    
    virtual std::shared_ptr<FontDesc> GetFont(ResourceID num) const
    {
        return fontResources.find(num)->second;
    }
};

#endif // MANIFESTBASEDRESOURCES_HPP
