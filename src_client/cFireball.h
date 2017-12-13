#pragma once

#include "cProjectile.h"

class cFireball: public cProjectile
{
public:
	cFireball();
	~cFireball();

	void Draw(int tex_id,int tex_w,int tex_h,bool run);
};