#include "cEnemy.h"

cEnemy::cEnemy()
{
	frame_walk_delay = 3;
	frame_attack_delay = 3;
	frame_dying_delay = 2;
	step_length = STEP_LENGTH;
}
cEnemy::~cEnemy(){}

void cEnemy::Draw(int tex_id,int tex_w,int tex_h,bool run)
{	
	float xo,yo,xf,yf;
	bool reverse=false;
	float tex_offset_x,tex_offset_y;

	tex_offset_x = 1.0f/float(tex_w/(frame_texture_tiles*TEXTURE_TILE_SIZE));
	tex_offset_y = 1.0f/float(tex_h/(frame_texture_tiles*TEXTURE_TILE_SIZE));

	switch(GetState())
	{
		case STATE_LOOKUP:		xo = tex_offset_x*5;   yo = tex_offset_y + tex_offset_y*1;
								break;
		case STATE_LOOKRIGHT:	xo = tex_offset_x*5;   yo = tex_offset_y + tex_offset_y*2;
								break;
		case STATE_LOOKDOWN:	xo = tex_offset_x*5;   yo = tex_offset_y + tex_offset_y*0;
								break;
		case STATE_LOOKLEFT:	xo = tex_offset_x*5;   yo = tex_offset_y + tex_offset_y*2;
								reverse = true;
								break;

		case STATE_WALKUP:		xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*1;
								if(run) NextFrame(6);
								break;
		case STATE_WALKRIGHT:	xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*2;
								if(run) NextFrame(6);
								break;
		case STATE_WALKDOWN:	xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*0;
								if(run) NextFrame(6);
								break;
		case STATE_WALKLEFT:	xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*2;
								if(run) NextFrame(6);
								reverse = true;
								break;

		case STATE_DAMAGEUP:	xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*4;
								if(run) NextFrame(6);
								break;
		case STATE_DAMAGERIGHT:	xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*5;
								if(run) NextFrame(6);
								break;
		case STATE_DAMAGEDOWN:	xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*3;
								if(run) NextFrame(6);
								break;
		case STATE_DAMAGELEFT:	xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*5;
								if(run) NextFrame(6);
								reverse = true;
								break;

		case STATE_ATTACKUP:	xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*7;
								if(run) NextFrame(10);
								break;
		case STATE_ATTACKRIGHT:	xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*8;
								if(run) NextFrame(10);
								break;
		case STATE_ATTACKDOWN:	xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*6;
								if(run) NextFrame(10);
								break;
		case STATE_ATTACKLEFT:	xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*8;
								if(run) NextFrame(10);
								reverse = true;
								break;

		case STATE_DYING:		xo = tex_offset_x*0 + (GetFrame()*tex_offset_x);   yo = tex_offset_y + tex_offset_y*9;
								if(run) NextFrame(10);
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
void cEnemy::DrawRect(int tex_id,float xo,float yo,float xf,float yf)
{
	int screen_x,screen_y;
	cRect hitbox = GetHitBox();
	int hitbox_w,hitbox_h;

	screen_x = GetX() + SCENE_Xo;
	screen_y = GetY() + SCENE_Yo;
	hitbox_w = hitbox.right - hitbox.left;
	hitbox_h = hitbox.top - hitbox.bottom;

	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glBegin(GL_QUADS);
		glTexCoord2f(xo,yo);	glVertex3i(screen_x - (TILE_SIZE*frame_texture_tiles - hitbox_w)/2,screen_y - (TILE_SIZE*frame_texture_tiles - hitbox_h)/2,screen_y);
		glTexCoord2f(xf,yo);	glVertex3i(screen_x + (TILE_SIZE*frame_texture_tiles + hitbox_w)/2,screen_y - (TILE_SIZE*frame_texture_tiles - hitbox_h)/2,screen_y);
		glTexCoord2f(xf,yf);	glVertex3i(screen_x + (TILE_SIZE*frame_texture_tiles + hitbox_w)/2,screen_y + (TILE_SIZE*frame_texture_tiles + hitbox_h)/2,screen_y);
		glTexCoord2f(xo,yf);	glVertex3i(screen_x - (TILE_SIZE*frame_texture_tiles - hitbox_w)/2,screen_y + (TILE_SIZE*frame_texture_tiles + hitbox_h)/2,screen_y);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}
void cEnemy::NextFrame(int max)
{
	SetDelay(GetDelay()+1);
	if(GetState() == STATE_ATTACKUP || GetState() == STATE_ATTACKRIGHT || GetState() == STATE_ATTACKDOWN || GetState() == STATE_ATTACKLEFT)
	{
		if(GetDelay() == frame_attack_delay)
		{
			SetFrame(GetFrame()+1);
			SetFrame(GetFrame()%max);
			SetDelay(0);
		}
	}
	else if(GetState() == STATE_DYING)
	{
		if(GetDelay() == frame_dying_delay)
		{
			SetFrame(GetFrame()+1);
			SetFrame(GetFrame()%max);
			SetDelay(0);
		}
	}
	{
		if(GetDelay() == frame_walk_delay)
		{
			SetFrame(GetFrame()+1);
			SetFrame(GetFrame()%max);
			SetDelay(0);
		}
	}
}

int cEnemy::GetFrameTop()
{
	return GetY() + GetHitHeight() + (frame_texture_tiles*TILE_SIZE - GetHitHeight())/2;
}
int cEnemy::GetFrameBottom()
{
	return GetY() - (frame_texture_tiles*TILE_SIZE - GetHitHeight())/2;
}
void cEnemy::SetFrameTexTiles(int ftt)
{
	frame_texture_tiles= ftt;
}

void cEnemy::Move()
{
	if(GetState() == STATE_WALKUP)		   SetY(GetY() + step_length);
	else if(GetState() == STATE_WALKDOWN)  SetY(GetY() - step_length);
	else if(GetState() == STATE_WALKRIGHT) SetX(GetX() + step_length);
	else if(GetState() == STATE_WALKLEFT)  SetX(GetX() - step_length);
}