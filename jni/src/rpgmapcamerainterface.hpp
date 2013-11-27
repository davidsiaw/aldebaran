#ifndef RPGMAPCAMERAINTERFACE_HPP
#define RPGMAPCAMERAINTERFACE_HPP

#include "rpgentity.hpp"
#include "rpgmapinterface.hpp"

class RPGMapCameraInterface
{
public:
	virtual TileCoord  GetColumns() const = 0;
	virtual TileCoord  GetRows() const = 0;
	virtual PixelCoord GetTop() const = 0;
	virtual PixelCoord GetLeft() const = 0;
};

#endif // RPGMAPCAMERAINTERFACE_HPP
