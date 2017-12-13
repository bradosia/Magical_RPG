#include "cPlayer.h"

cPlayer::cPlayer()
{
	SetBaseTilesHeight(1);
	SetBaseTilesWidth(1);
	SetHitHeight(TILE_SIZE+TILE_SIZE/2);
	SetHitWidth(TILE_SIZE);
	frame_walk_delay = 4;
	frame_attack_delay = 2;
	frame_texture_tiles = 4;
	step_length = STEP_LENGTH;
	cooldown = 0;
}
cPlayer::~cPlayer(){}

void cPlayer::DrawRect(int tex_id,float xo,float yo,float xf,float yf)
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
void cPlayer::NextFrame(int max)
{
	SetDelay(GetDelay()+1);
	if(GetState() == STATE_ATTACKUP || GetState() == STATE_ATTACKRIGHT || GetState() == STATE_ATTACKDOWN || GetState() == STATE_ATTACKLEFT || GetState() == STATE_SUPERATTACK)
	{
		if(GetDelay() == frame_attack_delay)
		{
			SetFrame(GetFrame()+1);
			SetFrame(GetFrame()%max);
			SetDelay(0);
		}
	}
	else
	{
		if(GetDelay() == frame_walk_delay)
		{
			SetFrame(GetFrame()+1);
			SetFrame(GetFrame()%max);
			SetDelay(0);
		}
	}
}

int cPlayer::GetFrameTop()
{
	return GetY() + GetHitHeight() + (frame_texture_tiles*TILE_SIZE - GetHitHeight())/2;
}
int cPlayer::GetFrameBottom()
{
	return GetY() - (frame_texture_tiles*TILE_SIZE - GetHitHeight())/2;
}
int cPlayer::GetFrameTexTiles()
{
	return frame_texture_tiles;
}

bool cPlayer::CollidesWithEnemy(std::list<cBicho*> &enemies,int next_tile)
{
	unsigned int i;
	std::list<cBicho*>::iterator it;
	for(it=enemies.begin(); it!=enemies.end(); it++)
	{
		if((*it)->GetState() != STATE_DYING)
		{
			std::vector<int> tiles;
			tiles = (*it)->GetTiles();
			for(i=0; i<tiles.size(); i++) if(tiles[i] == next_tile) return true;
		}
	}
	return false;
}
bool cPlayer::CollidesUD(bool *collision_map,std::list<cBicho*> &enemies,bool up)
{	
	int tile_x,tile_y; //son las coords en tiles donde esta el bicho despues de dar el paso
	int width_tiles; //num de tiles en horizontal con las que colisiona

	tile_x = GetX() / TILE_SIZE;
	if(up && ((GetY() + GetBaseHeight()) / TILE_SIZE) < SCENE_HEIGHT) tile_y = (GetY() + GetBaseHeight()) / TILE_SIZE; //la comprobacion de SCENE_HEIGHT esta para evitar hacer calculos con posiciones fuera de la matriz de colision
	else tile_y = GetY() / TILE_SIZE;

	width_tiles = GetBaseWidth()/TILE_SIZE;
	if((GetX() % TILE_SIZE) != 0) width_tiles++;

	//evaluo collisiones contra enemigos, no producen slide
	for(int j=0;j<width_tiles;j++)
	{
		if(CollidesWithEnemy(enemies,(tile_x+j) + (tile_y*SCENE_WIDTH))) return true;
	}

	//evaluo collisiones contra el escenario, producen slide
	for(int j=1;j<(width_tiles-1);j++)
	{
		if(collision_map[ (tile_x+j) + (tile_y*SCENE_WIDTH) ]) return true;
	}
	//si todas las tiles consultadas menos la ultima son traspasables y tengo potencial para girar a la izquierda, hago slideL
	if(!collision_map[ (tile_x) + (tile_y*SCENE_WIDTH) ])
	{ 
		if(collision_map[ (tile_x+(width_tiles-1)) + (tile_y*SCENE_WIDTH) ]) 
		{
			if(GetState() != STATE_WALKLEFT && GetState() != STATE_WALKRIGHT && GetState() != STATE_SKILLWALKLEFT && GetState() != STATE_SKILLWALKRIGHT /*&& (x % TILE_SIZE) < TILE_SIZE/2*/) SetX(GetX()-step_length);
			return true;
		}
		return false;
	}
	//si todas las tiles consultadas menos la primera son traspasables y tengo potencial para girar a la derecha, hago slideR
	else
	{
		if(!collision_map[ (tile_x+(width_tiles-1)) + (tile_y*SCENE_WIDTH) ]) 
		{
			if(GetState() != STATE_WALKLEFT && GetState() != STATE_WALKRIGHT && GetState() != STATE_SKILLWALKLEFT && GetState() != STATE_SKILLWALKRIGHT /*&& (x % TILE_SIZE) >= TILE_SIZE/2*/) SetX(GetX()+step_length);
		}
		return true;
	}
}
bool cPlayer::CollidesLR(bool *collision_map,std::list<cBicho*> &enemies,bool right,bool vertical)
{
	int tile_x,tile_y; //son las coords en tiles donde esta el bicho despues de dar el paso
	int height_tiles; //num de tiles en vertical con las que colisiona

	if(right && ((GetX() + GetBaseWidth()) / TILE_SIZE) < SCENE_WIDTH) tile_x = (GetX() + GetBaseWidth()) / TILE_SIZE;
	else tile_x = GetX() / TILE_SIZE;
	tile_y = GetY() / TILE_SIZE;

	height_tiles = GetBaseHeight()/TILE_SIZE;
	if((GetY() % TILE_SIZE) != 0) height_tiles++;

	//evaluo collisiones contra enemigos, no producen slide
	for(int j=0;j<height_tiles;j++)
	{
		if(CollidesWithEnemy(enemies,(tile_x) + ((tile_y+j)*SCENE_WIDTH))) return true;
	}

	//evaluo collisiones contra el escenario, producen slide
	for(int j=1;j<(height_tiles-1);j++)
	{
		return (collision_map[ (tile_x) + ((tile_y+j)*SCENE_WIDTH) ]);
	}
	//si todas las tiles consultadas menos la mas alta son traspasables y tengo potencial para ir hacia abajo, hago slideD
	if(!collision_map[ (tile_x) + (tile_y*SCENE_WIDTH) ])
	{ 
		if(collision_map[ (tile_x) + ((tile_y+(height_tiles-1))*SCENE_WIDTH) ]) 
		{
			if(!vertical /*&& (y % TILE_SIZE) < TILE_SIZE/2*/) SetY(GetY()-step_length);
			return true;
		}
		return false;
	}
	//si todas las tiles consultadas menos la mas baja son traspasables y tengo potencial para ir hacia arriba, hago slideU
	else
	{
		if(!collision_map[ (tile_x) + ((tile_y+(height_tiles-1))*SCENE_WIDTH) ]) 
		{
			if(!vertical /*&& (y % TILE_SIZE) >= TILE_SIZE/2*/) SetY(GetY()+step_length);
		}
		return true;
	}
}

void cPlayer::RestrictedMoveUp(bool *collision_map,std::list<cBicho*> &enemies,int h1,int h2,int game_height)
{
	if(h1 <= h2 || h1-h2 <= game_height - TILE_SIZE) MoveUp(collision_map,enemies);
	else 
	{
		if(GetState() != STATE_WALKLEFT && GetState() != STATE_WALKRIGHT && GetState() != STATE_SKILLWALKLEFT && GetState() != STATE_SKILLWALKRIGHT) 
		{
			if(GetState() != STATE_WALKUP && GetState() != STATE_SKILLWALKUP) {
				if(IsDoingSkill()) SetState(STATE_SKILLWALKUP);
				else SetState(STATE_WALKUP);
				SetFrame(0);
				SetDelay(0);
			}
		}
	}
}
void cPlayer::RestrictedMoveDown(bool *collision_map,std::list<cBicho*> &enemies,int h1,int h2,int game_height, int bottom_limit)
{
	if(h1 > bottom_limit && (h1 > h2 || h2-h1 <= game_height - TILE_SIZE)) MoveDown(collision_map,enemies);
	else
	{
		if(GetState() != STATE_WALKLEFT && GetState() != STATE_WALKRIGHT && GetState() != STATE_SKILLWALKLEFT && GetState() != STATE_SKILLWALKRIGHT) 
		{
			if(GetState() != STATE_WALKDOWN && GetState() != STATE_SKILLWALKDOWN) {
				if(IsDoingSkill()) SetState(STATE_SKILLWALKDOWN);
				else SetState(STATE_WALKDOWN);
				SetFrame(0);
				SetDelay(0);
			}
		}
	}
}
void cPlayer::MoveUp(bool *collision_map,std::list<cBicho*> &enemies)
{
	int yaux;
	
	if(GetState() != STATE_WALKLEFT && GetState() != STATE_WALKRIGHT && GetState() != STATE_SKILLWALKLEFT && GetState() != STATE_SKILLWALKRIGHT) 
	{
		if(GetState() != STATE_WALKUP && GetState() != STATE_SKILLWALKUP) {
			if(IsDoingSkill()) SetState(STATE_SKILLWALKUP);
		else SetState(STATE_WALKUP);
			SetFrame(0);
			SetDelay(0);
		}
	}
	//Whats next tile?
	if( (GetY() % TILE_SIZE) == 0 )
	{
		yaux = GetY();
		SetY(GetY()+step_length);
		if(CollidesUD(collision_map,enemies,true)) SetY(yaux);
	}
	//Advance, no problem
	else SetY(GetY()+step_length);
}
void cPlayer::MoveDown(bool *collision_map,std::list<cBicho*> &enemies)
{
	int yaux;

	if(GetState() != STATE_WALKLEFT && GetState() != STATE_WALKRIGHT && GetState() != STATE_SKILLWALKLEFT && GetState() != STATE_SKILLWALKRIGHT) 
	{
		if(GetState() != STATE_WALKDOWN && GetState() != STATE_SKILLWALKDOWN) {
			if(IsDoingSkill()) SetState(STATE_SKILLWALKDOWN);
			else SetState(STATE_WALKDOWN);
			SetFrame(0);
			SetDelay(0);
		}
	}
	//Whats next tile?
	if( (GetY() % TILE_SIZE) == 0)
	{
		yaux = GetY();
		SetY(GetY()-step_length);
		if(CollidesUD(collision_map,enemies,false)) SetY(yaux);
	}
	//Advance, no problem
	else SetY(GetY()-step_length);
}
void cPlayer::MoveLeft(bool *collision_map,std::list<cBicho*> &enemies,bool vertical)
{
	int xaux;
	
	if(GetState() != STATE_WALKLEFT && GetState() != STATE_SKILLWALKLEFT) {
		if(IsDoingSkill()) SetState(STATE_SKILLWALKLEFT);
		else SetState(STATE_WALKLEFT);
		SetFrame(0);
		SetDelay(0);
	}
	//Whats next tile?
	if( (GetX() % TILE_SIZE) == 0)   //habra un cambio de tile si me muevo
	{
		xaux = GetX();
		SetX(GetX()-step_length);
		if(CollidesLR(collision_map,enemies,false,vertical)) SetX(xaux);
	}
	//Advance, no problem
	else SetX(GetX()-step_length);
}
void cPlayer::MoveRight(bool *collision_map,std::list<cBicho*> &enemies,bool vertical)
{
	int xaux;

	if(GetState() != STATE_WALKRIGHT && GetState() != STATE_SKILLWALKRIGHT) {
		if(IsDoingSkill()) SetState(STATE_SKILLWALKRIGHT);
		else SetState(STATE_WALKRIGHT);
		SetFrame(0);
		SetDelay(0);
	}
	//Whats next tile?
	if( (GetX() % TILE_SIZE) == 0)
	{
		xaux = GetX();
		SetX(GetX()+step_length);

		if(CollidesLR(collision_map,enemies,true,vertical)) SetX(xaux);
	}
	//Advance, no problem
	else SetX(GetX()+step_length);
}

void cPlayer::SetMaxSP(int max_sp)
{
	max_SP = max_sp;
}
int cPlayer::GetMaxSP()
{
	return max_SP;
}
void cPlayer::SetSP(int sp)
{
	SP = sp;
}
int cPlayer::GetSP()
{
	return SP;
}
void cPlayer::SetCharge(int c)
{
	charge = c;
}
int cPlayer::GetCharge()
{
	return charge;
}
void cPlayer::SetFrameAttackDelay(int fad)
{
	frame_attack_delay = fad;
}
int cPlayer::GetFrameAttackDelay()
{
	return frame_attack_delay;
}

void cPlayer::StartSuperAttack()
{
	SetState(STATE_SUPERATTACK);
	
	SetFrame(0);
	SetDelay(0);
}
bool cPlayer::IsAttacking()
{
	if( GetState() == STATE_ATTACKUP   || GetState() == STATE_ATTACKDOWN  ||
	    GetState() == STATE_ATTACKLEFT || GetState() == STATE_ATTACKRIGHT ||
		GetState() == STATE_SUPERATTACK ) return true;
	return false;
}
bool cPlayer::IsSuperAttacking()
{
	if( GetState() == STATE_SUPERATTACK ) return true;
	return false;
}
bool cPlayer::IsDoingSkill()
{
	if( GetState() == STATE_SKILLUP		  || GetState() == STATE_SKILLDOWN      ||
	    GetState() == STATE_SKILLLEFT     || GetState() == STATE_SKILLRIGHT     ||
		GetState() == STATE_SKILLWALKUP   || GetState() == STATE_SKILLWALKDOWN  ||
	    GetState() == STATE_SKILLWALKLEFT || GetState() == STATE_SKILLWALKRIGHT  ) return true;
	return false;
}
bool cPlayer::IsWalking()
{
	if( GetState() == STATE_WALKUP        || GetState() == STATE_WALKDOWN       ||
	    GetState() == STATE_WALKLEFT      || GetState() == STATE_WALKRIGHT      ||
		GetState() == STATE_SKILLWALKUP   || GetState() == STATE_SKILLWALKDOWN  ||
	    GetState() == STATE_SKILLWALKLEFT || GetState() == STATE_SKILLWALKRIGHT  ) return true;
	return false;
}

void cPlayer::TransformIntoSkillState()
{
	switch(GetState())
	{
		case STATE_LOOKUP:		SetState(STATE_SKILLUP);		break;
		case STATE_LOOKRIGHT:	SetState(STATE_SKILLRIGHT);		break;
		case STATE_LOOKDOWN:	SetState(STATE_SKILLDOWN);		break;
		case STATE_LOOKLEFT:	SetState(STATE_SKILLLEFT);		break;

		case STATE_WALKUP:		SetState(STATE_SKILLWALKUP);	break;
		case STATE_WALKRIGHT:	SetState(STATE_SKILLWALKRIGHT);	break;
		case STATE_WALKDOWN:	SetState(STATE_SKILLWALKDOWN);	break;
		case STATE_WALKLEFT:	SetState(STATE_SKILLWALKLEFT);	break;
	}
	SetFrame(0);
	SetDelay(0);
}

void cPlayer::Stop()
{
	switch(GetState())
	{
		case STATE_WALKUP:			SetState(STATE_LOOKUP);		break;
		case STATE_WALKRIGHT:		SetState(STATE_LOOKRIGHT);	break;
		case STATE_WALKDOWN:		SetState(STATE_LOOKDOWN);	break;
		case STATE_WALKLEFT:		SetState(STATE_LOOKLEFT);	break;

		case STATE_SKILLWALKUP:		SetState(STATE_SKILLUP);	break;
		case STATE_SKILLWALKRIGHT:	SetState(STATE_SKILLRIGHT);	break;
		case STATE_SKILLWALKDOWN:	SetState(STATE_SKILLDOWN);	break;
		case STATE_SKILLWALKLEFT:	SetState(STATE_SKILLLEFT);	break;

		case STATE_ATTACKUP:		SetState(STATE_LOOKUP);		break;
		case STATE_ATTACKRIGHT:		SetState(STATE_LOOKRIGHT);	break;
		case STATE_ATTACKDOWN:		SetState(STATE_LOOKDOWN);	break;
		case STATE_ATTACKLEFT:		SetState(STATE_LOOKLEFT);	break;

		case STATE_DAMAGEUP:		SetState(STATE_LOOKUP);		break;
		case STATE_DAMAGERIGHT:		SetState(STATE_LOOKRIGHT);	break;
		case STATE_DAMAGEDOWN:		SetState(STATE_LOOKDOWN);	break;
		case STATE_DAMAGELEFT:		SetState(STATE_LOOKLEFT);	break;

		case STATE_SUPERATTACK:		SetState(STATE_LOOKDOWN);	break;
	}
}
void cPlayer::StopDoingSkill()
{
	switch(GetState())
	{
		case STATE_SKILLUP:			SetState(STATE_LOOKUP);		break;
		case STATE_SKILLRIGHT:		SetState(STATE_LOOKRIGHT);	break;
		case STATE_SKILLDOWN:		SetState(STATE_LOOKDOWN);	break;
		case STATE_SKILLLEFT:		SetState(STATE_LOOKLEFT);	break;

		case STATE_SKILLWALKUP:		SetState(STATE_LOOKUP);		break;
		case STATE_SKILLWALKRIGHT:	SetState(STATE_LOOKRIGHT);	break;
		case STATE_SKILLWALKDOWN:	SetState(STATE_LOOKDOWN);	break;
		case STATE_SKILLWALKLEFT:	SetState(STATE_LOOKLEFT);	break;
	}
}

void cPlayer::SetCooldown(int c)
{
	cooldown = c;
}
int cPlayer::GetCooldown()
{
	return cooldown;
}