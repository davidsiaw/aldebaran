#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <luacppinterface.h>

std::string LoadAllText(std::string file);
LuaTable LoadLuaConfiguration(std::string file);

#endif //UTILS_HPP
