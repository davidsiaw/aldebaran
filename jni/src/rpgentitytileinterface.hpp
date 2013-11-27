#ifndef RPGENTITYTILEINTERFACE_HPP
#define RPGENTITYTILEINTERFACE_HPP

class RPGEntityTileInterface
{
public:
	virtual RPGResourceID GetBasicTile(RPGEntity::Action action, RPGEntity::Direction direction, int tick)=0;
};

#endif // RPGENTITYTILEINTERFACE_HPP
