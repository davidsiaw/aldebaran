#ifndef RPGAUTOTILE_HPP
#define RPGAUTOTILE_HPP

#include <vector>
#include <SDL.h>
#include <boost/tr1/array.hpp>
#include <boost/tr1/memory.hpp>
#include <boost/shared_array.hpp>

#include "rpgresourceinterface.hpp"
#include "rpgtiletype.hpp"
#include "rpgsimpletile.hpp"

class RPGAutoTile : public RPGTileInterface
{
public:
	struct TileCorner
	{
		int sides[8];
	};

	RPGAutoTile(boost::shared_array<RPGSimpleTile> rawtiles, std::tr1::array<TileCorner, 4> posAndMaskToTile, RPGTileType type) : 
	rawtiles(rawtiles),
	posAndMaskToTile(posAndMaskToTile),
	type(type)
	{
	}

	virtual RPGTileType GetType()
	{
		return type;
	}

	virtual RPGResourceID GetBasicTile(unsigned char corner, unsigned char mask, int tick)
	{
		return rawtiles[posAndMaskToTile[corner].sides[mask]].GetBasicTile(corner, mask, tick);
	}

	static const std::tr1::array<std::function<unsigned char(unsigned char)>, 4> CornerMaskFuncs;

private:
	boost::shared_array<RPGSimpleTile> rawtiles;
	std::tr1::array<TileCorner, 4> posAndMaskToTile;
	RPGTileType type;

};


#endif // RPGAUTOTILE_HPP
