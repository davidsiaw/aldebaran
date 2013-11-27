#ifndef RPGSIMPLETILE_HPP
#define RPGSIMPLETILE_HPP

#include "rpgtileinterface.hpp"

class RPGSimpleTile : public RPGTileInterface
{
	std::vector<RPGResourceID> ids;
public:
	RPGSimpleTile(std::vector<RPGResourceID> ids) : ids(ids)
	{
	}

	virtual RPGTileType GetType()
	{
		return SIMPLE;
	}

	virtual RPGResourceID GetBasicTile(unsigned char corner, unsigned char mask, int tick)
	{
		return ids[tick % ids.size()];
	}
};

#endif // RPGSIMPLETILE_HPP
