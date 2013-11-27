#include "rpgscene.hpp"

#include "rpgautotile.hpp"
#include <boost/foreach.hpp>

RPGScene::RPGScene(
		std::tr1::shared_ptr<RPGMapCameraInterface> camera,
		std::tr1::shared_ptr<RPGMapInterface> map, 
		std::tr1::shared_ptr<RPGResourceInterface> resources) :
	running(true),
	camera(camera),
	map(map),
	resources(resources),
	tick(0)
{

}

RPGScene::~RPGScene()
{

}

void RPGScene::Init(SDL_Window* window, SDL_Renderer* renderer)
{

}

void RPGScene::Update(const InputState& inputs, Uint32 timestamp)
{
	tick = timestamp / 200;
}

SDL_Rect Center(SDL_Rect* tile, SDL_Rect* sprite)
{
	SDL_Rect rect;
	rect.w = sprite->w;
	rect.h = sprite->h;

	rect.x = tile->x - (tile->w - sprite->w) / 2;
	rect.y = tile->y - (tile->h - sprite->h);

	return rect;
}

void RPGScene::Render(SDL_Renderer *renderer)
{
	unsigned int x0 = camera->GetLeft();
	unsigned int y0 = camera->GetTop();
	
	for (int z=0; z<2; z++)
	{
		for (TileCoord y=0; y<camera->GetRows(); y++)
		{
			for (TileCoord x=0; x<camera->GetColumns(); x++)
			{
				auto tile = map->GetTile(x0 + x, y0 + y, z);

				unsigned char surround = (
					((map->GetTile(x+1, y-1,z).id == tile.id?1:0) << 0) |
					((map->GetTile(x+1, y+1,z).id == tile.id?1:0) << 1) |
					((map->GetTile(x-1, y+1,z).id == tile.id?1:0) << 2) |
					((map->GetTile(x-1, y-1,z).id == tile.id?1:0) << 3) |
					((map->GetTile(x  , y-1,z).id == tile.id?1:0) << 4) |
					((map->GetTile(x+1, y  ,z).id == tile.id?1:0) << 5) |
					((map->GetTile(x  , y+1,z).id == tile.id?1:0) << 6) |
					((map->GetTile(x-1, y  ,z).id == tile.id?1:0) << 7)
				);

				auto tilePtr = map->GetTile(tile.id);
				if (tilePtr)
				{
					for (int i=0; i<4; i++)
					{
						auto image = tilePtr->GetBasicTile(i, RPGAutoTile::CornerMaskFuncs[i](surround), tick);
						auto res = resources->GetImage(image);

						bool mx = (i >> 1) & 1;
						bool my = i & 1;

						SDL_Rect rect;
						rect.w = map->GetTileWidth() / 2;
						rect.h = map->GetTileHeight() / 2;

						int dx = (!mx) * rect.w;
						int dy = (mx ^ my) * rect.h;

						rect.x = x * map->GetTileWidth() + dx - camera->GetLeft();
						rect.y = y * map->GetTileHeight() + dy - camera->GetTop();

						SDL_Rect srcRect = rect;
						srcRect.x = res.second.x + dx;
						srcRect.y = res.second.y + dy;

						SDL_RenderCopy(renderer, res.first.get(), &srcRect, &rect);
					}
				}
			}
		}

		std::vector<std::tr1::shared_ptr<RPGEntity>> entities;

		map->ForeachEntity(x0, y0, x0 + camera->GetColumns(), y0, z,
		[&](std::tr1::shared_ptr<RPGEntity> ntt)
		{
			entities.push_back(ntt);
		});

		std::sort(entities.begin(), entities.end(), [](std::tr1::shared_ptr<RPGEntity> x, std::tr1::shared_ptr<RPGEntity> y) { return x->y < y->y; });

		BOOST_FOREACH(auto ntt, entities)
		{
			auto body = resources->GetImage(ntt->body);
			auto eyes = resources->GetImage(ntt->eyes);
			auto hair = resources->GetImage(ntt->hair);
			auto pants = resources->GetImage(ntt->pants);
			auto shoes = resources->GetImage(ntt->shoes);
			auto shirt = resources->GetImage(ntt->shirt);
				
			SDL_Rect rect;
			rect.w = map->GetTileWidth();
			rect.h = map->GetTileHeight();
			rect.x = (ntt->x) - camera->GetLeft();
			rect.y = (ntt->y) - camera->GetTop();

			SDL_Rect bodyrect = Center(&rect, &body.second);
			SDL_RenderCopy(renderer, body.first.get(), &body.second, &bodyrect);

			SDL_Rect eyesrect = Center(&rect, &eyes.second);
			SDL_RenderCopy(renderer, eyes.first.get(), &eyes.second, &eyesrect);

			SDL_Rect hairrect = Center(&rect, &hair.second);
			SDL_RenderCopy(renderer, hair.first.get(), &hair.second, &hairrect);

			SDL_Rect pantsrect = Center(&rect, &pants.second);
			SDL_RenderCopy(renderer, pants.first.get(), &pants.second, &pantsrect);

			SDL_Rect shoesrect = Center(&rect, &shoes.second);
			SDL_RenderCopy(renderer, shoes.first.get(), &shoes.second, &shoesrect);

			SDL_Rect shirtrect = Center(&rect, &shirt.second);
			SDL_RenderCopy(renderer, shirt.first.get(), &shirt.second, &shirtrect);
		}
	}
}

bool RPGScene::Running() const
{
	return running;
}
