#ifndef TESTSCENEMAP_HPP
#define TESTSCENEMAP_HPP

#include <boost/tr1/memory.hpp>
#include <boost/tr1/functional.hpp>
#include <boost/multi_array.hpp>
#include <boost/foreach.hpp>
#include <map>

#define amountPerTile 8

class TestSceneMap
{	
	struct Entity
	{
		int id;
		int dx;	// speed
		int dy;
		int x;	// tile position
		int y;
		int mx;	// sub-tile position
		int my;
		bool active; // is on the map?
		bool brakes; // brake on tile?
		int userdata;
	};
public:
	typedef std::tr1::shared_ptr<Entity> entity_type;

private:
	typedef boost::multi_array<entity_type, 2> map_type;
	typedef std::map<int, std::tr1::weak_ptr<Entity> > registry_type;

	int ids;
	map_type map;
	registry_type reg;

public:

	enum PositionCheck
	{
		FREE,
		OCCUPIED,
		BOUNDARY
	};
	
	TestSceneMap(int w, int h);

	~TestSceneMap();

	entity_type CreateEntity();
	entity_type GetEntity(int x, int y);
	entity_type GetEntity(int id);
	bool TrySetEntity(const entity_type &ntt, int x, int y);
	entity_type RemoveEntity(int x, int y);
	PositionCheck CheckPosIsFree(int x, int y);
	void Update(int timeSteps);
	void ForeachTile(std::tr1::function<void(int,int,const std::tr1::shared_ptr<Entity> &)> func);
	void ForeachEntity(std::tr1::function<void(const std::tr1::shared_ptr<Entity> &)> func);
};

#endif // TESTSCENEMAP_HPP
