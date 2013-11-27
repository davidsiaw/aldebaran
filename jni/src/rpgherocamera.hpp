#ifndef RPGHEROCAMERA_HPP
#define RPGHEROCAMERA_HPP

#include "rpgentity.hpp"
#include "rpgmapinterface.hpp"
#include "rpgmapcamerainterface.hpp"

#include <boost/tr1/memory.hpp>

class RPGHeroCamera : public RPGMapCameraInterface
{
	int columns;
	int rows;
	std::tr1::shared_ptr<RPGMapInterface> map;
	std::tr1::shared_ptr<RPGEntity> hero;
public:
	RPGHeroCamera(
		int columns,
		int rows, 
		std::tr1::shared_ptr<RPGMapInterface> map,
		RPGEntityID hero);

	virtual TileCoord GetColumns() const;
	virtual TileCoord GetRows() const;
	virtual PixelCoord GetTop() const;
	virtual PixelCoord GetLeft() const;
};

#endif // RPGHEROCAMERA_HPP
