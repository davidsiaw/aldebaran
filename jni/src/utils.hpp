#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <luacppinterface.h>

std::string LoadAllText(std::string file);
LuaTable LoadLuaConfiguration(std::string file);
void Init3D(int screenWidth, int screenHeight);

std::shared_ptr<SDL_Surface> MakeSurface(Uint16 width, Uint16 height);
std::shared_ptr<TTF_Font> MakeFont(const char *file, int ptsize);
std::shared_ptr<SDL_Surface> LoadImage(std::string filename);
int RandomBetween(int min, int max);

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

#endif //UTILS_HPP
