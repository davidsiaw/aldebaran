#include "rpgautotile.hpp"

const std::tr1::array<RPGAutoTile::TileCorner, 4> AutoTile12Map = 
{
	{
		{{10,  0,  1,  2,  6,  7,  9,  8}},
		{{10,  2,  3,  4,  0,  1,  9,  8}},
		{{10,  4,  5,  6,  2,  3,  9,  8}},
		{{10,  6,  7,  0,  4,  5,  9,  8}}
	}
};

const std::tr1::array<RPGAutoTile::TileCorner, 4> AutoTile94Map = 
{
	{
		{{ 0,  0,  1,  2,  6,  7,  9,  8}},
		{{ 2,  2,  3,  4,  0,  1, 10,  8}},
		{{ 4,  4,  5,  6,  2,  3, 11,  8}},
		{{ 6,  6,  7,  0,  4,  5, 12,  8}}
	}
};

unsigned char TopRightCornerMask(unsigned char surrounding)
{
	return (((surrounding >> 3) & 6) | (surrounding & 1));
}

unsigned char BottomRightCornerMask(unsigned char surrounding)
{
	return (((surrounding >> 4) & 6) | ((surrounding >> 1) & 1));
}

unsigned char BottomLeftCornerMask(unsigned char surrounding)
{
	return (((surrounding >> 5) & 6) | ((surrounding >> 2) & 1));
}

unsigned char TopLeftCornerMask(unsigned char surrounding)
{
	return (((surrounding >> 3) & 1) | ((surrounding >> 2) & 4) | ((surrounding >> 6) & 2));
}

const std::tr1::array<std::function<unsigned char(unsigned char)>, 4> RPGAutoTile::CornerMaskFuncs = 
{
	TopRightCornerMask,
	BottomRightCornerMask,
	BottomLeftCornerMask,
	TopLeftCornerMask,
};
