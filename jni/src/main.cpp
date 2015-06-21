#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <memory>
#include <functional>
#include <list>

#include <Box2D/Box2D.h>

#include "opengl.hpp"

#include "scene_interface.hpp"
#include "inputstate.hpp"

#include "printlog.hpp"

#include "inputmachine.hpp"

#include "manifestbasedresources.hpp"

#include "notifier.hpp"

#include "emptyscene.hpp"
#include "guiscene.hpp"
#include "vboscene.hpp"
#include "fpsscene.hpp"

#include "utils.hpp"

#include "defaultshader.hpp"
#include "wireframeshader.hpp"
#include "colorshader.hpp"
#include "trianglevbo.hpp"
#include "quadvbo.hpp"
#include "quadcollectionvbo.hpp"
#include "viewscene.hpp"
#include "compositescene.hpp"
#include "gamecontext.hpp"

#include "twodarray.hpp"


SDL_Surface* flipVert(SDL_Surface* sfc)
{
    SDL_Surface* result = SDL_CreateRGBSurface(sfc->flags, sfc->w, sfc->h,
                                               sfc->format->BytesPerPixel * 8, sfc->format->Rmask, sfc->format->Gmask,
                                               sfc->format->Bmask, sfc->format->Amask);
    if (result == NULL) return NULL;
    
    Uint8* pixels = (Uint8*) sfc->pixels;
    Uint8* rpixels = (Uint8*) result->pixels;
    
    Uint32 pitch = sfc->pitch;
    Uint32 pxlength = pitch*sfc->h;
    
    for(int line = 0; line < sfc->h; ++line) {
        Uint32 pos = line * pitch;
        memcpy(&rpixels[pos], &pixels[(pxlength-pos)-pitch], pitch);
    }
    
    return result;
}

void takeScreenShot(std::string filename, SDL_Window* window)
{
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	SDL_Surface* surf = SDL_CreateRGBSurface(
		SDL_SWSURFACE, 
		windowWidth,
		windowHeight,
		24, 
		0x000000FF, 
		0x0000FF00, 
		0x00FF0000, 
		0);
    
	if (surf == NULL) return;

	glReadPixels(0, 0, windowWidth, windowHeight, GL_RGB, GL_UNSIGNED_BYTE, surf->pixels);

    SDL_Surface * flip = flipVert(surf);
    if (flip == NULL) return;
    SDL_FreeSurface(surf);
    
    SDL_SaveBMP(flip, filename.c_str());
    
    SDL_FreeSurface(flip);
}

int exists(const char *fname)
{
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

void run(std::shared_ptr<SceneInterface> scene, std::shared_ptr<GameContext> context)
{
	InputState inputState;
	Uint32 time = SDL_GetTicks();
	scene->Init(context->GetWindow());
	
	while (scene->Running())
	{
		SDL_Event e;
		if ( SDL_PollEvent(&e) )
		{
			if (e.type == SDL_QUIT)
			{
				break;
			}
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_1 && (e.key.keysym.mod & (KMOD_CTRL | KMOD_SHIFT)) != 0)
                {
                    int count = 0;
                    std::string fname = "screenshot";
                    do
                    {
                        std::stringstream ss;
                        ss << "screenshot" << count << ".bmp";
                        fname = ss.str();
                        count++;
                    }
                    while (exists(fname.c_str()));
                    takeScreenShot(fname, context->GetWindow());
                }
            }
			CaptureInputState(context->GetKeyMap(), &inputState, &e);
		}
		
		Uint32 now = SDL_GetTicks();
		scene->Update(inputState, now);
		if (now - time > 15)
        {
            glClearColor( 1.0, 1.0, 1.0, 1.0 );
            glClear ( GL_COLOR_BUFFER_BIT );
            scene->Render(context);
            SDL_GL_SwapWindow(context->GetWindow());
			time = SDL_GetTicks();
		}
	}
}

class Tileset
{
    std::shared_ptr<SDL_Surface> tex;
    std::vector<std::shared_ptr<QuadVbo>> tiles;
    int tilesize;
    
public:
    Tileset(std::string textureFile, int tilesize) : tex(LoadImage(textureFile)), tilesize(tilesize)
    {
        for (int y=0; y<(tex->w / tilesize); y++)
        for (int x=0; x<(tex->h / tilesize); x++)
        {
            auto quad = std::make_shared<QuadVbo>(0, 0, 0, tilesize, tilesize,
                                                  x*tilesize, y*tilesize, tilesize, tilesize,
                                                  tex->w, tex->h);
            
            tiles.push_back(quad);
        }
    }
    
    std::shared_ptr<SDL_Surface> GetSurface() const
    {
        return tex;
    }
    
    int GetTilesize() const
    {
        return tilesize;
    }
    
    size_t GetTileCount() const
    {
        return tiles.size();
    }
    
    QuadVbo GetTile(int index, int x, int y) const
    {
        QuadVbo quad(*tiles[index], x*tilesize, y*tilesize, 0);
        return quad;
    }
};

class TileMapScene : public ComposableSceneInterface
{
    std::shared_ptr<ShaderInterface> shader;
    std::shared_ptr<QuadCollectionVbo> quads;
    std::shared_ptr<VboScene> scene;
    std::shared_ptr<Tileset> tiles;
    TwoDArray<size_t> quadIndices;
    int w, h;
    
public:
    TileMapScene(std::shared_ptr<Tileset> tiles, int w, int h) :
    shader(new DefaultShader()), quads(new QuadCollectionVbo()), tiles(tiles), w(w), h(h), quadIndices(w,h)
    {
        for (int y=0; y<h; y++)
        for (int x=0; x<w; x++)
        {
            quadIndices(x,y) = quads->Add(tiles->GetTile(0, x, y));
        }
        
        scene = std::make_shared<VboScene>(shader, quads, tiles->GetSurface());
        
    }
    
    virtual void Init(SDL_Window* window)
    {
        scene->Init(window);
    }
    
    virtual void Update(const InputState& inputs, Uint32 timestamp)
    {
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
    
    virtual void Set(int x, int y, int value)
    {
        quads->Modify(quadIndices(x,y), tiles->GetTile(value, x, y));
    }
    
    int GetTilesize() const
    {
        return tiles->GetTilesize();
    }
};

class Characterset
{
    std::shared_ptr<SDL_Surface> tex;
    int w,h;

public:
    Characterset(std::string bitmap, int w, int h) : tex(LoadImage(bitmap)), w(w), h(h)
    {
    }
    
    QuadVbo GetAnimated(int characterIdx, int spriteIdx, int x, int y, int z) const
    {
        QuadVbo quad(x, y, z, w, h,
                     w * characterIdx * 4, h * spriteIdx, w, h,
                     tex->w, tex->h, w, 0, 4);
        return quad;
    }
    
    QuadVbo GetStill(int characterIdx, int spriteIdx, int x, int y, int z) const
    {
        QuadVbo quad(x, y, z, w, h,
                     w * characterIdx * 4, h * spriteIdx, w, h,
                     tex->w, tex->h);
        return quad;
    }
    
    std::shared_ptr<SDL_Surface> GetSurface() const
    {
        return tex;
    }
    
    int GetCharTileWidth() const
    {
        return w;
    }
    
    int GetCharTileHeight() const
    {
        return h;
    }
};

class CharacterSceneElement
{
    int x,y;
    int charIdx;
    int variation;
    bool animated;
    
public:
    CharacterSceneElement(int x, int y, int charIdx, int variation, bool animated)
    :  x(x), y(y), charIdx(charIdx), variation(variation), animated(animated)
    {
    }
    
    bool IsAnimated() const
    {
        return animated;
    }
    
    int GetCharacterIdx() const
    {
        return charIdx;
    }
    
    int GetVariationIdx() const
    {
        return variation;
    }
    
    void SetPosition(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
    
    void SetX(int value)
    {
        x = value;
    }
    
    void SetY(int value)
    {
        y = value;
    }
    
    int GetX() const
    {
        return x;
    }
    
    int GetY() const
    {
        return y;
    }

    void SetVariation(int variation)
    {
        this->variation = variation;
    }
    
    void SetAnimated(bool animated)
    {
        this->animated = animated;
    }
};

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

class RPGMapScene : public ComposableSceneInterface
{
    class CharacterDetails
    {
    public:
        CharacterDetails(int id, int x, int y) :
        charid(id),
        mapx(x),
        mapy(y),
        amountX(0),
        amountY(0),
        speed(0),
        lastUpdate(0),
        movedDistanceX(0),
        movedDistanceY(0),
        direction(0)
        {
            
        }
        
        int charid;
        int mapx;
        int mapy;

        int amountX;
        int amountY;
        int speed;
        int lastUpdate;
        int movedDistanceX;
        int movedDistanceY;
        
        int direction;
    };
    
    int w,h;
    
    std::vector<std::shared_ptr<TileMapScene>> layers;
    std::shared_ptr<CharacterScene> characters;
    
    std::map<int, std::shared_ptr<CharacterDetails>> characterRegister;
    
    TwoDArray<std::shared_ptr<CharacterDetails>> characterGrid;
    
    std::shared_ptr<QuadCollectionVbo> vbo;
    std::shared_ptr<ShaderInterface> ds;
    std::shared_ptr<VboScene> debugScene;
    
public:
    RPGMapScene(int w, int h, std::shared_ptr<Tileset> tileset, std::shared_ptr<Characterset> charset) :
        characters(std::make_shared<CharacterScene>(charset, tileset->GetTilesize())),
        characterGrid(w,h),
        w(w),
        h(h),
        vbo(new QuadCollectionVbo()),
        ds(new WireframeShader()),
        debugScene(new VboScene(ds, vbo, nullptr))
    {
        for (int i=0; i<4; i++)
        {
            layers.push_back(std::make_shared<TileMapScene>(tileset, w * 2, h * 2));
        }
    }
    
    virtual void Init(SDL_Window* window)
    {
        for (int i=0; i<layers.size(); i++)
        {
            layers[i]->Init(window);
        }
        characters->Init(window);
        debugScene->Init(window);
    }
    
    bool CanMoveTo(int mapx, int mapy)
    {
        if (mapx < 0 || mapx >= w)
        {
            return false;
        }
        
        if (mapy < 0 || mapy >= h)
        {
            return false;
        }
        
        if (characterGrid(mapx, mapy))
        {
            return false;
        }
        
        return true;
    }
    
    virtual void Update(const InputState& inputs, Uint32 timestamp)
    {
        auto tilesize = layers[0]->GetTilesize() * 2;
        
        for (int i=0; i<layers.size(); i++)
        {
            layers[i]->Update(inputs, timestamp);
        }
        
        for (auto kvpair : characterRegister)
        {
            
            auto id = kvpair.first;
            auto info = kvpair.second;
            
            bool move = false;
            if (info->amountX > 0) { info->direction = 1; }
            if (info->amountX < 0) { info->direction = 3; }
            if (info->amountY > 0) { info->direction = 2; }
            if (info->amountY < 0) { info->direction = 0; }
            
            if (info->amountX == 0 && info->amountY == 0)
            {
                characters->SetCharacterAnimation(id, false, info->direction);
                continue;
            }

            if (info->amountX != 0 || info->amountY != 0)
            {
                move = true;
            }
            
            if (timestamp - info->lastUpdate > 1000 - info->speed)
            {
                auto character = characterGrid(info->mapx, info->mapy);
                characterGrid(info->mapx, info->mapy) = nullptr;
                
                int posX = characters->GetCharacterX(id);
                int posY = characters->GetCharacterY(id);
                
                if (characters->GetCharacterX(id) % tilesize == 0 && info->amountX != 0)
                {
                    int nextMapX = posX / tilesize + sgn(info->amountX);
                    if (CanMoveTo(nextMapX, info->mapy))
                    {
                        info->mapx = nextMapX;
                        posX = characters->GetCharacterX(id) + info->amountX;
                        info->movedDistanceX += info->amountX;
                    }
                    else
                    {
                        info->amountX = 0;
                        info->movedDistanceX = 0;
                    }
                }
                else
                {
                    posX = characters->GetCharacterX(id) + info->amountX;
                    info->movedDistanceX += info->amountX;
                    
                    if (posX % tilesize == 0)
                    {
                        info->amountX = 0;
                        info->movedDistanceX = 0;
                    }
                }

                
                if (characters->GetCharacterY(id) % tilesize == 0 && info->amountY != 0)
                {
                    int nextMapY = posY / tilesize + sgn(info->amountY);
                    if (CanMoveTo(info->mapx, nextMapY))
                    {
                        info->mapy = nextMapY;
                        posY = characters->GetCharacterY(id) + info->amountY;
                        info->movedDistanceY += info->amountY;
                    }
                    else
                    {
                        info->amountY = 0;
                        info->movedDistanceY = 0;
                    }
                }
                else
                {
                    posY = characters->GetCharacterY(id) + info->amountY;
                    info->movedDistanceY += info->amountY;
                    
                    if (posY % tilesize == 0)
                    {
                        info->amountY = 0;
                        info->movedDistanceY = 0;
                    }
                }
                
                characterGrid(info->mapx, info->mapy) = character;
                characters->SetCharacterPosition(id, posX, posY);
                info->lastUpdate = timestamp;
                characters->SetCharacterAnimation(id, move, info->direction);
                
                vbo->Modify(kvpair.first, QuadVbo(
                                                  info->mapx * tilesize,
                                                  info->mapy * tilesize, 0,
                                                  tilesize,
                                                  tilesize));
                
            }
            
        }
        
        
        for (int i=vbo->QuadCount(); i<characterRegister.size(); i++)
        {
            vbo->Add(QuadVbo::NullQuad());
        }
        
        debugScene->Update(inputs, timestamp);
        
        characters->Update(inputs, timestamp);
    }
    
    virtual void Render(std::shared_ptr<RenderContextInterface> renderContext)
    {
        for (int i=0; i<layers.size(); i++)
        {
            layers[i]->Render(renderContext);
            if (i == 1) { characters->Render(renderContext); }
        }
        
        debugScene->SetMatrixTo2DRectangle(0, 0, renderContext->GetScreenWidth(), renderContext->GetScreenHeight());
        debugScene->Render(renderContext);
    }
    
    virtual bool Running() const
    {
        return true;
    }
    
    virtual void SetOrigin(Uint16 x, Uint16 y, Uint16 z)
    {
        for (int i=0; i<layers.size(); i++)
        {
            layers[i]->SetOrigin(x, y, 0);
        }

        characters->SetOrigin(x, y, 0);
        
        debugScene->SetOrigin(x, y, 0);
    }
    
    virtual Uint16 GetOriginX() const
    {
        return layers[0]->GetOriginX();
    }
    
    virtual Uint16 GetOriginY() const
    {
        return layers[0]->GetOriginY();
    }
    
    virtual Uint16 GetOriginZ() const
    {
        return layers[0]->GetOriginZ();
    }

    void Set(int layer, int x, int y, int value)
    {
        layers[layer]->Set(x, y, value);
    }
    
    int SetCharacter(int idx, int mapx, int mapy)
    {
        auto tilesize = layers[0]->GetTilesize() * 2;
        auto id = characters->AddCharacter(idx, mapx * tilesize, mapy * tilesize);
        
        auto md = std::make_shared<CharacterDetails>(id, mapx, mapy);
        characterRegister[id] = md;
        characterGrid(mapx,mapy) = md;
        
        return id;
    }
    
    void MoveCharacter(int id, InputState::Key direction, int amount, int speed)
    {
        if (characterRegister.find(id) == characterRegister.end())
        {
            return;
        }

        auto md = characterRegister[id];
        
        if (md->amountX == 0 && md->amountY == 0)
        {
            md->lastUpdate = 0;
            md->movedDistanceX = 0;
            md->movedDistanceY = 0;
            md->amountX = 0;
            md->amountY = 0;
        }
        
        md->speed = speed;
        
        
        switch (direction) {
            case InputState::UP:
                md->amountY = -amount;
                break;
            case InputState::RIGHT:
                md->amountX = amount;
                break;
            case InputState::DOWN:
                md->amountY = amount;
                break;
            case InputState::LEFT:
                md->amountX = -amount;
                break;
            default:
                break;
        }
        

        

    }
};

class ProcessingScene : public ComposableSceneInterface
{
    std::function<void(const InputState&, Uint32)> update;
    std::function<bool()> running;
    
public:
    
    ProcessingScene(std::function<void(const InputState&, Uint32)> update, std::function<bool()> running) :
    update(update), running(running)
    {
        
    }
    
    virtual void Init(SDL_Window* window) { }

    virtual void Update(const InputState& inputs, Uint32 timestamp)
    {
        update(inputs, timestamp);
    }

    virtual void Render(std::shared_ptr<RenderContextInterface> renderContext) { }

    virtual bool Running() const
    {
        return running();
    }
    
    virtual void SetOrigin(Uint16 x, Uint16 y, Uint16 z) { }
    virtual Uint16 GetOriginX() const { return 0; }
    virtual Uint16 GetOriginY() const { return 0; }
    virtual Uint16 GetOriginZ() const { return 0; }

};

void game(std::shared_ptr<GameContext> context)
{
    auto tiles = std::make_shared<Tileset>("tiles/testtiles1.png", 24);
    auto charset = std::make_shared<Characterset>("sprites/output.png", 48, 64);
    
    auto scene = std::make_shared<RPGMapScene>(10, 10, tiles, charset);
    
    for (int x=0; x<20; x++)
    for (int y=0; y<20; y++)
        scene->Set(0, x, y, 21);
    
    int rikka = scene->SetCharacter(5, 0, 0);
    
    int rikka2 = scene->SetCharacter(5, 6, 6);
    
    //scene->Set(1, 0, 0, 22);
    //scene->Set(2, 0, 0, 21);
    //scene->Set(3, 0, 0, 21);
    
    auto wholeScene = std::make_shared<CompositeScene>();
    wholeScene->AddScene(scene, 50, 50);
    
    auto fpsScene = std::make_shared<FPSScene>();
    wholeScene->AddScene(fpsScene, 0, 0);
    
    auto processingScene = std::make_shared<ProcessingScene>(
        [&](const InputState& inputs, Uint32 timestamp)
        {
            if (inputs.GetButtonState(InputState::DOWN))
            {
                scene->MoveCharacter(rikka, InputState::DOWN, 4, 980);
            }
            
            if (inputs.GetButtonState(InputState::UP))
            {
                scene->MoveCharacter(rikka, InputState::UP, 4, 980);
            }

            if (inputs.GetButtonState(InputState::LEFT))
            {
                scene->MoveCharacter(rikka, InputState::LEFT, 4, 980);
            }

            if (inputs.GetButtonState(InputState::RIGHT))
            {
                scene->MoveCharacter(rikka, InputState::RIGHT, 4, 980);
            }

        },
        [&]() -> bool
        {
            return true;
        });
    
    wholeScene->AddScene(processingScene, 0, 0);

    run(wholeScene, context);
    
//    auto wholeScene = std::make_shared<CompositeScene>();
//
//    auto map = std::make_shared<TileMapScene>(tiles, 20,20);
//
//    auto charscene = std::make_shared<CharacterScene>(charset);
//    
//    for (int y=0; y<20; y++)
//    for (int x=0; x<20; x++)
//    {
//        map->Set(x, y, RandomBetween(21, 39));
//    }
//    
//    auto rikka = charscene->AddCharacter(5, 0, 48);
//    rikka->SetVariation(2);
//    rikka->SetAnimated(true);
//    
//    
//    auto togashi = charscene->AddCharacter(0, 48, 48);
//    togashi->SetVariation(2);
//    
//    wholeScene->AddScene(map, 50, 16);
//    wholeScene->AddScene(charscene, 50, 0);
//    
//    run(wholeScene, context);
}

int main(int argc, char** argv)
{
    std::map<int, InputState::Key> keyMap;

	keyMap[SDLK_UP] = InputState::UP;
	keyMap[SDLK_DOWN] = InputState::DOWN;
	keyMap[SDLK_LEFT] = InputState::LEFT;
	keyMap[SDLK_RIGHT] = InputState::RIGHT;
	keyMap[SDLK_a] = InputState::A;
	keyMap[SDLK_s] = InputState::B;
	keyMap[SDLK_z] = InputState::X;
	keyMap[SDLK_x] = InputState::Y;
	keyMap[SDLK_1] = InputState::L1;
	keyMap[SDLK_3] = InputState::L2;
	keyMap[SDLK_2] = InputState::R1;
	keyMap[SDLK_4] = InputState::R2;
	keyMap[SDLK_SPACE] = InputState::START;
    
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);
    
	// Something to note here: We don't initialize mp3 because there are too many
	// issues with it. Instead, simply make FLAC or OGG files. They are free
	// FLAC is better too because of its higher quality.
	Mix_Init(MIX_INIT_FLAC|MIX_INIT_MOD|MIX_INIT_OGG);
    

	int WIDTH = 960, HEIGHT = 640;
    
    if (argc == 2)
    {
        printlog("Configuration file provided: %s\n", argv[1]);
        LuaTable config = LoadLuaConfiguration(argv[1]);
    }
    
	//if (SDL_GetCurrentDisplayMode(0, &mode)==0)
	//{
	//	/* I read that android ignores these so you can just as well set
	//	them to 0 */
	//	WIDTH=mode.w;
	//	HEIGHT=mode.h;
	//}
    
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1)
	{
		printlog("Mix_OpenAudio: %s\n", Mix_GetError());
	}
    
	printlog("Window size: %d x %d!\n", WIDTH, HEIGHT);
    
	// Create an application window with the following settings:
	SDL_Window* window = SDL_CreateWindow(
                              "Aldebaran",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WIDTH,
                              HEIGHT,
                              SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL
                              );
    
	// Check that the window was successfully made
	if(window == NULL)
	{
		// In the event that the window could not be made...
		printlog("Could not create window: %s\n", SDL_GetError());
		return 1;
	}
	
	printlog("Beginning game with window=%p\n", window);
	try
	{
        /* Request opengl 3.2 context.
         * SDL doesn't have the ability to choose which profile at this time of writing,
         * but it should default to the core profile */
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        
        
        SDL_GLContext context = SDL_GL_CreateContext(window);
        
        
        printlog("OpenGL context created\n");
        
        glViewport(0, 0, WIDTH, HEIGHT);
        
        std::shared_ptr<GameContext> gameContext = std::make_shared<GameContext>(window, keyMap, WIDTH, HEIGHT);
        
		game(gameContext);
        
		SDL_GL_DeleteContext(context);
	}
	catch(std::exception e)
	{
		printlog("Caught a std::exception! %s\n", e.what());
        return EXIT_FAILURE;
	}
	catch(...)
	{
		printlog("Caught an unknown exception!\n");
        return EXIT_FAILURE;
	}
    
	SDL_DestroyWindow(window);
    
	// Clean up
	Mix_CloseAudio();
	Mix_Quit();
    
	TTF_Quit();
    
	SDL_Quit(); 
	
	printlog("Program Successfully Terminated!\n");
	
	return EXIT_SUCCESS;
}
