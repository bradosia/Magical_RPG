#pragma once

#include "cPlayer.h"
#include "cFireball.h"
class cPlayer2: public cPlayer
{
public:
	cPlayer2();
	~cPlayer2();

	void Draw(int tex_id,int tex_w,int tex_h,bool run);

	bool Attack(std::list<cBicho*> &enemies,std::list<cProjectile*> &projectiles,bool *proj_collision_map);

private:
};