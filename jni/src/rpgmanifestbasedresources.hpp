#ifndef RPGMANIFESTBASEDRESOURCES_HPP
#define RPGMANIFESTBASEDRESOURCES_HPP

#include <map>
#include <luacppinterface.h>
#include <SDL_image.h>

#include "rpgresourceinterface.hpp"
#include "utils.hpp"

class RPGManifestBasedResources : public RPGResourceInterface
{
	std::map<std::string, std::tr1::shared_ptr<SDL_Texture> > texturesByName;
	std::map<RPGResourceID, std::pair<std::tr1::shared_ptr<SDL_Texture>, SDL_Rect> > imageResources;

public:
	RPGManifestBasedResources(std::string manifest, SDL_Renderer* renderer)
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
					texturesByName[path] = std::tr1::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer, path.c_str()), SDL_DestroyTexture);
					texture = texturesByName.find(path);
				}

				imageResources[id] = std::pair<std::tr1::shared_ptr<SDL_Texture>, SDL_Rect> (texture->second, rect);
			}
		});
	}

	virtual std::pair<std::tr1::shared_ptr<SDL_Texture>, SDL_Rect> GetImage(RPGResourceID num) const
	{
		return imageResources.find(num)->second;
	}
};

#endif // RPGMANIFESTBASEDRESOURCES_HPP
