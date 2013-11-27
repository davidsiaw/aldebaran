#ifndef RPGSCENE_HPP
#define RPGSCENE_HPP

#include "scene_interface.hpp"

#include <vector>
#include <string>
#include <tuple>
#include <SDL_ttf.h>
#include <boost/shared_array.hpp>
#include <boost/tr1/array.hpp>
#include <boost/tr1/memory.hpp>
#include <boost/tr1/functional.hpp>

#include "rpgresourceinterface.hpp"
#include "rpgtiletype.hpp"
#include "rpgsimpletile.hpp"
#include "rpgautotile.hpp"
#include "rpgentity.hpp"
#include "rpgtile.hpp"
#include "rpgentitytileinterface.hpp"
#include "rpgtileinterface.hpp"
#include "rpgmapcamerainterface.hpp"
#include "rpgmapinterface.hpp"

// Scene that displays a 2d rpg map
class RPGScene : public SceneInterface
{
	std::tr1::shared_ptr<RPGMapCameraInterface> camera;
	std::tr1::shared_ptr<RPGMapInterface> map;
	std::tr1::shared_ptr<RPGResourceInterface> resources;
	int tick;

	bool running;

public:
	RPGScene(
		std::tr1::shared_ptr<RPGMapCameraInterface> camera, 
		std::tr1::shared_ptr<RPGMapInterface> map, 
		std::tr1::shared_ptr<RPGResourceInterface> resources);
	virtual ~RPGScene();
	
	virtual void Init(SDL_Window* window, SDL_Renderer* renderer);
	virtual void Update(const InputState& inputs, Uint32 timestamp);
	virtual void Render(SDL_Renderer *renderer);
	virtual bool Running() const;
};

#endif // RPGSCENE_HPP
