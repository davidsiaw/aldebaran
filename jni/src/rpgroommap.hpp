#ifndef RPGROOMMAP_HPP
#define RPGROOMMAP_HPP

#include <map>
#include <algorithm>

#include <boost/multi_array.hpp>
#include <boost/foreach.hpp>

#include "rpgmapinterface.hpp"

class RPGRoomMap : public RPGMapInterface
{
	boost::multi_array<RPGTile, 2> map;
	std::map<RPGEntityID, std::tr1::shared_ptr<RPGEntity>> entities;

	std::map<RPGEntityTileID, std::tr1::shared_ptr<RPGEntityTileInterface> > entityTiles;
	std::map<RPGTileID, std::tr1::shared_ptr<RPGTileInterface> > tiles;

public:
	RPGRoomMap(int width, int height) : map(boost::extents[width][height])
	{
		RPGTile tile;
		tile.id = 1;
		std::fill(map.data(), map.data() + map.num_elements(), tile);

		std::vector<RPGResourceID> resids;
		resids.push_back(1);
		tiles[1] = std::tr1::shared_ptr<RPGTileInterface>(new RPGSimpleTile(resids));
	}

	virtual RPGTile GetTile(TileCoord x, TileCoord y, int z) const
	{

		if (x < 0 || y < 0 || x >= map.shape()[0] || y >= map.shape()[1])
		{
			RPGTile tile;
			tile.id = NO_TILE;
			return tile;
		}
		return map[x][y];
	}

	virtual void ForeachEntity(PixelCoord x0, PixelCoord y0, PixelCoord x1, PixelCoord y1, int z, std::function<void(std::tr1::shared_ptr<RPGEntity>)> action) const
	{
		BOOST_FOREACH(auto ntt, entities)
		{
			if (ntt.second->x > x0 && ntt.second->x < x1 && ntt.second->y > y0 && ntt.second->y < y1 && ntt.second->z == z)
			{
				action(ntt.second);
			}
		};
	}

	virtual std::tr1::shared_ptr<RPGEntity> GetEntity(RPGEntityID id) const
	{
		auto item = entities.find(id);
		if (item == entities.end())
		{
			return std::tr1::shared_ptr<RPGEntity>();
		}
		return item->second;
	}

	virtual std::tr1::shared_ptr<RPGEntityTileInterface> GetEntityTile(RPGEntityTileID id) const
	{
		auto result = entityTiles.find(id);
		if (result != entityTiles.end() || id == NO_TILE)
		{
			return result->second;
		}
		return std::tr1::shared_ptr<RPGEntityTileInterface>();
	}

	virtual std::tr1::shared_ptr<RPGTileInterface> GetTile(RPGTileID id) const
	{
		auto result = tiles.find(id);
		if (result != tiles.end() && id != NO_TILE)
		{
			return result->second;
		}
		return std::tr1::shared_ptr<RPGTileInterface>();
	}
	
	virtual PixelCoord GetTileWidth() const
	{
		return 32;
	}

	virtual PixelCoord GetTileHeight() const
	{
		return 32;
	}

	virtual TileCoord GetTileCoord(PixelCoord pix) const
	{
		return pix >> 5;
	}

};

#endif // RPGROOMMAP_HPP
