#include "cProjectile.h"

cProjectile::cProjectile()
{
	seq = 0;
	delay = 0;
	frame_delay = 3;
	step_length = STEP_LENGTH*4;
	displace_up=false; displace_down=false;
}
cProjectile::~cProjectile(){}

void cProjectile::SetPosition(int posx,int posy)
{
	x = posx;
	y = posy;
}
int cProjectile::GetY()
{
	return y;
}
int cProjectile::GetX()
{
	return x;
}
void cProjectile::SetHitHeight(int hh)
{
	hit_h = hh;
}
int cProjectile::GetHitHeight()
{
	return hit_h;
}
void cProjectile::SetHitWidth(int hw)
{
	hit_w = hw;
}
int cProjectile::GetHitWidth()
{
	return hit_w;
}
void cProjectile::SetFrameTexTiles(int ftt)
{
	frame_texture_tiles = ftt;
}
int cProjectile::GetFrameTexTiles()
{
	return frame_texture_tiles;
}
cRect cProjectile::GetFrameBox()
{
	cRect framebox;
	framebox.top = y + hit_h + (frame_texture_tiles*TILE_SIZE - hit_h)/2;
	framebox.bottom = y - (frame_texture_tiles*TILE_SIZE - hit_h)/2;
	framebox.left =  x - (frame_texture_tiles*TILE_SIZE - hit_w)/2;
	framebox.right = x  + hit_w + (frame_texture_tiles*TILE_SIZE - hit_h)/2;
	return framebox;
}
cRect cProjectile::GetHitBox()
{
	cRect hitbox;
	hitbox.top = y + hit_h;
	hitbox.bottom = y;
	hitbox.left = x;
	hitbox.right = x + hit_w;
	return hitbox;
}
bool cProjectile::Intersection(cRect box1, cRect box2)
{
	if(box1.top > box2.bottom && box2.top > box1.bottom)
	{
		if(box1.left < box2.right && box2.left < box1.right) return true;
	}
	return false;
}

bool cProjectile::CollidesObjUD(bool *proj_collision_map,bool up)
{	
	int tile_x,tile_y;
	int width_tiles;

	tile_x = x / TILE_SIZE;
	if(up && ((y + hit_h) / TILE_SIZE) < SCENE_HEIGHT) tile_y = (y + hit_h) / TILE_SIZE;
	else tile_y = y / TILE_SIZE;

	width_tiles = 1 + ((hit_w - 1) / TILE_SIZE); //redondeo hacia arriba
	if( (x%TILE_SIZE + hit_w) > TILE_SIZE ) width_tiles++;

	for(int j=0;j<width_tiles;j++)
	{
		if(proj_collision_map[ (tile_x+j) + tile_y*SCENE_WIDTH ]) return true;
	}
	return false;
}

bool cProjectile::CollidesObjLR(bool *proj_collision_map,bool right)
{
	int tile_x,tile_y;
	int height_tiles;

	if(right && ((x + hit_w) / TILE_SIZE) < SCENE_WIDTH) tile_x = (x + hit_w) / TILE_SIZE;
	else tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;

	height_tiles = 1 + ((hit_h - 1) / TILE_SIZE);  //redondeo hacia arriba
	if( (y%TILE_SIZE + hit_h) > TILE_SIZE ) height_tiles++;

	for(int j=0;j<height_tiles;j++)
	{
		if(proj_collision_map[ tile_x + (tile_y+j)*SCENE_WIDTH ]) return true;
	}
	return false;
}

void cProjectile::LevitateUp(bool *proj_collision_map)
{
	//Whats next tile?
	if( (y % TILE_SIZE) == 0)
	{
		int yaux = y;
		y += step_length;
		if(CollidesObjUD(proj_collision_map,true))
		{
			y = yaux;
			state = STATE_ENDINGUP; //si choca contra algun objeto explotara
			seq = 0;
			delay = 0;
		}
	}
	//Advance, no problem
	else y += step_length;
}
void cProjectile::LevitateDown(bool *proj_collision_map)
{
	//Whats next tile?
	if( (y % TILE_SIZE) == 0)
	{
		int yaux = y;
		y -= step_length;
		if(CollidesObjUD(proj_collision_map,false))
		{
			y = yaux;
			state = STATE_ENDINGDOWN; //si choca contra algun objeto explotara
			seq = 0;
			delay = 0;
		}
	}
	//Advance, no problem
	else y -= step_length;
}
void cProjectile::LevitateLeft(bool *proj_collision_map)
{
	//Whats next tile?
	if( (x % TILE_SIZE) == 0)
	{
		int xaux = x;
		x -= step_length;
		if(CollidesObjLR(proj_collision_map,false))
		{
			x = xaux;
			state = STATE_ENDINGLEFT; //si choca contra algun objeto explotara
			seq = 0;
			delay = 0;
		}
	}
	//Advance, no problem
	else x -= step_length;
}
void cProjectile::LevitateRight(bool *proj_collision_map)
{
	//Whats next tile?
	if( (x % TILE_SIZE) == 0)
	{
		int xaux = x;
		x += step_length;
		if(CollidesObjLR(proj_collision_map,true))
		{
			x = xaux;
			state = STATE_ENDINGRIGHT; //si choca contra algun objeto explotara
			seq = 0;
			delay = 0;
		}
	}
	//Advance, no problem
	else x += step_length;
}

bool cProjectile::ComputeEnemyCollisions(std::list<cBicho*> &enemies)
{
	bool collision = false;
	std::list<cBicho*>::iterator it;
	for(it=enemies.begin(); it!=enemies.end(); it++)
	{
		if((*it)->GetState() != STATE_DYING && Intersection(GetHitBox(),(*it)->GetHitBox()) ) 
		{
			cPlayer1* player1 = dynamic_cast<cPlayer1*>(*it);
			if(player1 != NULL && player1->IsDoingSkill())
			{
				if(state == STATE_LEVITATEUP)		  player1->TestShieldProtection(STATE_ATTACKUP,GetDamage(),GetX(),GetY());
				else if(state == STATE_LEVITATEDOWN)  player1->TestShieldProtection(STATE_ATTACKDOWN,GetDamage(),GetX(),GetY());
				else if(state == STATE_LEVITATELEFT)  player1->TestShieldProtection(STATE_ATTACKLEFT,GetDamage(),GetX(),GetY());
				else if(state == STATE_LEVITATERIGHT) player1->TestShieldProtection(STATE_ATTACKRIGHT,GetDamage(),GetX(),GetY());
			}
			else
			{
				int new_health = (*it)->GetHealth() - GetDamage();
				if(new_health <= 0) (*it)->SetState(STATE_DYING);
				else if(dynamic_cast<cPlayer*>(*it) == NULL || !dynamic_cast<cPlayer*>(*it)->IsSuperAttacking())
				{
					if(state == STATE_LEVITATEUP)		  (*it)->SetState(STATE_DAMAGEDOWN);
					else if(state == STATE_LEVITATEDOWN)  (*it)->SetState(STATE_DAMAGEUP);
					else if(state == STATE_LEVITATELEFT)  (*it)->SetState(STATE_DAMAGERIGHT);
					else if(state == STATE_LEVITATERIGHT) (*it)->SetState(STATE_DAMAGELEFT);
					(*it)->SetFrame(0);
					(*it)->SetDelay(0);
				}
				if(new_health < 0) new_health = 0;
				(*it)->SetHealth(new_health);
			}
			collision = true;
		}
	}
	return collision;
}

bool cProjectile::Logic(bool *proj_collision_map,std::list<cBicho*> &enemies)
{
	if(!IsEnding())
	{
		if(ComputeEnemyCollisions(enemies))
		{
			//si choca contra algun enemigo se pondra en marcha la animacion de ending
			if(state == STATE_LEVITATEUP)		  state = STATE_ENDINGUP;
			else if(state == STATE_LEVITATEDOWN)  state = STATE_ENDINGDOWN;
			else if(state == STATE_LEVITATERIGHT) state = STATE_ENDINGRIGHT;
			else if(state == STATE_LEVITATELEFT)  state = STATE_ENDINGLEFT;
			seq = 0;
			delay = 0;
			return true;
		}
		else
		{
			if(state == STATE_LEVITATEUP || state == STATE_LEVITATEDOWN)
			{
				if(state == STATE_LEVITATEUP) LevitateUp(proj_collision_map);
				else LevitateDown(proj_collision_map);
			}
			else
			{
				if(state == STATE_LEVITATELEFT) LevitateLeft(proj_collision_map);
				else LevitateRight(proj_collision_map);
				
				if(displace_up) LevitateUp(proj_collision_map);
				else if(displace_down) LevitateDown(proj_collision_map);
			}
			if(IsEnding()) return true;
		}
	}
	return false;
}

void cProjectile::DrawRect(int tex_id,float xo,float yo,float xf,float yf)
{
	int screen_x,screen_y;

	screen_x = x + SCENE_Xo;
	screen_y = y + SCENE_Yo;

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D,tex_id);
	glBegin(GL_QUADS);
		glTexCoord2f(xo,yo);	glVertex3i(screen_x - (TILE_SIZE*frame_texture_tiles - hit_w)/2,screen_y - (TILE_SIZE*frame_texture_tiles - hit_h)/2,screen_y); //z ligeramente superior para dar sensacion de que esta por encima del nivel del suelo
		glTexCoord2f(xf,yo);	glVertex3i(screen_x + (TILE_SIZE*frame_texture_tiles + hit_w)/2,screen_y - (TILE_SIZE*frame_texture_tiles - hit_h)/2,screen_y);
		glTexCoord2f(xf,yf);	glVertex3i(screen_x + (TILE_SIZE*frame_texture_tiles + hit_w)/2,screen_y + (TILE_SIZE*frame_texture_tiles + hit_h)/2,screen_y);
		glTexCoord2f(xo,yf);	glVertex3i(screen_x - (TILE_SIZE*frame_texture_tiles - hit_w)/2,screen_y + (TILE_SIZE*frame_texture_tiles + hit_h)/2,screen_y);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

int cProjectile::GetState()
{
	return state;
}
void cProjectile::SetState(int s)
{
	state = s;
}

void cProjectile::NextFrame(int max)
{
	delay++;
	if(delay == frame_delay)
	{
		seq++;
		seq%=max;
		delay = 0;
	}
}
int cProjectile::GetFrame()
{
	return seq;
}
int cProjectile::GetDelay()
{
	return delay;
}

void cProjectile::SetDamage(int d)
{
	damage = d;
}
int cProjectile::GetDamage()
{
	return damage;
}

int cProjectile::GetStepLength()
{
	return step_length;
}

void cProjectile::SetDisplaceUp(bool b)
{
	displace_up = b;
}
void cProjectile::SetDisplaceDown(bool b)
{
	displace_down = b;
}

bool cProjectile::IsEnding()
{
	if(state == STATE_ENDINGUP    || state == STATE_ENDINGDOWN || 
	   state == STATE_ENDINGRIGHT || state == STATE_ENDINGLEFT  ) return true;
	else return false;
}