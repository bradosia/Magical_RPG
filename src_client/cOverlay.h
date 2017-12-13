#pragma once

#include "cScene.h"

class cOverlay
{
public:
	cOverlay();
	cOverlay(bool p1);
	~cOverlay();

	void SetX(int x);
	int  GetX();
	void SetY(int y);
	int  GetY();
	int  GetWidth();
	void SetBarOffsetX(int ox);
	void Draw(int tex_id);
	void DrawBars(int max1,int act1,int max2,int act2);

private:
	int x,y;
	int tiles_w,tiles_h;
	int bar_offset_x;
};