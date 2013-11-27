#ifndef RPGTILEINTERFACE_HPP
#define RPGTILEINTERFACE_HPP

#include "rpgtiletype.hpp"
#include "rpgresourceinterface.hpp"

class RPGTileInterface
{
public:
	virtual RPGTileType GetType()=0;
	virtual RPGResourceID GetBasicTile(unsigned char corner, unsigned char mask, int tick)=0;
};

#endif // RPGTILEINTERFACE_HPP
