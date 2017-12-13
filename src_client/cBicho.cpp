#include "cBicho.h"

cBicho::cBicho(void)
{
	seq = 0;
	delay = 0;
	state = STATE_LOOKDOWN;
}
cBicho::~cBicho(void){}

void cBicho::SetPosition(int posx,int posy)
{
	x = posx;
	y = posy;
}
void cBicho::SetX(int posx)
{
	x = posx;
}
int cBicho::GetX()
{
	return x;
}
void cBicho::SetY(int posy)
{
	y = posy;
}
int cBicho::GetY()
{
	return y;
}
void cBicho::SetBaseTilesHeight(int bth)
{
	base_tiles_h = bth;
}
int cBicho::GetBaseHeight()
{
	return base_tiles_h*TILE_SIZE;
}
void cBicho::SetBaseTilesWidth(int btw)
{
	base_tiles_w = btw;
}
int cBicho::GetBaseWidth()
{
	return base_tiles_w*TILE_SIZE;
}
void cBicho::SetHitHeight(int hh)
{
	hit_h = hh;
}
int cBicho::GetHitHeight()
{
	return hit_h;
}
void cBicho::SetHitWidth(int hw)
{
	hit_w = hw;
}
int cBicho::GetHitWidth()
{
	return hit_w;
}
std::vector<int> cBicho::GetTiles() //segun las dimensiones de la base
{
	int i,j;
	std::vector<int> tiles;
	int tile_x,tile_y;  //son las coords en tiles donde esta el bicho
	int occupied_tiles_w,occupied_tiles_h; //son el numero de tiles de terreno que ocupa la base del bicho
	
	tile_x = x/TILE_SIZE;
	tile_y = y/TILE_SIZE;
	occupied_tiles_w = base_tiles_w;
	occupied_tiles_h = base_tiles_h;
	if((x%TILE_SIZE) != 0) occupied_tiles_w++;
	if((y%TILE_SIZE) != 0) occupied_tiles_h++;

	for(i=0; i<occupied_tiles_w; i++)
	{
		for(j=0; j<occupied_tiles_h; j++) tiles.push_back( (tile_x+i) + ((tile_y+j)*SCENE_WIDTH) );
	}
	return tiles;
}

cRect cBicho::GetHitBox()
{
	cRect hitbox;
	hitbox.top = y + hit_h;
	hitbox.bottom = y;
	hitbox.left = x;
	hitbox.right = x + hit_w;
	return hitbox;
}
bool cBicho::Intersection(cRect box1, cRect box2)
{
	if(box1.top > box2.bottom && box2.top > box1.bottom)
	{
		if(box1.left < box2.right && box2.left < box1.right) return true;
	}
	return false;
}
bool cBicho::Intersection(cRect box1, int px, int py)
{
	if(px >= box1.left && px < box1.right && py >= box1.bottom && py < box1.top) return true; 
	return false;
}
void cBicho::SetTile(int tx,int ty)
{
	x = tx * TILE_SIZE;
	y = ty * TILE_SIZE;
}

void cBicho::Stop()
{
	switch(state)
	{
		case STATE_WALKUP:		state = STATE_LOOKUP;		break;
		case STATE_WALKRIGHT:	state = STATE_LOOKRIGHT;	break;
		case STATE_WALKDOWN:	state = STATE_LOOKDOWN;		break;
		case STATE_WALKLEFT:	state = STATE_LOOKLEFT;		break;

		case STATE_ATTACKUP:	state = STATE_LOOKUP;		break;
		case STATE_ATTACKRIGHT:	state = STATE_LOOKRIGHT;	break;
		case STATE_ATTACKDOWN:	state = STATE_LOOKDOWN;		break;
		case STATE_ATTACKLEFT:	state = STATE_LOOKLEFT;		break;

		case STATE_DAMAGEUP:	state = STATE_LOOKUP;		break;
		case STATE_DAMAGERIGHT:	state = STATE_LOOKRIGHT;	break;
		case STATE_DAMAGEDOWN:	state = STATE_LOOKDOWN;		break;
		case STATE_DAMAGELEFT:	state = STATE_LOOKLEFT;		break;
	}
}

void cBicho::StartAttack()
{
	if(state == STATE_LOOKUP	|| state == STATE_WALKUP)	 state = STATE_ATTACKUP;
	if(state == STATE_LOOKDOWN  || state == STATE_WALKDOWN)  state = STATE_ATTACKDOWN;
	if(state == STATE_LOOKLEFT  || state == STATE_WALKLEFT)  state = STATE_ATTACKLEFT;
	if(state == STATE_LOOKRIGHT || state == STATE_WALKRIGHT) state = STATE_ATTACKRIGHT;

	seq = 0;
	delay = 0;
}

void cBicho::SetFrame(int frame)
{
	seq = frame;
}
int cBicho::GetFrame()
{
	return seq;
}
int cBicho::GetState()
{
	return state;
}
void cBicho::SetState(int s)
{
	state = s;
}
void cBicho::SetDelay(int d)
{
	delay = d;
}
int cBicho::GetDelay()
{
	return delay;
}

void cBicho::SetMaxHealth(int max_h)
{
	max_health = max_h;
}
int cBicho::GetMaxHealth()
{
	return max_health;
}
void cBicho::SetHealth(int h)
{
	health = h;
}
int cBicho::GetHealth()
{
	return health;
}
void cBicho::SetDamage(int d)
{
	damage = d;
}
int cBicho::GetDamage()
{
	return damage;
}

bool cBicho::IsLooking()
{
	if( state == STATE_LOOKUP   || state == STATE_LOOKDOWN || 
	    state == STATE_LOOKLEFT || state == STATE_LOOKRIGHT ) return true;
	return false;
}
bool cBicho::IsWalking()
{
	if( state == STATE_WALKUP   || state == STATE_WALKDOWN || 
	    state == STATE_WALKLEFT || state == STATE_WALKRIGHT ) return true;
	return false;
}
bool cBicho::IsAttacking()
{
	if( state == STATE_ATTACKUP   || state == STATE_ATTACKDOWN  || 
	    state == STATE_ATTACKLEFT || state == STATE_ATTACKRIGHT  ) return true;
	return false;
}
bool cBicho::IsDamaged()
{
	if( state == STATE_DAMAGEUP   || state == STATE_DAMAGEDOWN || 
	    state == STATE_DAMAGELEFT || state == STATE_DAMAGERIGHT ) return true;
	return false;
}