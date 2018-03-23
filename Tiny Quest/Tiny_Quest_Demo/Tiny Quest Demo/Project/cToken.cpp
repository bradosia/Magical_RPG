#include "cToken.h"

cToken::cToken()
{
	seq = 0;
	delay = 0;
	frame_delay = 3;
	height_overground = TILE_SIZE/4;
}
cToken::~cToken(){}

void cToken::SetTile(int tx,int ty)
{
	x = tx * TILE_SIZE;
	y = ty * TILE_SIZE;
}
int cToken::GetX()
{
	return x;
}
int cToken::GetY()
{
	return y;
}
cRect cToken::GetHitBox()
{
	cRect hitbox;
	hitbox.top = y + TILE_SIZE;
	hitbox.bottom = y;
	hitbox.left = x;
	hitbox.right = x + TILE_SIZE;
	return hitbox;
}

void cToken::Draw(int tex_id,int tex_w,int tex_h,bool run)
{	
	float xo,yo,xf,yf;
	float tex_offset_x,tex_offset_y;

	tex_offset_x = 1.0f/float(tex_w/TEXTURE_TILE_SIZE);
	tex_offset_y = 1.0f/float(tex_h/TEXTURE_TILE_SIZE);

	xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*0;
	if(run) NextFrame(4); //total de frames de la animacion

	xf = xo + tex_offset_x;
	yf = yo - tex_offset_y;

	int screen_x,screen_y;

	screen_x = x + SCENE_Xo;
	screen_y = y + SCENE_Yo;
	
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glBegin(GL_QUADS);
		glTexCoord2f(xo,yo);	glVertex3i(screen_x			   ,screen_y			,y-height_overground);
		glTexCoord2f(xf,yo);	glVertex3i(screen_x + TILE_SIZE,screen_y			,y-height_overground);
		glTexCoord2f(xf,yf);	glVertex3i(screen_x + TILE_SIZE,screen_y + TILE_SIZE,y-height_overground);
		glTexCoord2f(xo,yf);	glVertex3i(screen_x			   ,screen_y + TILE_SIZE,y-height_overground);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}
void cToken::NextFrame(int max)
{
	delay++;
	if(delay == frame_delay)
	{
		seq++;
		seq%=max;
		delay = 0;
	}
}
int cToken::GetFrame()
{
	return seq;
}