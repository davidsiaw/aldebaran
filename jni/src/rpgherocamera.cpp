#include "rpgherocamera.hpp"

RPGHeroCamera::RPGHeroCamera(
	int columns,
	int rows, 
	std::tr1::shared_ptr<RPGMapInterface> map,
	RPGEntityID hero) :
columns(columns),
rows(rows),
map(map),
hero(map->GetEntity(hero))
{
}

TileCoord RPGHeroCamera::GetColumns() const
{
	return columns;
}

TileCoord RPGHeroCamera::GetRows() const
{
	return rows;
}

PixelCoord RPGHeroCamera::GetTop() const
{
	return hero->y;
}

PixelCoord RPGHeroCamera::GetLeft() const
{
	return hero->x;
}
