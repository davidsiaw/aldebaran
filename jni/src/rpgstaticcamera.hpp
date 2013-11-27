#ifndef RPGSTATICCAMERA_HPP
#define RPGSTATICCAMERA_HPP

#include "rpgmapcamerainterface.hpp"

#include <boost/tr1/memory.hpp>

class RPGStaticCamera : public RPGMapCameraInterface
{
	int columns;
	int rows;
	int top;
	int left;
public:
	RPGStaticCamera(
		int columns,
		int rows, 
		int top,
		int left) : 
	columns(columns),
	rows(rows),
	top(top),
	left(left)
	{}

	virtual TileCoord GetColumns() const { return columns; }
	virtual TileCoord GetRows() const { return rows; }
	virtual PixelCoord GetTop() const { return top; }
	virtual PixelCoord GetLeft() const { return left; }
};



#endif // RPGSTATICCAMERA_HPP
