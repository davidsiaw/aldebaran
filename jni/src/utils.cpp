#include "utils.hpp"

#include <set>
#include <SDL.h>
#include <SDL_image.h>
#include "printlog.hpp"
#include "opengl.hpp"

std::string LoadAllText(std::string file)
{
	SDL_RWops *rw = SDL_RWFromFile(file.c_str(), "r");

	auto size = rw->size(rw);
	char* script = new char[size + 1];
	memset(script, 0, size + 1);
	rw->read(rw, script, sizeof(char), size);
	rw->close(rw);

	std::string result = script;
	delete[] script;

	return result;
}

LuaTable LoadLuaConfiguration(std::string file)
{
	std::string script = LoadAllText(file);
	Lua lua;
	auto global = lua.GetGlobalEnvironment();

	std::set<std::string> includedFiles;
	
	auto includeonce = lua.CreateFunction<void(std::string)>([&](std::string includedFile)
	{
		if (includedFiles.find(includedFile) == includedFiles.end())
		{
			includedFiles.insert(includedFile);
			lua.RunScript(LoadAllText(includedFile));
		}
	});

	auto include = lua.CreateFunction<void(std::string)>([&](std::string includedFile)
	{
		includedFiles.insert(includedFile);
		lua.RunScript(LoadAllText(includedFile));
	});

	global.Set("include", include);
	global.Set("include_once", includeonce);

	lua.RunScript(script);
	return global;
}


void InitOpenGL()
{
    
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,8);
    
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
    
    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,8);
    
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,2);
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LESS);								// The Type Of Depth Testing To Do
    
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
}

std::shared_ptr<SDL_Surface> MakeSurface(Uint16 width, Uint16 height)
{
    
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    Uint32 rmask = 0xff000000;
    Uint32 gmask = 0x00ff0000;
    Uint32 bmask = 0x0000ff00;
    Uint32 amask = 0x000000ff;
#else
    Uint32 rmask = 0x000000ff;
    Uint32 gmask = 0x0000ff00;
    Uint32 bmask = 0x00ff0000;
    Uint32 amask = 0xff000000;
#endif
    
    std::shared_ptr<SDL_Surface> surface(SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask), SDL_FreeSurface);
    
    return surface;
}

std::shared_ptr<TTF_Font> MakeFont(const char *file, int ptsize)
{
    return std::shared_ptr<TTF_Font> (TTF_OpenFont(file, ptsize), TTF_CloseFont);
}

std::shared_ptr<SDL_Surface> LoadImage(std::string filename)
{
    std::shared_ptr<SDL_Surface> tex(IMG_Load(filename.c_str()), SDL_FreeSurface);
    if (!tex)
    {
        printlog("%s failed to load: %s \n", filename.c_str(), IMG_GetError());
    }
    return tex;
}
