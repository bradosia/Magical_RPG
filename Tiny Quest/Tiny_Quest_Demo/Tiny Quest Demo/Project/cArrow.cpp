#include "cArrow.h"

cArrow::cArrow()
{
	SetHitHeight(TILE_SIZE/4);
	SetHitWidth(TILE_SIZE/4);
	SetFrameTexTiles(2);
	SetDamage(10);
}
cArrow::~cArrow(){}

void cArrow::Draw(int tex_id,int tex_w,int tex_h,bool run)
{	
	float xo,yo,xf,yf;
	bool reverse=false;
	float tex_offset_x,tex_offset_y;

	tex_offset_x = 1.0f/float(tex_w/(GetFrameTexTiles()*TEXTURE_TILE_SIZE));
	tex_offset_y = 1.0f/float(tex_h/(GetFrameTexTiles()*TEXTURE_TILE_SIZE));

	switch(GetState())
	{
		case STATE_LEVITATEUP:		xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*0;
									break;
		case STATE_LEVITATERIGHT:	xo = tex_offset_x*2 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*0;
									break;
		case STATE_LEVITATEDOWN:	xo = tex_offset_x*1 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*0;
									break;
		case STATE_LEVITATELEFT:	xo = tex_offset_x*2 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*0;
									reverse = true;
									break;

		case STATE_ENDINGUP:		xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*0;
									NextFrame(1);
									break;
		case STATE_ENDINGRIGHT:		xo = tex_offset_x*2 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*0;
									NextFrame(1);
									break;
		case STATE_ENDINGDOWN:		xo = tex_offset_x*1 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*0;
									NextFrame(1);
									break;
		case STATE_ENDINGLEFT:		xo = tex_offset_x*2 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*0;
									NextFrame(1);
									reverse = true;
									break;
	}
	xf = xo + tex_offset_x;
	yf = yo - tex_offset_y;
	if(reverse) 
	{
		float aux = xo;
		xo = xf;  xf = aux;
	}
	DrawRect(tex_id,xo,yo,xf,yf);
}