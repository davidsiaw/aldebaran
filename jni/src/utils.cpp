#include "utils.hpp"

#include <set>
#include <SDL.h>

std::string LoadAllText(std::string file)
{
	SDL_RWops *rw = SDL_RWFromFile(file.c_str(), "r");

	auto size = rw->size(rw);
	char* script = new char[size + 1];
	memset(script, 0, size + 1);
	rw->read(rw, script, sizeof(char), size);
	rw->close(rw);
	SDL_FreeRW(rw);

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
