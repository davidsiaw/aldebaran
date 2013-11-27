#ifndef RPGENTITY_HPP
#define RPGENTITY_HPP

typedef unsigned int RPGEntityID;
typedef unsigned int RPGEntityTileID;
typedef int PixelCoord;

struct RPGEntity
{
	enum Action
	{
		STAND,
		WALK,
		/*PUSH,
		PULL,
		CAST,
		POKE,
		CLIMB,
		SIT,*/
	};

	enum Direction
	{
		DOWN,
		RIGHT,
		UP,
		LEFT
	};
	
	RPGEntityID id;
	RPGEntityTileID body;
	RPGEntityTileID eyes;
	RPGEntityTileID hair;
	RPGEntityTileID shirt;
	RPGEntityTileID shoes;
	RPGEntityTileID pants;
	PixelCoord x,y,z,dx,dy;

};

#endif // RPGENTITY_HPP
