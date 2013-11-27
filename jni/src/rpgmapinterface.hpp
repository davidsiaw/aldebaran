#ifndef RPGMAPINTERFACE_HPP
#define RPGMAPINTERFACE_HPP

#include <boost/tr1/memory.hpp>
#include <boost/tr1/functional.hpp>

#include "rpgtile.hpp"
#include "rpgtileinterface.hpp"
#include "rpgentitytileinterface.hpp"

typedef unsigned int TileCoord;

class RPGMapInterface
{
public:
	virtual RPGTile GetTile(TileCoord x, TileCoord y, int z) const = 0;
	virtual void ForeachEntity(PixelCoord x0, PixelCoord y0, PixelCoord x1, PixelCoord y1, int z, std::function<void(std::tr1::shared_ptr<RPGEntity>)> action) const = 0;
	virtual std::tr1::shared_ptr<RPGEntity> GetEntity(RPGEntityID id) const = 0;
	virtual std::tr1::shared_ptr<RPGEntityTileInterface> GetEntityTile(RPGEntityTileID id) const = 0;
	virtual std::tr1::shared_ptr<RPGTileInterface> GetTile(RPGTileID id) const = 0;
	virtual PixelCoord GetTileWidth() const = 0;
	virtual PixelCoord GetTileHeight() const = 0;
	virtual TileCoord GetTileCoord(PixelCoord pix) const = 0;
};

#endif // RPGMAPINTERFACE_HPP
