#pragma once

#include "cEnemy.h"
#include "cArrow.h"
class cMonster2: public cEnemy
{
public:
	cMonster2();
	~cMonster2();

	bool Attack(std::list<cBicho*> &players,std::list<cProjectile*> &projectiles,bool *proj_collision_map);
	bool IA(bool *collision_map,bool *proj_collision_map,std::list<cBicho*> &enemies,cBicho* p1,cBicho* p2);
};