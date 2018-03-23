#pragma once

#include "cProjectile.h"

class cArrow: public cProjectile
{
public:
	cArrow();
	~cArrow();

	void Draw(int tex_id,int tex_w,int tex_h,bool run);
};