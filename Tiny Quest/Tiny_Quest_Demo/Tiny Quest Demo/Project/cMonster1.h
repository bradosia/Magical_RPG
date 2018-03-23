#pragma once

#include "cEnemy.h"

class cMonster1: public cEnemy
{
public:
	cMonster1();
	~cMonster1();

	bool Attack(cBicho* player);
	bool IA(bool *collision_map,std::list<cBicho*> &enemies,cBicho* p1,cBicho* p2);
};