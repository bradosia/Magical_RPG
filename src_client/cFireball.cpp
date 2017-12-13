#include "cFireball.h"

cFireball::cFireball()
{
	SetHitHeight(TILE_SIZE);
	SetHitWidth(TILE_SIZE);
	SetFrameTexTiles(2);
	SetDamage(10);
}
cFireball::~cFireball(){}

void cFireball::Draw(int tex_id,int tex_w,int tex_h,bool run)
{	
	float xo,yo,xf,yf;
	bool reverse=false;
	float tex_offset_x,tex_offset_y;

	tex_offset_x = 1.0f/float(tex_w/(GetFrameTexTiles()*TEXTURE_TILE_SIZE));
	tex_offset_y = 1.0f/float(tex_h/(GetFrameTexTiles()*TEXTURE_TILE_SIZE));

	switch(GetState())
	{
		case STATE_LEVITATEUP:		xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*1;
									if(run) NextFrame(3);
									break;
		case STATE_LEVITATERIGHT:	xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*2;
									if(run) NextFrame(3);
									break;
		case STATE_LEVITATEDOWN:	xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*0;
									if(run) NextFrame(3);
									break;
		case STATE_LEVITATELEFT:	xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*2;
									reverse = true;
									if(run) NextFrame(3);
									break;

		case STATE_ENDINGUP:		if(GetFrame() <=3) {xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*3;}
									else {xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*4;}
									if(run) NextFrame(6);
									break;
		case STATE_ENDINGRIGHT:		if(GetFrame() <=3) {xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*3;}
									else {xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*4;}
									if(run) NextFrame(6);
									break;
		case STATE_ENDINGDOWN:		if(GetFrame() <=3) {xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*3;}
									else {xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*4;}
									if(run) NextFrame(6);
									break;
		case STATE_ENDINGLEFT:		if(GetFrame() <=3) {xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*3;}
									else {xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*4;}
									if(run) NextFrame(6);
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